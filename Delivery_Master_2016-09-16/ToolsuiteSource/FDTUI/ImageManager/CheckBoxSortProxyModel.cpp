#include "CheckBoxSortProxyModel.h"

#include "bspmemory.h"
#include "Image.h"
#include "imagedefinition.h"
#include "Project.h"
#include <QList>
#include <QSortFilterProxyModel>

CheckBoxSortProxyModel::CheckBoxSortProxyModel(Project* project, QObject *parent)
    : QSortFilterProxyModel(parent),
    m_externalFlashColumn(-1),
    m_project(project),
    m_imageDefinition(project->GetImageDefinition())
{
}

void CheckBoxSortProxyModel::SetExternalFlashColumn(int column, QList<quint32> usedImages)
{
    m_externalFlashColumn = column;

    m_usedImages.clear();
    foreach (quint32 imageID, usedImages)
        m_usedImages.append(imageID);
}

void CheckBoxSortProxyModel::SetCheckBoxColumns(QList<int> columns)
{
    m_columnsWithCheckBox.clear();

    foreach (int column, columns)
        m_columnsWithCheckBox.append(column);
}

QVariant CheckBoxSortProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        if (m_externalFlashColumn == index.column())
        {
            quint32 imageID = m_imageDefinition.Images().at(index.row())->GetID();
            if (m_usedImages.contains(imageID))
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
            else
            {
                return QVariant("Image not currently used");
            }
        }
        else if(m_columnsWithCheckBox.contains(index.column()))
        {
            return QVariant();
        }
    }

    else if(role == Qt::CheckStateRole)
    {
        if(m_externalFlashColumn == index.column())
        {
            quint32 imageID = m_imageDefinition.Images().at(index.row())->GetID();
            if (m_usedImages.contains(imageID) && m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize() > 0)
                return (index.data(Qt::EditRole).toBool() ? QVariant(Qt::Checked) : QVariant(Qt::Unchecked));

            return QVariant();
        }
        else if(m_columnsWithCheckBox.contains(index.column()))
        {
            return (index.data(Qt::EditRole).toBool() ? QVariant(Qt::Checked) : QVariant(Qt::Unchecked));
        }
    }

    return sourceModel()->data(index, role);
}

bool CheckBoxSortProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if ((m_externalFlashColumn == index.column() ||
         m_columnsWithCheckBox.contains(index.column()))
            && role == Qt::CheckStateRole)
    {
        QVariant data = (value.toInt() == Qt::Checked ? QVariant(true) : QVariant(false));
        return sourceModel()->setData(index, data, Qt::EditRole);
    }

    return sourceModel()->setData(index, value, role);
}

Qt::ItemFlags CheckBoxSortProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (m_externalFlashColumn == index.column())
    {
        quint32 imageID = m_imageDefinition.Images().at(index.row())->GetID();
        const BSPMemory* memory = m_project->GetBSPSpecification().GetMemory();
        if (m_usedImages.contains(imageID) && (memory->GetExternalMemorySize() > 0))
        {
            return Qt::ItemIsUserCheckable  | Qt::ItemIsEnabled;
        }
        else
        {
            return  Qt::ItemIsEnabled;
        }
    }
    else if(m_columnsWithCheckBox.contains(index.column()))
    {
        return Qt::ItemIsUserCheckable  | Qt::ItemIsEnabled;
    }

    return sourceModel()->flags(index);
}

