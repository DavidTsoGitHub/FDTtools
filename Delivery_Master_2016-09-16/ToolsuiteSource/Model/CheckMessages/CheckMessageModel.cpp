#include "CheckMessageModel.h"

#include "GCLSignalSource.h"
#include "Icon.h"
#include "Image.h"
#include "Label.h"
#include "Layout.h"
#include "Logger.h"

#include <QCheckBox>

CheckMessageModel::CheckMessageModel(CheckMessageDefinition& checkMessageDefinition, LabelDefinition& labelDefinition, ImageDefinition& imageDefinition, IconDefinition& iconDefinition, LayoutDefinition& layoutDefinition, QObject* parent) : QAbstractTableModel(parent),
    m_checkMessageDefinition(checkMessageDefinition),
    m_labelDefinition(labelDefinition),
    m_imageDefinition(imageDefinition),
    m_iconDefinition(iconDefinition),
    m_layoutDefinition(layoutDefinition)
{
    connect(this, &CheckMessageModel::rowsAboutToBeInserted, this, &CheckMessageModel::signal_modified);
    connect(this, &CheckMessageModel::rowsAboutToBeRemoved, this, &CheckMessageModel::signal_modified);
    connect(this, &CheckMessageModel::rowsAboutToBeMoved, this, &CheckMessageModel::signal_modified);
    connect(this, &CheckMessageModel::modelAboutToBeReset, this, &CheckMessageModel::signal_modified);

    connect(&m_layoutDefinition, &LayoutDefinition::signal_LayoutPriorityUpdated, this, &CheckMessageModel::slot_layoutModelChanged);
}

CheckMessageDefinition &CheckMessageModel::GetCheckMessageDefinition()
{
    return m_checkMessageDefinition;
}

void CheckMessageModel::RemoveWidgetSink(GCLSignalSink *sink)
{
    beginResetModel();
    m_checkMessageDefinition.RemoveWidgetSink(sink);
    endResetModel();
}

int CheckMessageModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_checkMessageDefinition.CheckMessages().count() + 1; // empty row at the end
}

int CheckMessageModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    int signalSources = m_checkMessageDefinition.GetSignalSources().count();
    int validLayouts = m_layoutDefinition.GetValidLayoutIDsForCheckMessages().count();
    int column = CheckMessageColumnCount + signalSources + validLayouts;

    return column;
}

QVariant CheckMessageModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::CheckStateRole)) return QVariant();

    if (index.row() > m_checkMessageDefinition.CheckMessages().count()-1)
    {
        LOG_DEBUG("CheckMessageModel tried out of bounds access");
        return QVariant();
    }
    CheckMessage* message = m_checkMessageDefinition.CheckMessages().at(index.row());

    switch (index.column())
    {
        case NameColumn:
            if(role != Qt::CheckStateRole)
            {
                return QVariant::fromValue(message->GetName());
            }
            else return QVariant();
        default:
        {
            if((role != Qt::CheckStateRole) && (index.column() <= m_checkMessageDefinition.GetSignalSources().count()))
            {
                int transposedColumn = index.column() - CheckMessageColumnCount;

                QString type = m_checkMessageDefinition.CheckMessages().first()->Items().at(transposedColumn)->Type;
                QString comboBoxText;
                if(type.compare("DynamicLabel") == 0)
                {
                    Label* label = m_labelDefinition.GetLabel(message->Items().at(transposedColumn)->ResourceID);
                    comboBoxText = label->GetMasterText();
                }
                else if(type.compare("DynamicImage") == 0)
                {
                    Image* image = m_imageDefinition.GetImageByID(message->Items().at(transposedColumn)->ResourceID);
                    comboBoxText = image->GetName();
                }
                else if(type.compare("DynamicIcon") == 0)
                {
                    Icon* icon = m_iconDefinition.GetIconByID(message->Items().at(transposedColumn)->ResourceID);
                    comboBoxText = icon->GetName();
                }

                return QVariant(comboBoxText);
            }
            else if((role == Qt::CheckStateRole) && (index.column() > m_checkMessageDefinition.GetSignalSources().count()))
            {
                int layoutIndex = index.column() - CheckMessageColumnCount - m_checkMessageDefinition.GetSignalSources().count();
                QList<quint32> layoutIDs = m_layoutDefinition.GetValidLayoutIDsForCheckMessages();
                if(layoutIndex >= layoutIDs.count())
                {
                    return QVariant();
                }
                bool allowed = m_checkMessageDefinition.CheckMessages().at(index.row())->IsAllowedInLayout(layoutIDs.at(layoutIndex));
                return (allowed ? Qt::Checked : Qt::Unchecked);
            }
        }
    }

    return QVariant();
}

