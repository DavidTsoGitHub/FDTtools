#include "SinkFinderVisitor.h"

#include "GCLSignalSink.h"

#include <QString>

SinkFinderVisitor::SinkFinderVisitor(const QString& componentName, const QString& signalName) : ProjectObjectVisitor(true),
    m_componentName(componentName),
    m_signalName(signalName)
{
}

void SinkFinderVisitor::VisitProjectObject(ProjectObject* object)
{
    if (object->objectName().compare(m_componentName) != 0) return;

    IGCLSignalSink* sinkObject = dynamic_cast<IGCLSignalSink*>(object);
    if (sinkObject == NULL) return;

    GCLSignalSink* sink = sinkObject->GetSignalSink(m_signalName);

    if (sink != NULL) m_foundSink = sink;
}
