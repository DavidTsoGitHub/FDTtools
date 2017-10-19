#ifndef TRANSITIONWIDGET
#define TRANSITIONWIDGET

class TransitionCondition;
class TransitionClause;
class QTreeWidgetItem;
class ProjectObject;
class Project;
class ProjectObjectPropertiesWidget;
class QComboBox;
class QLineEdit;
class QWidget;

class TransitionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransitionWidget(TransitionClause* transition, QTreeWidgetItem* transitionRootItem, ProjectObject* projectObject, Project *project, ProjectObjectPropertiesWidget* parent, quint8 index = 0);
    ~TransitionWidget();

private:
    ProjectObject* m_projectObject;
    Project* m_project;
    ProjectObjectPropertiesWidget* m_parent;
    quint8 m_index;
    TransitionClause* m_trigger;
    QTreeWidgetItem* m_triggerRootItem;
    QTreeWidgetItem* m_headerItem;

    QComboBox* m_additionalCompareComboBox;
    QComboBox* m_firstConditionComboBox;
    QComboBox* m_secondConditionComboBox;

    QLineEdit* m_firstConditionLineEdit;
    QLineEdit* m_secondConditionLineEdit;

    void addHeader();
    void addSignalSinkWidget(QTreeWidgetItem* parent, TransitionCondition* condition);
    void addConditionCombobox(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QComboBox* combobox);
    void addCompareValueLineEdit(QTreeWidgetItem* parent, TransitionCondition* transitionCondition, QLineEdit* lineEdit);
    void addAdditionalCompareCombobox(QTreeWidgetItem* parent);
    void addFirstCondition();
    void addSecondCondition();
    void additionalCompareComboBox_currentIndexChanged(int currentIndex);
    void conditionCombobox_currentIndexChanged(int currentIndex, TransitionCondition *transitionCondition);
    void removeTransition();
};

#endif // TRANSITIONWIDGET

