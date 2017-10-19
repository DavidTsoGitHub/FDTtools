#ifndef CANSPECIFICATION_H
#define CANSPECIFICATION_H

#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
#include "ProjectObject.h"

class CANMessage;
class CANSignal;
class GCLSignalEndpoint;
class QStringList;

class CANSpecification : public ProjectObject, public IGCLSignalSource, public IGCLSignalSink
{
public:

    typedef struct
    {
        quint8 PRESDIV : 8;
        quint8 RJW : 2;
        quint8 PSEG1 : 3;
        quint8 PSEG2 : 3;
        quint8 PROPSEG : 3;
    } DriverParameters;

    explicit CANSpecification(ProjectObject* parent = 0);
    ~CANSpecification();

    QStringList Modules;
    const QList<const CANMessage*> Messages() const;
    const CANSignal* GetCANSignalByGCLEndpoint(const GCLSignalEndpoint *endpoint) const;

    CANSpecification::DriverParameters GetDriverParameters() const;
    void SetDriverParameters(CANSpecification::DriverParameters driverParameters);

    void AddMessage(CANMessage* message);
    void AddGCLSignalSource(GCLSignalSource* source);
    void AddGCLSignalSink(GCLSignalSink* sink);
    void ParseGCLSignals(QString clusterECUName);

    void RemoveMessage(const CANMessage* message);

    void UpdateSpecification(CANSpecification& newCanSpecification);

    quint32 USDTFuncReqID() const;
    void SetUSDTFuncReqID(const quint32 &USDTFuncReqID);

    quint32 USDTReqID() const;
    void SetUSDTReqID(const quint32 &USDTReqID);

    quint32 USDTRespID() const;
    void SetUSDTRespID(const quint32 &USDTRespID);

    quint32 J1939SFID() const;
    void SetJ1939SFID(const quint32 &J1939SFID);
    quint32 J1939SFMask() const;
    void SetJ1939SFMask(const quint32 &J1939SFMask);
    quint32 J1939FFID() const;
    void SetJ1939FFID(const quint32 &J1939FFID);
    quint32 J1939FFMask() const;
    void SetJ1939FFMask(const quint32 &J1939FFMask);
    quint32 J1939CFID() const;
    void SetJ1939CFID(const quint32 &J1939CFID);
    quint32 J1939CFMask() const;
    void SetJ1939CFMask(const quint32 &J1939CFMask);
    quint32 j1939FCID() const;
    void setJ1939FCID(const quint32 &j1939FCID);
    quint32 j1939FCMask() const;
    void setJ1939FCMask(const quint32 &j1939FCMask);

private:
    QList<const CANMessage*> m_messages;
    QList<GCLSignalSource*> m_gclSignalSources;
    QList<GCLSignalSink*> m_gclSignalSinks;
    DriverParameters m_driverParameters;

    quint32 m_USDTFuncReqID;
    quint32 m_USDTReqID;
    quint32 m_USDTRespID;

    //J1939 MB configuration
    quint32 m_j1939SFID;
    quint32 m_j1939SFMask;
    quint32 m_j1939FFID;
    quint32 m_j1939FFMask;
    quint32 m_j1939CFID;
    quint32 m_j1939CFMask;

    QString getDataTypeForSignal(const CANSignal* signal) const;

    // IGCLSignalSource interface
public:
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;
    const QString GetComponentName() const;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // CANSPECIFICATION_H

