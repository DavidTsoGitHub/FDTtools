#ifndef COLUMNLEVELWIDGETGRAPHICSITEM_H
#define COLUMNLEVELWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"

class ColumnLevelWidgetGraphicsItem : public GraphicsItemBase
{
public:
    ColumnLevelWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height,
                                  QColor fgColor, QColor bgColor, quint16 spacing, quint8 numberOfBlocks,
                                  quint8 minBlockHeight, quint8 maxBlockHeight, bool drawChromaKeyColor);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    quint16 m_spacing;
    quint8 m_numberOfBlocks;
    quint8 m_minBlockHeight;
    quint8 m_maxBlockHeight;
};

#endif // COLUMNLEVELWIDGETGRAPHICSITEM_H
