#include "BSPRunnable.h"

BSPRunnable::BSPRunnable(QString initFunction, QString runnable, quint32 cycleTime, quint32 initialDelay) :
    m_initFunction(initFunction),
    m_runnableFunction(runnable),
    m_cycleTime(cycleTime),
    m_initialDelay(initialDelay)
{

}

const QString BSPRunnable::GetInitFunction() const
{
    return m_initFunction;
}

const QString BSPRunnable::GetRunnableFunction() const
{
    return m_runnableFunction;
}

quint32 BSPRunnable::GetCycleTime() const
{
    return m_cycleTime;
}

quint32 BSPRunnable::GetInitialDelay() const
{
    return m_initialDelay;
}
