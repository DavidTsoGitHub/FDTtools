#ifndef IMAGEWIDGET
#define IMAGEWIDGET

#include "UIWidgetBase.h"
#include "ImageWidgetGraphicsItem.h"
#include "LayoutDefinition.h"
#include "AnimationDefinition.h"
#include "ImageDefinition.h"
#include "Image.h"

class ImageWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit ImageWidget(LayoutDefinition& layoutDefinition, AnimationDefinition& animationDefinition, ImageDefinition& imageDefinition, ProjectObject* parent = 0);
    ImageWidget(LayoutDefinition& layoutDefinition, AnimationDefinition& animationDefinition, ImageDefinition& imageDefinition, quint32 uiWidgetID, ProjectObject* parent = 0);

    void SetImageID(quint32 id);
    quint32 GetImageID() const;
    const QString GetFullPath(void) const;

    void SetAnimationID(quint8 animationNumber, quint32 animationID);
    void SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers);

private:
    typedef enum
    {
        ImageIDColumn,
        Animation1Column,
        Animation1UseTriggers,
        Animation2Column,
        Animation2UseTriggers,
        ImageWidgetColumnCount
    } imageWidgetColumnIndex_t;

    quint32 m_imageID;
    ImageWidgetGraphicsItem* m_imageWidgetGraphicsItem;
    LayoutDefinition& m_layoutDefinition;
    AnimationDefinition& m_animationDefinition;
    ImageDefinition& m_imageDefinition;
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

    // UIWidgetBase interface
public:
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QString GetUIWidgetTypeName() const;

protected:
    UIWidgetBase* CloneThis(quint32 widgetID);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    QString LongDescription();

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


    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList* errorList);
};

#endif // IMAGEWIDGET
