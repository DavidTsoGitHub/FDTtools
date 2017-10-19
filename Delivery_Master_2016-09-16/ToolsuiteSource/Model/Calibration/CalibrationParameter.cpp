#include "CalibrationParameter.h"

CalibrationParameter::CalibrationParameter()
{
}

void CalibrationParameter::SetDescriptiveName(QString descriptiveName)
{
    m_descriptiveName = descriptiveName;
}

const QString CalibrationParameter::GetDescriptiveName() const
{
    return m_descriptiveName;
}

void CalibrationParameter::SetParameterName(QString parameterName)
{
    m_parameterName = parameterName;
}

const QString CalibrationParameter::GetParameterName() const
{
    return m_parameterName;
}

void CalibrationParameter::SetDescription(QString description)
{
    m_description = description;
}

const QString CalibrationParameter::GetDescription() const
{
    return m_description;
}

void CalibrationParameter::SetDataType(QString dataType)
{
    m_dataType = dataType;
}

const QString CalibrationParameter::GetDataType() const
{
    return m_dataType;
}

void CalibrationParameter::SetArrayLength(quint8 arrayLength)
{
    m_arrayLength = arrayLength;
}

quint8 CalibrationParameter::GetArrayLength() const
{
    return m_arrayLength;
}


