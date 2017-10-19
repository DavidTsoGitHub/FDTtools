#ifndef CURVEBEZIERSEGMENT_H
#define CURVEBEZIERSEGMENT_H

#include <QGraphicsItem>
#include <QPointF>

class CurveWidget;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;

class CurveBezierSegment : public QGraphicsItem
{
public:
    explicit CurveBezierSegment(CurveWidget *curveWidget);
    void SetP0(QPointF p);
    void SetP1(QPointF p);
    void SetC0(QPointF p);
    void SetC1(QPointF p);

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

private:
    CurveWidget* m_curveWidget;
    QPointF m_p0, m_p1, m_c0, m_c1;

};

#endif // CURVEBEZIERSEGMENT_H
