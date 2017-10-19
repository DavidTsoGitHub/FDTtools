#ifndef DYNAMICIMAGEWIDGET_H
#define DYNAMICIMAGEWIDGET_H

#include "UIWidgetBase.h"
#include "Animation.h"
#include "AnimationDefinition.h"
#include "TransitionClause.h"

class DynamicImageWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit DynamicImageWidget(AnimationDefinition& animationDefinition, ProjectObject* parent = 0);
    explicit DynamicImageWidget(AnimationDefinition& animationDefinition, quint32 widgetID, ProjectObject* parent = 0);
    ~DynamicImageWidget();

    void SetAnimationID(quint8 animationNumber, quint32 animationID);
    void SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers);

private:
    typedef enum
    {
        ImageIDSinkColumn,
        Animation1Column,
        Animation1UseTriggers,
        Animation2Column,
        Animation2UseTriggers,
        dynamicImageWidgetColumnCount
    } dynamicImageWidgetColumns_t;

    AnimationDefinition& m_animationDefinition;
    quint32 m_animation1;
    quint32 m_animation2;
    bool m_animation1UseTriggers;
    bool m_animation2UseTriggers;
    QList<TransitionClause*> m_animation1Triggers;
    QList<TransitionClause*> m_animation2Triggers;
    QList<TransitionClause*> m_emptyList;
    AnimationOnLoadBehavior m_animation1OnLoadBehavior;
    AnimationOnLoadBehavior m_animation2OnLoadBehavior;
    AnimationOnTriggerActiveBehavior m_animation1OnTriggerActiveBehavior;
    AnimationOnTriggerActiveBehavior m_animation2OnTriggerActiveBehavior;
    AnimationOnTriggerInactiveBehavior m_animation1OnTriggerInactiveBehavior;
    AnimationOnTriggerInactiveBehavior m_animation2OnTriggerInactiveBehavior;

public:
    QString GetUIWidgetTypeName() const;
    GraphicsItemBase *GetGraphicsItem();
    QString LongDescription();

protected:
    UIWidgetBase* CloneThis(quint32 widgetId);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;

    // UIWidgetBase interface
public:
    quint32 GetAnimationID(quint32 animationNumber);
    bool GetAnimationUseTriggers(quint32 animationNumber);
    const QList<TransitionClause *> &GetAnimationTriggers(quint32 animationNumber);
    void AddAnimationTrigger(quint32 animationNumber, TransitionClause *trigger);
    void RemoveAnimationTrigger(quint32 animationNumber, TransitionClause *trigger);

    void SetAnimationOnLoadBehavior(quint32 animationNumber, AnimationOnLoadBehavior behavior);
    AnimationOnLoadBehavior GetAnimationOnLoadBehavior(quint32 animationNumber);
    UIWidgetBase::AnimationOnTriggerActiveBehavior GetAnimationOnTriggerActiveBehavior(quint32 animationNumber);
    void SetAnimationOnTriggerActiveBehavior(quint32 animationNumber, AnimationOnTriggerActiveBehavior behavior);
    void SetAnimationOnTriggerInactiveBehavior(quint32 animationNumber, AnimationOnTriggerInactiveBehavior behavior);
    AnimationOnTriggerInactiveBehavior GetAnimationOnTriggerInactiveBehavior(quint32 animationNumber);
};

#endif // DYNAMICIMAGEWIDGET_H
