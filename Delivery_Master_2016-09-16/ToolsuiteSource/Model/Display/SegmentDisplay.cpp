#include "SegmentDisplay.h"

#include "BSPSegmentDisplay.h"
#include "ProjectObjectVisitor.h"

SegmentDisplay::SegmentDisplay(ProjectObject* parent) : ProjectObject(QString("SegmentDisplay"), parent),
    m_isEnabled(false),
    m_isUsed(false)
{
    addGCLSignalSink(new GCLSignalSink("SystemPowerMode", "SegmentDisplayManager", "PowerMode"));
    addGCLSignalSink(new GCLSignalSink("DisplayValue", "SegmentDisplayManager", "uint32"));

    m_tasks.append(new Task("SegmentDisplayManager_Init", "SegmentDisplayManager_Run", 25, 500));
}

SegmentDisplay::~SegmentDisplay()
{
}

void SegmentDisplay::SetBSPSegmentDefinition(const BSPSegmentDisplay& segmentDisplayDefinition)
{
    m_signalSinks.clear();
    addGCLSignalSink(new GCLSignalSink("PowerMode", "SegmentDisplayManager", "PowerMode"));
    addGCLSignalSink(new GCLSignalSink("DisplayValue", "SegmentDisplayManager", "uint32"));
    foreach (QString specialSegment, segmentDisplayDefinition.GetSpecialSegments())
    {
        QString signalName(specialSegment + "SegmentStatus");
        addGCLSignalSink(new GCLSignalSink(signalName, "SegmentDisplayManager", "uint8"));
    }
}

void SegmentDisplay::addGCLSignalSink(GCLSignalSink* sink)
{
    m_signalSinks.append(sink);
    sink->SetParent(this);
}

const QList<GCLSignalSink*>& SegmentDisplay::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* SegmentDisplay::GetSignalSink(QString signalName) const
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

const QList<const Task*>& SegmentDisplay::Tasks() const
{
    return m_tasks;
}


void SegmentDisplay::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitSegmentDisplay(this);
}
