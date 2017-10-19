#include "ProjectPropertiesWidget.h"

#include "Project.h"

#include <QLineEdit>
#include <QString>
#include <QWidget>

ProjectPropertiesWidget::ProjectPropertiesWidget(Project* project, QWidget* parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_projectNameEditor(NULL)
{
}

ProjectPropertiesWidget::~ProjectPropertiesWidget()
{
}

void ProjectPropertiesWidget::SetProject(Project* project)
{
    SetProjectObject(project);
}

void ProjectPropertiesWidget::slot_projectNameChanged(const QString& newName)
{
    getProject()->SetProjectName(newName);
}

void ProjectPropertiesWidget::beforePopulateSignals()
{
    m_projectNameEditor = new QLineEdit(getProject()->GetProjectName());
    AddEditableData("Project Name", m_projectNameEditor);
    connect(m_projectNameEditor, &QLineEdit::textChanged, this, &ProjectPropertiesWidget::slot_projectNameChanged);

    AddDisplayData("BSP Spec", getProject()->GetBSPSpecificationPath());
    AddDisplayData("CAN Spec", getProject()->GetCANSpecificationPath());
}
