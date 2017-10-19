#ifndef ANIMATIONFRAMEBYFRAME_H
#define ANIMATIONFRAMEBYFRAME_H

#include "Animation.h"

#include <QList>

class AnimationFrameByFrame : public Animation
{
public:
    typedef QList<AnimationFrameByFrame*> List;

    AnimationFrameByFrame();
    explicit AnimationFrameByFrame(quint32 id);
    ~AnimationFrameByFrame();

    void AddImageId(quint32 imageId);
    void RemoveImageId(quint32 imageId);
    QList<quint32> ImageIds() const;

    void SetFrameDurationMs(quint32 duration);
    quint32 GetFrameDurationMs() const;

    void Clear();

    // Animation interface
    Animation::Type GetAnimationType() const override;

protected:
    // ProjectObject interface
    void AcceptVisitor(ProjectObjectVisitor& visitor) override;

private:
    QList<quint32> m_imageIds;
    quint32 m_frameDurationMs;

    QString m_componentName;
};

#endif // ANIMATIONFRAME_H
