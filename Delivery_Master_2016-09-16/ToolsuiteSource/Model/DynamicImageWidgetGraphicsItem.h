#ifndef DYNAMICIMAGEWIDGETGRAPHICSITEM_H
#define DYNAMICIMAGEWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"

#include <QList>
#include <QRect>

class DynamicImageWidgetGraphicsItem : public GraphicsItemBase
{
public:
    DynamicImageWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor,
                                   QColor fgColor, const QList<QRect>& animPreviewRects);

    // QGraphicsItem interface
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QList<QRect> m_animPreviewRects;
};

#endif // DYNAMICIMAGEWIDGETGRAPHICSITEM_H
