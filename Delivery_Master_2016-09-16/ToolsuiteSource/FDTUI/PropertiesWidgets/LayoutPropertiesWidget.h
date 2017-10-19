#ifndef LAYOUTPROPERTIESWIDGET_H
#define LAYOUTPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class Layout;
class NameChangeWidget;
class Project;
class TransitionClause;
class TransitionWidget;

class QComboBox;
class QLabel;
template <typename T> class QList;
class QString;
class QSpinBox;
class QTimer;
class QTreeWidgetItem;
class QWidget;
typedef QList<QWidget *> QWidgetList;

class LayoutPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit LayoutPropertiesWidget(Project* project, QWidget* parent = 0);
    ~LayoutPropertiesWidget();

    void SetLayout(Layout* layout);

private:
    void populateImageComboBox();
    void populateTransitionsTree();
    void addNewTransition();
    void addTransitionToTreeView(TransitionClause *transition);
    void populateLayoutComboboxes();
    void connectLayoutComboBoxesSignals();

    Project* m_project;
    Layout* m_layout;
    NameChangeWidget* m_nameLineEdit;
    QLabel* m_componentNameLabel;
    QComboBox* m_upComboBox;
    QComboBox* m_downComboBox;
    QComboBox* m_leftComboBox;
    QComboBox* m_rightComboBox;
    QComboBox* m_setComboBox;
    QComboBox* m_clearComboBox;
    QSpinBox* m_backgroundColorRedSpinBox;
    QSpinBox* m_backgroundColorGreenSpinBox;
    QSpinBox* m_backgroundColorBlueSpinBox;
    quint16 m_currentIndex;
    quint16 m_transitionCounter;
    QTreeWidgetItem* m_backgroundColorRoot;
    QTreeWidgetItem* m_signalTransitionsRoot;
    QTreeWidgetItem* m_buttonTransitionsRoot;
    QList<TransitionWidget*> m_transitions;
    QTimer* m_nameEditedTimer;
    QString m_nameEditedTemporary;
    QWidgetList m_allPropertyWidgets;

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignalSources();
};

#endif // LAYOUTPROPERTIESWIDGET_H
