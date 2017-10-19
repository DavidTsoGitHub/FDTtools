#ifndef FONTMODEL_H
#define FONTMODEL_H

#include "LabelDefinition.h"
#include <QAbstractTableModel>

class FontModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FontModel(LabelDefinition& labelDefinition, QObject* parent = 0);

    void AddFont(QString fontPath);

    typedef enum
    {
        BDFPathColumn,
        DescriptionColumn,
        LineSpacingColumn,
        StoreFontInExternalFlashColumn,
        MemoryUsageColumn,
        FontColumnCount
    } FontColumns;

signals:
    void signal_modified();

public slots:
    void slot_ResetModel();

private:
    LabelDefinition& m_labelDefinition;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent  = QModelIndex()) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool removeRows(int row, int count, const QModelIndex& parent);
    Qt::ItemFlags flags(const QModelIndex& index) const;
};

#endif // FONTMODEL_H
