#ifndef DIAGDEFINITIONSERIALIZER
#define DIAGDEFINITIONSERIALIZER

#include <SerializerBase.h>
#include "DiagDefinition.h"
#include <QString>

class DiagDefinitionSerializer : public SerializerBase
{
public:
    explicit DiagDefinitionSerializer(DiagDefinition& diagDefinition);
    ~DiagDefinitionSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    DiagDefinition& m_diagDefinition;
};

#endif // DIAGDEFINITIONSERIALIZER
