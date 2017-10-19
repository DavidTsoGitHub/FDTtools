#include "AnimationPropertiesWidget.h"

#include "ProjectObjectPropertiesWidget.h"
#include "UIWidgetBase.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>

AnimationPropertiesWidget::AnimationPropertiesWidget(QTreeWidgetItem* rootItem, ProjectObject *projectObject, ProjectObjectPropertiesWidget *parent, Project *project, quint8 animationIndex) :
    m_rootItem(rootItem),
    m_parent(parent),
    m_project(project),
    m_index(animationIndex)
{
    m_uiWidget = static_cast<UIWidgetBase*>(projectObject);
    m_animationID = m_uiWidget->GetAnimationID(m_index);


    addProperties();
}

AnimationPropertiesWidget::~AnimationPropertiesWidget()
{

}

void AnimationPropertiesWidget::addProperties()
{
    const Animation* animation = m_project->GetAnimationDefinition().GetAnimationByID(m_animationID);
    if (animation == NULL)
    {
        return;
    }

    bool isBounceAnimation = animation->GetRepeatMode() == Animation::RepeatMode::BOUNCE_LOOP || animation->GetRepeatMode() == Animation::RepeatMode::BOUNCE;

    // Behaviour on Load
    QTreeWidgetItem* onLoadBehaviourItem = m_parent->AddContainer("Behaviour on Load", m_rootItem);
    m_parent->AddTreeWidgetItem(onLoadBehaviourItem, NULL, 1);

    QRadioButton* doNothingOnLoadRadioButton = new QRadioButton(this);
    m_parent->AddEditableData("Do Nothing", doNothingOnLoadRadioButton, onLoadBehaviourItem);

    QRadioButton* runOneCycleOnLoadRadioButton = new QRadioButton(this);
    m_parent->AddEditableData("Run One Cycle", runOneCycleOnLoadRadioButton, onLoadBehaviourItem);

    QRadioButton* showDefaultOnLoadRadioButton = new QRadioButton(this);
    m_parent->AddEditableData("Show Default", showDefaultOnLoadRadioButton, onLoadBehaviourItem);

    switch (m_uiWidget->GetAnimationOnLoadBehavior(m_index))
    {
        case UIWidgetBase::DO_NOTHING:
            doNothingOnLoadRadioButton->setChecked(true);
            break;
        case UIWidgetBase::RUN_ONE_CYCLE:
            runOneCycleOnLoadRadioButton->setChecked(true);
            break;
        case UIWidgetBase::SHOW_DEFAULT:
            showDefaultOnLoadRadioButton->setChecked(true);
            break;
        default:
            runOneCycleOnLoadRadioButton->setChecked(true);
            break;
    }

    connect(doNothingOnLoadRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::doNothingOnLoadRadioButton_toggled);
    connect(runOneCycleOnLoadRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::runOneCycleOnLoadRadioButton_toggled);
    connect(showDefaultOnLoadRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::showDefaultOnLoadRadioButton_toggled);

    QButtonGroup* onLoadButtonGroup = new QButtonGroup(this);
    onLoadButtonGroup->addButton(doNothingOnLoadRadioButton);
    onLoadButtonGroup->addButton(runOneCycleOnLoadRadioButton);
    onLoadButtonGroup->addButton(showDefaultOnLoadRadioButton);

    // Behaviour on Trigger Active
    QTreeWidgetItem* onTriggerActiveBehaviourItem = m_parent->AddContainer("Behaviour on Trigger Active", m_rootItem);
    m_parent->AddTreeWidgetItem(onTriggerActiveBehaviourItem, NULL, 1);

    QButtonGroup* onTriggerActiveButtonGroup = new QButtonGroup(this);

    QRadioButton* StartFromBeginningRadiobutton = new QRadioButton(this);
    m_parent->AddEditableData("Start from Beginning", StartFromBeginningRadiobutton, onTriggerActiveBehaviourItem);

    QRadioButton* StartFromDefaultRadioButton = new QRadioButton(this);
    StartFromDefaultRadioButton->setEnabled(!isBounceAnimation);
    QTreeWidgetItem* item = m_parent->AddEditableData("Start from Default Frame", StartFromDefaultRadioButton, onTriggerActiveBehaviourItem);
    item->setDisabled(isBounceAnimation);

    QRadioButton* StartFromCurrentFrameRadioButton = new QRadioButton(this);
    StartFromCurrentFrameRadioButton->setEnabled(!isBounceAnimation);
    item = m_parent->AddEditableData("Start from Current Frame", StartFromCurrentFrameRadioButton, onTriggerActiveBehaviourItem);
    item->setDisabled(isBounceAnimation);

    switch (m_uiWidget->GetAnimationOnTriggerActiveBehavior(m_index))
    {
        case UIWidgetBase::START_FROM_BEGINNING:
            StartFromBeginningRadiobutton->setChecked(true);
            break;
        case UIWidgetBase::START_FROM_DEFAULT_FRAME:
            if (isBounceAnimation)
            {
                StartFromBeginningRadiobutton->setChecked(true);
                StartFromBeginningRadiobutton_toggled(true);
            }
            else
            {
                StartFromDefaultRadioButton->setChecked(true);
            }
            break;
        case UIWidgetBase::CONTINUE_FROM_CURRENT_FRAME:
            if (isBounceAnimation)
            {
                StartFromBeginningRadiobutton->setChecked(true);
                StartFromBeginningRadiobutton_toggled(true);
            }
            else
            {
                StartFromCurrentFrameRadioButton->setChecked(true);
            }
            break;
        default:
            StartFromBeginningRadiobutton->setChecked(true);
            break;
    }

    connect(StartFromBeginningRadiobutton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::StartFromBeginningRadiobutton_toggled);
    connect(StartFromDefaultRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::StartFromDefaultRadioButton_toggled);
    connect(StartFromCurrentFrameRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::StartFromCurrentFrameRadioButton_toggled);

    onTriggerActiveButtonGroup->addButton(StartFromBeginningRadiobutton);
    onTriggerActiveButtonGroup->addButton(StartFromDefaultRadioButton);
    onTriggerActiveButtonGroup->addButton(StartFromCurrentFrameRadioButton);

    // Behaviour on Trigger Inactive
    QTreeWidgetItem* onTriggerInactiveBehaviourItem = m_parent->AddContainer("Behaviour on Trigger Inactive", m_rootItem);
    m_parent->AddTreeWidgetItem(onTriggerInactiveBehaviourItem, NULL, 1);

    QButtonGroup* onTriggerInactiveButtonGroup = new QButtonGroup(this);

    QRadioButton* stopOnTriggerInactiveRadiobutton = new QRadioButton(this);
    stopOnTriggerInactiveRadiobutton->setEnabled(!isBounceAnimation);
    item = m_parent->AddEditableData("Stop", stopOnTriggerInactiveRadiobutton, onTriggerInactiveBehaviourItem);
    item->setDisabled(isBounceAnimation);

    QRadioButton* stopAtEndOfCycleRadioButton = new QRadioButton(this);
    m_parent->AddEditableData("Stop at end of cycle", stopAtEndOfCycleRadioButton, onTriggerInactiveBehaviourItem);

    QRadioButton* showDefaultOnTriggerInactiveRadioButton = new QRadioButton(this);
    showDefaultOnTriggerInactiveRadioButton->setEnabled(!isBounceAnimation);
    item = m_parent->AddEditableData("Stop at Default Frame", showDefaultOnTriggerInactiveRadioButton, onTriggerInactiveBehaviourItem);
    item->setDisabled(isBounceAnimation);

    QRadioButton* continueOnTriggerInactiveRadioButton = new QRadioButton(this);
    m_parent->AddEditableData("Continue", continueOnTriggerInactiveRadioButton, onTriggerInactiveBehaviourItem);

    switch (m_uiWidget->GetAnimationOnTriggerInactiveBehavior(m_index))
    {
        case UIWidgetBase::STOP:
            if (isBounceAnimation)
            {
                stopAtEndOfCycleRadioButton->setChecked(true);
                stopAtEndOfCycleRadioButton_toggled(true);
            }
            else
            {
                stopOnTriggerInactiveRadiobutton->setChecked(true);
            }
            break;
        case UIWidgetBase::STOP_AT_END_OF_CYCLE:
            stopAtEndOfCycleRadioButton->setChecked(true);
            break;
        case UIWidgetBase::STOP_AT_DEFAULT_FRAME:
            if (isBounceAnimation)
            {
                stopAtEndOfCycleRadioButton->setChecked(true);
                stopAtEndOfCycleRadioButton_toggled(true);
            }
            else
            {
                showDefaultOnTriggerInactiveRadioButton->setChecked(true);
            }
            break;
        case UIWidgetBase::CONTINUE:
            continueOnTriggerInactiveRadioButton->setChecked(true);
            break;
        default:
            stopAtEndOfCycleRadioButton->setChecked(true);
            break;
    }

    connect(stopOnTriggerInactiveRadiobutton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::stopOnTriggerInactiveRadioButton_toggled);
    connect(stopAtEndOfCycleRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::stopAtEndOfCycleRadioButton_toggled);
    connect(showDefaultOnTriggerInactiveRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::showDefaultOnTriggerInactiveRadioButton_toggled);
    connect(continueOnTriggerInactiveRadioButton, &QRadioButton::toggled, this, &AnimationPropertiesWidget::continueOnTriggerInactiveRadioButton_toggled);

    onTriggerInactiveButtonGroup->addButton(stopOnTriggerInactiveRadiobutton);
    onTriggerInactiveButtonGroup->addButton(stopAtEndOfCycleRadioButton);
    onTriggerInactiveButtonGroup->addButton(showDefaultOnTriggerInactiveRadioButton);
    onTriggerInactiveButtonGroup->addButton(continueOnTriggerInactiveRadioButton);
}

