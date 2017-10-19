#include "CurveSegment.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "CurveWidget.h"

CurveSegment::CurveSegment(CurveWidget *curveWidget)
    : m_curveWidget(curveWidget)
{
    setZValue(1);
}

void CurveSegment::SetEndpoints(QPointF p1, QPointF p2)
{
    m_p1 = p1;
    m_p2 = p2;
    update();
}

void CurveSegment::SetEasingCurve(QEasingCurve curve)
{
    m_curve = curve;
    update();
}

QRectF CurveSegment::boundingRect() const
{
    auto rect = QRectF(m_p1, m_p2).normalized();
    auto mx = rect.width() / 2;
    auto my = rect.height() / 2;
    return rect.marginsAdded(QMarginsF(mx, my, mx, my));
}

void CurveSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath curvePath;
    curvePath.moveTo(m_p1);

    qreal curveScaleX = m_p2.x() - m_p1.x();
    qreal curveScaleY = m_p2.y() - m_p1.y();
    if (qFuzzyCompare(curveScaleX, qreal(0.)))
        return;

    for (qreal t = 0; t <= 1.0; t+=1.0/qAbs(curveScaleX)) {
         QPoint to;
         to.setX(m_p1.x() + curveScaleX * t);
         to.setY(m_p1.y() + curveScaleY * m_curve.valueForProgress(t));
         curvePath.lineTo(to);
    }

    painter->strokePath(curvePath, QColor(Qt::darkGray));
}

int CurveSegment::type() const
{
    return UserType + CurveWidget::GRAPHICS_ITEM_ID_SEGMENT;
}

