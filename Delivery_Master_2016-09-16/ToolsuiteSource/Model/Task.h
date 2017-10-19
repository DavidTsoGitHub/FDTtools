#ifndef TASK_H
#define TASK_H

class QString;

class Task
{
public:
    explicit Task();
    Task(const QString& initFunction, const QString& runnableFunction, quint32 startupDelay, quint32 periodicity);
    QString InitFunction;
    QString Runnable;
    quint32 StartupDelay;
    quint32 Periodicity;

    QString GetSchedulerEntry() const;
    QString GetRunnableFunctionPrototype() const;
    QString GetInitFunctionPrototype() const;
};

#endif // TASK_H
