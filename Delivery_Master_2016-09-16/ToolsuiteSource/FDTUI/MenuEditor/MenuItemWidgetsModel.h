#ifndef MENUITEMWIDGETSMODEL_H
#define MENUITEMWIDGETSMODEL_H

class MenuItem;
class UIWidgetBase;

class QAbstractListModel;
class QModelIndex;

class MenuItemWidgetsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MenuItemWidgetsModel(MenuItem* menuItem, QObject* parent = 0);

    void RemoveWidget(const QModelIndex& indexToRemove);
    QModelIndex FindIndexForWidget(UIWidgetBase* uiWidget);

signals:
    void signal_modified();

private:
    MenuItem* m_menuItem;

    void MenuItemPropertyUpdated();

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
};

#endif // MENUITEMWIDGETSMODEL_H
