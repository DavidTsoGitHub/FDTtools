#ifndef GCLSIGNALENDPOINT_H
#define GCLSIGNALENDPOINT_H

#include "ProjectObject.h"

class GCLSignalEndpoint : public ProjectObject
{
protected:
    GCLSignalEndpoint(const QString& signalName, const QString& componentName, const QString& dataType, ProjectObject* parent = 0);
    const QString m_signalName;
    const QString m_componentName;
    const QString m_dataType;

public:
    const QString SignalName(void) const;
    const QString ComponentName(void) const;
    const QString DataType(void) const;
    const QString LongName(void) const;
    bool IsBasicDataType() const;
    virtual const QString GetGCLFunctionSignature(void) const = 0;
    virtual const QString GetGCLFunctionName(void) const = 0;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // GCLSIGNALENDPOINT_H
