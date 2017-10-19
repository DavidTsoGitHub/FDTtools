#include "CurveBezierSegment.h"

#include <QLineF>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionGraphicsItem>

#include "CurveWidget.h"

CurveBezierSegment::CurveBezierSegment(CurveWidget *curveWidget)
    : m_curveWidget(curveWidget)
{
    setZValue(1);
}

void CurveBezierSegment::SetP0(QPointF p)
{
    m_p0 = p;
    update();
}

void CurveBezierSegment::SetP1(QPointF p)
{
    m_p1 = p;
    update();
}

void CurveBezierSegment::SetC0(QPointF p)
{
    m_c0 = p;
    update();
}

void CurveBezierSegment::SetC1(QPointF p)
{
    m_c1 = p;
    update();
}

QRectF CurveBezierSegment::boundingRect() const
{
    QPainterPath path;
    path.moveTo(m_p0);
    path.cubicTo(m_c0, m_c1, m_p1);
    return path.controlPointRect();
}

void CurveBezierSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Dashed control-point lines
    QLineF p0c0(m_p0, m_c0);
    QLineF p1c1(m_p1, m_c1);
    painter->setPen(Qt::DashLine);
    painter->drawLine(p0c0);
    painter->drawLine(p1c1);
    painter->setPen(Qt::SolidLine);

    // Bezier path
    QPainterPath path;
    path.moveTo(m_p0);
    path.cubicTo(m_c0, m_c1, m_p1);
    painter->strokePath(path, QColor(Qt::black));
}

int CurveBezierSegment::type() const
{
    return UserType + CurveWidget::GRAPHICS_ITEM_ID_BEZIER_SEGMENT;
}
