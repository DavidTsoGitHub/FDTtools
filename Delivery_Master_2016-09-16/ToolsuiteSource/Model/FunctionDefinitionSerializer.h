#ifndef FUNCTIONDEFINITIONSERIALIZER_H
#define FUNCTIONDEFINITIONSERIALIZER_H

#include "SerializerBase.h"
#include "FunctionDefinition.h"
#include <QString>

class FunctionDefinitionSerializer : SerializerBase
{
public:
    static QString Serialize(const FunctionDefinition& functionDefinition);
};

#endif // FUNCTIONDEFINITIONSERIALIZER_H
