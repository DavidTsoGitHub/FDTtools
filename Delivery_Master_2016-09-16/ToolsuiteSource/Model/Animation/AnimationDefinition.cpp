#include "AnimationDefinition.h"

#include "Logger.h"
#include "ProjectObjectVisitor.h"

AnimationDefinition::AnimationDefinition(ProjectObject *parent) : ProjectObject(QString("Animations"), parent)
{
}

AnimationDefinition::~AnimationDefinition()
{
}

const AnimationFrameByFrame::List &AnimationDefinition::FrameByFrameAnimations() const
{
    return m_frameAnimations;
}

const AnimationOpacity::List &AnimationDefinition::OpacityAnimations() const
{
    return m_opacityAnimations;
}

const AnimationTranslation::List &AnimationDefinition::TranslationAnimations() const
{
    return m_translationAnimations;
}

AnimationFrameByFrame* AnimationDefinition::AddAnimationFrameByFrame(quint32 id)
{
    AnimationFrameByFrame* animation = new AnimationFrameByFrame(id);
    m_frameAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

AnimationFrameByFrame* AnimationDefinition::AddAnimationFrameByFrame()
{
    AnimationFrameByFrame* animation = new AnimationFrameByFrame();
    m_frameAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

AnimationOpacity *AnimationDefinition::AddAnimationOpacity(quint32 id)
{
    AnimationOpacity* animation = new AnimationOpacity(id);
    m_opacityAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

AnimationOpacity *AnimationDefinition::AddAnimationOpacity()
{
    AnimationOpacity* animation = new AnimationOpacity();
    m_opacityAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

AnimationTranslation *AnimationDefinition::AddAnimationTranslation(quint32 id)
{
    AnimationTranslation* animation = new AnimationTranslation(id);
    m_translationAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

AnimationTranslation *AnimationDefinition::AddAnimationTranslation()
{
    AnimationTranslation* animation = new AnimationTranslation();
    m_translationAnimations.append(animation);
    animation->SetParent(this);
    NotifyPropertyUpdated();
    return animation;
}

void AnimationDefinition::RemoveFrameByFrameAnimation(AnimationFrameByFrame *anim)
{
    m_frameAnimations.removeAll(anim);
    anim->setParent(NULL);
    delete anim;
    NotifyPropertyUpdated();
}

void AnimationDefinition::RemoveOpacityAnimation(AnimationOpacity *anim)
{
    m_opacityAnimations.removeAll(anim);
    anim->setParent(NULL);
    delete anim;
    NotifyPropertyUpdated();
}

void AnimationDefinition::RemoveTranslationAnimation(AnimationTranslation *anim)
{
    m_translationAnimations.removeAll(anim);
    anim->setParent(NULL);
    delete anim;
    NotifyPropertyUpdated();
}

const Animation *AnimationDefinition::GetAnimationByID(quint32 id)
{
    foreach (const AnimationFrameByFrame* animation, FrameByFrameAnimations())
    {
        if (animation->GetID() == id)
        {
            return animation;
        }
    }

    foreach (const AnimationOpacity* animation, OpacityAnimations())
    {
        if (animation->GetID() == id)
        {
            return animation;
        }
    }

    foreach (const AnimationTranslation* animation, TranslationAnimations())
    {
        if (animation->GetID() == id)
        {
            return animation;
        }
    }

    return NULL;
}

void AnimationDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitAnimationDefinition(this);
}
