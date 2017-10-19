#include "DynamicImageWidget.h"
#include "GCLSignalSinkMetaType.h"
#include "GraphicsItemBase.h"
#include "DynamicImageWidgetGraphicsItem.h"

const QString DynamicImageWidget::TypeName = "DynamicImage";

DynamicImageWidget::DynamicImageWidget(AnimationDefinition &animationDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicImage, 0, parent),
    m_animationDefinition(animationDefinition),
    m_animation1(0),
    m_animation2(0),
    m_animation1UseTriggers(false),
    m_animation2UseTriggers(false),
    m_animation1OnLoadBehavior(RUN_ONE_CYCLE),
    m_animation2OnLoadBehavior(RUN_ONE_CYCLE),
    m_animation1OnTriggerActiveBehavior(START_FROM_BEGINNING),
    m_animation2OnTriggerActiveBehavior(START_FROM_BEGINNING),
    m_animation1OnTriggerInactiveBehavior(STOP_AT_END_OF_CYCLE),
    m_animation2OnTriggerInactiveBehavior(STOP_AT_END_OF_CYCLE)
{
    SetUserDefinedName(DynamicImageWidget::TypeName);
    SetNbrOfLayersNeeded(1);
    SetBPPMode(BPP_16_565);
}

DynamicImageWidget::DynamicImageWidget(AnimationDefinition &animationDefinition, quint32 widgetID, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicImage, widgetID, parent),
    m_animationDefinition(animationDefinition),
    m_animation1(0),
    m_animation2(0),
    m_animation1UseTriggers(false),
    m_animation2UseTriggers(false),
    m_animation1OnLoadBehavior(RUN_ONE_CYCLE),
    m_animation2OnLoadBehavior(RUN_ONE_CYCLE),
    m_animation1OnTriggerActiveBehavior(START_FROM_BEGINNING),
    m_animation2OnTriggerActiveBehavior(START_FROM_BEGINNING),
    m_animation1OnTriggerInactiveBehavior(STOP_AT_END_OF_CYCLE),
    m_animation2OnTriggerInactiveBehavior(STOP_AT_END_OF_CYCLE)
{
    SetUserDefinedName(DynamicImageWidget::TypeName);
    SetNbrOfLayersNeeded(1);
    SetBPPMode(BPP_16_565);

    if (widgetID > 0)
    {
		AddSignalSink("ResourceIDToDisplay", "uint32", false, true);
    }
}

DynamicImageWidget::~DynamicImageWidget()
{
}

QString DynamicImageWidget::GetUIWidgetTypeName() const
{
    return DynamicImageWidget::TypeName;
}

GraphicsItemBase * DynamicImageWidget::GetGraphicsItem()
{
    QList<QRect> animPreviewRects;
    for(AnimationTranslation *anim : m_animationDefinition.TranslationAnimations())
    {
        if(anim->GetID() != m_animation1 && anim->GetID() != m_animation2)
            continue;

        const quint32 frameIntevalMs = 50;
        for(quint32 ms = 0; ms <= anim->GetLengthMs(); ms += frameIntevalMs)
        {
            QPointF pos = anim->GetRelativePosAtTime(ms);
            animPreviewRects.append(QRect(pos.toPoint(), QSize(GetWidth(), GetHeight())));
        }
    }

    return new DynamicImageWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), true, Qt::red, animPreviewRects);
}

QString DynamicImageWidget::LongDescription()
{
    return "This widget can be used to display different images depending on an input value. " \
           "It takes an uint32 as input which represents the image ID which can be found in the Image Manager. " \
           "It is also important that the size of the images used is the exact same size as the widget, otherwise " \
           "they won't be displayed.";
}

UIWidgetBase* DynamicImageWidget::CloneThis(quint32 widgetId)
{
    return new DynamicImageWidget(m_animationDefinition, widgetId);
}

QVariant DynamicImageWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case ImageIDSinkColumn:
        {
            GCLSignalSink* currentValueSink = GetSignalSink("ResourceIDToDisplay");
            GCLSignalSinkMetaType gclSignalSinkMetaType(currentValueSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        case Animation1Column:
        {
            return m_animation1;
        }
        case Animation1UseTriggers:
        {
            return m_animation1UseTriggers;
        }
        case Animation2Column:
        {
            return m_animation2;
        }
        case Animation2UseTriggers:
        {
            return m_animation2UseTriggers;
        }
        default:
            return "";
    }
}

QString DynamicImageWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case ImageIDSinkColumn:
            return "ImageID to Display";
        case Animation1Column:
            return "Animation 1";
        case Animation1UseTriggers:
            return "Use Triggers 1";
        case Animation2Column:
            return "Animation 2";
        case Animation2UseTriggers:
            return "Use Triggers 2";
        default:
            return "INVALID";
            break;
    }
}

void DynamicImageWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case Animation1Column:
            if (value.canConvert<uint>())
            {
                SetAnimationID(1, value.toUInt());
                if (value.toUInt() == 0)
                {
                    SetAnimationUseTriggers(1, false);
                }
            }
            break;
        case Animation1UseTriggers:
            if (value.canConvert<bool>())
            {
                SetAnimationUseTriggers(1, value.toBool());
            }
            break;
        case Animation2Column:
            if (value.canConvert<uint>())
            {
                SetAnimationID(2, value.toUInt());
                if (value.toUInt() == 0)
                {
                    SetAnimationUseTriggers(2, false);
                }
            }
            break;
        case Animation2UseTriggers:
            if (value.canConvert<bool>())
            {
                SetAnimationUseTriggers(2, value.toBool());
            }
        case ImageIDSinkColumn:
        default:
            ; // Do nothing
    }

}

