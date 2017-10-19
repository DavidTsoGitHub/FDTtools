#include "DCU.h"

#include <QGraphicsPixmapItem>
#include <QBitmap>
#include <QRgb>

DCU::DCU(const BSPDisplay* display): m_display(display),
    m_layersUpdated(false)
{
    m_graphicsScene = new QGraphicsScene(0, 0, display->GetWidth(), display->GetHeight());
    QBrush backgroundBrush(QColor(Qt::red));
    m_graphicsScene->setBackgroundBrush(backgroundBrush);

    if (m_display->GetRenderingType() == BSPDisplay::HW)
    {
        for (int i=0; i<display->GetNumberOfLayers(); ++i)
        {
            DCULayer* layer = new DCULayer();
            layer->Enabled = false;
            m_layers.append(layer);
        }
    }
    else
    {
        DCULayer* layer = new DCULayer();
        layer->Enabled = false;
        m_layers.append(layer);
        SetCLUT(0, 0xFFFFFFFF);
        SetCLUT(1, 0xFF000000);
        SetLayerConfiguration(0, 0, 0, display->GetWidth(), display->GetHeight(), 255, 1, 0, false);
    }
}

DCU::~DCU()
{
    while (!m_layers.isEmpty())
    {
        delete m_layers.takeFirst();
    }
}

void DCU::SetLayerContent(quint8 layerID, QByteArray decodedFrameBuffer)
{
    if (layerID >= m_layers.size()) return;


    DCULayer* layer = m_layers.at(layerID);
    if (layer->BitsPerPixel == 1)
    {
        layer->Content = QImage((uchar*)decodedFrameBuffer.data(), layer->Width, layer->Height, 30, QImage::Format_MonoLSB);
    }
    else if (layer->BitsPerPixel == 3) // 8 BPP
    {
        layer->Content = QImage((uchar*)decodedFrameBuffer.data(), layer->Width, layer->Height, QImage::Format_Indexed8);
    }
    else if (layer->BitsPerPixel == 4) // 16 BPP
    {
        layer->Content = QImage((uchar*)decodedFrameBuffer.data(), layer->Width, layer->Height, QImage::Format_RGB16);
    }
    else
    {
        return;
    }

    if (layer->BitsPerPixel != 4)  ///Configure CLUT for layers other than 16 BPP
    {
        QMapIterator<quint8, quint32> colorIterator(m_DCU_colors);
        while (colorIterator.hasNext())
        {
            colorIterator.next();
            layer->Content.setColor(colorIterator.key(), colorIterator.value());
        }
    }

    refreshLayer(layerID);
}

void DCU::SetLayerConfiguration(quint8 layerID, quint16 x, quint16 y, quint16 width, quint16 height, quint8 opacity, quint8 bpp, quint8 clutOffset, bool enabled)
{
    // Later on this call could be used to emulate animation as we have the possibility to move and transform the QGraphicsItem
    // qDebug("SetLayer: %d @%d:%d  %dx%d %s", layerID, x, y, width, height, enabled ? "enabled" : "disabled");

    if (layerID >= m_layers.size()) return;

    DCULayer* layer = m_layers.at(layerID);
    layer->X = x;
    layer->Y = y;
    layer->Width = width;
    layer->Height = height;
    layer->Opacity = opacity;
    layer->BitsPerPixel = bpp;
    layer->CLUTOffset = clutOffset;
    layer->Enabled = enabled;

    if (!enabled)
    {
        refreshLayer(layerID);
    }
}

void DCU::LayersUpdatedComplete()
{
    if (m_layersUpdated == true)
    {
        foreach (quint8 layerID, m_updatedLayers.keys())
        {
            DCULayer* layer = m_layers.at(layerID);
            //removeLayer(layerID);
            m_graphicsScene->removeItem(m_sceneLayerItems.value(layerID));
            QGraphicsPixmapItem* item = m_graphicsScene->addPixmap(m_updatedLayers.value(layerID));
            item->setX(layer->X);
            item->setY(layer->Y);
            item->setZValue(layerID);
            item->setOpacity(layer->Opacity/255.0);
            m_sceneLayerItems.insert(layerID, item);
        }

        m_updatedLayers.clear();
        m_layersUpdated = false;
    }
}

void DCU::SetCLUT(quint8 index, quint32 argb)
{
    m_DCU_colors.insert(index, argb);
}

quint32 DCU::GetBackgroundColor(quint8 colorID)
{
    return m_DCU_colors[colorID];
}

void DCU::SetChromaKey(quint8 index, quint32 argbStart, quint32 argbEnd)
{
    QPair<quint32,quint32> chromaRange = qMakePair(argbStart, argbEnd);
    m_DCU_chromaKey.insert(index, chromaRange);
}

QGraphicsScene* DCU::GetGraphicsScene()
{
    return m_graphicsScene;
}

void DCU::refreshLayer(quint8 layerID)
{
    if (layerID >= m_layers.size()) return;

    DCULayer* layer = m_layers.at(layerID);

    if (layer->Enabled)
    {
        QPixmap pixmap;
        maskChromaKey(&pixmap, layerID);

        m_updatedLayers.insert(layerID, pixmap);
        m_layersUpdated = true;
    }
    else
    {
        removeLayer(layerID);
    }
}

void DCU::maskChromaKey(QPixmap* pixmap, quint8 layerID)
{

    //The internal representation in the original QImage may not be RGB, so we'll set this explicitly
    QImage rgbImageLayer = m_layers.at(layerID)->Content.convertToFormat(QImage::Format_ARGB32);

    //the chroma-key range to mask out
    quint16 rMin = (m_DCU_chromaKey.value(layerID).first & 0x00ff0000) >> 16;
    quint16 rMax = (m_DCU_chromaKey.value(layerID).second & 0x00ff0000) >> 16;
    quint16 gMin = (m_DCU_chromaKey.value(layerID).first & 0x0000ff00) >> 8;
    quint16 gMax = (m_DCU_chromaKey.value(layerID).second & 0x0000ff00) >> 8;
    quint16 bMin = (m_DCU_chromaKey.value(layerID).first & 0x000000ff);
    quint16 bMax = (m_DCU_chromaKey.value(layerID).second & 0x000000ff);

    for (int y = 0; y < rgbImageLayer.height(); y++)
    {
        QRgb* currentLine = reinterpret_cast<QRgb*>(rgbImageLayer.scanLine(y));

        for (int x = 0; x < rgbImageLayer.width(); x++)
        {
            if (((qRed(currentLine[x]) >= rMin) && (qRed(currentLine[x]) <= rMax)) &&
                ((qGreen(currentLine[x]) >= gMin) && (qGreen(currentLine[x]) <= gMax)) &&
                ((qBlue(currentLine[x]) >= bMin) && (qBlue(currentLine[x]) <= bMax)) )
            {
               currentLine[x] = currentLine[x] & 0x00ffffff;
            }
        }
    }

    *pixmap = QPixmap::fromImage(rgbImageLayer, Qt::AutoColor | Qt::ThresholdDither | Qt::AvoidDither);
}

void DCU::removeLayer(quint8 layerID)
{
    QGraphicsItem* item = m_sceneLayerItems.value(layerID);
    if (item != NULL)
    {
        m_graphicsScene->removeItem(m_sceneLayerItems.value(layerID));
        delete item;
    }
    m_sceneLayerItems.remove(layerID);
}
