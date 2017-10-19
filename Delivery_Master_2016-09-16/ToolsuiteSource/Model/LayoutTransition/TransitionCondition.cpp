#include "TransitionCondition.h"

#include "GCLSignalSink.h"

TransitionCondition::TransitionCondition(QString gclComponentName, QString gclPortName, ProjectObject *parent) : ProjectObject("TransitionCondition", parent)
{
    m_signalSinks.append(new GCLSignalSink(gclPortName, gclComponentName, "DynamicWidgetValue"));
    m_compareOperator = LayoutTransitionTypes::EqualTo;
    m_conditionValue = 0;

}

TransitionCondition::~TransitionCondition()
{
    foreach (GCLSignalSink* sink, m_signalSinks)
    {
        sink->DisconnectFromSource();
    }
}

LayoutTransitionTypes::TransitionCompare TransitionCondition::GetCompareOperator()
{
    return m_compareOperator;
}

void TransitionCondition::SetCompareOperator(LayoutTransitionTypes::TransitionCompare compareOperator)
{
    if (m_compareOperator != compareOperator)
    {
        m_compareOperator = compareOperator;
        NotifyPropertyUpdated();
    }
}

quint32 TransitionCondition::GetConditionValue()
{
    return m_conditionValue;
}

void TransitionCondition::SetConditionValue(quint32 conditionValue)
{
    if (m_conditionValue != conditionValue)
    {
        m_conditionValue = conditionValue;
        NotifyPropertyUpdated();
    }
}

const QList<GCLSignalSink*> &TransitionCondition::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* TransitionCondition::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, m_signalSinks)
    {
        if (sink->SignalName() == signalName)
        {
            return sink;
        }
    }

    return NULL;
}

void TransitionCondition::SetSignalSink(GCLSignalSink* sink){
    m_signalSinks.clear();
    m_signalSinks.append(sink);
    sink->setParent(this);
}
