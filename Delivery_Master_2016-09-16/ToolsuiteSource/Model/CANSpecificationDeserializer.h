#ifndef CANSPECIFICATIONDESERIALIZER_H
#define CANSPECIFICATIONDESERIALIZER_H

#include "CANSpecification.h"
#include "DeserializerBase.h"

#include <QXmlStreamReader>
#include <QFileInfo>

class CANSpecificationDeserializer : public DeserializerBase
{
public:
    explicit CANSpecificationDeserializer(CANSpecification& canSpecification);
    ~CANSpecificationDeserializer();

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    CANSpecification& m_canSpecification;

    static CANMessage* parseMessage(QXmlStreamReader& xml);
    static CANSignal* parseSignal(QXmlStreamReader& xml);
};

#endif // CANSPECIFICATIONDESERIALIZER_H
