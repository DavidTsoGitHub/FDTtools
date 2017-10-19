#include "CheckMessagePropertiesWidget.h"

#include "CheckMessage.h"
#include "NameChangeWidget.h"
#include "Project.h"

#include <QWidget>

CheckMessagePropertiesWidget::CheckMessagePropertiesWidget(Project *project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_project(project),
    m_checkMessageDefinition(m_project->GetCheckMessagesDefinition()),
    m_message(NULL),
    m_reoccurringCheckBox(NULL),
    m_clearConditionComboBox(NULL),
    m_timeoutSpinBox(NULL),
    m_nameChangeWidget(NULL)
{
}

CheckMessagePropertiesWidget::~CheckMessagePropertiesWidget()
{
}

void CheckMessagePropertiesWidget::beforePopulateSignals()
{
    m_nameChangeWidget = new NameChangeWidget(static_cast<ProjectObject*>(m_message));
    AddEditableData("Name", m_nameChangeWidget);

    m_reoccurringCheckBox = new QCheckBox();
    m_reoccurringCheckBox->setChecked(m_message->GetReoccurring());
    AddEditableData("Reoccurring", m_reoccurringCheckBox);

    connect(m_reoccurringCheckBox,
            &QCheckBox::clicked,
            [=](const bool& clicked)
    {
        m_message->SetReoccurring(clicked);
    });

    m_clearConditionComboBox = new QComboBox();
    m_clearConditionComboBox->addItems(m_message->GetClearConditionStrings());

    m_clearConditionComboBox->setCurrentIndex(static_cast<int>(m_message->GetClearCondition()));
    AddEditableData("Clear condition", m_clearConditionComboBox);
    connect(m_clearConditionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=] (int index)
    {
        CheckMessage::ClearCondition_e selectedCondition = static_cast<CheckMessage::ClearCondition_e>(index);
        m_message->SetClearCondition(selectedCondition);

        testReoccurringCheckBoxEnabled();
        testSpinBoxEnabled();

    });

    m_timeoutSpinBox = new QSpinBox();
    m_timeoutSpinBox->setMaximum(INT_MAX);
    m_timeoutSpinBox->setMinimum(0);
    m_timeoutSpinBox->setValue(m_message->GetTimeout());
    AddEditableData("Timeout (ms)", m_timeoutSpinBox);
    connect(m_timeoutSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] (int newValue)
    {
       m_message->SetTimeout(newValue);
    });

    testReoccurringCheckBoxEnabled();
    testSpinBoxEnabled();
}

void CheckMessagePropertiesWidget::testReoccurringCheckBoxEnabled()
{
    if (m_message->GetClearCondition() == CheckMessage::ClearCondition_e::Trigger)
    {
        m_message->SetReoccurring(false);
        m_reoccurringCheckBox->setChecked(false);
        m_reoccurringCheckBox->setEnabled(false);
    }
    else
    {
        m_reoccurringCheckBox->setEnabled(true);
    }
}

void CheckMessagePropertiesWidget::testSpinBoxEnabled()
{
    if (m_message->GetClearCondition() == CheckMessage::ClearCondition_e::Timeout)
    {
        m_timeoutSpinBox->setEnabled(true);
    }
    else
    {
        m_timeoutSpinBox->setEnabled(false);
    }
}

void CheckMessagePropertiesWidget::SetProjectObject(ProjectObject *projectObject)
{
    m_message = static_cast<CheckMessage*>(projectObject);
    ProjectObjectPropertiesWidget::SetProjectObject(projectObject);
}

