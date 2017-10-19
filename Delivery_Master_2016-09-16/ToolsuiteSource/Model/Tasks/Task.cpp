#include "Task.h"

#include <QString>

Task::Task()
{}

Task::Task(const QString& initFunction, const QString& runnableFunction, quint32 startupDelay, quint32 periodicity) :
    InitFunction(initFunction),
    Runnable(runnableFunction),
    StartupDelay(startupDelay),
    Periodicity(periodicity)
{
}

QString Task::GetSchedulerEntry() const
{
    QString initEntry = (InitFunction.isEmpty() ? "NULL" : InitFunction);
    QString runnableEntry = (Runnable.isEmpty() ? "NULL" : Runnable);
    return QString("{ %1, %2, %3u, %4u, 0u }").arg(initEntry).arg(runnableEntry).arg(StartupDelay).arg(Periodicity);
}

QString Task::GetRunnableFunctionPrototype() const
{
    return QString("void %1(void)").arg(Runnable);
}

QString Task::GetInitFunctionPrototype() const
{
    return QString("void %1(void)").arg(InitFunction);
}
