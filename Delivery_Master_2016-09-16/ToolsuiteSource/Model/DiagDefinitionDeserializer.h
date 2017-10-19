#ifndef DIAGDEFINITIONDESERIALIZER
#define DIAGDEFINITIONDESERIALIZER

#include "DiagDefinition.h"
#include "DeserializerBase.h"

#include <QString>

class DiagDefinitionDeserializer : public DeserializerBase
{
public:
    explicit DiagDefinitionDeserializer(DiagDefinition& diagDefinition);
    ~DiagDefinitionDeserializer(void);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
   DiagDefinition& m_diagDefinition;
};


#endif // DIAGDEFINITIONDESERIALIZER

