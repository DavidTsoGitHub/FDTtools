#include "OpacityCurveWidget.h"

#include "AnimationOpacity.h"
#include "Logger.h"

OpacityCurveWidget::OpacityCurveWidget(AnimationOpacity *anim, QWidget *parent)
    : CurveWidget(parent),
    m_animation(anim),
    m_xScale(0.2),
    m_yScale(2)
{

    // Control markers
    AddControlMarker(QPointF(), START_OPACITY_MARKER);
    AddControlMarker(QPointF(), STOP_OPACITY_MARKER);

    // X-axis
    AddAxis(QPointF(), QPointF(), XAXIS);
    AddLabel(QPointF(), "Time [ms]", Placement::Right, Qt::black, XAXIS_LABEL);

    // Y-axis
    AddAxis({0, 0}, {0, 110*-m_yScale}, YAXIS);
    AddLabel({0, 115*-m_yScale}, "Opacity [%]", Placement::Above, Qt::black, YAXIS_LABEL);
    SetAxisTicks(100*m_yScale, 10*m_yScale, YAXIS);

    // Curve
    AddCurveSegment(QPointF(), QPointF(), QEasingCurve(QEasingCurve::Type::InOutCubic), CURVE_SEGMENT);

    connect(m_animation, &ProjectObject::PropertyUpdated, this, &OpacityCurveWidget::setCurveFromAnimation);
    setCurveFromAnimation();
}

void OpacityCurveWidget::itemMoving(int id)
{
    if(id == START_OPACITY_MARKER || id == STOP_OPACITY_MARKER)
    {
        SetCurveSegmentEndPoints(GetControlMarkerPos(START_OPACITY_MARKER),
                                 GetControlMarkerPos(STOP_OPACITY_MARKER),
                                 CURVE_SEGMENT);
    }
}

void OpacityCurveWidget::itemMoved(int id)
{
    if(id == START_OPACITY_MARKER)
    {
        m_animation->SetStartOpacityPercent(qRound(GetControlMarkerPos(START_OPACITY_MARKER).y() / -m_yScale));
    }
    else if(id == STOP_OPACITY_MARKER)
    {
        auto pos = GetControlMarkerPos(STOP_OPACITY_MARKER);
        m_animation->SetLengthMs(qRound(pos.x() / m_xScale));
        m_animation->SetStopOpacityPercent(qRound(pos.y() / -m_yScale));
    }
}

void OpacityCurveWidget::setCurveFromAnimation()
{
    QPointF pt1 = QPointF(0, -(qreal)m_animation->GetStartOpacityPercent()*m_yScale);
    QPointF pt2 = QPointF(m_animation->GetLengthMs()*m_xScale,
                          -(qreal)m_animation->GetStopOpacityPercent()*m_yScale);

    SetControlMarkerPos(pt1, START_OPACITY_MARKER);
    SetControlMarkerPos(pt2, STOP_OPACITY_MARKER);
    SetCurveSegmentEndPoints(GetControlMarkerPos(START_OPACITY_MARKER),
                             GetControlMarkerPos(STOP_OPACITY_MARKER),
                             CURVE_SEGMENT);

    const qreal maxOpacity = 100;
    const qreal maxLengthMs = 10000;
    auto startMarkerValidRect = QRectF(0, maxOpacity*-m_yScale, 0, maxOpacity*m_yScale);
    SetControlMarkerValidRect(startMarkerValidRect, START_OPACITY_MARKER);
    auto stopMarkerValidRect = QRectF(0, maxOpacity*-m_yScale, maxLengthMs*m_xScale, maxOpacity*m_yScale);
    SetControlMarkerValidRect(stopMarkerValidRect, STOP_OPACITY_MARKER);

    const quint32 minXAxisLengthMs = 2200;
    qreal xAxisLengthMs = qMax(minXAxisLengthMs, m_animation->GetLengthMs() + 200);
    auto xAxisEndPos = QPointF(xAxisLengthMs*m_xScale, 0);
    SetAxisEndPoints({0, 0}, QPointF(xAxisLengthMs*m_xScale, 0), XAXIS);
    SetLabelPos(xAxisEndPos + QPointF(10, 0), XAXIS_LABEL);
    SetAxisTicks(1000*m_xScale, 100*m_xScale, XAXIS);

    SetCurveSegmentEasingCurve(m_animation->GetCurveType(), CURVE_SEGMENT);
}
