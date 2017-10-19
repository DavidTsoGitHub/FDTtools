#include "function.h"

Function::Function(QString functionName, type_t type, QString valueParameter,  qint32 indexParameter) :
    m_functionName(functionName),
    m_type(type),
    m_indexParameter(indexParameter),
    m_valueParameter(valueParameter)
{
}

Function* Function::Deserialize(const QJsonObject& json)
{
    QString functionName = json["function_name"].toString();
    type_t type;
    if (functionName.startsWith("BSP_Set"))
    {
        type = SET;
    }
    else if (functionName.startsWith("BSP_Get"))
    {
        type = GET;
    }
    else
    {
        type = NO_TYPE;
    }
    qint32 indexParameter = json["index_parameter"].toInt();
    QString valueParameter = json["value_parameter"].toString();

    return new Function(functionName, type, valueParameter, indexParameter);
}

void Function::Serialize(QJsonObject& json) const
{
    switch (m_type)
    {
    case GET: json["function_name"] = QString("PCIC_Get_") + m_functionName; break;
    case SET: json["function_name"] = QString("PCIC_Set_") + m_functionName; break;
    case NO_TYPE:
    default:
        json["function_name"] = QString("PCIC_") + m_functionName;
        break;
    }

    json["index_parameter"] = m_indexParameter;
    json["value_parameter"] = m_valueParameter;
}

QString Function::GetName()
{
    return m_functionName;
}

qint32 Function::GetIndex()
{
    return m_indexParameter;
}

QString Function::GetValue()
{
    return m_valueParameter;
}
