#include "LayoutsTreeModel.h"

#include "Layout.h"
#include "Project.h"

#include <QAbstractItemModel>
#include "QMessageBox"
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>

LayoutsTreeModel::LayoutsTreeModel(LayoutDefinition& layoutDefinition, QObject* parent) :
    QAbstractItemModel(parent),
    m_layoutDefinition(layoutDefinition)
{
    foreach (Layout* layout, m_layoutDefinition.Layouts())
    {
        foreach (DisplayArea* area, layout->DisplayAreas())
        {
            connect(area, &DisplayArea::WidgetAdded, this, &LayoutsTreeModel::widgetAdded);
        }
    }

    connect(this, &LayoutsTreeModel::rowsAboutToBeInserted, this, &LayoutsTreeModel::signal_modified);
    connect(this, &LayoutsTreeModel::rowsAboutToBeRemoved, this, &LayoutsTreeModel::signal_modified);
    connect(this, &LayoutsTreeModel::rowsAboutToBeMoved, this, &LayoutsTreeModel::signal_modified);
    connect(this, &LayoutsTreeModel::modelAboutToBeReset, this, &LayoutsTreeModel::signal_modified);
}

Layout* LayoutsTreeModel::AddLayout(const QString& name, LayoutDefinition::LayoutType type)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    Layout* layout = new Layout(name, &m_layoutDefinition);
    if(type == LayoutDefinition::Startup)
    {
        layout->SetStartupScreen(true);
    }
    else if(type == LayoutDefinition::CheckMessage)
    {
        layout->SetCheckMessageScreen(true);
    }

    m_layoutDefinition.AddLayout(layout);
    emit signal_layoutAdded(layout->GetID());

    if(type == LayoutDefinition::Startup)
    {
        m_layoutDefinition.SetStartupLayout(layout);
        m_layoutDefinition.SetStartupEnabled(true);
    }
    else if(type == LayoutDefinition::CheckMessage)
    {
        m_layoutDefinition.SetCheckMessageLayout(layout);
    }

    endInsertRows();

    return layout;
}

void LayoutsTreeModel::RemoveLayout(const QModelIndex& indexToRemove, Project* project)
{
    if (!indexToRemove.isValid()) return;

    Layout* layoutToRemove = static_cast<Layout*>(indexToRemove.internalPointer());
    quint32 layoutID = layoutToRemove->GetID();
    layoutToRemove->blockSignals(true);

    if (layoutToRemove == m_layoutDefinition.GetMenuLayout())
    {
        QMessageBox messageBox;
        messageBox.setText("The layout '" + layoutToRemove->Name() + "' contains the menu. You need to remove it in the Menu Editor or move it to another layout before deleting this layout.");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
        return;
    }
    else
    {
        if (layoutToRemove->IsCheckMessageScreen())
        {
            QList<GCLSignalSink*> sinks = QList<GCLSignalSink*>();
            layoutToRemove->GetAllSignalSinks(sinks);

            foreach(GCLSignalSink* sink, sinks)
            {
                project->GetCheckMessagesDefinition().RemoveWidgetSink(sink);
            }
        }

        beginRemoveRows(QModelIndex(), indexToRemove.row(), indexToRemove.row());
        m_layoutDefinition.RemoveLayout(layoutToRemove);
        endRemoveRows();
        emit signal_layoutRemoved(layoutID);
    }
}

void LayoutsTreeModel::MoveLayoutUp(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || indexToMove.row() == 0) return;
    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() - 1);

    Layout* layoutToMove = static_cast<Layout*>(indexToMove.internalPointer());

    m_layoutDefinition.MoveLayoutUp(layoutToMove);
    endMoveRows();
}

void LayoutsTreeModel::MoveLayoutDown(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || (rowCount(indexToMove.parent()) - indexToMove.row() <= 1)) return;

    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() + 2);

    Layout* layoutToMove = static_cast<Layout*>(indexToMove.internalPointer());
    m_layoutDefinition.MoveLayoutDown(layoutToMove);
    endMoveRows();
}