int DynamicImageWidget::GetWidgetPropertyCount() const
{
    return dynamicImageWidgetColumnCount;
}

UIWidgetBase::DataType_t DynamicImageWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case ImageIDSinkColumn:
            return SignalSink;
        case Animation1Column:
            return DynamicImageAnimationComboBox;
        case Animation1UseTriggers:
            return AnimationCheckBox;
        case Animation2Column:
            return DynamicImageAnimationComboBox;
        case Animation2UseTriggers:
            return AnimationCheckBox;
        default:
            return Not_Available;
    }
}

QString DynamicImageWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case ImageIDSinkColumn:
            return "The sink which will receive the current ImageID to display";
        case Animation1Column:
            return "First animation";
        case Animation1UseTriggers:
            return "Use Triggers to activate the first animation";
        case Animation2Column:
            return "Second animation";
        case Animation2UseTriggers:
            return "Use Triggers to activate the second animation";
        default:
            return "INVALID";
    }
}

void DynamicImageWidget::SetAnimationID(quint8 animationNumber, quint32 animationID)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1 = animationID;
            if (m_animation1 != 0)
            {
                const Animation* animation = m_animationDefinition.GetAnimationByID(m_animation1);
                if (animation->GetAnimationType() == Animation::Type::FrameByFrame)
                {
                    GetSignalSink("ResourceIDToDisplay")->ConnectTo(animation->GetSignalSource("CurrentImageID"));
                }
            }
            break;
        case 2:
            m_animation2 = animationID;
            if (m_animation2 != 0)
            {
                const Animation* animation = m_animationDefinition.GetAnimationByID(m_animation2);
                if (animation->GetAnimationType() == Animation::Type::FrameByFrame)
                {
                    GetSignalSink("ResourceIDToDisplay")->ConnectTo(animation->GetSignalSource("CurrentImageID"));
                }
            }
            break;
        default:
            break;
    }
}

void DynamicImageWidget::SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1UseTriggers = useTriggers;
            if (useTriggers == false)
            {
                foreach (TransitionClause* trigger, m_animation1Triggers)
                {
                    m_animation1Triggers.removeAll(trigger);
                    delete trigger;
                }
            }
            break;
        case 2:
            m_animation2UseTriggers = useTriggers;
            if (useTriggers == false)
            {
                foreach (TransitionClause* trigger, m_animation2Triggers)
                {
                    m_animation2Triggers.removeAll(trigger);
                    delete trigger;
                }
            }
            break;
        default:
            break;
    }
}

const QList<TransitionClause *> &DynamicImageWidget::GetAnimationTriggers(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1Triggers;
            break;
        case 2:
            return m_animation2Triggers;
            break;
        default:
            break;
    }

    return m_emptyList;
}

void DynamicImageWidget::AddAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1Triggers.append(trigger);
            break;
        case 2:
            m_animation2Triggers.append(trigger);
            break;
        default:
            // Do nothing
            break;
    }
}

void DynamicImageWidget::RemoveAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1Triggers.removeAll(trigger);
            break;
        case 2:
            m_animation2Triggers.removeAll(trigger);
            break;
        default:
            // Do nothing
            break;
    }
}

quint32 DynamicImageWidget::GetAnimationID(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1;
        case 2:
            return m_animation2;
        default:
            return 0;
    }
}

bool DynamicImageWidget::GetAnimationUseTriggers(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1UseTriggers;
        case 2:
            return m_animation2UseTriggers;
        default:
            return false;
    }
}

void DynamicImageWidget::SetAnimationOnLoadBehavior(quint32 animationNumber, AnimationOnLoadBehavior behavior)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1OnLoadBehavior = behavior;
            break;
        case 2:
            m_animation2OnLoadBehavior = behavior;
            break;
        default:
            break;
    }
}

UIWidgetBase::AnimationOnLoadBehavior DynamicImageWidget::GetAnimationOnLoadBehavior(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1OnLoadBehavior;
            break;
        case 2:
            return m_animation2OnLoadBehavior;
            break;
        default:
            break;
    }
    return SHOW_DEFAULT;
}

void DynamicImageWidget::SetAnimationOnTriggerActiveBehavior(quint32 animationNumber, AnimationOnTriggerActiveBehavior behavior)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1OnTriggerActiveBehavior = behavior;
            break;
        case 2:
            m_animation1OnTriggerActiveBehavior = behavior;
            break;
        default:
            break;
    }
}

UIWidgetBase::AnimationOnTriggerActiveBehavior DynamicImageWidget::GetAnimationOnTriggerActiveBehavior(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1OnTriggerActiveBehavior;
            break;
        case 2:
            return m_animation2OnTriggerActiveBehavior;
            break;
        default:
            break;
    }
    return START_FROM_BEGINNING;
}

void DynamicImageWidget::SetAnimationOnTriggerInactiveBehavior(quint32 animationNumber, AnimationOnTriggerInactiveBehavior behavior)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1OnTriggerInactiveBehavior = behavior;
            break;
        case 2:
            m_animation2OnTriggerInactiveBehavior = behavior;
            break;
        default:
            break;
    }
}

UIWidgetBase::AnimationOnTriggerInactiveBehavior DynamicImageWidget::GetAnimationOnTriggerInactiveBehavior(quint32 animationNumber)
{
    switch (animationNumber)
    {
        case 1:
            return m_animation1OnTriggerInactiveBehavior;
            break;
        case 2:
            return m_animation2OnTriggerInactiveBehavior;
            break;
        default:
            break;
    }
    return STOP_AT_DEFAULT_FRAME;
}
