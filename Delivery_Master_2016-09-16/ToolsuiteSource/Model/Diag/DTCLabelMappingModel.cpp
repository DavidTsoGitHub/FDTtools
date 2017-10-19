#include "DTCLabelMappingModel.h"

#include <QBrush>

DTCLabelMappingModel::DTCLabelMappingModel(DiagDefinition& diagDefinition, QObject* parent) :
    QAbstractTableModel(parent),
    m_diagDefinition(diagDefinition)
{
    m_headerFont.setBold(true);

    connect(this, &DTCLabelMappingModel::rowsAboutToBeInserted, this, &DTCLabelMappingModel::signal_modified);
    connect(this, &DTCLabelMappingModel::rowsAboutToBeRemoved, this, &DTCLabelMappingModel::signal_modified);
    connect(this, &DTCLabelMappingModel::rowsAboutToBeMoved, this, &DTCLabelMappingModel::signal_modified);
    connect(this, &DTCLabelMappingModel::modelAboutToBeReset, this, &DTCLabelMappingModel::signal_modified);
}

int DTCLabelMappingModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_diagDefinition.GetDTCLabelMappings().count() + 1; // Add one extra where we add new mappings
}

int DTCLabelMappingModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return DTCLabelMappingModelColumnCount;
}

QVariant DTCLabelMappingModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::BackgroundRole) return QVariant();
    if (index.row() >= m_diagDefinition.GetDTCLabelMappings().count()) return QVariant();

    DTCLabelMapping* mapping = m_diagDefinition.GetDTCLabelMappings().at(index.row());

    if (role == Qt::BackgroundRole)
    {
            if (index.column() != DTCColumn) return QVariant();
            for (int i=0; i<m_diagDefinition.GetDTCLabelMappings().count(); ++i)
            {
                if (i == index.row()) continue;
                DTCLabelMapping* otherMapping = m_diagDefinition.GetDTCLabelMappings().at(i);
                if (mapping->GetDTC() == otherMapping->GetDTC())
                {
                    return QBrush(Qt::red);
                }
            }

            return QBrush();
    }
    else
    {
        switch (index.column())
        {
            case DTCColumn:
                return QVariant::fromValue(mapping->GetDTC());
            case LabelColumn:
                return QVariant::fromValue(mapping->GetLabelID());
            default:
                return QVariant();
        }
    }
}

bool DTCLabelMappingModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    bool dataWasChanged = false;

    if (index.row() >= m_diagDefinition.GetDTCLabelMappings().count())
    {
        beginInsertRows(QModelIndex(), m_diagDefinition.GetDTCLabelMappings().count(), m_diagDefinition.GetDTCLabelMappings().count());
        m_diagDefinition.AddDTCLabelMapping(0, 0);
        endInsertRows();
        dataWasChanged = true;
        emit signal_dtcAdded(index);
    }

    DTCLabelMapping* mapping = m_diagDefinition.GetDTCLabelMappings().at(index.row());

    switch (index.column())
    {
        case DTCColumn:
        {
            quint32 newDTCValue = value.toUInt();
            if (mapping->GetDTC() != newDTCValue)
            {
                mapping->SetDTC(newDTCValue);
                dataWasChanged = true;
            }
            break;
        }
        case LabelColumn:
        {
            quint32 newLabelID = value.toUInt();
            if (mapping->GetLabelID() != newLabelID)
            {
                mapping->SetLabelID(newLabelID);
                dataWasChanged = true;
            }
            break;
        }
        default:
            break;
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
    }

    return dataWasChanged;
}

QVariant DTCLabelMappingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::FontRole) return m_headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch (orientation)
    {
        case Qt::Vertical:
            if (section >= m_diagDefinition.GetDTCLabelMappings().count()) return QVariant("+");
            return QVariant();
        case Qt::Horizontal:
            switch (section)
            {
                case DTCColumn:
                    return QVariant("DTC");
                case LabelColumn:
                    return QVariant("Label");
                default:
                    return QVariant();
            }
        default:
            return QVariant();
    }
}

bool DTCLabelMappingModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastIndexToRemove = row + count - 1;

    if (lastIndexToRemove >= m_diagDefinition.GetDTCLabelMappings().count())
    {
        lastIndexToRemove = m_diagDefinition.GetDTCLabelMappings().count() - 1;
    }

    if (lastIndexToRemove < row) return false;

    beginRemoveRows(parent, row, lastIndexToRemove);
    for (int index = lastIndexToRemove; index >= row; --index)
    {
        m_diagDefinition.RemoveDTCLabelMappingByIndex(index);
    }
    endRemoveRows();

    return true;
}

Qt::ItemFlags DTCLabelMappingModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