void LayoutsTreeModel::MoveDisplayAreaUp(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || indexToMove.row() == 0) return;
    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() - 1);

    DisplayArea* displayArea = static_cast<DisplayArea*>(indexToMove.internalPointer());
    Layout* layout = static_cast<Layout*>(displayArea->GetParent());

    layout->MoveDisplayAreaUp(displayArea);
    endMoveRows();
}

void LayoutsTreeModel::MoveDisplayAreaDown(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || (rowCount(indexToMove.parent()) - indexToMove.row() <= 1)) return;

    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() + 2);

    DisplayArea* displayArea = static_cast<DisplayArea*>(indexToMove.internalPointer());
    Layout* layout = static_cast<Layout*>(displayArea->GetParent());

    layout->MoveDisplayAreaDown(displayArea);
    endMoveRows();
}

void LayoutsTreeModel::MoveWidgetUp(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || indexToMove.row() == 0) return;
    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() - 1);

    UIWidgetBase* widget = static_cast<UIWidgetBase*>(indexToMove.internalPointer());
    DisplayArea* displayArea = static_cast<DisplayArea*>(widget->GetParent());

    displayArea->MoveWidgetUp(widget);
    endMoveRows();
}

void LayoutsTreeModel::MoveWidgetDown(const QModelIndex& indexToMove)
{
    if (!indexToMove.isValid() || (rowCount(indexToMove.parent()) - indexToMove.row() <= 1)) return;

    beginMoveRows(indexToMove.parent(), indexToMove.row(), indexToMove.row(), indexToMove.parent(), indexToMove.row() + 2);

    UIWidgetBase* widget = static_cast<UIWidgetBase*>(indexToMove.internalPointer());
    DisplayArea* displayArea = static_cast<DisplayArea*>(widget->GetParent());

    displayArea->MoveWidgetDown(widget);
    endMoveRows();
}

DisplayArea* LayoutsTreeModel::AddAreaToLayout(const QModelIndex& layoutIndex)
{
    if (!layoutIndex.isValid()) return NULL;

    Layout* parentLayout = static_cast<Layout*>(layoutIndex.internalPointer());

    beginInsertRows(layoutIndex, rowCount(layoutIndex), rowCount(layoutIndex));
    DisplayArea* newArea = AddAreaToLayout(parentLayout);
    endInsertRows();
    emit signal_areaAdded();

    return newArea;
}

DisplayArea* LayoutsTreeModel::AddAreaToLayout(Layout* parentLayout, const QString& areaName)
{
    DisplayArea* newArea = new DisplayArea();
    newArea->SetName(areaName);
    connect(newArea, &DisplayArea::WidgetAdded, this, &LayoutsTreeModel::widgetAdded);
    parentLayout->AddDisplayArea(newArea);

    return newArea;
}

void LayoutsTreeModel::RemoveAreaFromLayout(const QModelIndex& areaIndex)
{
    if (!areaIndex.isValid()) return;

    DisplayArea* areaToRemove = static_cast<DisplayArea*>(areaIndex.internalPointer());
    Layout* currentParent = static_cast<Layout*>(areaToRemove->GetParent());

    if (areaToRemove == m_layoutDefinition.GetMenuArea())
    {
        QMessageBox messageBox;
        messageBox.setText("The area '" + areaToRemove->GetName() + "' contains the menu. You need to remove it in the Menu Editor or move it to another area before deleting this area.");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
        return;
    }
    else
    {
        beginRemoveRows(areaIndex.parent(), areaIndex.row(), areaIndex.row());
        currentParent->RemoveDisplayArea(areaToRemove);
        endRemoveRows();
    }
}

