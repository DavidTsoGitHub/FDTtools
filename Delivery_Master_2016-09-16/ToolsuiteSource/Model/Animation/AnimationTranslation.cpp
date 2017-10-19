#include "AnimationTranslation.h"
#include "ProjectObjectVisitor.h"

AnimationTranslation::AnimationTranslation() : Animation(),
    m_lengthMs(1000),
    m_path(Path::Linear),
    m_start(QPoint(0, -100)),
    m_stop(QPoint(0, 0)),
    m_bezierC0(QPoint(0, -50)),
    m_bezierC1(QPoint(-50, 0)),
    m_easingCurveType(QEasingCurve::InOutExpo)
{
    SetName("TranslationAnimation");
    SetID(Animation::GetNewID());
    //m_componentName = "AnimationManager_Translation_" + QString::number(GetID());
    //AddSignalSource("CurrentImageID", m_componentName, "uint32");
}

AnimationTranslation::AnimationTranslation(quint32 id) : Animation(),
    m_lengthMs(1000),
    m_path(Path::Linear),
    m_start(QPoint(0,-100)),
    m_stop(QPoint(0, 0)),
    m_bezierC0(QPoint(0, -50)),
    m_bezierC1(QPoint(-50, 0)),
    m_easingCurveType(QEasingCurve::OutQuad)
{
    SetName("TranslationAnimation");
    SetID(id);
    //m_componentName = "AnimationManager_Translation_" + QString::number(GetID());
    //AddSignalSource("CurrentImageID", m_componentName, "uint32");
}

AnimationTranslation::~AnimationTranslation()
{
    //RemoveSignalSource("CurrentImageID");
}

QPointF AnimationTranslation::GetRelativePosAtTime(quint32 ms)
{
    // Time along path [0, 1]
    qreal timeFraction = (qreal)ms/(qreal)m_lengthMs;
    timeFraction = qMin(1.0, qMax(0.0, timeFraction));

    // Progress along path, 0 is start, 1 is stop. Values can
    // be < 0 and > 1 when there is overshoot
    QEasingCurve easingCurve(m_easingCurveType);
    qreal progress = easingCurve.valueForProgress(timeFraction);

    QPainterPath path;
    path.moveTo(m_start);

    // Cubic bezier curve
    if(m_path == Path::Bezier)
    {
        path.cubicTo(m_bezierC0, m_bezierC1, m_stop);

        // Overshoot from start point, calculate tangent to
        // curve at end point using the bezier c0 control point
        if(progress < 0 )
        {
            QPainterPath startOvershoot;
            QLineF startC0(m_start, 2*m_start-m_bezierC0);
            startC0.setLength(path.length());
            startOvershoot.moveTo(m_start);
            startOvershoot.lineTo(startC0.p2());
            return startOvershoot.pointAtPercent(qAbs(progress));
        }
        // Overshoot from stop point, calculate tangent to
        // curve at stop point using the bezier c1 control point
        else if(progress > 1)
        {
            QPainterPath stopOvershoot;
            QLineF stopC1(m_stop, 2*m_stop-m_bezierC1);
            stopC1.setLength(path.length());
            stopOvershoot.moveTo(m_stop);
            stopOvershoot.lineTo(stopC1.p2());
            return stopOvershoot.pointAtPercent(progress - 1);
        }

        // Normal case, no overshoot, use path
        return path.pointAtPercent(progress);
    }

    // Simple line segment
    else
    {
        path.lineTo(m_stop);

        // Overshoot from start point, calculate extension of line segment
        if(progress < 0 )
        {
            QPainterPath startOvershoot;
            QLineF startC0(m_start, 2*m_start-m_stop);
            startOvershoot.moveTo(m_start);
            startOvershoot.lineTo(startC0.p2());
            return startOvershoot.pointAtPercent(qAbs(progress));
        }
        // Overshoot from stop point, calculate extension of line segment
        else if(progress > 1)
        {
            QPainterPath stopOvershoot;
            QLineF stopC0(m_stop, 2*m_stop-m_start);
            stopOvershoot.moveTo(m_stop);
            stopOvershoot.lineTo(stopC0.p2());
            return stopOvershoot.pointAtPercent(progress - 1);
        }

        // Normal case, no overshoot, use path
        return path.pointAtPercent(progress);
    }
}

void AnimationTranslation::SetLengthMs(quint32 ms)
{
    m_lengthMs = ms;
    NotifyPropertyUpdated();
}

quint32 AnimationTranslation::GetLengthMs() const
{
    return m_lengthMs;
}

void AnimationTranslation::SetPathType(AnimationTranslation::Path path)
{
    m_path = path;
    NotifyPropertyUpdated();
}

AnimationTranslation::Path AnimationTranslation::GetPathType() const
{
    return m_path;
}

void AnimationTranslation::SetStartPoint(QPoint start)
{
    m_start = start;
    NotifyPropertyUpdated();
}

QPoint AnimationTranslation::GetStartPoint() const
{
    return m_start;
}

void AnimationTranslation::SetStopPoint(QPoint stop)
{
    m_stop = stop;
    NotifyPropertyUpdated();
}

QPoint AnimationTranslation::GetStopPoint() const
{
    return m_stop;
}

void AnimationTranslation::SetBezierC0(QPoint c0)
{
    m_bezierC0 = c0;
    NotifyPropertyUpdated();
}

QPoint AnimationTranslation::GetBezierC0() const
{
    return m_bezierC0;
}

void AnimationTranslation::SetBezierC1(QPoint c1)
{
    m_bezierC1 = c1;
    NotifyPropertyUpdated();
}

QPoint AnimationTranslation::GetBezierC1() const
{
    return m_bezierC1;
}

void AnimationTranslation::SetEasingCurveType(QEasingCurve::Type type)
{
    m_easingCurveType = type;
    NotifyPropertyUpdated();
}

QEasingCurve::Type AnimationTranslation::GetEasingCurveType() const
{
    return m_easingCurveType;
}

Animation::Type AnimationTranslation::GetAnimationType() const
{
    return Animation::Type::Translation;
}

const QString AnimationTranslation::GetComponentName() const
{
    return "";
}

void AnimationTranslation::AcceptVisitor(ProjectObjectVisitor &visitor)
{
    visitor.VisitAnimationTranslation(this);
}
