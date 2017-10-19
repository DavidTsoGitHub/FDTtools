#include "DynamicImageWidgetGraphicsItem.h"

DynamicImageWidgetGraphicsItem::DynamicImageWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor,
                                                               QColor fgColor, const QList<QRect>& animPreviewRects) :
    GraphicsItemBase(x, y, width, height, fgColor, Qt::black, drawChromaKeyColor), m_animPreviewRects(animPreviewRects)
{
}

void DynamicImageWidgetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GraphicsItemBase::paint(painter, option, widget);

    drawDynamicContentMarker(painter);
    drawAnimationPreviewRects(painter, m_animPreviewRects);
}
