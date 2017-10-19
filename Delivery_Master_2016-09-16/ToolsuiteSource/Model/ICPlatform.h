#ifndef ICPLATFORM_H
#define ICPLATFORM_H

#include "ProjectObject.h"
#include "IGCLSignalSource.h"
#include "IRunnable.h"

class ICPlatform : public ProjectObject, public IGCLSignalSource, public IRunnable
{
public:
    explicit ICPlatform(ProjectObject* parent = 0);
    ~ICPlatform();

private:
    QList<GCLSignalSource*> m_signalSources;
    QList<const Task*> m_tasks;

    // IGCLSignalSource interface
public:
    const QString GetComponentName() const;
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;

    // IRunnable interface
public:
    const QList<const Task*>& Tasks() const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // ICPLATFORM_H
