#include "MenuItemWidgetsModel.h"

#include "MenuItem.h"
#include "UIWidgetBase.h"

#include <QAbstractListModel>
#include <QModelIndex>

MenuItemWidgetsModel::MenuItemWidgetsModel(MenuItem* menuItem, QObject* parent) : QAbstractListModel(parent),
    m_menuItem(menuItem)
{
    connect(m_menuItem, &MenuItem::PropertyUpdated, this, &MenuItemWidgetsModel::MenuItemPropertyUpdated);
    connect(this, &MenuItemWidgetsModel::rowsAboutToBeInserted, this, &MenuItemWidgetsModel::signal_modified);
    connect(this, &MenuItemWidgetsModel::rowsAboutToBeRemoved, this, &MenuItemWidgetsModel::signal_modified);
    connect(this, &MenuItemWidgetsModel::rowsAboutToBeMoved, this, &MenuItemWidgetsModel::signal_modified);
    connect(this, &MenuItemWidgetsModel::modelAboutToBeReset, this, &MenuItemWidgetsModel::signal_modified);
}

void MenuItemWidgetsModel::MenuItemPropertyUpdated()
{
    beginResetModel();
    endResetModel();
}

void MenuItemWidgetsModel::RemoveWidget(const QModelIndex& indexToRemove)
{
    if (!indexToRemove.isValid()) return;

    UIWidgetBase* widgetToRemove = static_cast<UIWidgetBase*>(indexToRemove.internalPointer());

    beginRemoveRows(indexToRemove.parent(), indexToRemove.row(), indexToRemove.row());
    m_menuItem->RemoveUIWidget(widgetToRemove);
    endRemoveRows();
}

QModelIndex MenuItemWidgetsModel::FindIndexForWidget(UIWidgetBase* uiWidget)
{
    for (int i=0; i<m_menuItem->UIWidgets().size(); ++i)
    {
        UIWidgetBase* current = m_menuItem->UIWidgets().at(i);
        if (current == uiWidget)
        {
            return createIndex(i, 0, current);
        }
    }

    return QModelIndex();
}

QModelIndex MenuItemWidgetsModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    UIWidgetBase* widget = m_menuItem->UIWidgets().at(row);

    return createIndex(row, column, widget);
}

int MenuItemWidgetsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_menuItem->UIWidgets().size();
}

QVariant MenuItemWidgetsModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
        {
            UIWidgetBase* widget = static_cast<UIWidgetBase*>(index.internalPointer());
            return widget->GetUserDefinedName();
        }
        case Qt::DecorationRole:
        {
            UIWidgetBase* uiWidget = static_cast<UIWidgetBase*>(index.internalPointer());

            const QString iconFileName = ":/ToolIcons/" + uiWidget->GetUIWidgetTypeName() + "Icon.bmp";
            // If the file is not found, the pixmap will become a null pixmap (0x0px)
            return QPixmap(iconFileName);
        }
        default:
            return QVariant();
    }
}