bool CheckMessageModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if ((!index.isValid()) || (((role != Qt::DisplayRole) && (role != Qt::EditRole)) && (role != Qt::CheckStateRole))) return false;


    CheckMessage* message;

    if ((index.row() >= m_checkMessageDefinition.CheckMessages().count()))
    {
        QString name;
        if(index.column()==0)
        {
            name = value.toString();
        }
        else
        {
            name = "";
        }

        QList<CheckMessageItem*> items;
        for(int item = 0; item < m_checkMessageDefinition.GetSignalSources().count();++item)
        {
            CheckMessageItem* newItem = new CheckMessageItem();
            newItem->Type = getItemType(item);
            newItem->Index = item;
            newItem->ResourceID = 1;
            items.append(newItem);
        }

        message = new CheckMessage(name, index.row(), items);

        beginInsertRows(index.parent(), m_checkMessageDefinition.CheckMessages().count(), m_checkMessageDefinition.CheckMessages().count());
        m_checkMessageDefinition.AddCheckMessage(message);

        endInsertRows();
        emit signal_checkMessageAdded(index);
    }
    else
    {
        message = m_checkMessageDefinition.CheckMessages().at(index.row());
    }


    bool dataWasChanged = false;

    switch (index.column())
    {
        case NameColumn:
            message->SetName(value.toString());
            dataWasChanged = true;
            break;
        default:
        {
            if((role != Qt::CheckStateRole) && (index.column() <= m_checkMessageDefinition.GetSignalSources().count()))
            {
                int transposedColumn = index.column() - CheckMessageColumnCount;
                message->UpdateItemResourceID(transposedColumn, value.toInt());
                dataWasChanged = true;
            }
            else if((index.column() > m_checkMessageDefinition.GetSignalSources().count()))
            {
                int layoutIndex = index.column() - CheckMessageColumnCount - m_checkMessageDefinition.GetSignalSources().count();

                QList<quint32> layoutIDs = m_layoutDefinition.GetValidLayoutIDsForCheckMessages();
                if(layoutIndex >= layoutIDs.count())
                {
                    break;
                }
                message->SetAllowedInLayout(layoutIDs.at(layoutIndex), value.toBool());

                foreach (Layout* layout, m_layoutDefinition.Layouts())
                {
                    if (layout->GetID() == layoutIDs.at(layoutIndex))
                    {
                        if (value.toBool() == true)
                        {
                            layout->AddCheckMessage(message);
                        }
                        else
                        {
                            layout->RemoveCheckMessage(message);
                        }
                        dataWasChanged = true;
                        break;
                    }
                }
            }
        }
    }

    if (dataWasChanged)
    {
        emit dataChanged(index, index, QVector<int>() << role);
    }

    return dataWasChanged;
}

