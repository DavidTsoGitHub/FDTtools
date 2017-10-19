#ifndef UIWIDGETSMODEL_H
#define UIWIDGETSMODEL_H

#include <QAbstractItemModel>

class UIWidgetBase;

class UIWidgetsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UIWidgetsModel(QList<UIWidgetBase*>& widgets);
    UIWidgetBase* GetUIWidget(int row);

signals:
    void signal_modified();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<UIWidgetBase*>& m_widgets;
};

#endif // UIWIDGETSMODEL_H
