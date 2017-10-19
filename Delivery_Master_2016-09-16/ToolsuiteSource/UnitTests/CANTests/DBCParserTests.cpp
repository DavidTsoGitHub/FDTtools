#include "DBCParserTests.h"
#include "DBCParser.h"



void AppendCANMessageString(QString& dbcData, qint32 canID, QString name, qint8 dlc, QString producer)
{
    dbcData.append("BO_ ");
    dbcData.append(QString::number(canID));
    dbcData.append(" ");
    dbcData.append(name);
    dbcData.append(": ");
    dbcData.append(QString::number(dlc));
    dbcData.append(" ");
    dbcData.append(producer);
    dbcData.append("\n");
}

void AppendCANSignalString(QString& dbcData, QString name, qint8 startBit, qint8 bitLength, double scaling, double offset, double min, double max, QString uom, QString consumers)
{
    dbcData.append(" SG_ ");
    dbcData.append(name);
    dbcData.append(" : ");
    dbcData.append(QString::number(startBit));
    dbcData.append("|");
    dbcData.append(QString::number(bitLength));
    dbcData.append("@0+ (");
    dbcData.append(QString::number(scaling));
    dbcData.append(",");
    dbcData.append(QString::number(offset));
    dbcData.append(") [");
    dbcData.append(QString::number(min));
    dbcData.append("|");
    dbcData.append(QString::number(max));
    dbcData.append("] \"");
    dbcData.append(uom);
    dbcData.append("\"  ");
    dbcData.append(consumers);
    dbcData.append("\n");
}

void AppendMessageAttributeQInt32(QString& dbcData, qint32 canID, QString attributeName, qint32 attributeValue)
{
    dbcData.append("BA_ \"");
    dbcData.append(attributeName);
    dbcData.append("\" BO_ ");
    dbcData.append(QString::number(canID));
    dbcData.append(" ");
    dbcData.append(QString::number(attributeValue));
    dbcData.append(";\n");
}

void AppendMessageAttributeString(QString& dbcData, qint32 canID, QString attributeName, QString attributeValue)
{
    dbcData.append("BA_ \"");
    dbcData.append(attributeName);
    dbcData.append("\" BO_ ");
    dbcData.append(QString::number(canID));
    dbcData.append(" ");
    dbcData.append(attributeValue);
    dbcData.append(";\n");
}

void AppendDefaultAttributeQInt32(QString& dbcData, QString attributeName, qint32 attributeValue)
{
    dbcData.append("BA_DEF_DEF_ \"");
    dbcData.append(attributeName);
    dbcData.append("\" ");
    dbcData.append(QString::number(attributeValue));
    dbcData.append(";\n");
}

void AppendDefaultAttributeString(QString& dbcData, QString attributeName, QString attributeValue)
{
    dbcData.append("BA_DEF_DEF_ \"");
    dbcData.append(attributeName);
    dbcData.append("\" ");
    dbcData.append(attributeValue);
    dbcData.append(";\n");
}

void AppendEmptyLine(QString& dbcData)
{
    dbcData.append("\n");
}

void DBCParserTests::Parse_NoModuleDirectiveInData_EmptyModuleList()
{
    // Arrange
    QString dbcData("");
    DBCParser target(dbcData);

    // Act
    const QStringList* result = target.GetModuleNames();

    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 0, "The module count was not 0!");
}

void DBCParserTests::Parse_NoModulesInModuleDirective_EmptyModuleList()
{
    // Arrange
    QString dbcData("BU_: ");
    DBCParser target(dbcData);

    // Act
    const QStringList* result = target.GetModuleNames();

    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 0, "The module count was not 0!");
}

