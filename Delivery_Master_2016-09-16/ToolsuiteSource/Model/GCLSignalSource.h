#ifndef GCLSIGNALSOURCE_H
#define GCLSIGNALSOURCE_H

#include "GCLSignalEndpoint.h"

class GCLSignalSink;

class GCLSignalSource : public GCLSignalEndpoint
{
public:
    GCLSignalSource(const QString& signalName, const QString& componentName, const QString& dataType, ProjectObject* parent = 0);
    ~GCLSignalSource();

    void ConnectSink(GCLSignalSink* sink);
    void DisconnectSink(GCLSignalSink* sink);
    void DisconnectAllSinks();

    bool NeedsGCLMemberVariable() const;
    QString GCLMemberVariableName() const;
    bool HasDynamicWidgetValueSink() const;
    bool HasSinkWithUpdateNotification() const;

    const QList<GCLSignalSink*> GetConnectedSinks() const;

private:
    QList<GCLSignalSink*> m_sinks;

private slots:
    void slot_sinkDestroyed(QObject* sink);

    // GCLSignalEndpoint interface
public:
    const QString GetGCLFunctionSignature(void) const;
    const QString GetGCLFunctionName(void) const;
};
#endif // GCLSIGNALSOURCE_H
