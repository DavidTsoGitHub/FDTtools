#include "ProjectBrowser.h"

#include "FunctionBlock.h"
#include "FunctionBlockContainer.h"
#include "Project.h"
#include "ProjectBrowserModel.h"
#include "ProjectObject.h"

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include <QModelIndex>
#include <QSet>
#include <QTreeView>
#include <QWidget>

ProjectBrowser::ProjectBrowser(QWidget *parent) :
    QTreeView(parent),
    m_project(NULL),
    m_projectBrowserModel(NULL),
    m_functionBlockContainer(NULL)
{
    this->header()->setVisible(false);

    connect(this, &ProjectBrowser::expanded, [=] (const QModelIndex& expandedItem)
    {
        m_expandedItems.insert(static_cast<ProjectObject*>(expandedItem.internalPointer()));
    });

    connect(this, &ProjectBrowser::collapsed, [=] (const QModelIndex& expandedItem)
    {
        m_expandedItems.remove(static_cast<ProjectObject*>(expandedItem.internalPointer()));
    });

    connect(this, &ProjectBrowser::clicked, [=] (const QModelIndex& index)
    {
        if (index.isValid())
        {
            if (m_currentIndex == index)
            {
                ProjectObject* currentObject = static_cast<ProjectObject*>(index.internalPointer());

                if(dynamic_cast<LayoutDefinition*>(currentObject) != NULL)
                {
                    emit LayoutChoosenInProjectBrowser();
                }

                emit CurrentProjectObjectChanged(currentObject);
            }
        }
    });
}

ProjectBrowser::~ProjectBrowser()
{
}

void ProjectBrowser::SetProject(Project* project)
{
    m_expandedItems.clear();

    m_project = project;

    m_projectBrowserModel = new ProjectBrowserModel(project);
    setModel(m_projectBrowserModel);
    expandToDepth(0);
    resizeColumnToContents(0);

    connect(selectionModel(), &QItemSelectionModel::currentChanged, this, &ProjectBrowser::selectionModel_currentChanged);
    connect(m_projectBrowserModel, &ProjectBrowserModel::modelReset, this, &ProjectBrowser::restoreExpandedState);
}

void ProjectBrowser::ClearProject()
{
    m_project = NULL;
    delete m_projectBrowserModel;
    m_projectBrowserModel = NULL;
}

void ProjectBrowser::ResetModel()
{
    if(m_projectBrowserModel != NULL)
        m_projectBrowserModel->Reset();
}

void ProjectBrowser::restoreExpandedState()
{
    foreach (ProjectObject* expandedObject, m_expandedItems)
    {
        expand(m_projectBrowserModel->FindIndexForObject(expandedObject));
    }
}

void ProjectBrowser::selectionModel_currentChanged(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    if (!current.isValid()) return;

    m_currentIndex = current;
    ProjectObject* currentObject = static_cast<ProjectObject*>(current.internalPointer());

    if(dynamic_cast<LayoutDefinition*>(currentObject) != NULL)
    {
        emit LayoutChoosenInProjectBrowser();
    }

    emit CurrentProjectObjectChanged(currentObject);
}

void ProjectBrowser::slot_deleteFunctionBlock(void)
{
    if (!m_functionBlockToDelete.isValid())
    {
        return;
    }

    m_projectBrowserModel->RemoveFunctionBlock(m_functionBlockToDelete);
    m_functionBlockToDelete = QModelIndex();
}

void ProjectBrowser::contextMenuEvent(QContextMenuEvent* event)
{
    event->accept();

    if (m_project == NULL)
    {
        return;
    }

    QModelIndex selectedIndex = indexAt(event->pos());

    if (selectedIndex.isValid())
    {
        ProjectObject* projectObject = static_cast<ProjectObject*>(selectedIndex.internalPointer());
        FunctionBlock* functionBlockToDelete = dynamic_cast<FunctionBlock*>(projectObject);
        if (functionBlockToDelete != NULL)
        {
            m_functionBlockToDelete = selectedIndex;
            QMenu menu;
            QAction* deleteAction = menu.addAction(QString("Delete ").append(functionBlockToDelete->GetFunctionDefinition().GetName()));
            connect(deleteAction, &QAction::triggered, this, &ProjectBrowser::slot_deleteFunctionBlock);
            menu.exec(event->globalPos());
        }
    }
}