void AnimationPropertiesWidget::doNothingOnLoadRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnLoadBehavior(m_index, UIWidgetBase::DO_NOTHING);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::runOneCycleOnLoadRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnLoadBehavior(m_index, UIWidgetBase::RUN_ONE_CYCLE);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::showDefaultOnLoadRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnLoadBehavior(m_index, UIWidgetBase::SHOW_DEFAULT);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::StartFromBeginningRadiobutton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerActiveBehavior(m_index, UIWidgetBase::START_FROM_BEGINNING);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::StartFromDefaultRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerActiveBehavior(m_index, UIWidgetBase::START_FROM_DEFAULT_FRAME);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::StartFromCurrentFrameRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerActiveBehavior(m_index, UIWidgetBase::CONTINUE_FROM_CURRENT_FRAME);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::stopOnTriggerInactiveRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerInactiveBehavior(m_index, UIWidgetBase::STOP);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::stopAtEndOfCycleRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerInactiveBehavior(m_index, UIWidgetBase::STOP_AT_END_OF_CYCLE);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::showDefaultOnTriggerInactiveRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerInactiveBehavior(m_index, UIWidgetBase::STOP_AT_DEFAULT_FRAME);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}

void AnimationPropertiesWidget::continueOnTriggerInactiveRadioButton_toggled(bool checked)
{
    if (checked)
    {
        if (m_uiWidget != NULL)
        {
            m_uiWidget->SetAnimationOnTriggerInactiveBehavior(m_index, UIWidgetBase::CONTINUE);
            m_uiWidget->NotifyPropertyUpdated();
        }
    }
}
