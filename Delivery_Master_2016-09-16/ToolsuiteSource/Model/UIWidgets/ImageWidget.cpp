#include "ImageWidget.h"

#include <Exceptions/ObjectNotFoundException.h>
#include "GCLSignalSinkMetaType.h"
#include "Project.h"

#include <QFileInfo>
#include <limits>

const QString ImageWidget::TypeName = "Image";

ImageWidget::ImageWidget(LayoutDefinition& layoutDefinition, AnimationDefinition& animationDefinition, ImageDefinition &imageDefinition, ProjectObject *parent) :
    UIWidgetBase(UIWidget_Type_Image, 0, parent),
    m_imageID(std::numeric_limits<quint32>::max()),
    m_layoutDefinition(layoutDefinition),
    m_animationDefinition(animationDefinition),
    m_imageDefinition(imageDefinition),
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
    SetUserDefinedName(ImageWidget::TypeName);
    SetNbrOfLayersNeeded(1);
    SetBPPMode(BPP_16_565);
}

ImageWidget::ImageWidget(LayoutDefinition& layoutDefinition, AnimationDefinition& animationDefinition, ImageDefinition &imageDefinition, quint32 uiWidgetID, ProjectObject *parent) :
    UIWidgetBase(UIWidget_Type_Image, uiWidgetID, parent),
    m_imageID(std::numeric_limits<quint32>::max()),
    m_layoutDefinition(layoutDefinition),
    m_animationDefinition(animationDefinition),
    m_imageDefinition(imageDefinition),
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
    SetUserDefinedName(ImageWidget::TypeName);
    SetNbrOfLayersNeeded(1);
    SetBPPMode(BPP_16_565);

}

void ImageWidget::SetImageID(quint32 id)
{
    m_imageID = id;
}

quint32 ImageWidget::GetImageID() const
{
    return m_imageID;
}

UIWidgetBase* ImageWidget::CloneThis(quint32 widgetID)
{
    return new ImageWidget(m_layoutDefinition, m_animationDefinition, m_imageDefinition, widgetID);
}

GraphicsItemBase* ImageWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    Q_UNUSED(drawChromaKeyColor);

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

    return new ImageWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), /*drawChromaKeyColor*/ true, m_layoutDefinition.GetStartChromaKeyColor(), m_imageDefinition.GetResource(m_imageID), animPreviewRects);
}

QString ImageWidget::GetUIWidgetTypeName() const
{
    return ImageWidget::TypeName;
}

QVariant ImageWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case ImageIDColumn:
            return m_imageID;
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

QString ImageWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case ImageIDColumn:
            return "Image";
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
    }
}

void ImageWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case ImageIDColumn:
            if (value.canConvert<quint32>())
            {
                SetImageID(value.toUInt());
            }
            return;
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
        default:
            return;
    }
}

UIWidgetBase::DataType_t ImageWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case ImageIDColumn:
            return ImagesComboBox;
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

int ImageWidget::GetWidgetPropertyCount() const
{
    return ImageWidgetColumnCount;
}

QString ImageWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case ImageIDColumn:
            return "The image to display in this widget";
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

QString ImageWidget::LongDescription()
{
    return "This widget can be used to display a static Image. The image must " \
           "first be added in the Image Manager before it can be connected to this widget.";
}

void ImageWidget::SetAnimationID(quint8 animationNumber, quint32 animationID)
{
    switch (animationNumber)
    {
        case 1:
            m_animation1 = animationID;
            break;
        case 2:
            m_animation2 = animationID;
            break;
        default:
            break;
    }
}

void ImageWidget::SetAnimationUseTriggers(quint8 animationNumber, bool useTriggers)
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

const QList<TransitionClause *> &ImageWidget::GetAnimationTriggers(quint32 animationNumber)
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

void ImageWidget::AddAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
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

void ImageWidget::RemoveAnimationTrigger(quint32 animationNumber, TransitionClause *trigger)
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

quint32 ImageWidget::GetAnimationID(quint32 animationNumber)
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

bool ImageWidget::GetAnimationUseTriggers(quint32 animationNumber)
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

void ImageWidget::SetAnimationOnLoadBehavior(quint32 animationNumber, AnimationOnLoadBehavior behavior)
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

UIWidgetBase::AnimationOnLoadBehavior ImageWidget::GetAnimationOnLoadBehavior(quint32 animationNumber)
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
    return RUN_ONE_CYCLE;
}

void ImageWidget::SetAnimationOnTriggerActiveBehavior(quint32 animationNumber, AnimationOnTriggerActiveBehavior behavior)
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

UIWidgetBase::AnimationOnTriggerActiveBehavior ImageWidget::GetAnimationOnTriggerActiveBehavior(quint32 animationNumber)
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

void ImageWidget::SetAnimationOnTriggerInactiveBehavior(quint32 animationNumber, AnimationOnTriggerInactiveBehavior behavior)
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

UIWidgetBase::AnimationOnTriggerInactiveBehavior ImageWidget::GetAnimationOnTriggerInactiveBehavior(quint32 animationNumber)
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
    return STOP_AT_END_OF_CYCLE;
}

bool ImageWidget::objectSelfValidation(Project* project, QStringList* errorList)
{
    bool validation = true;
    validation &= UIWidgetBase::objectSelfValidation(project, errorList);

    if(m_imageID == 0)
    {
        errorList->append("Widget '" + GetUIWidgetUniqueName() + "' has no associated image");
        validation = false;
    }

    if (m_imageDefinition.GetImageByID(m_imageID) == &Image::UndefinedImage)
    {
        errorList->append("Widget '" + GetUIWidgetUniqueName() + "' was mapped to imageID " + QString::number(m_imageID) + ", but this imageID does no longer match any entry in the Image Manager.");
        validation = false;
    }

    return validation;
}
