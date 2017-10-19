#include "GCLSignalEndpoint.h"
#include "ProjectObjectVisitor.h"

GCLSignalEndpoint::GCLSignalEndpoint(const QString& signalName, const QString& componentName, const QString& dataType, ProjectObject* parent) :
    ProjectObject(QString(componentName).append(".").append(signalName), parent),
    m_signalName(signalName),
    m_componentName(componentName),
    m_dataType(dataType)
{
}

const QString GCLSignalEndpoint::SignalName() const
{
    return m_signalName;
}

const QString GCLSignalEndpoint::ComponentName() const
{
    return m_componentName;
}

const QString GCLSignalEndpoint::DataType() const
{
    return m_dataType;
}

const QString GCLSignalEndpoint::LongName() const
{
    return m_componentName + "." + m_signalName;
}

bool GCLSignalEndpoint::IsBasicDataType() const
{
    return m_dataType == "uint8" ||
           m_dataType == "uint16" ||
           m_dataType == "uint32" ||
           m_dataType == "sint8" ||
           m_dataType == "sint16" ||
           m_dataType == "sint32";
}


void GCLSignalEndpoint::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitGCLSignalEndpoint(this);
}
