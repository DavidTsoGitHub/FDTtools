#include "GCLSignalSink.h"
#include "GCLSignalSource.h"

GCLSignalSink::GCLSignalSink(const QString& signalName, const QString& componentName, const QString& dataType, bool updateNotification, bool mandatory, ProjectObject* parent) :
    GCLSignalEndpoint(signalName, componentName, dataType, parent),
    m_signalSource(NULL),
    m_updateNotification(updateNotification),
    m_mandatory(mandatory)
{
}

GCLSignalSink::GCLSignalSink(const GCLSignalSink& sink) :
    GCLSignalSink(sink.SignalName(), sink.ComponentName(), sink.DataType(), sink.m_updateNotification, sink.m_mandatory, sink.GetParent())
{
    m_signalSource = sink.m_signalSource;
}

GCLSignalSink::GCLSignalSink() :
    GCLSignalEndpoint("unnamed", "unnamed", "void*")
{
}

GCLSignalSink::~GCLSignalSink()
{
}

bool GCLSignalSink::AllowsConnectionTo(const GCLSignalSource* source) const
{
    // The source must exist
    if (source == NULL) return false;

    // We cannot map to ourselves
    if (source->ComponentName() == m_componentName) return false;

    // If the types are the same, no problem!
    if (source->DataType() == m_dataType) return true;

    // If we are a DynamicWidgetValue sink and the source is a basic data type, no problem!
    if (source->IsBasicDataType() && m_dataType == "DynamicWidgetValue") return true;

    return false;
}

void GCLSignalSink::ConnectTo(GCLSignalSource* newSource)
{
    if (m_signalSource != NULL)
    {
        disconnect(m_signalSource, &GCLSignalSource::destroyed, this, &GCLSignalSink::slot_sourceDestroyed);
        m_signalSource->DisconnectSink(this);
    }

    if (newSource != NULL)
    {
        newSource->ConnectSink(this);
        connect(newSource, &GCLSignalSource::destroyed, this, &GCLSignalSink::slot_sourceDestroyed);
    }

    m_signalSource = newSource;
    NotifyPropertyUpdated();
}

void GCLSignalSink::DisconnectFromSource()
{
    ConnectTo(NULL);
}

GCLSignalSource* GCLSignalSink::GetSignalSource() const
{
    return m_signalSource;
}

bool GCLSignalSink::IsMandatory() const
{
    return m_mandatory;
}

bool GCLSignalSink::HasUpdateNotification() const
{
    return m_updateNotification;
}

const QString GCLSignalSink::GetGCLFunctionSignature() const
{
    if (DataType() == "void")
    {
        return "void GCL_Read_" + ComponentName() + "_"  + SignalName() + "(uint8* status)";
    }
    else
    {
        return "void GCL_Read_" + ComponentName() + "_" + SignalName() + "(" + DataType() + "* value, uint8* status)";
    }
}

const QString GCLSignalSink::GetGCLFunctionName() const
{
    return "GCL_Read_" + ComponentName() + "_"  + SignalName();
}

const QString GCLSignalSink::GetNotificationFunctionSignature() const
{
    return "void " + ComponentName() + "_" + SignalName() + "_Indication(void)";
}

const QString GCLSignalSink::GetNotificationFunctionCall() const
{
    return ComponentName() + "_" + SignalName() + "_Indication()";
}

void GCLSignalSink::slot_sourceDestroyed(QObject* source)
{
    if (source == m_signalSource)
    {
        m_signalSource = NULL;
    }
    NotifyPropertyUpdated();
}
