#include "ProjectBrowserModel.h"

#include "Project.h"
#include "ProjectObject.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>

ProjectBrowserModel::ProjectBrowserModel(Project* project, QObject* parent) :
    QAbstractItemModel(parent),
    m_project(project)
{
    connect(project, &Project::PropertyUpdated, this, &ProjectBrowserModel::Reset);
    connect(this, &ProjectBrowserModel::rowsAboutToBeInserted, this, &ProjectBrowserModel::signal_modified);
    connect(this, &ProjectBrowserModel::rowsAboutToBeRemoved, this, &ProjectBrowserModel::signal_modified);
    connect(this, &ProjectBrowserModel::rowsAboutToBeMoved, this, &ProjectBrowserModel::signal_modified);
    connect(this, &ProjectBrowserModel::modelAboutToBeReset, this, &ProjectBrowserModel::signal_modified);
}

void ProjectBrowserModel::RemoveFunctionBlock(const QModelIndex& indexToRemove)
{
    beginRemoveRows(indexToRemove.parent(), indexToRemove.row(), indexToRemove.row());
    FunctionBlock* functionBlockToDelete = static_cast<FunctionBlock*>(indexToRemove.internalPointer());
    m_project->RemoveFunctionBlock(functionBlockToDelete);
    endRemoveRows();
}

QModelIndex ProjectBrowserModel::FindIndexForObject(ProjectObject* object, const QModelIndex& parent)
{
    for (int i=0; i<rowCount(parent); ++i)
    {
        QModelIndex current = index(i, 0, parent);
        if (current.internalPointer() == object)
        {
            return current;
        }

        QModelIndex foundChild = FindIndexForObject(object, current);
        if (foundChild.isValid()) return foundChild;
    }

    return QModelIndex();
}

void ProjectBrowserModel::Reset()
{
    beginResetModel();
    endResetModel();
}

bool ProjectBrowserModel::isFilteredNode(ProjectObject* node) const
{
    if (dynamic_cast<CANSpecification*>(node))
    {
        if (m_project->GetCANSpecificationPath().isEmpty())
        {
            return true;
        }
    }

    return false;
}

QModelIndex ProjectBrowserModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();
    if (!parent.isValid())
    {
        return createIndex(row, column, m_project);
    }

    ProjectObject* parentItem = static_cast<ProjectObject*>(parent.internalPointer());
    if (row < parentItem->GetEnabledChildren().size())
    {
        ProjectObject* childItem = parentItem->GetEnabledChildren().at(row);
//        if (isFilteredNode(childItem)) {
//            return QModelIndex();
//        }

        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProjectBrowserModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) return QModelIndex();

    ProjectObject* childItem = static_cast<ProjectObject*>(child.internalPointer());
    ProjectObject* parentItem = childItem->GetParent();
    if (parentItem == NULL) return QModelIndex();

    if (parentItem->parent() == NULL)
    {
        return createIndex(0, 0, parentItem);
    }
    else
    {
        int row = parentItem->GetParent()->GetEnabledChildren().indexOf(parentItem);
        return createIndex(row, 0, parentItem);
    }
}

int ProjectBrowserModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0) return 0;
    if (!parent.isValid()) return 1;

    ProjectObject* parentObject = static_cast<ProjectObject*>(parent.internalPointer());
    return parentObject->GetEnabledChildren().size();
}

int ProjectBrowserModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

QVariant ProjectBrowserModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    ProjectObject* object = static_cast<ProjectObject*>(index.internalPointer());
    return object->objectName();
}
