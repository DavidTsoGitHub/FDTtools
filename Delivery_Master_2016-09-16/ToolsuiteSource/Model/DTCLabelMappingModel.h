#ifndef DTCLABELMAPPINGMODEL_H
#define DTCLABELMAPPINGMODEL_H

#include "DiagDefinition.h"
#include <QAbstractTableModel>
#include <QFont>

typedef enum
{
    DTCColumn,
    LabelColumn,
    DTCLabelMappingModelColumnCount
} DTCLabelMappingModelColumns;

class DTCLabelMappingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DTCLabelMappingModel(DiagDefinition& diagDefinition, QObject* parent = 0);

signals:
    void signal_modified();
    void signal_dtcAdded(const QModelIndex&);

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
    Qt::ItemFlags flags(const QModelIndex& index) const;
};

#endif // DTCLABELMAPPINGMODEL_H
