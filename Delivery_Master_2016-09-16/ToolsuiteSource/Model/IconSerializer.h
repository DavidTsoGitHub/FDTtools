#ifndef ICONSERIALIZER_H
#define ICONSERIALIZER_H

#include "SerializerBase.h"
#include "IconDefinition.h"

class IconSerializer : public SerializerBase
{
public:
    explicit IconSerializer(IconDefinition& iconDefinition);
    ~IconSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    IconDefinition& m_iconDefinition;
};

#endif // ICONSERIALIZER_H
