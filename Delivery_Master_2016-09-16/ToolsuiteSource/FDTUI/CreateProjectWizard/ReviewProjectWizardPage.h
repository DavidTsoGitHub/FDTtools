#ifndef VERIFYPROJECTWIZARDPAGE_H
#define VERIFYPROJECTWIZARDPAGE_H

class ProjectSettings;
class QWizardPage;

namespace Ui
{
class VerifyProjectWizardPage;
}

class ReviewProjectWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ReviewProjectWizardPage(ProjectSettings& projectSettings, QWidget* parent = 0);
    ~ReviewProjectWizardPage(void);

    void initializePage(void);
    bool validatePage(void);

private:
    Ui::VerifyProjectWizardPage* m_ui;
    ProjectSettings& m_projectSettings;

    bool finalize(void);
    bool setupProjectRootFolder(void);
    bool copySpecificationFiles(void);
    bool createProjectFiles(void);
    bool createProjectDefinitionFile(void);
    bool createSignalMappingFile(void);
    bool createMenuDefinitionFile(void);
    bool createLabelDefinitionFile(void);
    bool createFontsDirectory(void);
    bool createCheckMessagesFile(void);
    bool createIconDefinitionFile(void);
    bool createImageDefinitionFile(void);
    bool createDiagDefinitionFile(void);
    bool createAnimationDefinitionFile(void);
    bool createIconsDirectory(void);
    bool createImagesDirectory(void);
    bool createProjectPropertiesFile(void);
    bool createLayoutFile(void);
    bool createSegmentDisplayFile(void);
};

#endif // VERIFYPROJECTWIZARDPAGE_H
