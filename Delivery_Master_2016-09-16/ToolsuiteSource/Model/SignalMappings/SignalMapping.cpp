#include "SignalMapping.h"

/*!
 * \brief SignalMapping::NeedsGCLMemberVariable
 * \return True if GCL needs to generate a member variable for the mapping, False otherwise.
 * Usually a GCL member variable is only needed if the source is a function block or UI Widget at least one sink
 * is also a function block or UI Widget.

bool SignalMapping::NeedsGCLMemberVariable() const
{
    if (Source->ComponentName() == "CAN" || Source->ComponentName() == "HAL")
    {
        // When CAN or HAL is source for a signal, GCL merely routes the GCL_Read function call to those components
        // Thus no member variable is needed in GCL.
        return false;
    }

    foreach (const GCLSignalSink* sink, m_sinks)
    {
        if (sink->ComponentName() != "CAN" && sink->ComponentName() != "HAL")
        {
            return Source->DataType() != "void";
        }
    }

    return false;
}

 */


SignalMapping::SignalMapping()
{
}

SignalMapping::~SignalMapping()
{
}

void SignalMapping::AddSource(QString componentName, QString signalName)
{
    m_source.Name = componentName;
    m_source.Signal = signalName;
}

SimpleGCLEndpointDescription& SignalMapping::GetSource()
{
    return m_source;
}

void SignalMapping::AddSink(QString componentName, QString signalName)
{
    SimpleGCLEndpointDescription sink;
    sink.Name = componentName;
    sink.Signal = signalName;
    m_sinks.append(sink);
}

const QList<SimpleGCLEndpointDescription>& SignalMapping::GetSinks()
{
    return m_sinks;
}
