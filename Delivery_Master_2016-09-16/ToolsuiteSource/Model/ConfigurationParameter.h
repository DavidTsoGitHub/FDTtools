#ifndef CONFIGURATIONPARAMETER_H
#define CONFIGURATIONPARAMETER_H

#include <QString>

class ConfigurationParameter
{
public:
    ConfigurationParameter(void);

    void SetDescriptiveName(QString descriptiveName);
    const QString GetDescriptiveName(void) const;

    void SetParameterName(QString parameterName);
    const QString GetParameterName(void) const;

    void SetDescription(QString description);
    const QString GetDescription(void) const;

    void SetDataType(QString dataType);
    const QString GetDataType(void) const;

    void SetArrayLength(quint8 parameterArrayLength);
    quint8 GetArrayLength(void) const;

private:
    QString m_descriptiveName;
    QString m_parameterName;
    QString m_description;
    QString m_dataType;
    quint8 m_arrayLength;
};

#endif // CONFIGURATION_H
