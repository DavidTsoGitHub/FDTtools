#include "ReviewProjectWizardPage.h"
#include "ui_ReviewProjectWizardPage.h"

#include "Logger.h"
#include "Project.h"
#include "ProjectSettings.h"
#include "ProjectSerializer.h"
#include "SignalMappingSerializer.h"
#include "LayoutDefinitionSerializer.h"
#include "LabelDefinitionSerializer.h"
#include "MenuDefinitionSerializer.h"
#include "AnimationSerializer.h"
#include "CheckMessagesSerializer.h"
#include "SegmentDisplaySerializer.h"
#include "IconSerializer.h"
#include "ImageSerializer.h"
#include "DiagDefinitionSerializer.h"
#include "Exceptions/CannotWriteFileException.h"

#include <QMessageBox>
#include <QTextStream>
#include <QWizardPage>

ReviewProjectWizardPage::ReviewProjectWizardPage(ProjectSettings& projectSettings, QWidget* parent) :
    QWizardPage(parent), m_ui(new Ui::VerifyProjectWizardPage),  m_projectSettings(projectSettings)
{
    m_ui->setupUi(this);
}

ReviewProjectWizardPage::~ReviewProjectWizardPage()
{
    delete m_ui;
}

void ReviewProjectWizardPage::initializePage()
{
    QStringList rows;
    rows.append("Project Name: " + m_projectSettings.ProjectName);
    rows.append("Project Root Folder: " + m_projectSettings.ProjectRoot.absolutePath());
    rows.append("BSP Specification: " + m_projectSettings.BSPSpecificationPath.absoluteFilePath());
    if (m_projectSettings.HasCANSpecification)
    {
        rows.append("CAN Specification: " + m_projectSettings.CANSpecificationPath.absoluteFilePath());
        rows.append("CAN ECU Name: " + m_projectSettings.CANECUName);
    }
    m_ui->ProjectSettingsOutput->setText(rows.join("\n"));
}

bool ReviewProjectWizardPage::validatePage()
{
    return finalize();
}

bool ReviewProjectWizardPage::finalize()
{
    return setupProjectRootFolder() &&
           copySpecificationFiles() &&
            createProjectFiles();
}

bool ReviewProjectWizardPage::setupProjectRootFolder()
{
    LOG_DEBUG("Entering");
    QDir dir(m_projectSettings.ProjectRoot);

    if (dir.entryList().count() > 2) // entryList() includes the special directories "." and ".."
    {
        QMessageBox::StandardButton response = QMessageBox::question(this, "Confirm possible overwrite", "Selected project root folder is not empty, all files will be replaced, is this OK?");
        if (response != QMessageBox::Yes) return false;

        LOG_DEBUG("Cleaning output folder");
        if (!dir.removeRecursively())
        {
            LOG_ERROR("Failed to clean output directory!");
            QMessageBox::warning(this, "Failed to clean output directory!", "Attempting to clean the output directory failed, try to remove the files manually and try again.", QMessageBox::Ok);
            return false;
        }
    }

    if (!dir.mkpath("."))
    {
        LOG_ERROR("Failed to create output directory!");
        QMessageBox::warning(this, "Failed to create output directory!", "Attempting to create the output folder failed!", QMessageBox::Ok);
        return false;
    }

    dir.refresh();

    return true;
}

