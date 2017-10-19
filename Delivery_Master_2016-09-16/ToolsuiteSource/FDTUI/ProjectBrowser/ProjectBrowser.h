#ifndef PROJECTBROWSER_H
#define PROJECTBROWSER_H

class FunctionBlockContainer;
class Project;
class ProjectBrowserModel;
class ProjectObject;

class QModelIndex;
template <class T> class QSet;
class QTreeView;
class QWidget;

class ProjectBrowser : public QTreeView
{
    Q_OBJECT

public:
    explicit ProjectBrowser(QWidget *parent = 0);
    ~ProjectBrowser();

public slots:
    void SetProject(Project* project);
    void ClearProject(void);
    void ResetModel(void);

signals:
    void CurrentProjectObjectChanged(ProjectObject* object);
    void LayoutChoosenInProjectBrowser(void);

private slots:
    void slot_deleteFunctionBlock(void);
    void selectionModel_currentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    Project* m_project;
    ProjectBrowserModel* m_projectBrowserModel;
    FunctionBlockContainer* m_functionBlockContainer;
    QModelIndex m_functionBlockToDelete;
    QModelIndex m_currentIndex;

    QSet<ProjectObject*> m_expandedItems;
    void restoreExpandedState();

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent* event);
};

#endif // PROJECTBROWSER_H
