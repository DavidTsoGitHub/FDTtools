#ifndef LAYOUTDEFINITIONSERIALIZER_H
#define LAYOUTDEFINITIONSERIALIZER_H

#include "SerializerBase.h"

class LayoutDefinition;
class QString;

class LayoutDefinitionSerializer : public SerializerBase
{
public:
    explicit LayoutDefinitionSerializer(LayoutDefinition& layoutDefinition);
    ~LayoutDefinitionSerializer();

    QString Serialize();

private:
    LayoutDefinition& m_layoutDefinition;
};

#endif // LAYOUTDEFINITIONSERIALIZER_H
