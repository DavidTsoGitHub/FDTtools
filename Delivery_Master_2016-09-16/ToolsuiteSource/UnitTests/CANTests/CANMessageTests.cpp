#include "CANMessageTests.h"
#include "CANMessage.h"



void CANMessageTests::SendTypeFromString_ValidStrings_ValidEnumValues()
{
    QCOMPARE(CANMessage::SendTypeFromString("Cyclic"), CANMessage::Cyclic);
    QCOMPARE(CANMessage::SendTypeFromString("Event"), CANMessage::Event);
    QCOMPARE(CANMessage::SendTypeFromString("CyclicAndEvent"), CANMessage::CyclicAndEvent);
    QCOMPARE(CANMessage::SendTypeFromString("Unsupported"), CANMessage::Unsupported);
    QCOMPARE(CANMessage::SendTypeFromString("Undefined"), CANMessage::Undefined);
    QCOMPARE(CANMessage::SendTypeFromString("JibberishString"), CANMessage::Undefined);
}

void CANMessageTests::SendTypeString_ValidEnumValues_ValidStrings()
{
    QCOMPARE(CANMessage::SendTypeString(CANMessage::Cyclic), QString("Cyclic"));
    QCOMPARE(CANMessage::SendTypeString(CANMessage::Event), QString("Event"));
    QCOMPARE(CANMessage::SendTypeString(CANMessage::CyclicAndEvent), QString("CyclicAndEvent"));
    QCOMPARE(CANMessage::SendTypeString(CANMessage::Unsupported), QString("Unsupported"));
    QCOMPARE(CANMessage::SendTypeString(CANMessage::Undefined), QString("Undefined"));
    QCOMPARE(CANMessage::SendTypeString((CANMessage::SendType)254), QString("Unknown"));
}


