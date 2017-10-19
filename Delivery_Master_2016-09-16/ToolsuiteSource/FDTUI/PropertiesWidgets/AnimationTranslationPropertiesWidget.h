#ifndef ANIMATIONTRANSLATIONPROPERTIESWIDGET_H
#define ANIMATIONTRANSLATIONPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class AnimationTranslation;
class NameChangeWidget;

class QComboBox;
class QEasingCurve;
class QIcon;
class QLabel;
class QLineEdit;
class QSize;
class QSpinBox;

class AnimationTranslationPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT
public:
    explicit AnimationTranslationPropertiesWidget(Project* project, QWidget* parent = 0);
    void SetAnimationTranslation(AnimationTranslation* anim);

protected:
    // ProjectObjectPropertiesWidget interface
    void beforePopulateSignalSources();

private:
    AnimationTranslation* m_animation;
    NameChangeWidget* m_nameLineEdit;
    QLabel* m_componentNameLabel;
    QComboBox* m_repeatModeComboBox;
    QComboBox* m_defaultFrameComboBox;
    QSpinBox* m_lengthMsSpinBox;
    QComboBox* m_pathTypeComboBox;
    QSpinBox* m_startXSpinBox;
    QSpinBox* m_startYSpinBox;
    QSpinBox* m_bezierC0XSpinBox;
    QSpinBox* m_bezierC0YSpinBox;
    QSpinBox* m_bezierC1XSpinBox;
    QSpinBox* m_bezierC1YSpinBox;
    QComboBox* m_easingCurveTypeComboBox;

    QMap<QEasingCurve::Type, QIcon> m_easingCurveTypeIcons;
    const QSize m_easingCurveIconSize;

    void setWidgetsFromAnimation();
};

#endif // ANIMATIONTRANSLATIONPROPERTIESWIDGET_H
