#ifndef LABELDEFINITIONSERIALIZER_H
#define LABELDEFINITIONSERIALIZER_H

#include "SerializerBase.h"

#include <QString>

class Font;
class LabelDefinition;

class LabelDefinitionSerializer : public SerializerBase
{
public:
    explicit LabelDefinitionSerializer(LabelDefinition& labelDefintion);
    ~LabelDefinitionSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    LabelDefinition& m_labelDefinition;

    bool isDefaultFont(const Font* defaultFont, const Font* currentFont);
};

#endif // LABELDEFINITIONSERIALIZER_H
