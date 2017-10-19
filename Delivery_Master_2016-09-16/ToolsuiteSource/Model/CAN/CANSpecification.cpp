#include "CANSpecification.h"

#include "CANMessage.h"
#include "ProjectObjectVisitor.h"

CANSpecification::CANSpecification(ProjectObject* parent) :
    ProjectObject(QString("CAN"), parent)
{
    m_driverParameters.PRESDIV = 0;
    m_driverParameters.PROPSEG = 0;
    m_driverParameters.PSEG1 = 0;
    m_driverParameters.PSEG2 = 0;
    m_driverParameters.RJW = 0;
    m_USDTFuncReqID = 0;
    m_USDTReqID = 0;
    m_USDTRespID = 0;

}

CANSpecification::~CANSpecification()
{
    while (!m_gclSignalSinks.isEmpty())
    {
        delete m_gclSignalSinks.takeFirst();
    }

    while (!m_gclSignalSources.isEmpty())
    {
        delete m_gclSignalSources.takeFirst();
    }
}

const QList<const CANMessage *> CANSpecification::Messages() const
{
    return m_messages;
}

CANSpecification::DriverParameters CANSpecification::GetDriverParameters() const
{
    return m_driverParameters;
}

void CANSpecification::SetDriverParameters(CANSpecification::DriverParameters driverParameters)
{
    m_driverParameters = driverParameters;
    NotifyPropertyUpdated();
}

void CANSpecification::AddMessage(CANMessage *message)
{
    m_messages.append(message);
}

void CANSpecification::AddGCLSignalSource(GCLSignalSource* source)
{
    m_gclSignalSources.append(source);
}

void CANSpecification::AddGCLSignalSink(GCLSignalSink* sink)
{
    m_gclSignalSinks.append(sink);
}

void CANSpecification::ParseGCLSignals(QString clusterECUName)
{
    foreach (const CANMessage* message, Messages())
    {
        bool isSink = false;

        if (message->Producer == clusterECUName)
        {
            isSink = true;
        }
        foreach (const CANSignal* signal, message->Signals())
        {
            QString dataType = getDataTypeForSignal(signal);

            if (isSink)
            {
                GCLSignalSink* sink = new GCLSignalSink(signal->Name, "CAN", dataType);

                AddGCLSignalSink(sink);
            }
            else if (signal->Consumers.contains(clusterECUName))
            {
                GCLSignalSource* source = new GCLSignalSource(signal->Name, "CAN", dataType);

                AddGCLSignalSource(source);
            }
        }
    }
}

void CANSpecification::RemoveMessage(const CANMessage *message)
{
    foreach(const CANMessage* oldMessage, m_messages)
    {
        if(message->CANID == oldMessage->CANID)
        {
            m_messages.removeAll(oldMessage);
        }
    }
}

void CANSpecification::UpdateSpecification(CANSpecification& newCanSpecification)
{    
    QList<GCLSignalSource*> allNewSources = newCanSpecification.GetSignalSources();
    QList<GCLSignalSource*> allOldSources = m_gclSignalSources;

    foreach (GCLSignalSource* oldSource, m_gclSignalSources)
    {
        foreach(GCLSignalSource* newSource, newCanSpecification.GetSignalSources())
        {
            if(newSource->SignalName() == oldSource->SignalName())
            {
                allNewSources.removeAll(newSource);
                allOldSources.removeAll(oldSource);
            }
        }
    }

    QList<GCLSignalSink*> allNewSinks = newCanSpecification.GetSignalSinks();
    QList<GCLSignalSink*> allOldSinks = m_gclSignalSinks;

    foreach (GCLSignalSink* oldSink, m_gclSignalSinks)
    {
        foreach(GCLSignalSink* newSink, newCanSpecification.GetSignalSinks())
        {
            if(newSink->SignalName() == oldSink->SignalName())
            {
                allNewSinks.removeAll(newSink);
                allOldSinks.removeAll(oldSink);
            }
        }
    }

    foreach (GCLSignalSource* source, allNewSources)
    {
        GCLSignalSource* newSource = new GCLSignalSource(source->SignalName(), "CAN", source->DataType());
        AddGCLSignalSource(newSource);
    }

    foreach (GCLSignalSink* sink, allNewSinks)
    {
        GCLSignalSink* newSink = new GCLSignalSink(sink->SignalName(), "CAN", sink->DataType());
        AddGCLSignalSink(newSink);
    }

    foreach (GCLSignalSource* source, allOldSources)
    {
        m_gclSignalSources.removeAll(source);
        delete source;
    }

    foreach (GCLSignalSink* sink, allOldSinks)
    {
        m_gclSignalSinks.removeAll(sink);
        sink->DisconnectFromSource();
        delete sink;
    }

    while (!m_messages.isEmpty())
    {
        delete m_messages.takeFirst();
    }

    foreach(const CANMessage* newMessage, newCanSpecification.Messages())
    {
        m_messages.append(newMessage);
    }
}

