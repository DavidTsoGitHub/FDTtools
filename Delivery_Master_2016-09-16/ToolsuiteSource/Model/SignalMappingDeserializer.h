#ifndef SIGNALMAPPINGDESERIALIZER_H
#define SIGNALMAPPINGDESERIALIZER_H

#include <QString>

class SignalMapping;

class QXmlStreamReader;

class SignalMappingDeserializer
{
public:
    explicit SignalMappingDeserializer(const QString& mappingXml);

    bool Deserialize(QList<SignalMapping*>& signalMappings);
    const QString Error(void) const;

private:
    QString m_mappingXml;
    QString m_error;

    static bool getNextElement(QXmlStreamReader& sr, QString elementName);
    bool parseSignalMapping(QXmlStreamReader& xml, QList<SignalMapping*>& signalMappings);
};

#endif // SIGNALMAPPINGDESERIALIZER_H
