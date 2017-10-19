#include "AnimationFBFPropertiesWidget.h"

#include "AnimationFrameByFrame.h"
#include "Image.h"
#include "NameChangeWidget.h"
#include "Project.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

AnimationFBFPropertiesWidget::AnimationFBFPropertiesWidget(Project* project, QWidget* parent) :
  ProjectObjectPropertiesWidget(project, parent),
  m_project(project),
  m_animation(NULL),
  m_nameLineEdit(NULL),
  m_componentNameLabel(NULL),
  m_frameDurationMsSpinBox(NULL),
  m_repeatModeComboBox(NULL),
  m_defaultFrameComboBox(NULL)
{
}

void AnimationFBFPropertiesWidget::SetAnimationFrameByFrame(AnimationFrameByFrame* anim)
{
    m_animation = anim;
    SetProjectObject(anim);
}

void AnimationFBFPropertiesWidget::beforePopulateSignalSources()
{
    // Name
    m_nameLineEdit = new NameChangeWidget(static_cast<ProjectObject*>(m_animation));
    AddEditableData("Name", m_nameLineEdit);

    // Component Name
    m_componentNameLabel = new QLabel(this);
    m_componentNameLabel->setText(m_animation->GetComponentName());
    AddEditableData("GCL Name", m_componentNameLabel);

    // Frame Duration
    m_frameDurationMsSpinBox = new QSpinBox(this);
    m_frameDurationMsSpinBox->setMinimum(1);
    m_frameDurationMsSpinBox->setMaximum(60000);
    m_frameDurationMsSpinBox->setValue(m_animation->GetFrameDurationMs());
    AddEditableData("Frame Duration [ms]", m_frameDurationMsSpinBox);
    connect(m_frameDurationMsSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        m_animation->SetFrameDurationMs(value);
    });

    // Repeat Mode
    m_repeatModeComboBox = new QComboBox(this);
    m_repeatModeComboBox->addItem("One Shot", static_cast<int>(Animation::RepeatMode::ONESHOT));
    m_repeatModeComboBox->addItem("Loop", static_cast<int>(Animation::RepeatMode::LOOP));
    m_repeatModeComboBox->addItem("Bounce", static_cast<int>(Animation::RepeatMode::BOUNCE));
    m_repeatModeComboBox->addItem("Bounce Loop", static_cast<int>(Animation::RepeatMode::BOUNCE_LOOP));
    m_repeatModeComboBox->setCurrentIndex(m_repeatModeComboBox->findData(static_cast<int>(m_animation->GetRepeatMode())));
    AddEditableData("Repeat Mode", m_repeatModeComboBox);
    connect(m_repeatModeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=]()
    {
        int repeatValue = m_repeatModeComboBox->currentData().toInt();
        auto mode = static_cast<Animation::RepeatMode>(repeatValue);
        m_animation->SetRepeatMode(mode);
    });

    // Default Step
    m_defaultFrameComboBox = new QComboBox(this);
    m_defaultFrameComboBox->addItem("<unset>", -1);
    int stepIndex = 0;
    foreach (quint32 imageID, m_animation->ImageIds())
    {
        foreach (Image* image, m_project->GetImageDefinition().Images())
        {
            if (image->GetID() == imageID)
            {
                m_defaultFrameComboBox->addItem(image->GetName(), stepIndex++);
            }
        }

    }

    m_defaultFrameComboBox->setCurrentIndex(m_defaultFrameComboBox->findData(m_animation->GetDefaultStep()));
    AddEditableData("Default Frame", m_defaultFrameComboBox);
    connect(m_defaultFrameComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]
    {
        m_animation->SetDefaultStep(m_defaultFrameComboBox->currentData().toUInt());
    });
}
