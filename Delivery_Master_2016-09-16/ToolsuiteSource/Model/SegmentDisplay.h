#ifndef SEGMENTDISPLAY_H
#define SEGMENTDISPLAY_H

#include "ProjectObject.h"
#include "IGCLSignalSink.h"
#include "IRunnable.h"

class BSPSegmentDisplay;

template <typename T> class QList;

class SegmentDisplay : public ProjectObject, public IGCLSignalSink, public IRunnable
{
public:
    explicit SegmentDisplay(ProjectObject* parent = 0);
    virtual ~SegmentDisplay();

    void SetBSPSegmentDefinition(const BSPSegmentDisplay& segmentDisplayDefinition);

private:
    void addGCLSignalSink(GCLSignalSink* sink);

    QList<GCLSignalSink*> m_signalSinks;
    QList<const Task*> m_tasks;

    bool m_isEnabled;
    bool m_isUsed;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // IRunnable interface
public:
    const QList<const Task*>& Tasks() const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // SEGMENTDISPLAY_H
