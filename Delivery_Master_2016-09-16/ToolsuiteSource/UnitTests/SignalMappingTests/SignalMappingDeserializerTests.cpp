#include "SignalMappingDeserializerTests.h"

#include "SignalMapping.h"
#include "SignalMappingDeserializer.h"
#include "Project.h"

void SignalMappingDeserializerTests::Deserialize_EmptyXML_ReturnsFalseAndNoMappings()
{
    // Arrange
    const QString expectedError("XML string is empty!");
    SignalMappingDeserializer deserializer(QString(""));
    QList<SignalMapping*> target;

    // Act
    bool result = deserializer.Deserialize(target);

    // Assert
    QCOMPARE(result, false);
    QCOMPARE(target.count(), 0);
    QCOMPARE(deserializer.Error(), expectedError);
}

void SignalMappingDeserializerTests::Deserialize_ValidXMLWithoutMappings_ReturnsTrueAndNoMappings()
{
    // Arrange
    const QString expectedError("");
    QString mappingXML;
    mappingXML.append("<?xml version=\"1.0\"?>");
    mappingXML.append("<SignalMappings>");
    mappingXML.append("</SignalMappings>");
    SignalMappingDeserializer deserializer(mappingXML);
    QList<SignalMapping*> target;

    // Act
    bool result = deserializer.Deserialize(target);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(target.count(), 0);
    QCOMPARE(deserializer.Error(), expectedError);
}

void SignalMappingDeserializerTests::Deserialize_ValidXMLPointToPointMapping_ReturnsTrueAndOneMapping()
{
    // Arrange
    const QString expectedError("");
    QString mappingXML;
    mappingXML.append("<?xml version=\"1.0\"?>");
    mappingXML.append("<SignalMappings>");
    mappingXML.append("<SignalMapping Name=\"sourceA\">");
    mappingXML.append("<Source ComponentName=\"A\" Port=\"sourceA\" />");
    mappingXML.append("<Sink ComponentName=\"B\" Port=\"sinkB\" />");
    mappingXML.append("</SignalMapping>");
    mappingXML.append("</SignalMappings>");
    SignalMappingDeserializer deserializer(mappingXML);
    QList<SignalMapping*> target;

    // Act
    bool result = deserializer.Deserialize(target);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(target.count(), 1);
    QCOMPARE(target.at(0)->GetSource().Name, QString("A"));
    QCOMPARE(target.at(0)->GetSource().Signal, QString("sourceA"));
    QCOMPARE(target.at(0)->GetSinks().count(), 1);
    QCOMPARE(target.at(0)->GetSinks().at(0).Name, QString("B"));
    QCOMPARE(target.at(0)->GetSinks().at(0).Signal, QString("sinkB"));
    QCOMPARE(deserializer.Error(), expectedError);
}

void SignalMappingDeserializerTests::Deserialize_ValidXMLPointToMultiPointMapping_ReturnsTrueAndOneMapping()
{
    // Arrange
    const QString expectedError("");
    QString mappingXML;
    mappingXML.append("<?xml version=\"1.0\"?>");
    mappingXML.append("<SignalMappings>");
    mappingXML.append("<SignalMapping>");
    mappingXML.append("<Source ComponentName=\"A\" Port=\"sourceA\" />");
    mappingXML.append("<Sink ComponentName=\"B\" Port=\"sinkB\" />");
    mappingXML.append("<Sink ComponentName=\"C\" Port=\"sinkC\" />");
    mappingXML.append("</SignalMapping>");
    mappingXML.append("</SignalMappings>");

    SignalMappingDeserializer deserializer(mappingXML);
    QList<SignalMapping*> target;

    // Act
    bool result = deserializer.Deserialize(target);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(target.count(), 1);
    QCOMPARE(target.at(0)->GetSource().Name, QString("A"));
    QCOMPARE(target.at(0)->GetSource().Signal, QString("sourceA"));
    QCOMPARE(target.at(0)->GetSinks().count(), 2);
    QCOMPARE(deserializer.Error(), expectedError);
}
