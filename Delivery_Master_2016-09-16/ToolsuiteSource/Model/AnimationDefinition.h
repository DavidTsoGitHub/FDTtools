#ifndef ANIMATIONDEFINITION
#define ANIMATIONDEFINITION

#include "AnimationFrameByFrame.h"
#include "AnimationOpacity.h"
#include "AnimationTranslation.h"
#include "ProjectObject.h"

class AnimationDefinition : public ProjectObject
{
    Q_OBJECT

public:
    explicit AnimationDefinition(ProjectObject* parent = 0);
    ~AnimationDefinition();

    const AnimationFrameByFrame::List& FrameByFrameAnimations(void) const;
    const AnimationOpacity::List& OpacityAnimations(void) const ;
    const AnimationTranslation::List& TranslationAnimations(void) const;

    AnimationFrameByFrame *AddAnimationFrameByFrame(quint32 id);
    AnimationFrameByFrame *AddAnimationFrameByFrame();
    AnimationOpacity *AddAnimationOpacity(quint32 id);
    AnimationOpacity *AddAnimationOpacity();
    AnimationTranslation *AddAnimationTranslation(quint32 id);
    AnimationTranslation *AddAnimationTranslation();

    void RemoveFrameByFrameAnimation(AnimationFrameByFrame* anim);
    void RemoveOpacityAnimation(AnimationOpacity* anim);
    void RemoveTranslationAnimation(AnimationTranslation* anim);

    const Animation *GetAnimationByID(quint32 id);

signals:
    void signal_AnimationRemoved(quint32 animationID);

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor &visitor);

private:
    AnimationFrameByFrame::List m_frameAnimations;
    AnimationOpacity::List m_opacityAnimations;
    AnimationTranslation::List m_translationAnimations;
};

#endif // ANIMATIONDEFINITION

