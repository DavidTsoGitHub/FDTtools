#include "AnimationOpacityPropertiesWidget.h"

#include "AnimationCurveTypeHelper.h"
#include "AnimationOpacity.h"
#include "NameChangeWidget.h"

#include <QComboBox>
#include <QEasingCurve>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMetaEnum>
#include <QMetaObject>
#include <QSize>
#include <QSpinBox>

AnimationOpacityPropertiesWidget::AnimationOpacityPropertiesWidget(Project* project, QWidget* parent) :
  ProjectObjectPropertiesWidget(project, parent),
  m_animation(NULL),
  m_nameLineEdit(NULL),
  m_componentNameLabel(NULL),
  m_repeatModeComboBox(NULL),
  m_lengthMsSpinBox(NULL),
  m_startOpacitySpinBox(NULL),
  m_stopOpacitySpinBox(NULL),
  m_defaultFrameComboBox(NULL),
  m_curveTypeComboBox(NULL),
  m_curveIconSize(QSize(32, 32))
{
    // Pre-render curve type icons
    m_curveTypeIcons = AnimationCurveTypeHelper::curveTypeIcons(m_curveIconSize, 3, Qt::white, Qt::black);
}

void AnimationOpacityPropertiesWidget::SetAnimationOpacity(AnimationOpacity* anim)
{
    m_animation = anim;
    SetProjectObject(anim);

    // Update widgets that show data that have been edited elsewhere
    connect(anim, &ProjectObject::PropertyUpdated,
            this, &AnimationOpacityPropertiesWidget::setWidgetsFromAnimation);
}

void AnimationOpacityPropertiesWidget::beforePopulateSignalSources()
{
    // Name
    m_nameLineEdit = new NameChangeWidget(static_cast<ProjectObject*>(m_animation));
    AddEditableData("Name", m_nameLineEdit);

    // Component Name
    m_componentNameLabel = new QLabel(this);
    m_componentNameLabel->setText(m_animation->GetComponentName());
    AddEditableData("GCL Name", m_componentNameLabel);

    // Length
    m_lengthMsSpinBox = new QSpinBox(this);
    m_lengthMsSpinBox->setMinimum(1);
    m_lengthMsSpinBox->setMaximum(10000);
    m_lengthMsSpinBox->setValue(m_animation->GetLengthMs());
    AddEditableData("Length [ms]", m_lengthMsSpinBox);
    connect(m_lengthMsSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        m_animation->SetLengthMs(value);
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

    // Start opacity
    m_startOpacitySpinBox = new QSpinBox(this);
    m_startOpacitySpinBox->setMinimum(0);
    m_startOpacitySpinBox->setMaximum(100);
    m_startOpacitySpinBox->setValue(m_animation->GetStartOpacityPercent());
    AddEditableData("Start opacity [%]", m_startOpacitySpinBox);
    connect(m_startOpacitySpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        m_animation->SetStartOpacityPercent(value);
    });

    // Stop opacity
    m_stopOpacitySpinBox = new QSpinBox(this);
    m_stopOpacitySpinBox->setMinimum(0);
    m_stopOpacitySpinBox->setMaximum(100);
    m_stopOpacitySpinBox->setValue(m_animation->GetStopOpacityPercent());
    AddEditableData("Stop opacity [%]", m_stopOpacitySpinBox);
    connect(m_stopOpacitySpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        m_animation->SetStopOpacityPercent(value);
    });

    // Default Step
    m_defaultFrameComboBox = new QComboBox(this);
    m_defaultFrameComboBox->addItem("<unset>", -1);
    m_defaultFrameComboBox->addItem("Start Opacity", 0);
    m_defaultFrameComboBox->addItem("Stop Opacity", 1);
    m_defaultFrameComboBox->setCurrentIndex(m_defaultFrameComboBox->findData(m_animation->GetDefaultStep()));
    AddEditableData("Default Opacity", m_defaultFrameComboBox);
    connect(m_defaultFrameComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]
    {
        m_animation->SetDefaultStep(m_defaultFrameComboBox->currentData().toInt());
    });

    // Curve type combo box
    m_curveTypeComboBox = new QComboBox(this);
    m_curveTypeComboBox->setIconSize(m_curveIconSize);

    // Not included in list
    const QList<QEasingCurve::Type> excludedTypes = {QEasingCurve::BezierSpline,
                                                     QEasingCurve::TCBSpline,
                                                     QEasingCurve::Custom};

    // Add combo box item with curve type icon and name for all curve types
    // that should be available
    for(int i = 0; i < QEasingCurve::NCurveTypes; ++i)
    {
        auto curveType = static_cast<QEasingCurve::Type>(i);
        if(excludedTypes.contains(curveType))
            continue;

        auto curveName = AnimationCurveTypeHelper::curveTypeStr(curveType);
        auto curveIcon = m_curveTypeIcons[curveType];
        m_curveTypeComboBox->addItem(curveIcon, curveName, i);
    }

    m_curveTypeComboBox->setCurrentIndex(m_curveTypeComboBox->findData(static_cast<int>(m_animation->GetCurveType())));
    AddEditableData("Curve Type", m_curveTypeComboBox);
    connect(m_curveTypeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=]()
    {
        int typeValue = m_curveTypeComboBox->currentData().toInt();
        auto type = static_cast<QEasingCurve::Type>(typeValue);
        m_animation->SetCurveType(type);
    });
}

void AnimationOpacityPropertiesWidget::setWidgetsFromAnimation()
{
    m_lengthMsSpinBox->setValue(m_animation->GetLengthMs());
    m_startOpacitySpinBox->setValue(m_animation->GetStartOpacityPercent());
    m_stopOpacitySpinBox->setValue(m_animation->GetStopOpacityPercent());
}