bool ReviewProjectWizardPage::copySpecificationFiles()
{
    QString bspOutputPath(m_projectSettings.ProjectRoot.absoluteFilePath(m_projectSettings.BSPSpecificationPath.fileName()));
    QFile bspFile(m_projectSettings.BSPSpecificationPath.absoluteFilePath());

    if (!bspFile.copy(bspOutputPath))
    {
        QMessageBox::warning(this, "File copy failed!", "Failed to copy the BSP specification file to the output folder! \n" + bspFile.errorString(), QMessageBox::Ok);
        return false;
    }

    if (m_projectSettings.HasCANSpecification)
    {
        QString canOutputPath(m_projectSettings.ProjectRoot.absoluteFilePath(m_projectSettings.CANSpecificationPath.fileName()));
        QFile canFile(m_projectSettings.CANSpecificationPath.absoluteFilePath());

        if (!canFile.copy(canOutputPath))
        {
            QMessageBox::warning(this, "File copy failed!", "Failed to copy the CAN specification file to the output folder! \n" + canFile.errorString(), QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

bool ReviewProjectWizardPage::createProjectFiles()
{
    return createProjectDefinitionFile() &&
           createSignalMappingFile() &&
           createMenuDefinitionFile() &&
           createLabelDefinitionFile() &&
           createCheckMessagesFile() &&
           createIconDefinitionFile() &&
           createImageDefinitionFile() &&
           createDiagDefinitionFile() &&
           createAnimationDefinitionFile() &&
           createFontsDirectory() &&
           createIconsDirectory() &&
           createImagesDirectory() &&
           createLayoutFile() &&
           createSegmentDisplayFile();
}

bool ReviewProjectWizardPage::createProjectDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(m_projectSettings.ProjectName + ".fdt"));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    Project project;
    project.SetProjectName(m_projectSettings.ProjectName);
    project.SetBSPSpecificationPath(m_projectSettings.BSPSpecificationPath.fileName());

    if (m_projectSettings.HasCANSpecification)
    {
        project.SetCANSpecificationPath(m_projectSettings.CANSpecificationPath.fileName());
        project.SetCANClusterECUName(m_projectSettings.CANECUName);
    }

    ProjectSerializer projectSerializer(project);
    QString projectFileContents = projectSerializer.Serialize();
    QTextStream projectFileStream(&file);
    projectFileStream << projectFileContents;
    projectFileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createSignalMappingFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::SignalMappingPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    Project p;
    SignalMappingSerializer signalMappingSerializer(p);
    QString fileContents = signalMappingSerializer.Serialize();

    QTextStream mappingFileStream(&file);
    mappingFileStream << fileContents;
    mappingFileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createMenuDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::MenuDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    MenuDefinition* menudefinition = MenuDefinition::GetEmptyMenuDefinition();

    MenuDefinitionSerializer menuDefinitionSerializer(*menudefinition);
    QString fileContents = menuDefinitionSerializer.Serialize();

    QTextStream fileStream(&file);
    fileStream << fileContents;
    fileStream.flush();

    delete menudefinition;

    return true;
}

bool ReviewProjectWizardPage::createLabelDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::LabelDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    LabelDefinition labelDefinition;

    LabelDefinitionSerializer labelDefinitionSerializer(labelDefinition);
    QString fileContents = labelDefinitionSerializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(labelDefinitionSerializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createCheckMessagesFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::CheckMessagesDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    CheckMessageDefinition checkMessagesDefinition;

    CheckMessagesSerializer serializer(checkMessagesDefinition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createIconDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::IconDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    IconDefinition definition;

    IconSerializer serializer(definition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createImageDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::ImageDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    ImageDefinition definition;

    ImageSerializer serializer(definition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createDiagDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::DiagDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    DiagDefinition definition;

    DiagDefinitionSerializer serializer(definition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createAnimationDefinitionFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::AnimationDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    AnimationDefinition definition;

    AnimationSerializer serializer(definition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createLayoutFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::LayoutDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    LayoutDefinition layoutDefinition;
    LayoutDefinitionSerializer serializer(layoutDefinition);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createSegmentDisplayFile()
{
    QFile file(m_projectSettings.ProjectRoot.absoluteFilePath(Project::SegmentDefinitionPath));

    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo fileInfo(file);
        throw CannotWriteFileException(fileInfo, file.errorString());
    }

    SegmentDisplay display;
    display.SetEnabled(false);

    SegmentDisplaySerializer serializer(display);
    QString fileContents = serializer.Serialize();

    QTextStream fileStream(&file);
    fileStream.setCodec(serializer.GetEncoding());
    fileStream << fileContents;
    fileStream.flush();

    return true;
}

bool ReviewProjectWizardPage::createFontsDirectory()
{
    bool result = m_projectSettings.ProjectRoot.mkdir(Project::FontsDirectoryName);
    if (!result)
    {
        QMessageBox::warning(this, "Error while creating fonts directory", "Couldn't create the \"Fonts\" subdirectory in the project root!");
        return false;
    }
    return true;
}

bool ReviewProjectWizardPage::createIconsDirectory()
{
    bool result = m_projectSettings.ProjectRoot.mkdir(Project::IconsDirectoryName);
    if (!result)
    {
        QMessageBox::warning(this, "Error while creating icons directory", "Couldn't create the \"Icons\" subdirectory in the project root!");
        return false;
    }
    return true;
}

bool ReviewProjectWizardPage::createImagesDirectory()
{
    bool result = m_projectSettings.ProjectRoot.mkdir(Project::ImagesDirectoryName);
    if (!result)
    {
        QMessageBox::warning(this, "Error while creating images directory", "Couldn't create the \"Images\" subdirectory in the project root!");
        return false;
    }
    return true;
}
