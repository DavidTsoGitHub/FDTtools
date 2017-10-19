#include "AnimationOpacity.h"
#include "ProjectObjectVisitor.h"

AnimationOpacity::AnimationOpacity() : Animation(),
    m_lengthMs(500),
    m_startOpacity(0),
    m_stopOpacity(100),
    m_curveType(QEasingCurve::Type::Linear)
{
    SetName("OpacityAnimation");
    SetID(Animation::GetNewID());
    //AddSignalSource("CurrentImageID", "uint32");
}

AnimationOpacity::AnimationOpacity(quint32 id) : Animation(),
    m_lengthMs(500),
    m_startOpacity(0),
    m_stopOpacity(100),
    m_curveType(QEasingCurve::Type::Linear)
{
    SetName("OpacityAnimation");
    SetID(id);
    //AddSignalSource("CurrentImageID", m_componentName, "uint32");
}

AnimationOpacity::~AnimationOpacity()
{
    //RemoveSignalSource("CurrentImageID");
}

qreal AnimationOpacity::GetOpacityAtTime(quint32 ms)
{
    QEasingCurve curve(m_curveType);
    qreal progress = (qreal)ms/(qreal)m_lengthMs;
    progress = qMin(1.0, qMax(0.0, progress));
    qreal start = (qreal)m_startOpacity;

    qreal value = 0;
    if (m_startOpacity > m_stopOpacity)
    {
        qreal diff = (qreal)(m_startOpacity - m_stopOpacity)/100.0;
        value = start - diff*curve.valueForProgress(progress)*100.0;
    }
    else
    {
        qreal diff = (qreal)(m_stopOpacity - m_startOpacity)/100.0;
        value = start + diff*curve.valueForProgress(progress)*100.0;
    }

    value = qMin(100.0, qMax(0.0, value));
    return value;
}

void AnimationOpacity::SetLengthMs(quint32 ms)
{
    m_lengthMs = ms;
    NotifyPropertyUpdated();
}

quint32 AnimationOpacity::GetLengthMs() const
{
    return m_lengthMs;
}

void AnimationOpacity::SetStartOpacityPercent(quint32 percent)
{
    m_startOpacity = percent;
    NotifyPropertyUpdated();
}

quint32 AnimationOpacity::GetStartOpacityPercent() const
{
    return m_startOpacity;
}

void AnimationOpacity::SetStopOpacityPercent(quint32 opacity)
{
    m_stopOpacity = opacity;
    NotifyPropertyUpdated();
}

quint32 AnimationOpacity::GetStopOpacityPercent() const
{
    return m_stopOpacity;
}

void AnimationOpacity::SetCurveType(QEasingCurve::Type type)
{
    m_curveType = type;
    NotifyPropertyUpdated();
}

QEasingCurve::Type AnimationOpacity::GetCurveType() const
{
    return m_curveType;
}

Animation::Type AnimationOpacity::GetAnimationType() const
{
    return Animation::Type::Opacity;
}

const QString AnimationOpacity::GetComponentName() const
{
    return "";
}

void AnimationOpacity::AcceptVisitor(ProjectObjectVisitor &visitor)
{
    visitor.VisitAnimationOpacity(this);
}

