#ifndef LABELDEFINITIONDESERIALIZER_H
#define LABELDEFINITIONDESERIALIZER_H

#include "LabelDefinition.h"
#include "DeserializerBase.h"

#include <QString>
#include <QXmlStreamReader>

class LabelDefinitionDeserializer : public DeserializerBase
{
public:
    explicit LabelDefinitionDeserializer(LabelDefinition& labelDefinition);
    ~LabelDefinitionDeserializer();

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    LabelDefinition& m_labelDefinition;

    static bool isEndOfElement(QXmlStreamReader& xmlReader, QString elementName);
};

#endif // LABELDEFINITIONDESERIALIZER_H