void DBCParserTests::Parse_ModulesInData_NonEmptyModuleList()
{
    // Arrange
    QString& module1 = *new QString("CIU");
    QString& module2 = *new QString("CIM");
    QString& module3 = *new QString("ECM");
    QString dbcData("BU_: ");
    dbcData.append(module1).append(" ").append(module2).append(" ").append(module3);
    DBCParser target(dbcData);

    // Act
    const QStringList* result = target.GetModuleNames();

    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 3, "The module count was not 3!");
    QVERIFY2(result->contains(module1), "The module list did not contain CIU!");
    QVERIFY2(result->contains(module2), "The module list did not contain CIM!");
    QVERIFY2(result->contains(module3), "The module list did not contain ECM!");
}

void DBCParserTests::Parse_NoAttributesInData_EmptyAttributeList()
{
    // Arrange
    QString dbcData("");
    DBCParser target(dbcData);

    // Act
    const QStringList* result = target.GetAttributeNames();


    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 0, "The module count was not 0!");
}

void DBCParserTests::Parse_AttributesInData_NonEmptyAttributeList()
{
    // Arrange
    QString& attribute1 = *new QString("GenMsgCycleTimeFast");
    QString& attribute2 = *new QString("GenMsgCycleTime");
    QString& attribute3 = *new QString("GenSigTimeoutMsg_YRS");
    QString dbcData("");
    dbcData.append("BA_DEF_ BO_  \"").append(attribute1).append("\" INT 0 100000;\n");
    dbcData.append("BA_DEF_ BO_  \"").append(attribute2).append("\" INT 0 100000;\n");
    dbcData.append("BA_DEF_ SG_  \"").append(attribute3).append("\" HEX 0 2047;");
    DBCParser target(dbcData);

    // Act
    const QStringList* result = target.GetAttributeNames();

    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 3, "The attribute name count was not 3!");
    QVERIFY2(result->contains(attribute1), "The attribute list did not contain GenMsgCycleTimeFast!");
    QVERIFY2(result->contains(attribute2), "The attribute list did not contain GenMsgCycleTime!");
    QVERIFY2(result->contains(attribute3), "The attribute list did not contain GenSigTimeoutMsg_YRS!");

}

void DBCParserTests::Parse_NoMessagesInData_EmptyMessageList()
{
    // Arrange
    QString dbcData("");
    DBCParser target(dbcData);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting string list was null!");
    QVERIFY2(result->count() == 0, "The attribute name count was not 3!");
}

void DBCParserTests::Parse_SingleMessageInData_SingleMessageInList()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signalName("FuelSysEmsRldMlfAtv");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM");
    QString dbcData;
    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signalName, startBit, bitLength, scaling, offset, min, max, uom, consumers);

    DBCParser target(dbcData);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    QCOMPARE(result->at(0)->CANID, canID);
    QCOMPARE(result->at(0)->Name, messageName);
    QCOMPARE(result->at(0)->DLC, dlc);
    QCOMPARE(result->at(0)->Producer, producer);
    QCOMPARE(result->at(0)->Signals().count(), 1);
    QCOMPARE(result->at(0)->Signals().at(0)->Name, signalName);
    QCOMPARE(result->at(0)->Signals().at(0)->StartBit, startBit);
    QCOMPARE(result->at(0)->Signals().at(0)->BitLength, bitLength);
    QCOMPARE(result->at(0)->Signals().at(0)->Scaling, scaling);
    QCOMPARE(result->at(0)->Signals().at(0)->Offset, offset);
    QCOMPARE(result->at(0)->Signals().at(0)->Min, min);
    QCOMPARE(result->at(0)->Signals().at(0)->Max, max);
    QCOMPARE(result->at(0)->Signals().at(0)->UOM, uom);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.count(), 1);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.contains(consumers), true);
}

