#include "AnimationTranslationPropertiesWidget.h"
#include "AnimationCurveTypeHelper.h"

#include "AnimationTranslation.h"
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

AnimationTranslationPropertiesWidget::AnimationTranslationPropertiesWidget(Project* project, QWidget* parent) :
  ProjectObjectPropertiesWidget(project, parent),
  m_animation(NULL),
  m_nameLineEdit(NULL),
  m_componentNameLabel(NULL),
  m_repeatModeComboBox(NULL),
  m_defaultFrameComboBox(NULL),
  m_lengthMsSpinBox(NULL),
  m_pathTypeComboBox(NULL),
  m_startXSpinBox(NULL),
  m_startYSpinBox(NULL),
  m_bezierC0XSpinBox(NULL),
  m_bezierC0YSpinBox(NULL),
  m_bezierC1XSpinBox(NULL),
  m_bezierC1YSpinBox(NULL),
  m_easingCurveTypeComboBox(NULL),
  m_easingCurveIconSize(QSize(32, 32))
{
    // Pre-render curve type icons
    m_easingCurveTypeIcons = AnimationCurveTypeHelper::curveTypeIcons(m_easingCurveIconSize, 3, Qt::white, Qt::black);
}

void AnimationTranslationPropertiesWidget::SetAnimationTranslation(AnimationTranslation* anim)
{
    m_animation = anim;
    SetProjectObject(anim);

    // Update widgets that show data that have been edited elsewhere
    connect(anim, &ProjectObject::PropertyUpdated,
            this, &AnimationTranslationPropertiesWidget::setWidgetsFromAnimation);
}

