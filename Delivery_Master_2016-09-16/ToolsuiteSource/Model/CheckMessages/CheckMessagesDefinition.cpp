#include "CheckMessagesDefinition.h"

#include "CheckMessage.h"
#include "ProjectObjectVisitor.h"

#include <SignalMapping.h>

CheckMessageDefinition::CheckMessageDefinition(ProjectObject* parent) : ProjectObject(QString("CheckMessages"), parent)
{
    addGCLSignalSink(new GCLSignalSink("SystemPowerMode", "CheckMessageManager", "PowerMode"));
    addGCLSignalSink(new GCLSignalSink("ClearIndication", "CheckMessageManager", "uint8"));
}

CheckMessageDefinition::~CheckMessageDefinition()
{

}

void CheckMessageDefinition::AddCheckMessage(CheckMessage* checkMessage)
{
    m_checkMessages.append(checkMessage);
    checkMessage->SetParent(this);

    addGCLSignalSink(checkMessage->GetTrigger());

    NotifyPropertyUpdated();
}

void CheckMessageDefinition::AddWidgetSink(GCLSignalSink *sink)
{
    GCLSignalSource* source = new GCLSignalSource(sink->ComponentName() + "_" + sink->SignalName(), "CheckMessageManager", sink->DataType());
    sink->ConnectTo(source);
    m_allSources.append(source);
    source->setParent(this);
}

int CheckMessageDefinition::GetIndexOfSource(const QString& signalName, const QString& componentName)
{
    int counter = 0;
    foreach(GCLSignalSource* source, m_allSources)
    {
        if (source->SignalName().contains(signalName) && source->SignalName().contains(componentName))
        {
            return counter;
        }
        counter++;
    }

    return -1;
}

void CheckMessageDefinition::RemoveWidgetSink(GCLSignalSink *sink)
{
    int indexOfSource = GetIndexOfSource(sink->SignalName(), sink->ComponentName());

    if (indexOfSource < 0)
    {
        return;
    }

    foreach(GCLSignalSource* source, m_allSources)
    {
        if (source->SignalName().contains(sink->SignalName()) && source->SignalName().contains(sink->ComponentName()))
        {
            m_allSources.removeAll(source);
            sink->DisconnectFromSource();
            delete source;
            break;
        }  
    }
}

QList<CheckMessage *> &CheckMessageDefinition::CheckMessages()
{
    return m_checkMessages;
}

void CheckMessageDefinition::addGCLSignalSink(GCLSignalSink* sink)
{
    m_signalSinks.append(sink);
    sink->SetParent(this);
}

void CheckMessageDefinition::slot_RemoveWidgetSink(GCLSignalSink *sink)
{
    RemoveWidgetSink(sink);
}

void CheckMessageDefinition::RemoveMessages(quint32 index, quint32 count)
{
    //boundary check since the last empty row is selectable
    if ((index + count) > (quint32)m_checkMessages.count())
        count--;

    for (int i = count; i > 0; --i)
    {
        CheckMessage* message = m_checkMessages.at(index);

        GCLSignalSink* sink = message->GetTrigger();
        sink->DisconnectFromSource();
        m_signalSinks.removeOne(sink);

        m_checkMessages.removeOne(message);
        delete message;
    }

    if (m_checkMessages.count() == 0)
    {
        m_tasks.clear();
    }

    NotifyPropertyUpdated();
}

void CheckMessageDefinition::UpdateCheckMessageItemOrder(QList<quint32> associatedWidgetIDs)
{
    foreach (CheckMessage* checkMessage, m_checkMessages)
    {
        checkMessage->UpdateItemOrder(associatedWidgetIDs);
    }
    NotifyPropertyUpdated();
}

void CheckMessageDefinition::RemoveCheckMessageItemEverywhere(quint32 associatedWidgetID)
{
    foreach(CheckMessage* checkMessage, m_checkMessages)
    {
        checkMessage->RemoveItemByWidgetID(associatedWidgetID);
    }
    NotifyPropertyUpdated();
}

const QList<GCLSignalSink*>& CheckMessageDefinition::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* CheckMessageDefinition::GetSignalSink(QString signalName) const
{
    foreach(GCLSignalSink* sink, m_signalSinks)
    {
        if(sink->SignalName() == signalName)
        {
            return sink;
        }
    }
    return NULL;
}

void CheckMessageDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitCheckMessages(this);
}


const QString CheckMessageDefinition::GetComponentName() const
{
    return "CheckMessageManager";
}

const QList<GCLSignalSource*> &CheckMessageDefinition::GetSignalSources() const
{
    return m_allSources;
}

GCLSignalSource *CheckMessageDefinition::GetSignalSource(QString signalName) const
{
    foreach(GCLSignalSource* source, m_allSources)
    {
        if(source->SignalName() == signalName)
        {
            return source;
        }
    }
    return NULL;
}


const QList<const Task *> &CheckMessageDefinition::Tasks() const
{
    return m_tasks;
}
