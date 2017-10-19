#ifndef ANIMATIONOPACITYPROPERTIESWIDGET_H
#define ANIMATIONOPACITYPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class AnimationOpacity;
class NameChangeWidget;

class QComboBox;
class QEasingCurve;
class QIcon;
class QLabel;
class QLineEdit;
class QSize;
class QSpinBox;

class AnimationOpacityPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit AnimationOpacityPropertiesWidget(Project* project, QWidget* parent = 0);
    void SetAnimationOpacity(AnimationOpacity* anim);

protected:
    // ProjectObjectPropertiesWidget interface
    void beforePopulateSignalSources();

private:
    AnimationOpacity* m_animation;
    NameChangeWidget* m_nameLineEdit;
    QLabel* m_componentNameLabel;
    QComboBox* m_repeatModeComboBox;
    QSpinBox* m_lengthMsSpinBox;
    QSpinBox* m_startOpacitySpinBox;
    QSpinBox* m_stopOpacitySpinBox;
    QComboBox* m_defaultFrameComboBox;
    QComboBox* m_curveTypeComboBox;

    QMap<QEasingCurve::Type, QIcon> m_curveTypeIcons;
    const QSize m_curveIconSize;

    void setWidgetsFromAnimation();

};

#endif // ANIMATIONOPACITYPROPERTIESWIDGET_H
