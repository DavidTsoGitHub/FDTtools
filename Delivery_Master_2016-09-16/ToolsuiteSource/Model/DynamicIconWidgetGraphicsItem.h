#ifndef DYNAMICICONCONTAINERGRAPHICSITEM_H
#define DYNAMICICONCONTAINERGRAPHICSITEM_H

#include <GraphicsItemBase.h>

class DynamicIconWidgetGraphicsItem : public GraphicsItemBase
{
public:
    DynamicIconWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // DYNAMICICONCONTAINERGRAPHICSITEM_H
