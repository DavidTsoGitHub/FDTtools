#ifndef CHECKMESSAGES_H
#define CHECKMESSAGES_H

#include "ProjectObject.h"
#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
#include "IRunnable.h"

class CheckMessage;

class CheckMessageDefinition : public ProjectObject, public IGCLSignalSink, public IGCLSignalSource, public IRunnable
{
    Q_OBJECT

public:
    explicit CheckMessageDefinition(ProjectObject* parent = 0);
    ~CheckMessageDefinition();

    void AddCheckMessage(CheckMessage* checkMessage);
    void AddWidgetSink(GCLSignalSink* sink);
    void RemoveWidgetSink(GCLSignalSink *sink);
    void RemoveMessages(quint32 index, quint32 count);
    void RemoveCheckMessageItemEverywhere(quint32 associatedWidgetID);
    int GetIndexOfSource(const QString& signalName, const QString& componentName);
    QList<CheckMessage *> &CheckMessages(void);
    quint8 DisplayArea;
    void UpdateCheckMessageItemOrder(QList<quint32> associatedWidgetIDs);

private:
    void addGCLSignalSink(GCLSignalSink* sink);

    QList<CheckMessage*> m_checkMessages;
    QList<GCLSignalSink *> m_signalSinks;
    QList<GCLSignalSource *> m_allSources;
    QList<const Task*> m_tasks;

public slots:
    void slot_RemoveWidgetSink(GCLSignalSink* sink);

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink *>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // IGCLSignalSource interface
public:
    const QString GetComponentName() const;
    const QList<GCLSignalSource *> &GetSignalSources() const;
    GCLSignalSource *GetSignalSource(QString signalName) const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // IRunnable interface
public:
    const QList<const Task *> &Tasks() const;
};

#endif // CHECKMESSAGES_H
