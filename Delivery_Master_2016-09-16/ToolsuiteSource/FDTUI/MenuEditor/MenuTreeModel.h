#ifndef MENUTREEMODEL_H
#define MENUTREEMODEL_H

class MenuDefinition;
class MenuItem;
class Project;

class QAbstractItemModel;
class QModelIndex;
class QObject;

class MenuTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MenuTreeModel(Project* project, QObject* parent = 0);

    QModelIndex FindIndexForMenuItem(const MenuItem* menuItem, const QModelIndex& parent = QModelIndex());

    void AddMenuItem(const QModelIndex& parent);
    void RemoveMenuItem(const QModelIndex& indexToRemove);
    void MoveMenuItemUp(const QModelIndex& indexToMove);
    void MoveMenuItemDown(const QModelIndex& indexToMove);

signals:
    void signal_modified();

private:
    void RemoveSubMenuItem(MenuItem *menuItem, bool blockSignals = false);

    Project* m_project;
    MenuDefinition* m_menuDefinition;

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    void RemoveMenu(void);
};

#endif // MENUTREEMODEL_H
