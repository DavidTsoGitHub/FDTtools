#ifndef ANIMATIONTRANSLATION_H
#define ANIMATIONTRANSLATION_H

#include "Animation.h"

#include <QEasingCurve>
#include <QPoint>

class AnimationTranslation : public Animation
{
public:
    typedef QList<AnimationTranslation*> List;

    enum class Path { Linear, Bezier };

    AnimationTranslation();
    explicit AnimationTranslation(quint32 id);
    ~AnimationTranslation();

    QPointF GetRelativePosAtTime(quint32 ms);

    void SetLengthMs(quint32 ms);
    quint32 GetLengthMs() const;

    void SetPathType(Path path);
    Path GetPathType() const;

    void SetStartPoint(QPoint start);
    QPoint GetStartPoint() const;

    void SetStopPoint(QPoint stop);
    QPoint GetStopPoint() const;

    void SetBezierC0(QPoint c0);
    QPoint GetBezierC0() const;

    void SetBezierC1(QPoint c1);
    QPoint GetBezierC1() const;

    void SetEasingCurveType(QEasingCurve::Type type);
    QEasingCurve::Type GetEasingCurveType() const;

    // Animation interface
    Animation::Type GetAnimationType() const override;

    // IGCLSignalSource interface
    const QString GetComponentName() const override;

protected:
    // ProjectObject interface
    void AcceptVisitor(ProjectObjectVisitor& visitor) override;

private:
    quint32 m_lengthMs;
    Path m_path;
    QPoint m_start;
    QPoint m_stop;
    QPoint m_bezierC0;
    QPoint m_bezierC1;
    QEasingCurve::Type m_easingCurveType;

    QString m_componentName;
};

#endif // ANIMATIONTRANSLATION_H
