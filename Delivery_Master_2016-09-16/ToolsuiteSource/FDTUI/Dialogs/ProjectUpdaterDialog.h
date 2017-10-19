#ifndef PROJECTUPDATERDIALOG_H
#define PROJECTUPDATERDIALOG_H

#include <QDialog>
#include <QDir>
#include "XMLUpgrader/VersionNumber.h"

class QFileInfo;
class Project;
class XMLUpgraderBase;

namespace Ui {
class ProjectUpdaterDialog;
}

class ProjectUpdaterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectUpdaterDialog(const QFileInfo& projectFile, Project* project, QWidget *parent = 0);
    ~ProjectUpdaterDialog();
    typedef enum {
        UpgraderIndex = Qt::UserRole + 1
    } UpdaterRoles;

    typedef enum
    {
        UpgradeUnknown = 0,
        UpgradeFailed,
        UpgradeSkipped,
        UpgradeSuccessful
    } UpgradeStatus;

    bool IsUpdateRequired(void);
    UpgradeStatus GetUpdateSuccessful(void);

public slots:
    void slot_applyUpdates(void);
    void slot_selectionChanged(void);
    void slot_cancel();

private:
    Ui::ProjectUpdaterDialog* m_ui;
    Project* m_project;
    const QFileInfo& m_projectFile;

    QList<XMLUpgraderBase*> m_upgraderList;
    QDir m_baseDirPath;

    VersionNumber m_toVersionNumber;
    bool m_updateRequired;
    UpgradeStatus m_upgradeStatus;

    void populateList();
    VersionNumber getCurrentFileVersion(QString fileName);
};

#endif // PROJECTUPDATERDIALOG_H
