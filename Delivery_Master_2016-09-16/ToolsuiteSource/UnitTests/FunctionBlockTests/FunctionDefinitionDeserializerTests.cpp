#include "FunctionDefinitionDeserializerTests.h"
#include "FunctionDefinitionDeserializer.h"

void FunctionDefinitionDeserializerTests::Deserialize_ValidXML_ReturnsTrueAndSetsFunctionParameters()
{
    // Arrange
    const QString functionDefinitionName = "VehSpdGauge";
    const QString taskInitFunction = "Gauge1_Init";
    const QString taskRunnableFunction = "Gauge1_Run";
    const quint32 taskStartupDelay = 10;
    const quint32 taskPeriodicity = 50;
    const QString inputSignal1Name = "VehSpd";
    const QString inputSignal1DataType = "uint32";
    const QString inputSignal1StatusFlag = "False";
    const QString inputSignal2Name = "SystemPowerMode";
    const QString inputSignal2DataType = "uint8";
    const QString inputSignal2StatusFlag = "True";
    const QString outputSignal1Name = "VehSpdGaugeValue";
    const QString outputSignal1DataType = "uint16";
    const QString outputSignal2Name = "VehSpdGaugeRestPos";
    const QString outputSignal2DataType = "uint8";
    FunctionDefinition functionDefinition;
    QStringList xml;
    xml.append("<?xml version=\"1.0\"?>");
    xml.append("<FunctionDefinition Name=\"" + functionDefinitionName + "\">");
    xml.append("    <Tasks>");
    xml.append("        <Task>");
    xml.append("            <InitFunction>" + taskInitFunction + "</InitFunction>");
    xml.append("            <Runnable>" + taskRunnableFunction + "</Runnable>");
    xml.append("            <StartupDelay>" + QString::number(taskStartupDelay) + "</StartupDelay>");
    xml.append("            <Periodicity>" + QString::number(taskPeriodicity) + "</Periodicity>");
    xml.append("        </Task>");
    xml.append("    </Tasks>");
    xml.append("    <SignalInterface>");
    xml.append("        <InputSignals>");
    xml.append("            <Signal Name=\"" + inputSignal1Name + "\" DataType=\"" + inputSignal1DataType + "\" StatusFlag=\"" + inputSignal1StatusFlag + "\"/>");
    xml.append("            <Signal Name=\"" + inputSignal2Name + "\" DataType=\"" + inputSignal2DataType + "\" StatusFlag=\"" + inputSignal2StatusFlag + "\"/>");
    xml.append("        </InputSignals>");
    xml.append("        <OutputSignals>");
    xml.append("            <Signal Name=\"" + outputSignal1Name + "\" DataType=\"" + outputSignal1DataType + "\"/>");
    xml.append("            <Signal Name=\"" + outputSignal2Name + "\" DataType=\"" + outputSignal2DataType + "\"/>");
    xml.append("        </OutputSignals>");
    xml.append("    </SignalInterface>");
    xml.append("</FunctionDefinition>");

    FunctionDefinitionDeserializer target(xml.join("\n"));

    // Act
    bool result = target.Deserialize(functionDefinition);

    // Assert
    QVERIFY2(result == true, "The deserialization of the function definition failed!");
    QCOMPARE(functionDefinition.GetName(), functionDefinitionName);

    QCOMPARE(functionDefinition.Tasks().count(), 1);
    QCOMPARE(functionDefinition.Tasks().at(0)->InitFunction, taskInitFunction);
    QCOMPARE(functionDefinition.Tasks().at(0)->Runnable, taskRunnableFunction);
    QCOMPARE(functionDefinition.Tasks().at(0)->StartupDelay, taskStartupDelay);
    QCOMPARE(functionDefinition.Tasks().at(0)->Periodicity, taskPeriodicity);

    QCOMPARE(functionDefinition.Sinks().count(), 2);
    QCOMPARE(functionDefinition.Sinks().at(0)->SignalName(), inputSignal1Name);
    QCOMPARE(functionDefinition.Sinks().at(0)->DataType(), inputSignal1DataType);
    QCOMPARE(functionDefinition.Sinks().at(1)->SignalName(), inputSignal2Name);
    QCOMPARE(functionDefinition.Sinks().at(1)->DataType(), inputSignal2DataType);

    QCOMPARE(functionDefinition.Sources().count(), 2);
    QCOMPARE(functionDefinition.Sources().at(0)->SignalName(), outputSignal1Name);
    QCOMPARE(functionDefinition.Sources().at(0)->DataType(), outputSignal1DataType);
    QCOMPARE(functionDefinition.Sources().at(1)->SignalName(), outputSignal2Name);
    QCOMPARE(functionDefinition.Sources().at(1)->DataType(), outputSignal2DataType);
}

void FunctionDefinitionDeserializerTests::Deserialize_EmptyXML_ReturnsFalse(void)
{
    // Arrange
    const QString expectedError("XML string is empty!");
    FunctionDefinition functionDefinition;
    FunctionDefinitionDeserializer target(QString(""));

    // Act
    bool result = target.Deserialize(functionDefinition);

    // Assert
    QVERIFY2(result == false, "Attempting to deserialize an empty function definition XML string should return false!");
    QCOMPARE(target.Error(), expectedError);
}

void FunctionDefinitionDeserializerTests::Deserialize_NoFunctionDefinitionElement_ReturnsFalseAndSetsErrorText()
{
    // Arrange
    const QString expectedError("No FunctionDefinition element found in XML!");
    FunctionDefinition functionDefinition;
    FunctionDefinitionDeserializer target(QString("<Stuff></Stuff>"));

    // Act
    bool result = target.Deserialize(functionDefinition);

    // Assert
    QVERIFY2(result == false, "Deserializing an XML string without a FunctionDefinition element should return false!");
    QCOMPARE(target.Error(), expectedError);
}
