#ifndef CURVECONTROLMARKER_H
#define CURVECONTROLMARKER_H

#include <QColor>
#include <QGraphicsItem>
#include <QRectF>
#include <QVariant>

class CurveWidget;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;

class CurveControlMarker : public QGraphicsItem
{
public:
    CurveControlMarker(CurveWidget *curveWidget, QColor color1, QColor color2);

    void SetValidRect(QRectF rect);

private:
    CurveWidget* m_curveWidget;
    QRectF m_validRect;
    QColor m_color1;
    QColor m_color2;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // CURVECONTROLMARKER_H
