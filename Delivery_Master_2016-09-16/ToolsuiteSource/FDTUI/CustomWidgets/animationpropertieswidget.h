#ifndef ANIMATIONPROPERTIESWIDGET
#define ANIMATIONPROPERTIESWIDGET

class ProjectObjectPropertiesWidget;
class ProjectObject;
class Project;
class UIWidgetBase;
class QTreeWidgetItem;
class QWidget;

class AnimationPropertiesWidget : public QWidget
{
public:
    explicit AnimationPropertiesWidget(QTreeWidgetItem* rootItem, ProjectObject* projectObject,ProjectObjectPropertiesWidget* parent, Project* project, quint8 animationIndex);
    ~AnimationPropertiesWidget();

private:
    void addProperties(void);

    QTreeWidgetItem* m_rootItem;
    ProjectObjectPropertiesWidget* m_parent;
    Project* m_project;
    quint8 m_index;
    UIWidgetBase* m_uiWidget;
    quint32 m_animationID;

    void showDefaultOnLoadRadioButton_toggled(bool checked);
    void runOneCycleOnLoadRadioButton_toggled(bool checked);
    void doNothingOnLoadRadioButton_toggled(bool checked);
    void alwaysStartAnimationFromBeginningCheckBox_toggled(bool checked);
    void stopOnTriggerInactiveRadioButton_toggled(bool checked);
    void stopAtEndOfCycleRadioButton_toggled(bool checked);
    void showDefaultOnTriggerInactiveRadioButton_toggled(bool checked);
    void continueOnTriggerInactiveRadioButton_toggled(bool checked);
    void StartFromCurrentFrameRadioButton_toggled(bool checked);
    void StartFromDefaultRadioButton_toggled(bool checked);
    void StartFromBeginningRadiobutton_toggled(bool checked);
};

#endif // ANIMATIONPROPERTIESWIDGET