void AnimationTranslationPropertiesWidget::beforePopulateSignalSources()
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

    // Default Step
    m_defaultFrameComboBox = new QComboBox(this);
    m_defaultFrameComboBox->addItem("<unset>", -1);
    m_defaultFrameComboBox->addItem("Start Position", 0);
    m_defaultFrameComboBox->addItem("Stop Position", 1);
    m_defaultFrameComboBox->setCurrentIndex(m_defaultFrameComboBox->findData(m_animation->GetDefaultStep()));
    AddEditableData("Default Position", m_defaultFrameComboBox);
    connect(m_defaultFrameComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=]
    {
        m_animation->SetDefaultStep(m_defaultFrameComboBox->currentData().toInt());
    });

    // Path type
    m_pathTypeComboBox = new QComboBox(this);
    m_pathTypeComboBox->addItem("Line segment", static_cast<int>(AnimationTranslation::Path::Linear));
    m_pathTypeComboBox->addItem("Bezier Curve", static_cast<int>(AnimationTranslation::Path::Bezier));
    m_pathTypeComboBox->setCurrentIndex(m_pathTypeComboBox->findData(static_cast<int>(m_animation->GetPathType())));
    AddEditableData("Path", m_pathTypeComboBox);
    connect(m_pathTypeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=]()
    {
        int pathValue = m_pathTypeComboBox->currentData().toInt();
        auto path = static_cast<AnimationTranslation::Path>(pathValue);
        m_animation->SetPathType(path);
    });

    // Start X
    m_startXSpinBox = new QSpinBox(this);
    m_startXSpinBox->setMinimum(-4096);
    m_startXSpinBox->setMaximum(4096);
    m_startXSpinBox->setValue(m_animation->GetStartPoint().x());
    AddEditableData("Relative start X", m_startXSpinBox);
    connect(m_startXSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetStartPoint();
        p.setX(value);
        m_animation->SetStartPoint(p);
    });

    // Start Y
    m_startYSpinBox = new QSpinBox(this);
    m_startYSpinBox->setMinimum(-4096);
    m_startYSpinBox->setMaximum(4096);
    m_startYSpinBox->setValue(m_animation->GetStartPoint().y());
    AddEditableData("Relative start Y", m_startYSpinBox);
    connect(m_startYSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetStartPoint();
        p.setY(value);
        m_animation->SetStartPoint(p);
    });

    // Bezier C0 X
    m_bezierC0XSpinBox = new QSpinBox(this);
    m_bezierC0XSpinBox->setMinimum(-4096);
    m_bezierC0XSpinBox->setMaximum(4096);
    m_bezierC0XSpinBox->setValue(m_animation->GetBezierC0().x());
    AddEditableData("Bezier C0 X", m_bezierC0XSpinBox);
    connect(m_bezierC0XSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetBezierC0();
        p.setX(value);
        m_animation->SetBezierC0(p);
    });

    // Bezier C0 Y
    m_bezierC0YSpinBox = new QSpinBox(this);
    m_bezierC0YSpinBox->setMinimum(-4096);
    m_bezierC0YSpinBox->setMaximum(4096);
    m_bezierC0YSpinBox->setValue(m_animation->GetBezierC0().y());
    AddEditableData("Bezier C0 Y", m_bezierC0YSpinBox);
    connect(m_bezierC0YSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetBezierC0();
        p.setY(value);
        m_animation->SetBezierC0(p);
    });

    // Bezier C1 X
    m_bezierC1XSpinBox = new QSpinBox(this);
    m_bezierC1XSpinBox->setMinimum(-4096);
    m_bezierC1XSpinBox->setMaximum(4096);
    m_bezierC1XSpinBox->setValue(m_animation->GetBezierC1().x());
    AddEditableData("Bezier C1 X", m_bezierC1XSpinBox);
    connect(m_bezierC1XSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetBezierC1();
        p.setX(value);
        m_animation->SetBezierC1(p);
    });

    // Bezier C1 Y
    m_bezierC1YSpinBox = new QSpinBox(this);
    m_bezierC1YSpinBox->setMinimum(-4096);
    m_bezierC1YSpinBox->setMaximum(4096);
    m_bezierC1YSpinBox->setValue(m_animation->GetBezierC1().y());
    AddEditableData("Bezier C1 Y", m_bezierC1YSpinBox);
    connect(m_bezierC1YSpinBox,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [=](int value)
    {
        QPoint p = m_animation->GetBezierC1();
        p.setY(value);
        m_animation->SetBezierC1(p);
    });


    // Easing curve type combo box
    m_easingCurveTypeComboBox = new QComboBox(this);
    m_easingCurveTypeComboBox->setIconSize(m_easingCurveIconSize);

    // Not included in list
    const QList<QEasingCurve::Type> excludedTypes = {QEasingCurve::SineCurve,
                                                     QEasingCurve::CosineCurve,
                                                     QEasingCurve::BezierSpline,
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
        auto curveIcon = m_easingCurveTypeIcons[curveType];
        m_easingCurveTypeComboBox->addItem(curveIcon, curveName, i);
    }

    m_easingCurveTypeComboBox->setCurrentIndex(m_easingCurveTypeComboBox->findData(static_cast<int>(m_animation->GetEasingCurveType())));
    AddEditableData("Easing Curve Type", m_easingCurveTypeComboBox);
    connect(m_easingCurveTypeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [=]()
    {
        int typeValue = m_easingCurveTypeComboBox->currentData().toInt();
        auto type = static_cast<QEasingCurve::Type>(typeValue);
        m_animation->SetEasingCurveType(type);
    });
}

void AnimationTranslationPropertiesWidget::setWidgetsFromAnimation()
{
    m_startXSpinBox->setValue(m_animation->GetStartPoint().x());
    m_startYSpinBox->setValue(m_animation->GetStartPoint().y());
    m_bezierC0XSpinBox->setValue(m_animation->GetBezierC0().x());
    m_bezierC0YSpinBox->setValue(m_animation->GetBezierC0().y());
    m_bezierC1XSpinBox->setValue(m_animation->GetBezierC1().x());
    m_bezierC1YSpinBox->setValue(m_animation->GetBezierC1().y());
}
