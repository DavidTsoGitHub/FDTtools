#ifndef GCLSIGNALSINK_H
#define GCLSIGNALSINK_H

#include "GCLSignalEndpoint.h"

class GCLSignalSource;

class GCLSignalSink : public GCLSignalEndpoint
{
public:
    GCLSignalSink(const QString& signalName, const QString& componentName, const QString& dataType, bool updateNotification = false, bool mandatory = true, ProjectObject* parent = 0);
    GCLSignalSink(const GCLSignalSink& sink);
    GCLSignalSink();
    ~GCLSignalSink();

    bool AllowsConnectionTo(const GCLSignalSource* source) const;
    void ConnectTo(GCLSignalSource* source);
    void DisconnectFromSource();
    GCLSignalSource* GetSignalSource() const;

    bool IsMandatory() const;
    bool HasUpdateNotification() const;
    const QString GetNotificationFunctionSignature() const;
    const QString GetNotificationFunctionCall() const;

private:
    GCLSignalSource* m_signalSource;
    bool m_updateNotification;
    bool m_mandatory;

private slots:
    void slot_sourceDestroyed(QObject* source);

    // GCLSignalEndpoint interface
public:
    const QString GetGCLFunctionSignature() const;
    const QString GetGCLFunctionName(void) const;
};

Q_DECLARE_METATYPE(GCLSignalSink)

#endif // GCLSIGNALSINK_H
