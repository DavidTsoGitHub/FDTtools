#include "SignalSourceFinderVisitor.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"

SignalSourceFinderVisitor::SignalSourceFinderVisitor(const GCLSignalSink* sinkToMatch) : ProjectObjectVisitor(true),
    m_sinkToMatch(sinkToMatch)
{
}

const QList<GCLSignalSource*>& SignalSourceFinderVisitor::GetFoundSources() const
{
    return m_foundSources;
}

void SignalSourceFinderVisitor::VisitProjectObject(ProjectObject* projectObject)
{
    IGCLSignalSource* sourceObject = dynamic_cast<IGCLSignalSource*>(projectObject);

    if (sourceObject != NULL)
    {
        foreach (GCLSignalSource* source, sourceObject->GetSignalSources())
        {
            if (m_sinkToMatch == NULL || m_sinkToMatch->AllowsConnectionTo(source))
            {
                m_foundSources.append(source);
            }
        }
    }
}
