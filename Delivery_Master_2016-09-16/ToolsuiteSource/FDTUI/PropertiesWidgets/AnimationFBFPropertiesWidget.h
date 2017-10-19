#ifndef ANIMATIONFBFPROPERTIESWIDGET_H
#define ANIMATIONFBFPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class AnimationFrameByFrame;
class NameChangeWidget;
class Project;

class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class AnimationFBFPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit AnimationFBFPropertiesWidget(Project* project, QWidget* parent = 0);

    void SetAnimationFrameByFrame(AnimationFrameByFrame* anim);

private:
    Project* m_project;
    AnimationFrameByFrame* m_animation;
    NameChangeWidget* m_nameLineEdit;
    QLabel* m_componentNameLabel;
    QSpinBox* m_frameDurationMsSpinBox;
    QComboBox* m_repeatModeComboBox;
    QComboBox* m_defaultFrameComboBox;

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignalSources();
};

#endif // ANIMATIONFBFPROPERTIESWIDGET_H
