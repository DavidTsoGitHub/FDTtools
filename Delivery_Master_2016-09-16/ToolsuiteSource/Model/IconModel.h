#ifndef ICONMODEL_H
#define ICONMODEL_H

#include "IconDefinition.h"

#include <QAbstractTableModel>
#include <QFont>

class IconModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit IconModel(IconDefinition& icons, QObject* parent = 0);

    void AddIcon(QString name, QString path);

    IconDefinition* GetIconDefinition() const;

    typedef enum
    {
        NameColumn,
        PathColumn,
        DescriptionColumn,
        IconModelColumnCount
    } IconModelColumns;

signals:
    void signal_modified();
    void signal_IconDataChanged();

private:
    IconDefinition& m_iconDefinition;

    QFont m_headerFont;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool removeRows(int row, int count, const QModelIndex& parent);

};

#endif // ICONMODEL_H
