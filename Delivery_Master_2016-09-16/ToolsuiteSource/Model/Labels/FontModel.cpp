#include "FontModel.h"
#include "Project.h"

#include "bspmemory.h"
#include <QBrush>
#include <QColor>
#include <QFont>

FontModel::FontModel(LabelDefinition& labelDefinition, QObject* parent) :
    QAbstractTableModel(parent),
    m_labelDefinition(labelDefinition)
{
    connect(this, &FontModel::rowsAboutToBeInserted, this, &FontModel::signal_modified);
    connect(this, &FontModel::rowsAboutToBeRemoved, this, &FontModel::signal_modified);
    connect(this, &FontModel::rowsAboutToBeMoved, this, &FontModel::signal_modified);
    connect(this, &FontModel::modelAboutToBeReset, this, &FontModel::signal_modified);
}

void FontModel::AddFont(QString fontPath)
{
    Font* font = new Font();
    font->BDFPath = fontPath;
    font->ID = m_labelDefinition.GetNextFontID();
    font->LineSpacing = 0;
    font->StoreInExternalFlash = false;

    beginInsertRows(QModelIndex(), m_labelDefinition.Fonts().count(), m_labelDefinition.Fonts().count());
    m_labelDefinition.AddFont(font);
    endInsertRows();
}

void FontModel::slot_ResetModel()
{
    beginResetModel();
    endResetModel();
}

int FontModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return m_labelDefinition.Fonts().count();
}

int FontModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return FontColumnCount;
}

QVariant FontModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::ForegroundRole && role != Qt::CheckStateRole)) return QVariant();
    if (index.row() >= m_labelDefinition.Fonts().count())
    {
        return QVariant();
    }

    switch (role)
    {
        case Qt::ForegroundRole:
        {
            switch (index.column())
            {
                case BDFPathColumn:
                    return QBrush(QColor(0x78, 0x78, 0x78));
                default:
                    return QBrush();
            }
        }
        case Qt::DisplayRole:
        {
            switch (index.column() == StoreFontInExternalFlashColumn)
            {
                Project* project = static_cast<Project*>(m_labelDefinition.GetParent());
                if (project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
                {
                    return QVariant();
                }
                else
                {
                    return QVariant("");
                }
            }
        }
        case Qt::EditRole:
        {
            const Font* font = m_labelDefinition.Fonts().at(index.row());
            switch (index.column())
            {
                case BDFPathColumn:
                    return font->BDFPath;
                case DescriptionColumn:
                    return font->Description;
                case LineSpacingColumn:
                    return font->LineSpacing;
                case MemoryUsageColumn:
                    return font->MemoryUsage;
                default:
                    return QVariant();
            }
        }
        case Qt::CheckStateRole:
        {
            switch (index.column())
            {
                case StoreFontInExternalFlashColumn:
                {
                    Project* project = static_cast<Project*>(m_labelDefinition.GetParent());

                    if (project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
                    {
                        const Font* font = m_labelDefinition.Fonts().at(index.row());
                        return (font->StoreInExternalFlash ? Qt::Checked : Qt::Unchecked);
                    }
                    else
                    {
                        return QVariant();
                    }
                }
                default:
                    return QVariant();
            }
        }
        default:
            return QVariant();
    }
}

bool FontModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::CheckStateRole)) return false;

    bool dataWasChanged = false;

    if (index.row() >= m_labelDefinition.Fonts().count()) return false;

    Font* rowFont = m_labelDefinition.Fonts().at(index.row());
    if (rowFont == NULL) return false;

    quint32 fontID = rowFont->ID;

    switch (index.column())
    {
        case BDFPathColumn:
            m_labelDefinition.SetFontBDFPath(fontID, value.toString());
            dataWasChanged = true;
            break;
        case DescriptionColumn:
            m_labelDefinition.SetFontDescription(fontID, value.toString());
            dataWasChanged = true;
            break;
        case LineSpacingColumn:
            m_labelDefinition.SetFontLineSpacing(fontID, value.toUInt());
            dataWasChanged = true;
            break;
        case StoreFontInExternalFlashColumn:
            m_labelDefinition.SetFontStoreInExternalFlash(fontID, value.toBool());
            dataWasChanged = true;
            break;
        case MemoryUsageColumn:
            m_labelDefinition.UpdateFontMemoryUsage(fontID);
            dataWasChanged = true;
        default:
            break;
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
    }

    return dataWasChanged;
}

QVariant FontModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::FontRole) return QVariant();

    if (orientation == Qt::Vertical)
    {
        return QVariant::fromValue(m_labelDefinition.Fonts().at(section)->ID);
    }
    else
    {
        if (role == Qt::FontRole)
        {
            QFont font;
            font.setBold(true);
            return QVariant::fromValue(font);
        }

        switch (section)
        {
            case BDFPathColumn:
                return QString("BDFPath");
            case DescriptionColumn:
                return QString("Description");
            case LineSpacingColumn:
                return QString("Line Spacing");
            case StoreFontInExternalFlashColumn:
                return QString("Store in External Flash");
            case MemoryUsageColumn:
                return QString("Memory Usage (bytes)");
            default:
                return QVariant("");
        }
    }
}

bool FontModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastRowIndex = row+count-1;

    Font* fontToRemove = m_labelDefinition.Fonts().at(lastRowIndex);
    if (fontToRemove != NULL)
    {
        beginRemoveRows(parent, row, lastRowIndex);
        m_labelDefinition.RemoveFontID(fontToRemove->ID);
        endRemoveRows();

        return true;
    }
    else
    {
        return false;
    }
}

Qt::ItemFlags FontModel::flags(const QModelIndex& index) const
{
    switch (index.column())
    {
        case DescriptionColumn:
        case LineSpacingColumn:
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

        case StoreFontInExternalFlashColumn:
        {
            Project* project = static_cast<Project*>(m_labelDefinition.GetParent());
            const BSPMemory* memory = project->GetBSPSpecification().GetMemory();
            if (memory->GetExternalMemorySize() > 0)
            {
                return Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
            }
            else
            {
                return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
            }
        }
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        case BDFPathColumn:
        case MemoryUsageColumn:
        default:
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}
