#include "LayoutDefinitionDeserializerTests.h"
#include "LayoutDefinitionDeserializer.h"
#include "IconDefinition.h"
#include "Project.h"
#include "LayoutDefinition.h"
#include <QXmlStreamWriter>

void BeginDisplayXml(QXmlStreamWriter&);
void BeginDisplayRootXml(QXmlStreamWriter&);
void BeginDisplayAreas(QXmlStreamWriter&);
void AddDisplayArea(QXmlStreamWriter&, quint8 ID, QString name, quint16 height, quint16 width, qint32 x, qint32 y, quint8 border);
void EndDisplayAreas(QXmlStreamWriter&);
void EndDisplayRootXml(QXmlStreamWriter&);

void LayoutDefinitionDeserializerTests::Deserialize_EmptyXML_ReturnsFalseAndSetsErrorMessage()
{
    // Arrange
    const QString expectedError("XML string is empty!");
    const QString displayXml;
    LayoutDefinition display;
    LayoutDefinitionDeserializer target(displayXml);
    Project project;

    // Act
    bool result = target.Deserialize(display, project);

    // Assert
    QCOMPARE(result, false);
    QCOMPARE(target.Error(), expectedError);
}

void LayoutDefinitionDeserializerTests::Deserialize_SingleDisplayAreaXml_ReturnsTrueAndSetsDisplayParameters()
{
    // Arrange
    const QString expectedError("");
    QString displayXml;
    QXmlStreamWriter xmlWriter(&displayXml);

    BeginDisplayXml(xmlWriter);
    BeginDisplayRootXml(xmlWriter);
    EndDisplayRootXml(xmlWriter);

    LayoutDefinition display;
    LayoutDefinitionDeserializer target(displayXml);
    Project project;

    // Act
    bool result = target.Deserialize(display, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(target.Error(), expectedError);
    QCOMPARE(display.DisplayAreas().count(), 0);
}

void LayoutDefinitionDeserializerTests::Deserialize_DisplayAreaWithChild_ReturnsTrueAndSetsChildParameters()
{
    // Arrange
    const quint16 expectedWidth = 240;
    const quint16 expectedHeight = 320;
    const quint8 expectedID = 0;
    const qint32 expectedX = 100;
    const qint32 expectedY = 200;
    const QString expectedError("");
    const QString expectedName("menuarea");
    QString displayXml;
    QXmlStreamWriter xmlWriter(&displayXml);
    quint8 border = 1;

    BeginDisplayXml(xmlWriter);
    BeginDisplayRootXml(xmlWriter);
    BeginDisplayAreas(xmlWriter);
    AddDisplayArea(xmlWriter, expectedID, expectedName, expectedWidth, expectedHeight, expectedX, expectedY, border);
    EndDisplayAreas(xmlWriter);
    EndDisplayRootXml(xmlWriter);

    LayoutDefinition display;
    LayoutDefinitionDeserializer target(displayXml);
    Project project;

    // Act
    bool result = target.Deserialize(display, project);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(target.Error(), expectedError);
    QCOMPARE(display.DisplayAreas().count(), 1);
    QCOMPARE(display.DisplayAreas().at(0)->X, expectedX);
    QCOMPARE(display.DisplayAreas().at(0)->Y, expectedY);
    QCOMPARE(display.DisplayAreas().at(0)->Name, expectedName);
    QCOMPARE(display.DisplayAreas().at(0)->Width, expectedWidth);
    QCOMPARE(display.DisplayAreas().at(0)->Height, expectedHeight);
    QCOMPARE(display.DisplayAreas().at(0)->Border, border);
}


void BeginDisplayXml(QXmlStreamWriter& xml)
{
    xml.writeStartDocument();
}

void BeginDisplayRootXml(QXmlStreamWriter& xml)
{
    xml.writeStartElement("Layouts");
}

void BeginDisplayAreas(QXmlStreamWriter& xml)
{
    xml.writeStartElement("DisplayAreas");
}

void AddDisplayArea(QXmlStreamWriter& xml, quint8 ID, const QString name, quint16 width, quint16 height, qint32 x, qint32 y, quint8 border)
{
    xml.writeStartElement("DisplayArea");
    xml.writeAttribute("ID", QString::number(ID));
    xml.writeAttribute("Name", name);
    xml.writeAttribute("Width", QString::number(width));
    xml.writeAttribute("Height", QString::number(height));
    xml.writeAttribute("X", QString::number(x));
    xml.writeAttribute("Y", QString::number(y));
    xml.writeAttribute("Border", QString::number(border));
    xml.writeEndElement();
}

void EndDisplayAreas(QXmlStreamWriter& xml)
{
    xml.writeEndElement();
}

void EndDisplayRootXml(QXmlStreamWriter& xml)
{
    xml.writeEndElement();
}
