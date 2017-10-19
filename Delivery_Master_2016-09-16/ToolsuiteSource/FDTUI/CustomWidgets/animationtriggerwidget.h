#ifndef ANIMATIONTRIGGERWIDGET
#define ANIMATIONTRIGGERWIDGET

class Project;
class ProjectObject;
class ProjectObjectPropertiesWidget;
class TransitionCondition;
class TransitionClause;
class UIWidgetBase;
class QComboBox;
class QLineEdit;
class QTreeWidgetItem;
class QWidget;

class AnimationTriggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationTriggerWidget(TransitionClause* transition, QTreeWidgetItem* transitionRootItem, ProjectObject* projectObject, Project *project, ProjectObjectPropertiesWidget* parent, quint8 index = 0);
    ~AnimationTriggerWidget();

private:
    ProjectObject* m_projectObject;
    Project* m_project;
    ProjectObjectPropertiesWidget* m_parent;
    quint8 m_index;
    TransitionClause* m_trigger;
    QTreeWidgetItem* m_triggerRootItem;
    UIWidgetBase* m_uiWidget;
    QTreeWidgetItem* m_headerItem;

    QComboBox* m_additionalCompareComboBox;
    QComboBox* m_firstConditionComboBox;
    QComboBox* m_secondConditionComboBox;

    QLineEdit* m_firstConditionLineEdit;
    QLineEdit* m_secondConditionLineEdit;

    void addHeader();
    void addProperties();
    void addSignalSinkWidget(QTreeWidgetItem* parent, TransitionCondition* condition);
    void addConditionCombobox(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QComboBox* combobox);
    void addCompareValueLineEdit(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QLineEdit* lineEdit);
    void addAdditionalCompareCombobox(QTreeWidgetItem* parent);
    void addFirstCondition();
    void addSecondCondition();
    void additionalCompareComboBox_currentIndexChanged(int currentIndex);
    void conditionCombobox_currentIndexChanged(int currentIndex, TransitionCondition *transitionCondition);
    void removeTrigger();

};

#endif // ANIMATIONTRIGGERWIDGET

