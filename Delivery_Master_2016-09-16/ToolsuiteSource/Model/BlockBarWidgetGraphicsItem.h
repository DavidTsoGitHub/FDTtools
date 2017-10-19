#ifndef BLOCKBARWIDGETGRAPHICSITEM_H
#define BLOCKBARWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"

class BlockBarWidgetGraphicsItem : public GraphicsItemBase
{
public:
    BlockBarWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height,
                               QColor fgColor, QColor bgColor, quint16 spacing, quint8 numberOfBlocks, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    quint16 m_spacing;
    quint8 m_numberOfBlocks;
};

#endif // BLOCKBARWIDGETGRAPHICSITEM_H
