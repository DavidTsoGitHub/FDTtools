#ifndef CURVESEGMENT_H
#define CURVESEGMENT_H

#include <QEasingCurve>
#include <QGraphicsItem>
#include <QVariant>

class CurveWidget;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;

class CurveSegment : public QGraphicsItem
{
public:
    explicit CurveSegment(CurveWidget *curveWidget);
    void SetEndpoints(QPointF p1, QPointF p2);
    void SetEasingCurve(QEasingCurve curve);

private:
    CurveWidget* m_curveWidget;
    QPointF m_p1;
    QPointF m_p2;
    QEasingCurve m_curve;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;
};

#endif // CURVESEGMENT_H
