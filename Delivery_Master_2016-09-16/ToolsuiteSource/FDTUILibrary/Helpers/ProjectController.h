#ifndef PROJECTHANDLER_H
#define PROJECTHANDLER_H

#include <QObject>
#include <QStringList>
#include "ProjectUpdaterDialog.h"

class Project;
class QDateTime;
class QFileInfo;

class ProjectController : public QObject
{
    Q_OBJECT

public:
    explicit ProjectController(QObject* parent = 0);
    ~ProjectController(void);

    const QDateTime GetLastSavedTime();

public slots:
    void SaveProject();
    void CloseProject();
    void OpenProject(const QFileInfo& projectFile);
    void ReloadProject();

signals:
    void ProjectFunctionLibraryPathError(const QString& errorMessage);
    void ProjectLoadError(const QFileInfo& projectFile, const QString& errorMessage);
    void ProjectSaveError(const QString& errorMessage);
    void ProjectLoaded(Project* project);
    void ProjectClosed();
    void ProjectSaved();

private slots:

private:
    Project* m_project;
    QString m_lastProjectRoot;
    QStringList m_warnings;
    QWidget* m_parentWidget;
    void saveProject();
    void closeProject();
    void openProject(const QFileInfo& projectFile);
    bool isValidFileName(QString filename);
    bool validateFunctionLibraryPath();
    ProjectUpdaterDialog::UpgradeStatus checkProjectFilesForVersion(const QFileInfo& projectFile, Project* project);
};

#endif // PROJECTHANDLER_H
