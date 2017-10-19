#include "LabelModel.h"

#include "Label.h"

#include <QStringList>
#include <QFont>

LabelModel::LabelModel(LabelDefinition& labelDefinition, QObject* parent) : QAbstractTableModel(parent),
    m_labelDefinition(labelDefinition)
{
    connect(this, &LabelModel::rowsAboutToBeInserted, this, &LabelModel::signal_modified);
    connect(this, &LabelModel::rowsAboutToBeRemoved, this, &LabelModel::signal_modified);
    connect(this, &LabelModel::rowsAboutToBeMoved, this, &LabelModel::signal_modified);
    connect(this, &LabelModel::modelAboutToBeReset, this, &LabelModel::signal_modified);
}

void LabelModel::addLanguage(const QString& languageToAdd)
{
    if (m_labelDefinition.Languages().contains(languageToAdd))
    {
        return;
    }

    int columnIndex = LabelColumnCount + (m_labelDefinition.Languages().count() * 2);
    beginInsertColumns(QModelIndex(), columnIndex, columnIndex + 1);
    m_labelDefinition.AddLanguage(languageToAdd);
    endInsertColumns();
}

bool LabelModel::removeLanguage(const QString& languageToRemove)
{
    if (!m_labelDefinition.Languages().contains(languageToRemove))
    {
        return false;
    }

    int languageIndex = m_labelDefinition.Languages().indexOf(languageToRemove);
    int columnIndex = LabelColumnCount + (languageIndex * 2);

    beginRemoveColumns(QModelIndex(), columnIndex, columnIndex + 1);
    m_labelDefinition.RemoveLanguage(languageToRemove);
    endRemoveColumns();
    return true;
}

int LabelModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_labelDefinition.Labels().count();  // + 1; // empty row at the end
}

int LabelModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    return (m_labelDefinition.Languages().count() * 2) + LabelColumnCount;
}

QVariant LabelModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return QVariant();

    if (index.row() >= m_labelDefinition.Labels().count())
    {
        return QVariant();
    }

    Label* label = m_labelDefinition.Labels().at(index.row());

    switch (index.column())
    {
        case IDColumn:
            return QVariant::fromValue(label->GetID());
        case DescriptionColumn:
            return QVariant::fromValue(label->GetDescription());
        case MasterTextColumn:
            return QVariant::fromValue(label->GetMasterText());
        default:
        {
            int transposedColumn = index.column() - LabelColumnCount;
            int languageIndex = transposedColumn / 2;
            if (languageIndex < m_labelDefinition.Languages().count())
            {
                QString language = m_labelDefinition.Languages().at(languageIndex);
                Translation* translation = label->GetTranslation(language);
                if (translation != NULL)
                {
                    if (transposedColumn % 2 == 0)
                    {
                        return QVariant::fromValue(translation->GetText());
                    }
                    else
                    {
                        return QVariant::fromValue(m_labelDefinition.GetFontByID(translation->GetFontID())->Description);
                    }
                }
            }
            else
            {
                qDebug("Translation index out of bounds!");
            }
        }
    }

    return QVariant();
}

bool LabelModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return false;

    Label* label = m_labelDefinition.Labels().at(index.row());


    bool dataWasChanged = false;

    switch (index.column())
    {
        case IDColumn:
            break;
        case DescriptionColumn:
            label->SetDescription(value.toString());
            dataWasChanged = true;
            break;
        case MasterTextColumn:
            label->SetMasterText(value.toString());
            dataWasChanged = true;
            m_labelDefinition.CalculateFontMemoryUsage();
            break;
        default:
        {
            int transposedColumn = index.column() - LabelColumnCount;
            int languageIndex = transposedColumn / 2;

            if (languageIndex < m_labelDefinition.Languages().count())
            {
                QString language = m_labelDefinition.Languages().at(languageIndex);
                Translation* translation = label->GetTranslation(language);
                if (translation == NULL)
                {
                    translation = new Translation();
                    translation->SetLanguage(language);
                    translation->SetFontID(0);
                    label->AddTranslation(translation);
                }

                if (transposedColumn % 2 == 0)
                {
                    translation->SetText(value.toString());
                    if (translation->GetText().isEmpty())
                    {
                        label->RemoveTranslation(language);
                    }
                }
                else
                {
                    translation->SetFontID(value.toInt());
                }
                dataWasChanged = true;
                m_labelDefinition.CalculateFontMemoryUsage();
            }
            else
            {
                qDebug("Translation index out of bounds!");
            }
        }
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
    }

    return dataWasChanged;
}

QVariant LabelModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::FontRole) return QVariant();

    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::FontRole)
        {
            QFont font = QFont();
            font.setBold(true);
            return QVariant::fromValue(font);
        }

        switch (section)
        {
            case IDColumn:
                return QVariant("ID");
            case DescriptionColumn:
                return QVariant("Description");
            case MasterTextColumn:
                return QVariant("MasterText");
            default:
            {
                int transposedColumn = section - LabelColumnCount;
                int languageIndex = transposedColumn / 2;

                if (languageIndex >= m_labelDefinition.Languages().count())
                {
                        return QVariant("INVALID LANGUAGE INDEX!");
                }

                QString language = m_labelDefinition.Languages().at(languageIndex);

                if (transposedColumn % 2 == 0)
                {
                    return QString("Text %1").arg(language);
                }
                else
                {
                    return QString("Font %1").arg(language);
                }
            }

        }
    }

    return QVariant();
}

Qt::ItemFlags LabelModel::flags(const QModelIndex& index) const
{
    if (index.column() == IDColumn)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    }
}

bool LabelModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastRowIndex = row + count - 1;
    beginRemoveRows(parent, row, lastRowIndex);

    m_labelDefinition.RemoveLabels(row, count);

    endRemoveRows();

    return true;
}

bool LabelModel::insertRows(int /*row*/, int count, const QModelIndex& parent)
{
    for (int i = 0; i < count; i++)
    {
        beginInsertRows(parent, m_labelDefinition.Labels().count(), m_labelDefinition.Labels().count());
        m_labelDefinition.AddLabel(new Label());
        endInsertRows();
    }

    return true;
}
