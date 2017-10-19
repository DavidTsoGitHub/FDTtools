#include "MenuTreeModel.h"

#include "Label.h"
#include "MenuDefinition.h"
#include "MenuItem.h"
#include "Project.h"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>

MenuTreeModel::MenuTreeModel(Project* project, QObject* parent) : QAbstractItemModel(parent),
    m_project(project),
    m_menuDefinition(project->GetLayoutDefinition().GetMenu())
{
    connect(this, &MenuTreeModel::rowsAboutToBeInserted, this, &MenuTreeModel::signal_modified);
    connect(this, &MenuTreeModel::rowsAboutToBeRemoved, this, &MenuTreeModel::signal_modified);
    connect(this, &MenuTreeModel::rowsAboutToBeMoved, this, &MenuTreeModel::signal_modified);
    connect(this, &MenuTreeModel::modelAboutToBeReset, this, &MenuTreeModel::signal_modified);
}

QModelIndex MenuTreeModel::FindIndexForMenuItem(const MenuItem* menuItem, const QModelIndex& parent)
{
    for (int i=0; i<rowCount(parent); ++i)
    {
        QModelIndex current = index(i, 0, parent);
        if (current.internalPointer() == menuItem)
        {
            return current;
        }

        QModelIndex foundChild = FindIndexForMenuItem(menuItem, current);
        if (foundChild.isValid()) return foundChild;
    }

    return QModelIndex();
}

void MenuTreeModel::AddMenuItem(const QModelIndex& parent)
{
    if (parent.isValid())
    {
        beginInsertRows(parent, rowCount(parent), rowCount(parent));
        ProjectObject* currentMenuObject = static_cast<ProjectObject*>(parent.internalPointer());

        if (currentMenuObject == m_menuDefinition->MenuRootPointer())
        {
            m_menuDefinition->MenuRoot().AddMenuItem(new MenuItem("NewMenuItem"));
        }
        else
        {
            MenuItem* menuItem = static_cast<MenuItem*>(currentMenuObject);
            menuItem->AddSubMenuItem(new MenuItem("NewMenuItem"));
        }
        endInsertRows();
    }
}

void MenuTreeModel::RemoveMenu()
{
    RootMenuItemContainer* rootMenu = m_menuDefinition->MenuRootPointer();
    rootMenu->blockSignals(true);
    foreach (MenuItem* menuItem, rootMenu->MenuItems())
    {
        menuItem->blockSignals(true);
        RemoveSubMenuItem(menuItem, true);
        rootMenu->RemoveMenuItem(menuItem);
    }
    rootMenu->blockSignals(false);
    rootMenu->NotifyPropertyUpdated();
    m_menuDefinition = m_menuDefinition->GetEmptyMenuDefinition();
}

void MenuTreeModel::RemoveSubMenuItem(MenuItem* menuItem, bool blockSignals)
{
    if (blockSignals)
    {
        menuItem->blockSignals(true);
    }

    if (menuItem->SubMenuItems().count() > 0)
    {
        foreach (MenuItem* subMenuItem, menuItem->SubMenuItems())
        {
            RemoveSubMenuItem(subMenuItem, blockSignals);
        }
    }
    MenuItem* parent = dynamic_cast<MenuItem*>(menuItem->GetParent());

    if (parent != NULL)
    {
        parent->RemoveSubMenuItem(menuItem, blockSignals);
    }
}

void MenuTreeModel::RemoveMenuItem(const QModelIndex& indexToRemove)
{
    if (!indexToRemove.isValid()) return;
    beginRemoveRows(indexToRemove.parent(), indexToRemove.row(), indexToRemove.row());

    MenuItem* menuItemToRemove = static_cast<MenuItem*>(indexToRemove.internalPointer());

    ProjectObject* parent = menuItemToRemove->GetParent();
    MenuItem* parentItem = dynamic_cast<MenuItem*>(parent);
    if (parentItem != NULL)
    {
        parentItem->RemoveSubMenuItem(menuItemToRemove);
    }
    else
    {
        RootMenuItemContainer* parentRoot = dynamic_cast<RootMenuItemContainer*>(parent);
        if (parentRoot != NULL)
        {
            parentRoot->RemoveMenuItem(menuItemToRemove);
        }
    }

    endRemoveRows();
}

void MenuTreeModel::MoveMenuItemUp(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || indexToMove.row() == 0) return;

    MenuItem* menuItemToMove = static_cast<MenuItem*>(indexToMove.internalPointer());
    ProjectObject* parentObject = menuItemToMove->GetParent();

    MenuItem* parentItem = dynamic_cast<MenuItem*>(parentObject);
    if (parentItem != NULL)
    {
        beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() - 1);
        parentItem->MoveSubMenuItemUp(menuItemToMove);
        endMoveRows();
        return;
    }

    RootMenuItemContainer* parentRoot = dynamic_cast<RootMenuItemContainer*>(parentObject);
    if (parentRoot != NULL)
    {
        beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() - 1);
        parentRoot->MoveMenuItemUp(menuItemToMove);
        endMoveRows();
        return;
    }
}

