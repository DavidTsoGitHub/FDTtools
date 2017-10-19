#ifndef LISTNAVIGATIONGRAPHICSITEM_H
#define LISTNAVIGATIONGRAPHICSITEM_H

#include "GraphicsItemBase.h"

class ListNavigationWidgetGraphicsItem : public GraphicsItemBase
{
public:
    ListNavigationWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
};

#endif // LISTNAVIGATIONGRAPHICSITEM_H