void LayoutsTreeModel::MoveToArea(const QModelIndex& itemToMove, DisplayArea* destination)
{
    if (!itemToMove.isValid()) return;

    ProjectObject* objectToMove = static_cast<ProjectObject*>(itemToMove.internalPointer());

    UIWidgetBase* widgetToMove = dynamic_cast<UIWidgetBase*>(objectToMove);
    if (widgetToMove != NULL)
    {
        DisplayArea* currentParent = static_cast<DisplayArea*>(widgetToMove->GetParent());
        QModelIndex currentParentIndex = getIndexForArea(currentParent);
        QModelIndex destinationParentIndex = getIndexForArea((destination));

        beginRemoveRows(currentParentIndex, currentParent->UIWidgets().indexOf(widgetToMove), currentParent->UIWidgets().indexOf(widgetToMove));
        currentParent->RemoveUIWidget(widgetToMove);

        endRemoveRows();
        beginInsertRows(destinationParentIndex, rowCount(destinationParentIndex), rowCount(destinationParentIndex));
        destination->AddUIWidget(widgetToMove);
        endInsertRows();

        return;
    }

    if (dynamic_cast<MenuDefinition*>(objectToMove) != NULL)
    {
        MenuDefinition* menuDefinition = m_layoutDefinition.GetMenu();
        Layout* currentLayout = m_layoutDefinition.GetMenuLayout();
        DisplayArea* currentArea = m_layoutDefinition.GetMenuArea();
        QModelIndex currentAreaIndex = getIndexForArea(currentArea);

        Layout* destinationLayout = static_cast<Layout*>(destination->GetParent());
        QModelIndex destinationAreaIndex = getIndexForArea(destination);

        beginRemoveRows(currentAreaIndex, currentArea->UIWidgets().size() + 1, currentArea->UIWidgets().size() + 1);
        currentLayout->RemoveMenu(false);
        endRemoveRows();

        beginInsertRows(destinationAreaIndex, destination->UIWidgets().size() + 1, destination->UIWidgets().size() + 1);
        destinationLayout->SetMenu(menuDefinition, destination);
        endInsertRows();

    }

    qWarning("Requested to move unknown widget in LayoutEditor: %s", qUtf8Printable(objectToMove->objectName()));
}

void LayoutsTreeModel::RemoveWidget(const QModelIndex& indexToRemove)
{
    if (!indexToRemove.isValid()) return;

    ProjectObject* projectObject = static_cast<ProjectObject*>(indexToRemove.internalPointer());
    UIWidgetBase* widgetToRemove = static_cast<UIWidgetBase*>(indexToRemove.internalPointer());
    DisplayArea* currentParent = static_cast<DisplayArea*>(widgetToRemove->GetParent());

    quint32 widgetID = widgetToRemove->GetUIWidgetID();

    if (dynamic_cast<MenuDefinition*>(projectObject) != NULL)
    {
        QMessageBox messageBox;
        messageBox.setText("The widget is the menu contents. You remove it by deleting the menu in the Menu Editor.");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
        return;
    }
    else
    {
        beginRemoveRows(indexToRemove.parent(), indexToRemove.row(), indexToRemove.row());
        currentParent->RemoveUIWidget(widgetToRemove);
        delete widgetToRemove;
        endRemoveRows();
        emit signal_widgetRemoved(widgetID);
    }
}

void LayoutsTreeModel::widgetAdded(UIWidgetBase* widget)
{
    DisplayArea* area = static_cast<DisplayArea*>(widget->GetParent());
    Layout* layout = static_cast<Layout*>(area->GetParent());

    QModelIndex layoutIndex = index(m_layoutDefinition.Layouts().indexOf(layout), 0, QModelIndex());
    QModelIndex areaIndex = index(layout->DisplayAreas().indexOf(area), 0, layoutIndex);

    beginInsertRows(areaIndex, area->UIWidgets().size() - 1, area->UIWidgets().size() - 1);
    endInsertRows();
}

QModelIndex LayoutsTreeModel::getIndexForLayout(Layout* layout)
{
    int row = m_layoutDefinition.Layouts().indexOf(layout);
    return index(row, 0, QModelIndex());
}

