#include "ListNavigationWidgetGraphicsItem.h"

ListNavigationWidgetGraphicsItem::ListNavigationWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor) :
    GraphicsItemBase(x, y, width, height, fgColor, bgColor, drawChromaKeyColor)
{}

void ListNavigationWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(m_drawChromaKeyColor == true)
    {
        painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(m_bgColor));
    }

    drawDynamicContentMarker(painter);

    drawBorder(painter);
}
