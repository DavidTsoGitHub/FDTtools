#include "BSPSpecificationDeserializerTests.h"
#include "BSPSpecificationDeserializer.h"

#include "BSPDisplay.h"
#include "BSPEnum.h"
#include "BSPFunction.h"
#include "BSPSpecification.h"
#include "GCLSignalEndpoint.h"

void AddRootStartElement(QString& xml)
{
    xml.append("<BSP>");
}

void AddRootEndElement(QString& xml)
{
    xml.append("</BSP>");
}

void AddEnumerationssStartElement(QString& xml)
{
    xml.append("<BSPEnumerations>");
}

void AddEnumeration(QString& xml, QString enumName, QString enumDescription, quint32 valueCount)
{
    xml.append("<BSPEnum Name=\"").append(enumName).append("\" Description=\"").append(enumDescription).append("\">");
    xml.append("<Items>");
    for (quint32 i=0; i<valueCount; ++i)
    {
        QString numberString = QString::number(i+1);
        xml.append("<Item Value=\"").append(numberString).append("\" Identifier=\"").append(numberString).append("\" Description=\"").append("Comment for " + numberString).append("\">");
        xml.append("</Item>");
    }

    xml.append("</Items>");
    xml.append("</BSPEnum>");
}

void AddEnumerationsEndElement(QString& xml)
{
    xml.append("</BSPEnumerations>");
}

void AddFunctionsStartElement(QString& xml)
{
    xml.append("<BSPFunctions>");
}

void AddFunctionsEndElement(QString& xml)
{
    xml.append("</BSPFunctions>");
}


void BSPSpecificationDeserializerTests::Deserialize_NoData_ReturnsTrueAndEmptySpecification(void)
{
    // Arrange
    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(QString("<BSP></BSP>"));

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Enums().count(), 0);
    QCOMPARE(spec.Functions().count(), 0);
    QCOMPARE(spec.HasDisplay(), false);
    QCOMPARE(spec.HasSegmentDisplay(), false);

    delete target;
}

void BSPSpecificationDeserializerTests::Deserialize_Display_ReturnsTrueAndDisplay(void)
{
    // Arrange
    const quint16 width = 240;
    const quint16 height = 320;
    const quint32 availableMemory = 160000;
    bool useGram = true;
    const quint16 numberOfLayers = 16;
    BSPDisplay::BSPRenderingType renderingType = BSPDisplay::HW;
    BSPDisplay::BSPDisplayOrientation renderingOrientation = BSPDisplay::CounterClockWise;

    QString xml;
    AddRootStartElement(xml);
    xml.append("<BSPTypes>" \
               "<BSPEnumerations><BSPEnum Description=\"BPP\" Name=\"BSP_DCU_BPPModes\"><Items>" \
               "<Item Description=\"\" Identifier=\"DCU_1_BPP\" Value=\"0\"/>" \
               "</Items></BSPEnum></BSPEnumerations></BSPTypes>"
               );
    xml.append("<BSPDisplay Width=\"" + QString::number(width)
               + "\" Height=\"" + QString::number(height)
               + "\" AvailableMemory=\"" + QString::number(availableMemory)
               + "\" UseGRAM=\"" + ((useGram == true) ? "true" : "false")
               + "\" NumberOfLayers=\"" + QString::number(numberOfLayers)
               + "\" RenderingType=\"" + ((renderingType == BSPDisplay::HW) ? "HW" : "SW")
               + "\" RenderingOrientation=\"" + ((renderingOrientation == BSPDisplay::CounterClockWise) ? "CCW" : "Normal")
               + "\"></BSPDisplay>");
    AddRootEndElement(xml);

    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(xml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.HasDisplay(), true);
    QCOMPARE(spec.GetDisplay()->GetWidth(), width);
    QCOMPARE(spec.GetDisplay()->GetHeight(), height);
    QCOMPARE(spec.GetDisplay()->GetAvailableMemory(), availableMemory);
    QCOMPARE(spec.GetDisplay()->GetUseGRAM(), useGram);
    QCOMPARE(spec.GetDisplay()->GetNumberOfLayers(), numberOfLayers);
    QCOMPARE(spec.GetDisplay()->GetRenderingType(), BSPDisplay::HW);
    QCOMPARE(spec.GetDisplay()->GetRenderingOrientation(), BSPDisplay::CounterClockWise);

    delete target;
}

