#ifndef LAYOUTSTREEMODEL_H
#define LAYOUTSTREEMODEL_H

#include "LayoutDefinition.h"

class Layout;
class Project;
class UIWidgetBase;

class QAbstractItemModel;
class QModelIndex;
class QObject;
class QString;
class QVariant;

class LayoutsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    typedef enum
    {
        LayoutNameColumn,
        LayoutTreeModelColumnCount
    } LayoutTreeModelColumns;

public:
    explicit LayoutsTreeModel(LayoutDefinition& layoutDefinition, QObject* parent = 0);

    Layout* AddLayout(const QString& name, LayoutDefinition::LayoutType type = LayoutDefinition::Regular);
    void RemoveLayout(const QModelIndex& indexToRemove, Project *project);
    void MoveLayoutUp(const QModelIndex &indexToMove);
    void MoveLayoutDown(const QModelIndex &indexToMove);

    DisplayArea *AddAreaToLayout(const QModelIndex& layoutIndex);
    DisplayArea *AddAreaToLayout(Layout *parentLayout, const QString& areaName = "New Area");
    void RemoveAreaFromLayout(const QModelIndex& areaIndex);
    void MoveDisplayAreaUp(const QModelIndex& indexToMove);
    void MoveDisplayAreaDown(const QModelIndex& indexToMove);

    void MoveToArea(const QModelIndex& itemToMove, DisplayArea* destination);
    void RemoveWidget(const QModelIndex& widgetToRemove);
    void MoveWidgetUp(const QModelIndex& indexToMove);
    void MoveWidgetDown(const QModelIndex& indexToMove);

signals:
    void signal_modified();

    void signal_layoutRemoved(quint32 layoutID);
    void signal_widgetRemoved(quint32 widgetID);
    void signal_layoutAdded(quint32 layoutID);

    void signal_areaAdded();

private:
    LayoutDefinition& m_layoutDefinition;

    void widgetAdded(UIWidgetBase* widget);
    QModelIndex getIndexForLayout(Layout* layout);
    QModelIndex getIndexForArea(DisplayArea* area);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

};



#endif // LAYOUTSTREEMODEL_H
