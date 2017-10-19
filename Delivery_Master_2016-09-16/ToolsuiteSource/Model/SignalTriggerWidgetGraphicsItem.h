#ifndef SIGNALTRIGGERWIDGETGRAPHICSITEM_H
#define SIGNALTRIGGERWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"

class SignalTriggerWidgetGraphicsItem : public GraphicsItemBase
{
public:
    SignalTriggerWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SIGNALTRIGGERWIDGETGRAPHICSITEM_H
