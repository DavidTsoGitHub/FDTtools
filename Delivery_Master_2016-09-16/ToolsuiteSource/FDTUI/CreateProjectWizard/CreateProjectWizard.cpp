#include "CreateProjectWizard.h"
#include "ui_CreateProjectWizard.h"

#include "ProjectDefinitionWizardPage.h"
#include "ReviewProjectWizardPage.h"
#include <QWizard>

CreateProjectWizard::CreateProjectWizard(QWidget* parent) : QWizard(parent), m_ui(new Ui::CreateProjectWizard)
{
    m_ui->setupUi(this);
    m_projectSettings.HasCANSpecification = false;
    m_definitionPage = new ProjectDefinitionWizardPage(m_projectSettings);
    m_verificationPage = new ReviewProjectWizardPage(m_projectSettings);

    this->addPage(m_definitionPage);
    this->addPage(m_verificationPage);
}

CreateProjectWizard::~CreateProjectWizard()
{
    delete m_definitionPage;
    delete m_verificationPage;
    delete m_ui;
}

QString CreateProjectWizard::GetProjectFilePath()
{
    return m_projectSettings.ProjectRoot.absoluteFilePath(m_projectSettings.ProjectName.append(".fdt"));
}
