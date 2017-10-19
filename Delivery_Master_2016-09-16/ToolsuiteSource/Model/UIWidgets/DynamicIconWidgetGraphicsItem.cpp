#include "DynamicIconWidgetGraphicsItem.h"
#include "Logger.h"

#include <QDir>

DynamicIconWidgetGraphicsItem::DynamicIconWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor)
    : GraphicsItemBase(x, y, width, height, Qt::white, Qt::black, drawChromaKeyColor)
{}


void DynamicIconWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawDynamicContentMarker(painter);
}
