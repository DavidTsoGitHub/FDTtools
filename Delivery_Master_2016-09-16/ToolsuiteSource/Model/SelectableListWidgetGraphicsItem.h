#ifndef SELECTABLELISTWIDGETGRAPHICSITEM_H
#define SELECTABLELISTWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"

class SelectableListWidgetGraphicsItem : public GraphicsItemBase
{
public:
    SelectableListWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SELECTABLELISTWIDGETGRAPHICSITEM_H
