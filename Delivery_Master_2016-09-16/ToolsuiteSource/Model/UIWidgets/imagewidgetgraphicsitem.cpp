#include "ImageWidgetGraphicsItem.h"
#include "Logger.h"

#include <QDir>
#include <QPixmap>
#include <QBitmap>

ImageWidgetGraphicsItem::ImageWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor, QColor chromaKeyColor,
                                                 const QString& resourcePath, const QList<QRect> &animPreviewRects)
    : GraphicsItemBase(x, y, width, height, Qt::black, Qt::black, drawChromaKeyColor),
    m_chromaKeyColor(chromaKeyColor),
    m_resourcePath(resourcePath),
    m_animPreviewRects(animPreviewRects)
{}

const QString ImageWidgetGraphicsItem::GetResourcePath()
{
    if (m_resourcePath.isEmpty())
    {
        LOG_ERROR("No resource path set.");
    }

    return m_resourcePath;
}

void ImageWidgetGraphicsItem::SetResourcePath(QString resourcePath)
{
    m_resourcePath = resourcePath;
}

void ImageWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    QImage image;
    image.load(GetResourcePath());

    if (m_drawChromaKeyColor == false)
    {
        QPixmap pixmap = QPixmap::fromImage(image, Qt::AutoColor | Qt::ThresholdDither | Qt::AvoidDither);
        QBitmap mask = QBitmap(pixmap.createMaskFromColor(m_chromaKeyColor, Qt::MaskInColor));
        pixmap.setMask(mask);
        painter->drawPixmap(0, 0, GetWidth(), GetHeight(), pixmap);
    }
    else
    {
        painter->drawPixmap(0, 0, GetWidth(), GetHeight(), QPixmap::fromImage(image));
    }


    drawAnimationPreviewRects(painter, m_animPreviewRects);
}
