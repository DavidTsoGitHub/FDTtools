#ifndef LABELMODEL_H
#define LABELMODEL_H

#include "LabelDefinition.h"

#include <QAbstractTableModel>

class LabelModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit LabelModel(LabelDefinition& labels, QObject* parent = 0);

    bool removeLanguage(const QString& languageToRemove);
    void addLanguage(const QString& languageToAdd);

    typedef enum
    {
        IDColumn,
        DescriptionColumn,
        MasterTextColumn,
        LabelColumnCount
    } LabelColumns;

signals:
    void signal_modified();
    void signal_labelAdded(const QModelIndex& index);

private:
    LabelDefinition& m_labelDefinition;

    void updateFontMemoryUsage();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool removeRows(int row, int count, const QModelIndex& parent);
    bool insertRows(int row, int count, const QModelIndex &parent);
};

#endif // LABELMODEL_H
