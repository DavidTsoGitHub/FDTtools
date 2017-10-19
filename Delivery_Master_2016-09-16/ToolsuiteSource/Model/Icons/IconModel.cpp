#include "IconModel.h"

#include "Icon.h"

#include <QColor>
#include <QBrush>
#include <QFont>

IconModel::IconModel(IconDefinition& icons, QObject* parent) : QAbstractTableModel(parent),
    m_iconDefinition(icons)
{
    m_headerFont.setBold(true);

    connect(this, &IconModel::rowsAboutToBeInserted, this, &IconModel::signal_modified);
    connect(this, &IconModel::rowsAboutToBeRemoved, this, &IconModel::signal_modified);
    connect(this, &IconModel::rowsAboutToBeMoved, this, &IconModel::signal_modified);
    connect(this, &IconModel::modelAboutToBeReset, this, &IconModel::signal_modified);
}

void IconModel::AddIcon(QString name, QString path)
{
    quint32 nextAvailableId = m_iconDefinition.GetNextAvailableIconId();
    Icon* newIcon = new Icon();
    newIcon->SetName(name);
    newIcon->SetPath(path);
    newIcon->SetID(nextAvailableId);

    beginInsertRows(QModelIndex(), m_iconDefinition.Icons().count(), m_iconDefinition.Icons().count());
    m_iconDefinition.AddIcon(newIcon);
    endInsertRows();
    emit signal_IconDataChanged();
}

IconDefinition* IconModel::GetIconDefinition() const
{
    return &m_iconDefinition;
}

int IconModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_iconDefinition.Icons().count();
}

int IconModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return IconModelColumnCount;
}

QVariant IconModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= m_iconDefinition.Icons().count()) return QVariant();

    Icon* icon = m_iconDefinition.Icons().at(index.row());
    switch (role)
    {
        case Qt::ForegroundRole:
            switch (index.column())
            {
                case PathColumn:
                    return QBrush(QColor(0x78, 0x78, 0x78));
                default:
                    return QBrush();
            }
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch (index.column())
            {
                case NameColumn:
                    return QVariant::fromValue(icon->GetName());
                case PathColumn:
                    return QVariant::fromValue(icon->GetPath());
                case DescriptionColumn:
                    return QVariant::fromValue(icon->GetDescription());
                default:
                    return QVariant();
            }
        default:
            return QVariant();
    }
}

bool IconModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    bool dataWasChanged = false;

    Icon* icon = m_iconDefinition.Icons().at(index.row());

    switch (index.column())
    {
        case NameColumn:
            if (icon->GetName().compare(value.toString()) != 0 && !value.toString().isEmpty())
            {
                icon->SetName(value.toString());
                dataWasChanged = true;
            }
            break;
        case DescriptionColumn:
            if (icon->GetDescription().compare(value.toString()) != 0)
            {
                icon->SetDescription(value.toString());
                dataWasChanged = true;
            }
           break;
        case PathColumn:
            if (icon->GetPath().compare(value.toString()) != 0)
            {
                icon->SetPath(value.toString());
                dataWasChanged = true;
            }
        default:
            break;
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        emit signal_IconDataChanged();
    }

    return dataWasChanged;
}

QVariant IconModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
    {
        if (section >= m_iconDefinition.Icons().count()) return QVariant("");
        if (role != Qt::DisplayRole) return QVariant();

        return QVariant::fromValue(m_iconDefinition.Icons().at(section)->GetID());
    }
    else
    {
        switch (role)
        {
            case Qt::FontRole:
                return QVariant::fromValue(m_headerFont);
            case Qt::DisplayRole:
                switch (section)
                {
                    case NameColumn:
                        return QVariant("Name");
                    case DescriptionColumn:
                        return QVariant("Description");
                    case PathColumn:
                        return QVariant("Path");
                    default:
                        return QVariant();
                }
            case Qt::TextAlignmentRole:
                switch (section)
                {
                    case DescriptionColumn:
                        return Qt::AlignLeft;
                    default:
                        return QVariant();
                }
            default:
                return QVariant();
        }
    }

}

Qt::ItemFlags IconModel::flags(const QModelIndex& index) const
{
    switch (index.column())
    {
        case NameColumn:
        case DescriptionColumn:
            return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
        default:
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}

bool IconModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastRowIndex = row+count-1;

    beginRemoveRows(parent, row, lastRowIndex);
    int indexToRemove = lastRowIndex;
    for (; indexToRemove >= row; --indexToRemove)
    {
        m_iconDefinition.RemoveIconAtIndex(indexToRemove);
    }
    endRemoveRows();

    emit signal_IconDataChanged();

    return true;

}
