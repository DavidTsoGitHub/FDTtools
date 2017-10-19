#ifndef CHECKMESSAGEPROPERTIESWIDGET_H
#define CHECKMESSAGEPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class CheckMessageDefinition;
class NameChangeWidget;
class Project;

class QCheckBox;
class QComboBox;
class QLineEdit;
class QSpinBox;
class QWidget;

class CheckMessagePropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit CheckMessagePropertiesWidget(Project* project, QWidget* parent = 0);
    ~CheckMessagePropertiesWidget();

private:
    Project* m_project;
    CheckMessageDefinition& m_checkMessageDefinition;
    CheckMessage* m_message;
    QCheckBox *m_reoccurringCheckBox;
    QComboBox* m_clearConditionComboBox;
    QSpinBox* m_timeoutSpinBox;
    NameChangeWidget* m_nameChangeWidget;

    void testReoccurringCheckBoxEnabled();
    void testSpinBoxEnabled();

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();

    // ProjectObjectPropertiesWidget interface
public:
    void SetProjectObject(ProjectObject *projectObject);
};

#endif // CHECKMESSAGEPROPERTIESWIDGET_H
