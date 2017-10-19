#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

class IconModel;
class Project;
class ProjectObject;
class QDir;
class QWidget;

namespace Ui
{
class IconManager;
}

class IconManager : public QWidget
{
    Q_OBJECT

public:
    explicit IconManager(QWidget *parent = 0);
    ~IconManager();

signals:
    void signal_modified();
    void signal_iconSelectionChanged(ProjectObject* projectObject);

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void JustSelected();

private:
    Ui::IconManager* m_ui;
    Project* m_project;
    IconModel* m_iconModel;
    QDir m_projectRoot;
    ProjectObject* m_currentSelectedIcon;

private slots:
    void slot_iconSelectionChanged(const QItemSelection&selected, const QItemSelection&);
    void slot_addIconButtonClicked();
    void slot_removeIconButtonClicked();
    void slot_replaceIconButtonClicked();
};

#endif // RESOURCEMANAGER_H
