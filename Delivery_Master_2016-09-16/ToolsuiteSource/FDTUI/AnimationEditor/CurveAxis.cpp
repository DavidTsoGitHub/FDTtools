#include "CurveAxis.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

#include "CurveWidget.h"

CurveAxis::CurveAxis(QPointF p1, QPointF p2, CurveWidget *curveWidget)
    : m_curveWidget(curveWidget), m_p1(p1), m_p2(p2), m_minorTick(0), m_majorTick(0)
{
    setCacheMode(DeviceCoordinateCache);
    setZValue(0);
}

void CurveAxis::SetEndpoints(QPointF p1, QPointF p2)
{
    m_p1 = p1;
    m_p2 = p2;
    update();
}

void CurveAxis::SetMinorTickInterval(qreal interval)
{
    m_minorTick = interval;
}

void CurveAxis::SetMajorTickInterval(qreal interval)
{
    m_majorTick = interval;
}

QRectF CurveAxis::boundingRect() const
{
    const qreal extra = (qreal)ARROW_SIZE / 2.0;
    return QRectF(m_p1, QSizeF(m_p2.x() - m_p1.x(), m_p2.y() - m_p1.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void CurveAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Draw axis line
    QLineF line(m_p1, m_p2);
    painter->setPen(QPen(Qt::darkGray, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw minor ticks
    if(m_minorTick > 0)
    {
        const qreal minorTickLength = 2;
        QLineF tickStart = line;
        tickStart.setLength(m_minorTick);
        while(tickStart.length() < line.length())
        {
            QLineF tickLine = line.normalVector().translated(tickStart.p2());
            tickLine.setLength(minorTickLength);
            painter->drawLine(tickLine);
            tickStart.setLength(tickStart.length() + m_minorTick);
        }
    }

    // Draw major ticks
    if(m_majorTick > 0)
    {
        const qreal majorTickLength = 4;
        QLineF tickStart = line;
        tickStart.setLength(m_majorTick);
        while(tickStart.length() < line.length())
        {
            QLineF tickLine = line.normalVector().translated(tickStart.p2());
            tickLine.setLength(majorTickLength);
            painter->drawLine(tickLine);
            tickStart.setLength(tickStart.length() + m_majorTick);
        }
    }

    // Draw arrow head
    const double pi = 3.14159;
    double angle = qAcos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = 2.0 * pi - angle;

    const qreal arrowSize = (qreal)ARROW_SIZE;
    QPointF arrowPt1 = m_p2 + QPointF(qSin(angle - pi / 3) * arrowSize,
                                              qCos(angle - pi / 3) * arrowSize);
    QPointF arrowPt2 = m_p2 + QPointF(qSin(angle - pi + pi / 3) * arrowSize,
                                              qCos(angle - pi + pi / 3) * arrowSize);

    painter->setBrush(Qt::darkGray);
    painter->drawPolygon(QPolygonF() << m_p2 << arrowPt1 << arrowPt2);
}

int CurveAxis::type() const
{
    return UserType + CurveWidget::GRAPHICS_ITEM_ID_AXIS;
}