void DBCParserTests::Parse_SingleMessageWithMultipleSignalsInData_SingleMessageWithMultipleSignalsInList()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signal1Name("FuelSysEmsRldMlfAtv");
    const QString signal2Name("FuelSysEmsRldMlfAtv2");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM");

    QString dbcData;

    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signal1Name, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendCANSignalString(dbcData, signal2Name, startBit, bitLength, scaling, offset, min, max, uom, consumers);

    DBCParser target(dbcData);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    CANMessage& message = *result->first();
    QCOMPARE(message.CANID, canID);
    QCOMPARE(message.Name, messageName);
    QCOMPARE(message.DLC, dlc);
    QCOMPARE(message.Producer, producer);
    QCOMPARE(message.Signals().count(), 2);
}

void DBCParserTests::Parse_MultipleMessagesWithMultipleSignalsInData_MultipleMessagesWithMultipleSignalsInList()
{
    // Arrange
    const qint32 canID(1241);
    const QString message1Name("mPPEFueSysGenSta");
    const QString message2Name("mPPEFueSysGenSta2");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signal1aName("FuelSysEmsRldMlfAtv");
    const QString signal1bName("FuelSysEmsRldMlfAtv2");
    const QString signal2aName("FuelSysEmsRldMlfAtvb");
    const QString signal2bName("FuelSysEmsRldMlfAtvb2");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM");
    QString dbcData;

    AppendCANMessageString(dbcData, canID, message1Name, dlc, producer);
    AppendCANSignalString(dbcData, signal1aName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendCANSignalString(dbcData, signal1bName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendEmptyLine(dbcData);
    AppendCANMessageString(dbcData, canID, message2Name, dlc, producer);
    AppendCANSignalString(dbcData, signal2aName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendCANSignalString(dbcData, signal2bName, startBit, bitLength, scaling, offset, min, max, uom, consumers);


    DBCParser target(dbcData);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 2);
    QCOMPARE(result->at(0)->Name, message1Name);
    QCOMPARE(result->at(0)->Signals().count(), 2);
    QCOMPARE(result->at(1)->Name, message2Name);
    QCOMPARE(result->at(1)->Signals().count(), 2);
}

void DBCParserTests::Parse_SingleMessageWithMultipleConsumersInData_SingleMessageWithMultipleConsumersInList()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signalName("FuelSysEmsRldMlfAtv");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM,BCM,TCM");
    QString dbcData;

    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signalName, startBit, bitLength, scaling, offset, min, max, uom, consumers);

    DBCParser target(dbcData);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    QCOMPARE(result->at(0)->Signals().count(), 1);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.count(), 3);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.contains("ECM"), true);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.contains("BCM"), true);
    QCOMPARE(result->at(0)->Signals().at(0)->Consumers.contains("TCM"), true);
}

void DBCParserTests::Parse_SingleMessageWithAssociatedAttributesInData_SingleMessageWithAttributesInList()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signalName("FuelSysEmsRldMlfAtv");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM,BCM,TCM");
    const QString cycleTimeAttribute("GenMsgCycleTime");
    const QString delayTimeAttribute("GenMsgDelayTime");
    const QString initialDelayTimeAttribute("GenMsgInitialDelayTime");
    const QString sendTypeAttribute("GenMsgSendType");
    const qint32 cycleTime(1000);
    const qint32 delayTime(100);
    const qint32 initialDelayTime(100);
    QString dbcData;

    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signalName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendEmptyLine(dbcData);
    AppendMessageAttributeQInt32(dbcData, canID, cycleTimeAttribute, cycleTime);
    AppendMessageAttributeQInt32(dbcData, canID, delayTimeAttribute, delayTime);
    AppendMessageAttributeQInt32(dbcData, canID, initialDelayTimeAttribute, initialDelayTime);
    AppendMessageAttributeString(dbcData, canID, sendTypeAttribute, "1");

    MessageAttributeMapping mapping;
    mapping.CycleTimeAttributeName = cycleTimeAttribute;
    mapping.DelayTimeAttributeName = delayTimeAttribute;
    mapping.InitialDelayTimeAttributeName = initialDelayTimeAttribute;
    mapping.SendTypeAttributeName = sendTypeAttribute;
    mapping.SendTypeMapping.insert("0", CANMessage::Cyclic);
    mapping.SendTypeMapping.insert("1", CANMessage::Event);
    mapping.SendTypeMapping.insert("4", CANMessage::CyclicAndEvent);


    DBCParser target(dbcData, &mapping);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    QCOMPARE(result->at(0)->CycleTime, cycleTime);
    QCOMPARE(result->at(0)->Delay, delayTime);
    QCOMPARE(result->at(0)->InitialDelay, initialDelayTime);
    QCOMPARE(result->at(0)->CANMessageSendType, CANMessage::Event);
}

