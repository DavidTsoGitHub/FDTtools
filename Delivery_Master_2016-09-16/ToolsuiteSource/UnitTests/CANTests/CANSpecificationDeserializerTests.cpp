#include "CANSpecificationDeserializerTests.h"

#include "CANMessage.h"
#include "CANSpecificationDeserializer.h"
#include <iostream>

void StartCANDocument(QXmlStreamWriter& xml)
{
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("CAN");
}

void BeginMessage(QXmlStreamWriter& xml, qint32 canId)
{
    xml.writeStartElement("Message");
    xml.writeAttribute("ID", QString::number(canId));
}

void AddMessageParameters(QXmlStreamWriter& xml, const CANMessage& message)
{
    xml.writeTextElement("Name", message.Name);
    xml.writeTextElement("Producer", message.Producer);
    xml.writeTextElement("DLC", QString::number(message.DLC));
    xml.writeStartElement("Timing");
    xml.writeAttribute("Type", CANMessage::SendTypeString(message.CANMessageSendType));
    xml.writeTextElement("Cycle", QString::number(message.CycleTime));
    xml.writeTextElement("Delay", QString::number(message.Delay));
    xml.writeTextElement("InitialDelay", QString::number(message.InitialDelay));
    xml.writeEndElement();
}

void AddBasicMessageParameters(QXmlStreamWriter& xml)
{
    CANMessage basicMessage(123, "m_VehInfo_Disp", 0, "IC");
    basicMessage.CycleTime = 50;
    basicMessage.InitialDelay = 10;
    basicMessage.Delay = 50;
    basicMessage.CANMessageSendType = CANMessage::Cyclic;
    AddMessageParameters(xml, basicMessage);
}

void BeginSignal(QXmlStreamWriter& xml, QString signalName)
{
    xml.writeStartElement("Signal");
    xml.writeAttribute("Name", signalName);
}

void AddSignalParameters(QXmlStreamWriter& xml, const CANSignal& signal)
{
    xml.writeStartElement("Position");
    xml.writeTextElement("StartBit", QString::number(signal.StartBit));
    xml.writeTextElement("Length", QString::number(signal.BitLength));
    xml.writeEndElement();
    xml.writeStartElement("Value");
    xml.writeTextElement("Offset", QString::number(signal.Offset));
    xml.writeTextElement("Min", QString::number(signal.Min));
    xml.writeTextElement("Max", QString::number(signal.Max));
    xml.writeTextElement("Resolution", QString::number(signal.Scaling));
    xml.writeTextElement("UOM", signal.UOM);
    xml.writeEndElement();
    xml.writeStartElement("Consumers");
    foreach (QString consumer, signal.Consumers)
    {
        xml.writeTextElement("Consumer", consumer);
    }
    xml.writeEndElement();


}


void CANSpecificationDeserializerTests::Deserialize_NoData_ReturnsTrueAndEmptySpecification()
{
    // Arrange
    CANSpecification spec;
    QString canXml;
    QXmlStreamWriter xml(&canXml);

    StartCANDocument(xml);
    xml.writeEndDocument();

    // Act
    CANSpecificationDeserializer target(spec);
    DeserializerBase::DeserializerRespons result = target.Deserialize(canXml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Messages().count(), 0);
    QCOMPARE(spec.Modules.count(), 0);
}

void CANSpecificationDeserializerTests::Deserialize_SingleMessageNoSignals_ReturnsTrueAndMessageData()
{
    // Arrange
    CANMessage expectedMessage(786, "m_VehInfo_Disp", 3, "IC");
    expectedMessage.CycleTime = 50;
    expectedMessage.InitialDelay = 10;
    expectedMessage.Delay = 50;
    expectedMessage.CANMessageSendType = CANMessage::Cyclic;
    QString canXml;
    QXmlStreamWriter xml(&canXml);

    StartCANDocument(xml);                          // CAN
    xml.writeStartElement("Messages");              // CAN.Messages
    BeginMessage(xml, expectedMessage.CANID);       // CAN.Messages.Message
    AddMessageParameters(xml, expectedMessage);     // CAN.Messages.Message....
    xml.writeEndDocument();

    CANSpecification spec;

    // Act
    CANSpecificationDeserializer target(spec);
    DeserializerBase::DeserializerRespons result = target.Deserialize(canXml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Modules.count(), 0);
    QCOMPARE(spec.Messages().count(), 1);
    QCOMPARE(spec.Messages().at(0)->CANID, expectedMessage.CANID);
    QCOMPARE(spec.Messages().at(0)->Name, expectedMessage.Name);
    QCOMPARE(spec.Messages().at(0)->Producer, expectedMessage.Producer);
    QCOMPARE(spec.Messages().at(0)->DLC, expectedMessage.DLC);
    QCOMPARE(spec.Messages().at(0)->CANMessageSendType, expectedMessage.CANMessageSendType);
    QCOMPARE(spec.Messages().at(0)->CycleTime, expectedMessage.CycleTime);
    QCOMPARE(spec.Messages().at(0)->InitialDelay, expectedMessage.InitialDelay);
    QCOMPARE(spec.Messages().at(0)->Delay, expectedMessage.Delay);
}

