#ifndef PROJECTDEFINITIONWIZARDPAGE_H
#define PROJECTDEFINITIONWIZARDPAGE_H

class ProjectSettings;
class QWizardPage;

namespace Ui
{
class ProjectDefinitionWizardPage;
}

class ProjectDefinitionWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProjectDefinitionWizardPage(ProjectSettings& projectSettings, QWidget* parent = 0);
    ~ProjectDefinitionWizardPage(void);

    bool isComplete(void) const;


private:

    ProjectSettings& m_projectSettings;
    Ui::ProjectDefinitionWizardPage* m_ui;

    void refreshCANECUNames(void);
    void setupSignals(void);
    bool isValidProjectPath(QString filename);

private slots:
    void slot_projectNameChanged(const QString& name);
    void slot_projectRootChanged(const QString& dir);
    void slot_bspSpecificationChanged(const QString& bspPath);
    void slot_canSpecificationEdited(const QString& canPath);
    void slot_canSpecificationEditFinished(void);
    void slot_canECUNameChanged(const QString& canECUName);

    void slot_selectProjectRootButtonClicked(void);
    void slot_selectCANSpecificationButtonClicked(void);
    void slot_selectBSPSpecificationButtonClicked(void);
};

#endif // PROJECTDEFINITIONWIZARDPAGE_H
