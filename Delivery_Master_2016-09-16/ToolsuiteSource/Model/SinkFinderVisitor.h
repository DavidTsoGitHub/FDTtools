#ifndef SINKFINDERVISITOR_H
#define SINKFINDERVISITOR_H

#include "ProjectObjectVisitor.h"
#include <QString>

class GCLSignalSink;

class SinkFinderVisitor : public ProjectObjectVisitor
{
public:
    SinkFinderVisitor(const QString& componentName, const QString& signalName);

    GCLSignalSink* GetFoundSink();

private:
    QString m_componentName;
    QString m_signalName;

    GCLSignalSink* m_foundSink;

    // ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject*);
};

#endif // SINKFINDERVISITOR_H
