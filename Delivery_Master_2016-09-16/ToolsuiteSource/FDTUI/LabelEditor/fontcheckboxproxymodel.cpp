#include "FontCheckBoxProxyModel.h"

#include "bspmemory.h"
#include "Project.h"
#include <QObject>
#include <QSortFilterProxyModel>

FontCheckBoxProxyModel::FontCheckBoxProxyModel(Project *project, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_externalFlashColumn(-1),
    m_project(project)
{

}

void FontCheckBoxProxyModel::SetExternalFlashColumn(int column)
{
    m_externalFlashColumn = column;
}

QVariant FontCheckBoxProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if (m_externalFlashColumn == index.column())
        {
            if (m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
            {
                return QVariant();
            }
            else
            {
                return QVariant("");
            }
        }
    }

    else if(role == Qt::CheckStateRole)
    {
        if(m_externalFlashColumn == index.column())
        {
            if (m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
            {
                return (index.data(Qt::EditRole).toBool() ? QVariant(Qt::Checked) : QVariant(Qt::Unchecked));
            }

            return QVariant();
        }
    }

    return sourceModel()->data(index, role);
}

bool FontCheckBoxProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (m_externalFlashColumn == index.column() && role == Qt::CheckStateRole)
    {
        QVariant data = (value.toInt() == Qt::Checked ? QVariant(true) : QVariant(false));
        return sourceModel()->setData(index, data, Qt::EditRole);
    }

    return sourceModel()->setData(index, value, role);
}

Qt::ItemFlags FontCheckBoxProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (m_externalFlashColumn == index.column())
    {
        const BSPMemory* memory = m_project->GetBSPSpecification().GetMemory();
        if (memory->GetExternalMemorySize() > 0)
        {
            return Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }

    return sourceModel()->flags(index);
}
