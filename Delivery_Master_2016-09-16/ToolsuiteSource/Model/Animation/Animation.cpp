#include "Animation.h"
#include "IGCLSignalSink.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"


quint32 Animation::animationCount = 0;

Animation::Animation() : ProjectObject(QString("Animation")),
    m_id(0),
    m_name("Animation"),
    m_gclComponentName(""),
    m_repeatMode(RepeatMode::ONESHOT)
{
}

Animation::~Animation()
{
}

quint32 Animation::GetID() const
{
    return m_id;
}

void Animation::SetID(const quint32 id)
{
    m_id = id;
    NotifyPropertyUpdated();
}

quint32 Animation::GetNewID()
{
    return ++animationCount;
}

quint32 Animation::GetAnimationCount()
{
    return animationCount;
}

void Animation::SetAnimationCount(quint32 id)
{
    animationCount = id;
}

void Animation::ResetAnimationCount()
{
    animationCount = 0;
}

const QString Animation::GetName() const
{
    return m_name;
}

void Animation::SetName(const QString& name)
{
    m_name = name;
    setObjectName(name);
    m_gclComponentName = "AnimationManager_" + m_name.replace(" ", "_") + "_" + QString::number(GetID());
    //UpdateAllInternalSinks();
    UpdateAllConnectedSinks();
    NotifyPropertyUpdated();
}

void Animation::SetRepeatMode(Animation::RepeatMode mode)
{
    m_repeatMode = mode;
    NotifyPropertyUpdated();
}

Animation::RepeatMode Animation::GetRepeatMode() const
{
    return m_repeatMode;
}

void Animation::SetDefaultStep(int value)
{
    m_defaultStep = value;
    NotifyPropertyUpdated();
}

int Animation::GetDefaultStep() const
{
    return m_defaultStep;
}

const QString Animation::GetComponentName() const
{
    return m_gclComponentName;
}

const QList<GCLSignalSource *> &Animation::GetSignalSources() const
{
    return m_sources;
}

GCLSignalSource *Animation::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_sources)
    {
        if(source->SignalName().compare(signalName)==0)
        {
            return source;
        }
    }
    return NULL;
}

void Animation::AddSignalSource(QString signalName, QString dataType)
{
    m_gclComponentName = "AnimationManager_" + m_name + "_" + QString::number(GetID());
    GCLSignalSource* source = new GCLSignalSource(signalName, m_gclComponentName, dataType, this);
    m_sources.append(source);
    source->SetParent(this);
    NotifyPropertyUpdated();
}

void Animation::RemoveSignalSource(QString signalName)
{
    int i = 0;
    foreach (const GCLSignalSource* source, m_sources)
    {
        if (0 == source->SignalName().compare(signalName))
        {
            m_sources.removeAt(i);
            NotifyPropertyUpdated();
            delete source;
        }
        ++i;
    }
}

void Animation::UpdateAllConnectedSinks()
{
    QList<GCLSignalSource*> newSignalSources;

    foreach (GCLSignalSource* source, m_sources)
    {
        GCLSignalSource* newSource = new GCLSignalSource(source->SignalName(), m_gclComponentName, source->DataType());
        newSignalSources.append(newSource);

        foreach (GCLSignalSink* sink, source->GetConnectedSinks())
        {
            sink->ConnectTo(newSource);
        }
        m_sources.removeAll(source);
    }

    foreach (GCLSignalSource* source, newSignalSources)
    {
        AddGCLSignalSource(source);
    }
}

void Animation::AddGCLSignalSource(GCLSignalSource* source)
{
    m_sources.append(source);
    source->SetParent(this);
}