QModelIndex LayoutsTreeModel::getIndexForArea(DisplayArea* area)
{
    Layout* parentLayout = static_cast<Layout*>(area->GetParent());
    QModelIndex parentIndex = getIndexForLayout(parentLayout);

    int row = parentLayout->DisplayAreas().indexOf(area);
    return index(row, 0, parentIndex);
}

/* QAbstractItemModel overrides*/

QModelIndex LayoutsTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    if (!parent.isValid())
    {
        ProjectObject* childItem = static_cast<ProjectObject*>(m_layoutDefinition.GetLayoutFromIndex(row));
        return createIndex(row, 0, childItem);
    }

    ProjectObject* parentObject = static_cast<ProjectObject*>(parent.internalPointer());

    Layout* parentLayout = dynamic_cast<Layout*>(parentObject);
    if (parentLayout != NULL)
    {
        DisplayArea* area = parentLayout->GetAreaFromIndex(row);
        return createIndex(row, 0, area);
    }

    DisplayArea* parentArea = dynamic_cast<DisplayArea*>(parentObject);
    if (parentArea != NULL)
    {
        if (row >= parentArea->UIWidgets().size())
        {
            MenuDefinition* menu = m_layoutDefinition.GetMenu();
            return createIndex(row, 0, menu);
        }
        else
        {
            UIWidgetBase* widget = parentArea->GetUIWidgetFromIndex(row);
            return createIndex(row, 0, widget);
        }
    }

    return QModelIndex();
}

QModelIndex LayoutsTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) return QModelIndex();

    ProjectObject* childObject = static_cast<ProjectObject*>(child.internalPointer());
    ProjectObject* parentObject = childObject->GetParent();

    Layout* layoutObject = dynamic_cast<Layout*>(parentObject);
    if (layoutObject != NULL)
    {
        return createIndex(m_layoutDefinition.Layouts().indexOf(layoutObject), 0, parentObject);
    }

    DisplayArea* areaObject = dynamic_cast<DisplayArea*>(parentObject);
    if (areaObject != NULL)
    {
        Layout* areaParent = static_cast<Layout*>(areaObject->GetParent());
        return createIndex(areaParent->DisplayAreas().indexOf(areaObject), 0, areaObject);
    }

    return QModelIndex();
}

int LayoutsTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0) return 0;
    if (!parent.isValid())
    {
        return m_layoutDefinition.Layouts().size();
    }

    ProjectObject* parentObject = static_cast<ProjectObject*>(parent.internalPointer());
    DisplayArea* displayArea = dynamic_cast<DisplayArea*>(parentObject);
    if (displayArea != NULL)
    {
        if (m_layoutDefinition.GetMenuArea() == displayArea)
        {
            return displayArea->UIWidgets().size() + 1;
        }
        else
        {
            return displayArea->UIWidgets().size();
        }
    }

    Layout* layout = dynamic_cast<Layout*>(parentObject);
    if (layout != NULL)
    {
        return layout->DisplayAreas().size();
    }

    return 0;
}

int LayoutsTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
    return LayoutTreeModelColumnCount;
}

QVariant LayoutsTreeModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
        {
            ProjectObject* object = static_cast<ProjectObject*>(index.internalPointer());
            return object->objectName();
        }
        case Qt::DecorationRole:
        {
            ProjectObject* object = static_cast<ProjectObject*>(index.internalPointer());
            UIWidgetBase* uiWidget = dynamic_cast<UIWidgetBase*>(object);
            if (uiWidget != NULL)
            {
                const QString iconFileName = ":/ToolIcons/" + uiWidget->GetUIWidgetTypeName() + "Icon.bmp";
                // If the file is not found, the pixmap will become a null pixmap (0x0px)
                return QPixmap(iconFileName);
            }
        }
        default:
            return QVariant();
    }
}

QVariant LayoutsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case LayoutNameColumn:
                return QVariant("Layout Name");
            default:
                return QVariant();
        }
    }
    else if (orientation == Qt::Vertical)
    {
        return QVariant(QString::number(section));
    }

    return QVariant();
}
