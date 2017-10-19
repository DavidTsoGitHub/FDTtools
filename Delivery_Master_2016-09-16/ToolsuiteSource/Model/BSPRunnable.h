#ifndef BSPRUNNABLE_H
#define BSPRUNNABLE_H

#include <QString>

class BSPRunnable
{
public:
    BSPRunnable(QString initFunction, QString runnableFunction, quint32 cycletime, quint32 initialDelay);

    const QString GetInitFunction() const;
    const QString GetRunnableFunction() const;
    quint32 GetCycleTime() const;
    quint32 GetInitialDelay() const;

private:
    const QString m_initFunction;
    const QString m_runnableFunction;
    const quint32 m_cycleTime;
    const quint32 m_initialDelay;
};

#endif // BSPRUNNABLE_H
