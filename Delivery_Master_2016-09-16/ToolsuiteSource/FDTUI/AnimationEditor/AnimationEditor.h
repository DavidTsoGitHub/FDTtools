#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

class AnimationListModel;
class FrameSelectionWidget;
class OpacityCurveWidget;
class Project;
class ProjectObject;
class SimpleEditableListWidget;
class TranslationCurveWidget;

class QWidget;

namespace Ui
{
class AnimationEditor;
}

class AnimationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationEditor(QWidget *parent = 0);
    ~AnimationEditor(void);

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void RefreshAvailableImages();
    void JustSelected();

signals:
    void signal_AnimationEditorItemSelected(ProjectObject* projectObject);
    void signal_modified();

private:
    void initAnimationList(SimpleEditableListWidget* list, AnimationListModel* model);
    void deinitAnimationList(SimpleEditableListWidget* list, AnimationListModel* model);

    void populateFrameSelection();
    void updateFrameByFrameAnimationDefinition();

    Ui::AnimationEditor* m_ui;

    Project* m_project;

    AnimationListModel* m_frameAnimModel;
    AnimationListModel* m_opacityAnimModel;
    AnimationListModel* m_translationAnimModel;

    SimpleEditableListWidget* m_frameAnimList;
    SimpleEditableListWidget* m_opacityAnimList;
    SimpleEditableListWidget* m_translationAnimList;

    FrameSelectionWidget* m_frameSelectionWidget;
    OpacityCurveWidget* m_opacityCurveWidget;
    TranslationCurveWidget* m_translationCurveWidget;

};

#endif // ANIMATIONEDITOR_H
