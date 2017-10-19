#ifndef CURVEAXIS_H
#define CURVEAXIS_H

#include <QGraphicsItem>
#include <QVariant>

class CurveWidget;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;

class CurveAxis : public QGraphicsItem
{
public:
    CurveAxis(QPointF p1, QPointF p2, CurveWidget *curveWidget);
    void SetEndpoints(QPointF p1, QPointF p2);
    void SetMinorTickInterval(qreal interval);
    void SetMajorTickInterval(qreal interval);

private:
    static const int ARROW_SIZE = 10;

    CurveWidget* m_curveWidget;
    QPointF m_p1;
    QPointF m_p2;
    qreal m_minorTick;
    qreal m_majorTick;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;


};

#endif // CURVEAXIS_H
