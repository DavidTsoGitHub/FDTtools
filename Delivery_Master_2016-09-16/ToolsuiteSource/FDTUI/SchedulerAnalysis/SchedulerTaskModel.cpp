#include "SchedulerTaskModel.h"

#include "Project.h"
#include "Task.h"

#include <QAbstractTableModel>
#include <QList>

SchedulerTaskModel::SchedulerTaskModel(const Project& project)
{
    project.GetAllTasks(m_unchangedTasks);

    Reset();
}

void SchedulerTaskModel::Reset()
{
    beginResetModel();
    m_tasks.clear();
    foreach (const Task* task, m_unchangedTasks)
    {
        m_tasks.append(new Task(task->InitFunction, task->Runnable, task->StartupDelay, task->Periodicity));
    }
    endResetModel();
}

int SchedulerTaskModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_tasks.count();
}

int SchedulerTaskModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return SchedulerTaskColumnCount;
}

QVariant SchedulerTaskModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) return QVariant();

    const Task* task = m_tasks.at(index.row());

    switch (index.column())
    {
        case RunnableFunctionColumn:
            return QVariant::fromValue(task->Runnable);
        case StartupDelayColumn:
            return QVariant::fromValue(task->StartupDelay);
        case PeriodicityColumn:
            return QVariant::fromValue(task->Periodicity);
        default:
            return QVariant();
    }
}

QVariant SchedulerTaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

    switch (section)
    {
        case RunnableFunctionColumn:
            return QVariant("Task Runnable");
        case StartupDelayColumn:
            return QVariant("Startup delay");
        case PeriodicityColumn:
            return QVariant("Periodicity");
        default:
            return QVariant();
    }
}

Qt::ItemFlags SchedulerTaskModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    switch (index.column())
    {
        case StartupDelayColumn:
        case PeriodicityColumn:
            return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
        default:
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}

bool SchedulerTaskModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    switch (index.column())
    {
        case PeriodicityColumn:
            if (value.canConvert(QVariant::Int) && value.toInt() > 0)
            {
                m_tasks.at(index.row())->Periodicity = value.toInt();
                emit dataChanged(index, index, QVector<int>() << role);
                return true;
            }
            return false;
        case StartupDelayColumn:
            if (value.canConvert(QVariant::Int))
            {
                m_tasks.at(index.row())->StartupDelay = value.toInt();
                emit dataChanged(index, index, QVector<int>() << role);
                return true;
            }
            return false;
        default:
            return false;
    }

}