void BSPSpecificationDeserializerTests::Deserialize_SingleEnum_ReturnsTrueAndOneEnum(void)
{   
    // Arrange
    const QString enumName("EnumName");
    const QString enumDescription("Description for Enum");
    QString xml;
    AddRootStartElement(xml);
    xml.append("<BSPTypes>");
    AddEnumerationssStartElement(xml);
    AddEnumeration(xml, enumName, enumDescription, 3);
    AddEnumerationsEndElement(xml);
    xml.append("</BSPTypes>");
    AddRootEndElement(xml);

    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(xml);

    // Assert
    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Enums().count(), 1);
    QCOMPARE(spec.Enums().at(0)->Name, enumName);
    QCOMPARE(spec.Enums().at(0)->Description, enumDescription);
    QCOMPARE(spec.Enums().at(0)->EnumItems.count(), 3);
    QCOMPARE(spec.Functions().count(), 0);

    delete target;
}

void BSPSpecificationDeserializerTests::Deserialize_SingleFunction_ReturnsTrueAndOneFunction(void)
{
    // Arrange
    const QString functionName("Function_Name");
    const QString functionReturnType("uint32*");
    const QString argumentName("ArgumentName");
    const QString argumentType("uint8*");
    const quint32 argumentOrder = 1;
    const QString gclSignalName("GCLSIGNAL");

    QString xml;
    AddRootStartElement(xml);
    AddFunctionsStartElement(xml);
    xml.append("<BSPFunction FunctionName=\"").append(functionName).append("\" GCLSignalName=\"").append(gclSignalName).append("\" GCLSignalType=\"Source").append("\" Return=\"").append(functionReturnType).append("\">");
    xml.append("<Arguments>");
    xml.append("<Argument Order=\"").append(QString::number(argumentOrder)).append("\" ArgumentName=\"").append(argumentName).append("\" ArgumentType=\"").append(argumentType).append("\">");
    xml.append("</Argument>");
    xml.append("</Arguments>");
    xml.append("</BSPFunction>");
    AddFunctionsEndElement(xml);
    AddRootEndElement(xml);

    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(xml);

    // Assert

    QCOMPARE(result, DeserializerBase::DeserializationOK);
    QCOMPARE(spec.Functions().count(), 1);
    const BSPFunction* function = spec.Functions().at(0);
    QCOMPARE(function->Name, functionName);
    QCOMPARE(spec.GetSignalsForFunction(function).first()->SignalName(), gclSignalName);
    QCOMPARE(function->GCLSignalType, BSPFunction::Source);
    QCOMPARE(function->ReturnType, functionReturnType);
    QCOMPARE(function->Arguments.count(), 1);
    QCOMPARE(function->Arguments.first().Name, argumentName);
    QCOMPARE(function->Arguments.first().Order, argumentOrder);
    QCOMPARE(function->Arguments.first().Type, argumentType);
    QCOMPARE(spec.Enums().count(), 0);
    delete target;
}

void BSPSpecificationDeserializerTests::Deserialize_EmptyXML_ReturnsFalse(void)
{
    // Arrange
    const QString expectedErrorMessage("XML string is empty!");
    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(QString(""));

    // Assert
    QCOMPARE(result, DeserializerBase::ErrorFileEmpty);
    QCOMPARE(target->Error(), expectedErrorMessage);

    delete target;
}

void BSPSpecificationDeserializerTests::Deserialize_UnknownRootElement_ReturnsFalseAndErrorMessage(void)
{
    // Arrange
    const QString expectedErrorMessage("No BSP root element found in XML!");

    BSPSpecification spec;
    BSPSpecificationDeserializer* target = new BSPSpecificationDeserializer(spec);

    // Act
    DeserializerBase::DeserializerRespons result = target->Deserialize(QString("<trams></trams>"));

    // Assert
    QCOMPARE(result, DeserializerBase::ErrorTagNotFound);
    QCOMPARE(target->Error(), expectedErrorMessage);

    delete target;
}
