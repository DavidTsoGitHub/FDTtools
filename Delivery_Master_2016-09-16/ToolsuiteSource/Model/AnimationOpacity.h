#ifndef ANIMATIONOPACITY_H
#define ANIMATIONOPACITY_H

#include "Animation.h"

#include <QEasingCurve>

class AnimationOpacity : public Animation
{
public:
    typedef QList<AnimationOpacity*> List;

    AnimationOpacity();
    explicit AnimationOpacity(quint32 id);
    ~AnimationOpacity();

    qreal GetOpacityAtTime(quint32 ms);

    void SetLengthMs(quint32 ms);
    quint32 GetLengthMs() const;

    void SetStartOpacityPercent(quint32 percent);
    quint32 GetStartOpacityPercent() const;

    void SetStopOpacityPercent(quint32 percent);
    quint32 GetStopOpacityPercent() const;

    void SetCurveType(QEasingCurve::Type type);
    QEasingCurve::Type GetCurveType() const;

    // Animation interface
    Animation::Type GetAnimationType() const override;

    // IGCLSignalSource interface
    const QString GetComponentName() const override;

protected:
    // ProjectObject interface
    void AcceptVisitor(ProjectObjectVisitor& visitor) override;

private:
    quint32 m_lengthMs;
    quint32 m_startOpacity;
    quint32 m_stopOpacity;
    QEasingCurve::Type m_curveType;
};

#endif // ANIMATIONOPACITY_H