QString CANSpecification::getDataTypeForSignal(const CANSignal* signal) const
{
    if (signal->BitLength > 16) return "uint32";
    if (signal->BitLength > 8) return "uint16";
    return "uint8";
}

const CANSignal* CANSpecification::GetCANSignalByGCLEndpoint(const GCLSignalEndpoint* endpoint) const
{
    if (endpoint->ComponentName() != "CAN") return NULL;

    foreach (const CANMessage* message, Messages())
    {
        foreach (const CANSignal* signal, message->Signals())
        {
            if (signal->Name == endpoint->SignalName())
            {
                return signal;
            }
        }
    }
    return NULL;
}

const QList<GCLSignalSource*>& CANSpecification::GetSignalSources() const
{
    return m_gclSignalSources;
}

const QList<GCLSignalSink*>& CANSpecification::GetSignalSinks() const
{
    return m_gclSignalSinks;
}

GCLSignalSink* CANSpecification::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, m_gclSignalSinks)
    {
        if (sink->SignalName() == signalName)
        {
            return sink;
        }
    }
    return NULL;
}

quint32 CANSpecification::USDTFuncReqID() const
{
    return m_USDTFuncReqID;
}

void CANSpecification::SetUSDTFuncReqID(const quint32 &USDTFuncReqID)
{
    m_USDTFuncReqID = USDTFuncReqID;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::USDTReqID() const
{
    return m_USDTReqID;
}

void CANSpecification::SetUSDTReqID(const quint32 &USDTReqID)
{
    m_USDTReqID = USDTReqID;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::USDTRespID() const
{
    return m_USDTRespID;
}

void CANSpecification::SetUSDTRespID(const quint32 &USDTRespID)
{
    m_USDTRespID = USDTRespID;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939CFMask() const
{
    return m_j1939CFMask;
}

void CANSpecification::SetJ1939CFMask(const quint32 &j1939CFMask)
{
    m_j1939CFMask = j1939CFMask;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939CFID() const
{
    return m_j1939CFID;
}

void CANSpecification::SetJ1939CFID(const quint32 &j1939CFID)
{
    m_j1939CFID = j1939CFID;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939FFMask() const
{
    return m_j1939FFMask;
}

void CANSpecification::SetJ1939FFMask(const quint32 &j1939FFMask)
{
    m_j1939FFMask = j1939FFMask;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939FFID() const
{
    return m_j1939FFID;
}

void CANSpecification::SetJ1939FFID(const quint32 &j1939FFID)
{
    m_j1939FFID = j1939FFID;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939SFMask() const
{
    return m_j1939SFMask;
}

void CANSpecification::SetJ1939SFMask(const quint32 &j1939SFMask)
{
    m_j1939SFMask = j1939SFMask;
    NotifyPropertyUpdated();
}

quint32 CANSpecification::J1939SFID() const
{
    return m_j1939SFID;
}

void CANSpecification::SetJ1939SFID(const quint32 &j1939SFID)
{
    m_j1939SFID = j1939SFID;
    NotifyPropertyUpdated();
}

GCLSignalSource* CANSpecification::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_gclSignalSources)
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }
    return NULL;
}

const QString CANSpecification::GetComponentName() const
{
    return "CAN";
}

void CANSpecification::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitCANSpecification(this);
}
