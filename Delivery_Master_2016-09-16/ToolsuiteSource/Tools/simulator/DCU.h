#ifndef DCU_H
#define DCU_H

#include "BSPDisplay.h"

#include <QGraphicsScene>
#include <QImage>
#include <QMap>
#include <QPair>

class DCULayer
{
public:
    quint16 X;
    quint16 Y;
    quint16 Width;
    quint16 Height;
    quint8 Opacity;
    quint8 BitsPerPixel;
    quint8 CLUTOffset;
    bool Enabled;
    QImage Content;
};

class DCU
{
public:
    explicit DCU(const BSPDisplay* display);
    DCU(const DCU&) = delete;
    ~DCU();

    void SetLayerContent(quint8 layerID, QByteArray decodedFrameBuffer);
    void SetLayerConfiguration(quint8 layerID, quint16 x, quint16 y, quint16 width, quint16 height, quint8 opacity, quint8 bpp, quint8 clutOffset, bool enabled);
    void LayersUpdatedComplete(void);
    void SetCLUT(quint8 index, quint32 argb);
    void SetChromaKey(quint8 index, quint32 argbStart, quint32 argbEnd);
    quint32 GetBackgroundColor(quint8 colorID);

    QGraphicsScene* GetGraphicsScene();

private:
    const BSPDisplay* m_display;
    QMap<quint8, quint32> m_DCU_colors;
    QMap<quint8, QPair<quint32, quint32>> m_DCU_chromaKey;
    QList<DCULayer*> m_layers;
    QMap<quint8, QPixmap> m_updatedLayers;
    QMap<quint8, QGraphicsItem*> m_sceneLayerItems;
    QGraphicsScene* m_graphicsScene;
    quint8 m_backgroundColorID;
    bool m_layersUpdated;

    void removeLayer(quint8 layerID);
    void refreshLayer(quint8 layer);
    void maskChromaKey(QPixmap* pixmap, quint8 layerID);
};

#endif // DCU_H
