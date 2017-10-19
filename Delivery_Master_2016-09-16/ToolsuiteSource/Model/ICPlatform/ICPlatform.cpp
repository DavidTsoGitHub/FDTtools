#include "ICPlatform.h"
#include "ProjectObjectVisitor.h"

ICPlatform::ICPlatform(ProjectObject* parent) : ProjectObject(QString("Platform"), parent)
{
    m_signalSources.append(new GCLSignalSource("PowerMode", "Platform", "PowerMode", this));

    m_tasks.append(new Task("ModeManager_Init", "ModeManager_Run", 15, 50));
    m_tasks.append(new Task("Diag_Init", "Diag_Run", 15, 50));
    m_tasks.append(new Task("SoundManager_Init", "", 0xFFFF, 0xFFFF));
    m_tasks.append(new Task("COM_Stack_Init", "", 0, 5));
}

ICPlatform::~ICPlatform()
{
    while (!m_signalSources.isEmpty())
    {
        delete m_signalSources.takeFirst();
    }
}

const QString ICPlatform::GetComponentName() const
{
    return "Platform";
}

const QList<GCLSignalSource*>& ICPlatform::GetSignalSources() const
{
    return m_signalSources;
}

GCLSignalSource* ICPlatform::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_signalSources)
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }

    return NULL;
}

const QList<const Task*>&ICPlatform::Tasks() const
{
    return m_tasks;
}


void ICPlatform::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitICPlatform(this);
}