void MenuTreeModel::MoveMenuItemDown(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || (rowCount(indexToMove.parent()) - indexToMove.row() <= 1)) return;

    MenuItem* menuItemToMove = static_cast<MenuItem*>(indexToMove.internalPointer());
    ProjectObject* parentObject = menuItemToMove->GetParent();

    MenuItem* parentItem = dynamic_cast<MenuItem*>(parentObject);
    if (parentItem != NULL)
    {
        beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() + 2);
        parentItem->MoveSubMenuItemDown(menuItemToMove);
        endMoveRows();
        return;
    }

    RootMenuItemContainer* parentRoot = dynamic_cast<RootMenuItemContainer*>(parentObject);
    if (parentRoot != NULL)
    {
        beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() + 2);
        parentRoot->MoveMenuItemDown(menuItemToMove);
        endMoveRows();
        return;
    }
}

/* QAbstractItemModel overrides*/

QModelIndex MenuTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();
    if (!parent.isValid()) return createIndex(row, column, m_menuDefinition->MenuRootPointer());

    ProjectObject* parentObject = static_cast<ProjectObject*>(parent.internalPointer());

    MenuItem* parentMenu = dynamic_cast<MenuItem*>(parentObject);
    if (parentMenu != NULL)
    {
        MenuItem* childItem = parentMenu->SubMenuItems().at(row);
        return createIndex(row, column, childItem);
    }

    RootMenuItemContainer* menuRoot = dynamic_cast<RootMenuItemContainer*>(parentObject);
    if (menuRoot != NULL)
    {
        MenuItem* childItem = menuRoot->MenuItems().at(row);
        return createIndex(row, column, childItem);
    }

    qWarning("%s parent is undetermined", __FUNCTION__);
    return QModelIndex();
}

QModelIndex MenuTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) return QModelIndex();

    ProjectObject* childObject = static_cast<ProjectObject*>(child.internalPointer());
    if (childObject == m_menuDefinition->MenuRootPointer()) return QModelIndex();

    ProjectObject* parentObject = childObject->GetParent();
    if (parentObject == NULL) return QModelIndex();
    MenuItem* parentMenu = dynamic_cast<MenuItem*>(parentObject);
    if (parentMenu != NULL)
    {
        RootMenuItemContainer* parentsParentAsRoot = dynamic_cast<RootMenuItemContainer*>(parentMenu->GetParent());
        if (parentsParentAsRoot != NULL)
        {
            return createIndex(parentsParentAsRoot->MenuItems().indexOf(parentMenu), 0, parentMenu);
        }

        MenuItem* parentsParentAsMenuItem = dynamic_cast<MenuItem*>(parentMenu->GetParent());
        if (parentsParentAsMenuItem != NULL)
        {
            return createIndex(parentsParentAsMenuItem->SubMenuItems().indexOf(parentMenu), 0, parentMenu);
        }

        qWarning("%s parent is MenuItem and parent's parent is undetermined", __FUNCTION__);
        return QModelIndex();
    }

    RootMenuItemContainer* parentMenuRoot = dynamic_cast<RootMenuItemContainer*>(parentObject);
    if (parentMenuRoot != NULL)
    {
        return createIndex(0, 0, parentMenuRoot);
    }

    qWarning("%s parent is undetermined", __FUNCTION__);
    return QModelIndex();
}

int MenuTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0) return 0;
    if (!parent.isValid()) return 1;

    ProjectObject* parentObject = static_cast<ProjectObject*>(parent.internalPointer());
    MenuItem* parentMenu = dynamic_cast<MenuItem*>(parentObject);
    if (parentMenu != NULL)
    {
        return parentMenu->SubMenuItems().size();
    }

    RootMenuItemContainer* menuRoot = dynamic_cast<RootMenuItemContainer*>(parentObject);
    if (menuRoot != NULL)
    {
        return menuRoot->MenuItems().size();
    }

    qWarning("%s %d parent couldn't be determined!", __FUNCTION__, 0);
    return 0;
}

int MenuTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

QVariant MenuTreeModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        ProjectObject* object = static_cast<ProjectObject*>(index.internalPointer());

        MenuItem* menuItem = dynamic_cast<MenuItem*>(object);
        if (menuItem)
        {
            if (menuItem->GetLabelID() > 0)
            {
                return m_project->GetLabelDefinition().GetLabel(menuItem->GetLabelID())->GetMasterText();
            }
            else
            {
                return menuItem->GetName();
            }
        }

        return QString("Menu Root");
    }
    return QVariant();
}
