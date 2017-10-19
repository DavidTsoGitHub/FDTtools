#ifndef DIDTOCALMAPPINGMODEL_H
#define DIDTOCALMAPPINGMODEL_H

#include "DiagDefinition.h"
#include <QAbstractTableModel>
#include <QFont>

typedef enum
{
    DIDColumn,
    CALColumn,
    DIDToCALMappingModelColumnCount
} DIDToCALMappingModelColumns;

class DIDToCALMappingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DIDToCALMappingModel(DiagDefinition& diagDefinition, QObject* parent = 0);

    void ResetData();

signals:
    void signal_modified();
    void signal_didMappingAdded(const QModelIndex&);

private:
    DiagDefinition& m_diagDefinition;
    QFont m_headerFont;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex& parent);

    // QAbstractItemModel interface
public:
    Qt::ItemFlags flags(const QModelIndex& index) const;
};

#endif // DIDTOCALMAPPINGMODEL_H
