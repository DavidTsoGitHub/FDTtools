#include "DIDToCALMappingModel.h"

#include <QBrush>

DIDToCALMappingModel::DIDToCALMappingModel(DiagDefinition& diagDefinition, QObject* parent) : QAbstractTableModel(parent),
    m_diagDefinition(diagDefinition)
{
    m_headerFont.setBold(true);

    connect(this, &DIDToCALMappingModel::rowsAboutToBeInserted, this, &DIDToCALMappingModel::signal_modified);
    connect(this, &DIDToCALMappingModel::rowsAboutToBeRemoved, this, &DIDToCALMappingModel::signal_modified);
    connect(this, &DIDToCALMappingModel::rowsAboutToBeMoved, this, &DIDToCALMappingModel::signal_modified);
    connect(this, &DIDToCALMappingModel::modelAboutToBeReset, this, &DIDToCALMappingModel::signal_modified);
}

void DIDToCALMappingModel::ResetData()
{
    beginResetModel();
    endResetModel();
}

int DIDToCALMappingModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_diagDefinition.GetDIDToCALMappings().count() + 1;
}

int DIDToCALMappingModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return DIDToCALMappingModelColumnCount;
}

QVariant DIDToCALMappingModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::BackgroundRole) return QVariant();
    if (index.row() >= m_diagDefinition.GetDIDToCALMappings().count()) return QVariant();

    DIDToCALMapping* mapping = m_diagDefinition.GetDIDToCALMappings().at(index.row());

    if (role == Qt::BackgroundRole)
    {
        if (index.column() != DIDColumn) return QVariant();
        for (int i=0; i<m_diagDefinition.GetDIDToCALMappings().count(); ++i)
        {
            if (i == index.row()) continue;
            DIDToCALMapping* otherMapping = m_diagDefinition.GetDIDToCALMappings().at(i);
            if (mapping->GetDID() == otherMapping->GetDID())
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
            case DIDColumn:
                return QVariant::fromValue(mapping->GetDID());
            case CALColumn:
                return QVariant::fromValue(mapping->GetCALParameter());
            default:
                return QVariant();
        }
    }
}

bool DIDToCALMappingModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) return false;
    if (role != Qt::EditRole) return false;

    bool dataWasChanged = false;

    if (index.row() >= m_diagDefinition.GetDIDToCALMappings().count())
    {
        beginInsertRows(QModelIndex(), m_diagDefinition.GetDIDToCALMappings().count(), m_diagDefinition.GetDIDToCALMappings().count());
        m_diagDefinition.AddDIDToCALMapping(0, QString(""));
        endInsertRows();
        dataWasChanged = true;
        emit signal_didMappingAdded(index);
    }

    DIDToCALMapping* mapping = m_diagDefinition.GetDIDToCALMappings().at(index.row());

    switch (index.column())
    {
        case DIDColumn:
        {
            quint16 newDID = value.toUInt();
            if (mapping->GetDID() != newDID)
            {
                mapping->SetDID(newDID);
                dataWasChanged = true;
            }
            break;
        }
        case CALColumn:
        {
            QString newCALParam = value.toString();
            if (mapping->GetCALParameter().compare(newCALParam) != 0)
            {
                mapping->SetCALParameter(newCALParam);
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

QVariant DIDToCALMappingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::FontRole) return m_headerFont;
    if (role != Qt::DisplayRole) return QVariant();

    switch (orientation)
    {
        case Qt::Horizontal:
        {
            switch (section)
            {
                case DIDColumn:
                    return QVariant("DID");
                case CALColumn:
                    return QVariant("Calibration Parameter");
                default:
                    return QVariant();
            }
        }
        case Qt::Vertical:
            if (section >= m_diagDefinition.GetDIDToCALMappings().count()) return QVariant("+");
            return QVariant();
        default:
            return QVariant();
    }
}

Qt::ItemFlags DIDToCALMappingModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}


bool DIDToCALMappingModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastIndexToRemove = row + count - 1;

    if (lastIndexToRemove >= m_diagDefinition.GetDIDToCALMappings().count())
    {
        lastIndexToRemove = m_diagDefinition.GetDIDToCALMappings().count() - 1;
    }

    if (lastIndexToRemove < row) return false;

    beginRemoveRows(parent, row, lastIndexToRemove);
    for (int index = lastIndexToRemove; index >= row; --index)
    {
        m_diagDefinition.RemoveDIDToCALMappingByIndex(index);
    }
    endRemoveRows();

    return true;


}
