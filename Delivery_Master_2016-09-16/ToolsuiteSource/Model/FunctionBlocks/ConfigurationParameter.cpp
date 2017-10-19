#include "ConfigurationParameter.h"

ConfigurationParameter::ConfigurationParameter()
{
}

void ConfigurationParameter::SetDescriptiveName(QString descriptiveName)
{
    m_descriptiveName = descriptiveName;
}

const QString ConfigurationParameter::GetDescriptiveName() const
{
    return m_descriptiveName;
}

void ConfigurationParameter::SetParameterName(QString parameterName)
{
    m_parameterName = parameterName;
}

const QString ConfigurationParameter::GetParameterName() const
{
    return m_parameterName;
}

void ConfigurationParameter::SetDescription(QString description)
{
    m_description = description;
}

const QString ConfigurationParameter::GetDescription() const
{
    return m_description;
}

void ConfigurationParameter::SetDataType(QString dataType)
{
    m_dataType = dataType;
}

const QString ConfigurationParameter::GetDataType() const
{
    return m_dataType;
}

void ConfigurationParameter::SetArrayLength(quint8 arrayLength)
{
    m_arrayLength = arrayLength;
}

quint8 ConfigurationParameter::GetArrayLength() const
{
    return m_arrayLength;
}