QVariant CheckMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::FontRole) return QVariant();

    // Row headers
    if (orientation == Qt::Vertical)
    {
        if (section >= m_checkMessageDefinition.CheckMessages().count())
        {
            return QVariant("+");
        }
        else
        {
            return QVariant::fromValue(section);
        }
    }
    else
    {

        if (role == Qt::FontRole)
        {
            QFont font = QFont();
            font.setBold(true);
            return QVariant::fromValue(font);
        }

        switch (section)
        {
            case NameColumn:
                return QVariant("Name");
            default:
            {
                QList<QString> names = getDynamicWidgetNames();
                if(section <= names.count())
                {
                    if (names.count() == 0)
                    {
                        return QVariant();
                    }
                    else
                    {
                        return names.at(section-CheckMessageColumnCount);
                    }
                }
                else
                {
                    int transposedToLayout = section - CheckMessageColumnCount - names.count();
                    QList<Layout*> layouts = m_layoutDefinition.GetValidLayoutsForCheckMessages();
                    if(transposedToLayout >= layouts.count())
                    {
                        return QVariant();
                    }

                    if (!(transposedToLayout > layouts.count()))
                        return QVariant(layouts.at(transposedToLayout)->Name());
                }
            }
        }
    }

    return QVariant();
}

bool CheckMessageModel::removeRows(int row, int count, const QModelIndex& parent)
{
    int lastRowIndex = row + count - 1;
    beginRemoveRows(parent, row, lastRowIndex);
    m_checkMessageDefinition.RemoveMessages(row, count);
    endRemoveRows();
    return true;
}

Qt::ItemFlags CheckMessageModel::flags(const QModelIndex& index) const
{
    if (index.row() < m_checkMessageDefinition.CheckMessages().count()) // Last row is not selectable
    {
        if(index.column() == NameColumn)
        {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }
        else if(index.column() < m_checkMessageDefinition.GetSignalSources().count() + CheckMessageColumnCount)
        {
            return  Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
        }
        else
        {
            return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable ;
        }
    }
    else
    {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
}

void CheckMessageModel::slot_moveSelectedItemUp(const QModelIndex& index)
{
    if(index.row() > 0)
    {
        beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), index.row()-1);
        CheckMessage* message = m_checkMessageDefinition.CheckMessages().takeAt(index.row());
        m_checkMessageDefinition.CheckMessages().insert(index.row()-1, message);
        reCalcPriority();
        endMoveRows();
    }
}

//To move an item down, we move the following item up instead.
void CheckMessageModel::slot_moveSelectedItemDown(const QModelIndex& index)
{
    if(index.row() <= m_checkMessageDefinition.CheckMessages().count()-2)
    {
        beginMoveRows(index.parent(), index.row()+1, index.row()+1, index.parent(), index.row());
        CheckMessage* message = m_checkMessageDefinition.CheckMessages().takeAt(index.row()+1);
        m_checkMessageDefinition.CheckMessages().insert(index.row(), message);
        reCalcPriority();
        endMoveRows();
    }
}

void CheckMessageModel::slot_layoutModelChanged(void)
{
    beginResetModel();
    endResetModel();
}

QString CheckMessageModel::getItemType(quint32 item)
{
    int signed_item = static_cast<int>(item);
    if (m_checkMessageDefinition.GetSignalSources().count() > signed_item
            && m_checkMessageDefinition.GetSignalSources().at(signed_item)->GetConnectedSinks().count() > 0)
    {
        QString componentName = m_checkMessageDefinition.GetSignalSources().at(signed_item)->GetConnectedSinks().first()->ComponentName();

        QStringList splittedList = componentName.split('_');

        if (splittedList.count() > 0)
        {
            return splittedList.at(1);
        }
    }

    return QString("");
}

void CheckMessageModel::reCalcPriority()
{
    quint32 prio = 0;
    foreach(CheckMessage* message, m_checkMessageDefinition.CheckMessages())
    {
        message->SetPrio(prio);
        ++prio;
    }
}

QList<QString> CheckMessageModel::getDynamicWidgetNames(void) const
{
    if(m_layoutDefinition.GetCheckMessageLayout() == NULL)
    {
        return QList<QString>();
    }

    QList<QString> dynamicWidgetNames;
    QList<UIWidgetBase*> widgets;

    m_layoutDefinition.GetCheckMessageLayout()->GetUIWidgets(widgets);

    foreach(UIWidgetBase* widget, widgets)
    {
        if(widget->GetUIWidgetTypeName().startsWith("Dynamic"))
        {
            dynamicWidgetNames.append(widget->GetUserDefinedName());
        }
    }
    return dynamicWidgetNames;
}
