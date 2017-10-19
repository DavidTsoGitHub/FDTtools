#ifndef CHECKMESSAGESDESERIALIZER_H
#define CHECKMESSAGESDESERIALIZER_H

#include "CheckMessagesDefinition.h"
#include "LayoutDefinition.h"
#include "DeserializerBase.h"

#include <QString>
#include <QXmlStreamReader>

class CheckMessagesDeserializer : public DeserializerBase
{
public:
    explicit CheckMessagesDeserializer(LayoutDefinition &layoutDefinition, CheckMessageDefinition& checkMessagesDefinition);
    ~CheckMessagesDeserializer();

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    LayoutDefinition& m_layoutDefinition;
    CheckMessageDefinition& m_checkMessagesDefinition;
    //bool isEndOfElement(QXmlStreamReader& xmlReader, QString elementName);
};
#endif // CHECKMESSAGESDESERIALIZER_H
