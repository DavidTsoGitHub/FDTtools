#ifndef SIGNALSOURCEFINDERVISITOR_H
#define SIGNALSOURCEFINDERVISITOR_H

#include "ProjectObjectVisitor.h"

class GCLSignalSink;
class GCLSignalSource;

class SignalSourceFinderVisitor : public ProjectObjectVisitor
{
public:
    explicit SignalSourceFinderVisitor(const GCLSignalSink* sinkToMatch = 0);

    const QList<GCLSignalSource*>& GetFoundSources() const;

private:
    QList<GCLSignalSource*> m_foundSources;
    const GCLSignalSink* m_sinkToMatch;

    // ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject* projectObject);
};

#endif // SIGNALSOURCEFINDERVISITOR_H
