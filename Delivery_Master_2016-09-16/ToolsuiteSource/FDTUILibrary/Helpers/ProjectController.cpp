#include "ProjectController.h"

#include "DirtyObjectsVisitor.h"
#include "Exceptions/CannotOpenFileException.h"
#include "Exceptions/DeserializationExeception.h"
#include "Exceptions/ProjectException.h"
#include "Project.h"
#include "ProjectDeserializer.h"
#include "ProjectSerializerVisitor.h"
#include "RegEx/RegExPatterns.h"
#include "SettingsHandler.h"
#include "ProjectUpdaterDialog.h"

#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>

#include <Exceptions/ProjectInitializationException.h>

ProjectController::ProjectController(QObject* parent) : QObject(parent),
    m_project(NULL),
    m_parentWidget(qobject_cast<QWidget*>(parent))
{}

ProjectController::~ProjectController()
{
    delete m_project;
}

const QDateTime ProjectController::GetLastSavedTime()
{
    if (m_project == NULL) return QDateTime();

    QFileInfo projectfile(m_project->GetProjectFile());
    projectfile.refresh();
    return projectfile.lastModified();
}

void ProjectController::OpenProject(const QFileInfo& projectFile)
{
    CloseProject();

    if (m_project == NULL)
    {
        openProject(projectFile);
    }

    if (m_project != NULL)
    {
        m_lastProjectRoot = m_project->GetProjectRoot().absolutePath();
    }
}

void ProjectController::ReloadProject()
{
    if (m_project != NULL)
    {
        OpenProject(m_project->GetProjectFile());
    }
}

void ProjectController::CloseProject()
{
    if (m_project != NULL)
    {
        DirtyObjectsVisitor dirtyFinder;
        m_project->Accept(dirtyFinder);
        if (dirtyFinder.GetDirtyObjects().size() > 0)
        {
            qDebug("Closing project, dirty objects:");
            foreach (const ProjectObject* dirtyObject, dirtyFinder.GetDirtyObjects())
            {
                qDebug("  %s", qUtf8Printable(dirtyObject->objectName()));
            }

            QMessageBox::StandardButton result = QMessageBox::information(m_parentWidget, "Save changes?", "You have unsaved changes, save before proceeding?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

            switch (result)
            {
                case QMessageBox::Cancel:
                    return;
                case QMessageBox::Yes:
                    saveProject();
                case QMessageBox::No:
                    closeProject();
                    break;
                default:
                    break;
            }
        }
        else
        {
            closeProject();
        }
    }
}

void ProjectController::SaveProject()
{
    if (m_project != NULL)
    {
        saveProject();
    }
}

void ProjectController::openProject(const QFileInfo& projectFile)
{
    if (m_project != NULL)
    {
        qWarning("m_project != NULL, m_project should be deleted/NULL:ed before opening a new project");
    }

    if (!validateFunctionLibraryPath())
    {
        return;
    }

    if(!isValidFileName(projectFile.absoluteFilePath()))
    {
        delete m_project;
        emit ProjectLoadError(projectFile, "Compilers can't handle file paths with spaces or special characters, please move project to a more suitable path and reopen.");
        return;
    }

    QFile file(projectFile.absoluteFilePath());
    if (!file.open(QFile::ReadOnly))
    {
        delete m_project;
        emit ProjectLoadError(projectFile, file.errorString());
        return;
    }

    ProjectUpdaterDialog::UpgradeStatus upgradeResult;
    upgradeResult = checkProjectFilesForVersion(projectFile, m_project);
    if (upgradeResult == ProjectUpdaterDialog::UpgradeFailed)
    {
        delete m_project;
        emit ProjectLoadError(projectFile, "Project version update failed");
        return;
    }
    else if(upgradeResult == ProjectUpdaterDialog::UpgradeSkipped)
    {
        delete m_project;
        return;
    }

    QString errorText;
    bool result;
    QTextStream projectXml(&file);
    m_project = ProjectDeserializer::Deserialize(projectXml.readAll(), &result, &errorText);

    if (!result)
    {
        delete m_project;
        emit ProjectLoadError(projectFile, errorText);
        return;
    }

    SettingsHandler settingsHandler;
    try
    {
        m_project->Initialize(projectFile, QDir(settingsHandler.GetToolSettings().FunctionLibraryPath));
    }
    catch(ProjectInitializationException ex)
    {
        delete m_project;
        m_project = NULL;
        throw ProjectInitializationException("Project initialization unsuccessfully.");
    }

    if(!m_project->IsInitialized())
    {
        delete m_project;
        m_project = NULL;
        return;
    }

    emit ProjectLoaded(m_project);
}

void ProjectController::saveProject()
{
    if (m_project == NULL)
    {
        qWarning("m_project == NULL");
        return;
    }

    ProjectSerializerVisitor serializer(m_project->GetProjectRoot());
    m_project->Accept(serializer);

    if (serializer.Result() == ProjectSerializerVisitor::Success)
    {
        m_project->ClearDirtyRecursive();
        emit ProjectSaved();
    }
    else
    {
        emit ProjectSaveError(serializer.ErrorString());
    }
}

void ProjectController::closeProject()
{
    delete m_project;
    m_project = NULL;
    emit ProjectClosed();
}

bool ProjectController::isValidFileName(QString filename)
{
    bool isValid = true;
    for(int i = 0; i < filename.size(); ++i)
    {
        if(filename.at(i).unicode() > 127 ||     // Non Ascii
            filename.at(i).unicode() < 32 ||     // Unprintable
            filename.at(i).unicode() == 32 ||    // Space
            filename.at(i).unicode() == 34  ||   // "
            filename.at(i).unicode() == 42  ||   // *
            ((filename.at(i).unicode() == 58) && (i != 1))  ||   // : on other position then 2:nd
            filename.at(i).unicode() == 60  ||   // <
            filename.at(i).unicode() == 62  ||   // >
            filename.at(i).unicode() == 63  ||   // ?
            filename.at(i).unicode() == 124 ||   // |
            filename.at(i).unicode() == 127 )    // DEL
        {
            isValid = false;
        }
    }

    return isValid;
}

bool ProjectController::validateFunctionLibraryPath()
{
    bool result = true;
    QSettings settings;

    QString functionLibraryPath = settings.value("FunctionLibraryPath").toString();
    QString platformGeneratorFilePath = settings.value("PlatformGeneratorPath").toString();
    QString simulatorFilePath = settings.value("SimulatorFilePath").toString();
    QString builderFilePath = settings.value("BuilderFilePath").toString();

    m_warnings.clear();

    if (functionLibraryPath.isEmpty() || !QDir(functionLibraryPath).exists())
    {
        m_warnings.append("Error Loading Project!\n\nFunction Library Path hasn't been set or is incorrect.");
        emit ProjectFunctionLibraryPathError(m_warnings.join("\n") + "\n\nPlease update the path in Settings.");
        result = false;
    }

    return result;
}

ProjectUpdaterDialog::UpgradeStatus ProjectController::checkProjectFilesForVersion(const QFileInfo& projectFile, Project* project)
{
    ProjectUpdaterDialog projectUpdaterDialog(projectFile, project);
    if(projectUpdaterDialog.IsUpdateRequired())
    {
        projectUpdaterDialog.exec();

        return projectUpdaterDialog.GetUpdateSuccessful();
    }

    return ProjectUpdaterDialog::UpgradeSuccessful;
}
