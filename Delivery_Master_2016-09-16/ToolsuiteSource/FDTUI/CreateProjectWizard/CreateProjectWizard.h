#ifndef CREATEPROJECTWIZARD_H
#define CREATEPROJECTWIZARD_H

#include "ProjectSettings.h"

class ProjectDefinitionWizardPage;
class ReviewProjectWizardPage;
class QWizard;

namespace Ui
{
class CreateProjectWizard;
}

class CreateProjectWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CreateProjectWizard(QWidget* parent = 0);
    ~CreateProjectWizard(void);
    QString GetProjectFilePath(void);

private:

    ProjectDefinitionWizardPage* m_definitionPage;
    ReviewProjectWizardPage* m_verificationPage;

    ProjectSettings m_projectSettings;

    Ui::CreateProjectWizard* m_ui;
};

#endif // CREATEPROJECTWIZARD_H
