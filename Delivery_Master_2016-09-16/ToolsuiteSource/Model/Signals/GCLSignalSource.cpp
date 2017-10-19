#include "GCLSignalSource.h"

#include "GCLSignalSink.h"

GCLSignalSource::GCLSignalSource(const QString& signalName, const QString& componentName, const QString& dataType, ProjectObject* parent) :
    GCLSignalEndpoint(signalName, componentName, dataType, parent)
{
}

GCLSignalSource::~GCLSignalSource()
{
}

void GCLSignalSource::DisconnectAllSinks()
{
    foreach (GCLSignalSink* sink, m_sinks)
    {
        sink->DisconnectFromSource();
        DisconnectSink(sink);
    }
}

const QList<GCLSignalSink*> GCLSignalSource::GetConnectedSinks() const
{
    return m_sinks;
}

void GCLSignalSource::ConnectSink(GCLSignalSink* sink)
{
    if (m_sinks.contains(sink)) return;

    m_sinks.append(sink);
    connect(sink, &GCLSignalSink::destroyed, this, &GCLSignalSource::slot_sinkDestroyed);
    NotifyPropertyUpdated();
}

void GCLSignalSource::DisconnectSink(GCLSignalSink* sink)
{
    m_sinks.removeAll(sink);
    disconnect(sink, &GCLSignalSink::destroyed, this, &GCLSignalSource::slot_sinkDestroyed);
    NotifyPropertyUpdated();
}

bool GCLSignalSource::NeedsGCLMemberVariable() const
{
    if (m_componentName == "CAN" || m_componentName == "HAL")
    {
        // When CAN or HAL is source for a signal, GCL merely routes the GCL_Read function call to those components
        // Thus no member variable is needed in GCL.
        return false;
    }
    else if (m_dataType == "void")
    {
        return false;
    }
    else
    {
        foreach (GCLSignalSink* sink, m_sinks)
        {
            if (sink->ComponentName() != "CAN" && sink->ComponentName() != "HAL")
            {
                return true;
            }
        }
        return false;
    }
}

QString GCLSignalSource::GCLMemberVariableName() const
{
    return m_componentName + "_" + m_signalName;
}

bool GCLSignalSource::HasDynamicWidgetValueSink() const
{
    foreach (const GCLSignalSink* sink, m_sinks)
    {
        if (sink->DataType() == "DynamicWidgetValue")
        {
            return true;
        }
    }

    return false;
}

bool GCLSignalSource::HasSinkWithUpdateNotification() const
{
    foreach (const GCLSignalSink* sink, m_sinks)
    {
        if (sink->HasUpdateNotification())
        {
            return true;
        }
    }
    return false;
}

void GCLSignalSource::slot_sinkDestroyed(QObject* sink)
{
    m_sinks.removeAll(static_cast<GCLSignalSink*>(sink));
}

const QString GCLSignalSource::GetGCLFunctionSignature() const
{
    if (DataType() == "void")
    {
        return "void GCL_Write_" + ComponentName() + "_" + SignalName() + "(void)";
    }
    else
    {
        return "void GCL_Write_" + ComponentName() + "_" + SignalName() + "(" + DataType() + " value)";
    }
}

const QString GCLSignalSource::GetGCLFunctionName() const
{
    return "GCL_Write_" + ComponentName() + "_" + SignalName();
}


