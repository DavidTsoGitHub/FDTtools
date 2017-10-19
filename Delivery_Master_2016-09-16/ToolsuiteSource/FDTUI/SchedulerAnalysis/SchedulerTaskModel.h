#ifndef SCHEDULERTASKMODEL_H
#define SCHEDULERTASKMODEL_H

class Project;
class Task;

class QAbstractTableModel;
template <typename T> class QList;

class SchedulerTaskModel : public QAbstractTableModel
{
public:
    explicit SchedulerTaskModel(const Project& project);
    void Reset();

    typedef enum
    {
        RunnableFunctionColumn,
        StartupDelayColumn,
        PeriodicityColumn,
        SchedulerTaskColumnCount
    } SchedulerTaskColumns;

private:
    QList<const Task*> m_unchangedTasks;
    QList<Task*> m_tasks;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
};

#endif // SCHEDULERTASKMODEL_H