void DBCParserTests::Parse_SingleMessageWithUnmappedSendType_SingleMessageWithUnsupportedSendType()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signalName("FuelSysEmsRldMlfAtv");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM,BCM,TCM");
    const QString sendTypeAttribute("GenMsgSendType");
    QString dbcData;

    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signalName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendEmptyLine(dbcData);
    AppendMessageAttributeString(dbcData, canID, sendTypeAttribute, "2");

    MessageAttributeMapping mapping;
    mapping.SendTypeAttributeName = sendTypeAttribute;
    mapping.SendTypeMapping.insert("0", CANMessage::Cyclic);
    mapping.SendTypeMapping.insert("1", CANMessage::Event);
    mapping.SendTypeMapping.insert("4", CANMessage::CyclicAndEvent);


    DBCParser target(dbcData, &mapping);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    QCOMPARE(result->at(0)->CANMessageSendType, CANMessage::Unsupported);
}

void DBCParserTests::Parse_SingleMessageWithoutAttributesInData_SingleMessageWithDefaultAttributes()
{
    // Arrange
    const qint32 canID(1241);
    const QString messageName("mPPEFueSysGenSta");
    const qint8 dlc(3);
    const QString producer("CIU");
    const QString signalName("FuelSysEmsRldMlfAtv");
    const qint8 startBit(7);
    const qint8 bitLength(1);
    const double scaling(0.3245);
    const double offset(-40);
    const double min(0);
    const double max(240);
    const QString uom("m/s");
    const QString consumers("ECM,BCM,TCM");
    const QString cycleTimeAttribute("GenMsgCycleTime");
    const qint32 defaultCycleTime(500);
    const QString delayTimeAttribute("GenMsgDelayTime");
    const QString initialDelayTimeAttribute("GenMsgInitialDelayTime");
    const qint32 delayTime(50);
    const qint32 initialDelayTime(50);
    QString dbcData;

    AppendCANMessageString(dbcData, canID, messageName, dlc, producer);
    AppendCANSignalString(dbcData, signalName, startBit, bitLength, scaling, offset, min, max, uom, consumers);
    AppendEmptyLine(dbcData);
    AppendDefaultAttributeQInt32(dbcData, cycleTimeAttribute, defaultCycleTime);
    AppendDefaultAttributeQInt32(dbcData, delayTimeAttribute, delayTime);
    AppendDefaultAttributeQInt32(dbcData, initialDelayTimeAttribute, initialDelayTime);


    MessageAttributeMapping* mapping = new MessageAttributeMapping();
    mapping->DelayTimeAttributeName = delayTimeAttribute;
    mapping->InitialDelayTimeAttributeName = initialDelayTimeAttribute;
    mapping->CycleTimeAttributeName = cycleTimeAttribute;

    DBCParser target(dbcData, mapping);

    // Act
    const QList<CANMessage*>* result = target.GetMessages();

    // Assert
    QVERIFY2(result != NULL, "Resulting message list was null!");
    QCOMPARE(result->count(), 1);
    QCOMPARE(result->at(0)->CycleTime, defaultCycleTime);
    QCOMPARE(result->at(0)->Delay, delayTime);
    QCOMPARE(result->at(0)->InitialDelay, initialDelayTime);
}
