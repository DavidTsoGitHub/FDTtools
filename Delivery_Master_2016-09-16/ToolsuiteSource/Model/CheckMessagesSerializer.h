#ifndef CHECKMESSAGESSERIALIZER_H
#define CHECKMESSAGESSERIALIZER_H

#include "SerializerBase.h"
#include "CheckMessage.h"
#include "CheckMessagesDefinition.h"

class CheckMessagesSerializer : public SerializerBase
{
public:
    explicit CheckMessagesSerializer(CheckMessageDefinition& checkMessagesDefinition);
    ~CheckMessagesSerializer(void);

    // SerializerBase interface
public:
    QString Serialize();

private:
    CheckMessageDefinition& m_checkMessagesDefinition;
    QString getConditionString(CheckMessage::ClearCondition_e cond);
};

#endif // CHECKMESSAGESSERIALIZER_H