void CANSpecificationDeserializerTests::Deserialize_SingleMessageOneSignal_ReturnsTrueAndSignalData()
{
    // Arrange
    const qint32 messageCANID(786);
    QStringList consumers;
    consumers << "RestBus" << "Bus2";
    CANSignal expectedSignal("VehSpd_Disp", false, false, 1, 16, 1.0, 10, -10, 500, "km/h", consumers);
    QString canXml;
    QXmlStreamWriter xml(&canXml);
    StartCANDocument(xml);                          // CAN
    xml.writeStartElement("Messages");              // CAN.Messages
    BeginMessage(xml, messageCANID);                // CAN.Messages.Message
    AddBasicMessageParameters(xml);                 // CAN.Messages.Message....
    xml.writeStartElement("Signals");               // CAN.Messages.Message.Signals
    BeginSignal(xml, expectedSignal.Name);          // CAN.Messages.Message.Signals.Signal
    AddSignalParameters(xml, expectedSignal);       // CAN.Messages.Message.Signals.Signal....
    xml.writeEndDocument();

    CANSpecification spec;

    // Act
    CANSpecificationDeserializer target(spec);
    DeserializerBase::DeserializerRespons result = target.Deserialize(canXml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Modules.count(), 0);
    QCOMPARE(spec.Messages().count(), 1);
    QCOMPARE(spec.Messages().at(0)->Signals().count(), 1);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Name, expectedSignal.Name);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->BitLength, expectedSignal.BitLength);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->StartBit, expectedSignal.StartBit);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Max, expectedSignal.Max);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Min, expectedSignal.Min);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Offset, expectedSignal.Offset);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Scaling, expectedSignal.Scaling);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->UOM, expectedSignal.UOM);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Consumers.count(), 2);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Consumers.contains("RestBus"), true);
    QCOMPARE(spec.Messages().at(0)->Signals().at(0)->Consumers.contains("Bus2"), true);

}

void CANSpecificationDeserializerTests::Deserialize_MultipleMessages_ReturnsTrueAndAllMessages()
{
    // Arrange
    CANSpecification spec;
    CANMessage expectedMessage(786, "m_VehInfo_Disp", 3, "IC");
    expectedMessage.CycleTime = 50;
    expectedMessage.InitialDelay = 10;
    expectedMessage.Delay = 50;
    expectedMessage.CANMessageSendType = CANMessage::Cyclic;
    QString canXml;
    QXmlStreamWriter xml(&canXml);

    StartCANDocument(xml);                          // CAN
    xml.writeStartElement("Messages");              // CAN.Messages
    BeginMessage(xml, expectedMessage.CANID);       // CAN.Messages.Message
    AddMessageParameters(xml, expectedMessage);     // CAN.Messages.Message....
    xml.writeEndElement();                          // CAN.Messages
    BeginMessage(xml, 1234);
    AddBasicMessageParameters(xml);
    xml.writeEndDocument();

    // Act
    CANSpecificationDeserializer target(spec);
    DeserializerBase::DeserializerRespons result = target.Deserialize(canXml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Modules.count(), 0);
    QCOMPARE(spec.Messages().count(), 2);
}

void CANSpecificationDeserializerTests::Deserialize_EmptyXML_ReturnsFalseAndSetsError()
{
    // Arrange
    const QString expectedErrorMessage("XML string is empty!");
    CANSpecification spec;
    CANSpecificationDeserializer target(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target.Deserialize(QString(""));

    // Assert
    QCOMPARE(result, DeserializerBase::ErrorFileEmpty);
    QCOMPARE(target.Error(), expectedErrorMessage);
}
