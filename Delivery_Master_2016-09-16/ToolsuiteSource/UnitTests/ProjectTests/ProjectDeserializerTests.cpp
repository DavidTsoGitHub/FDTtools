#include "ProjectDeserializerTests.h"

#include "FunctionBlock.h"
#include "Project.h"
#include "ProjectDeserializer.h"

void ProjectDeserializerTests::Deserialize_ValidXML_ReturnsTrueAndSetsProjectParameters()
{
    // Arrange
    const QString projectName("BasicCluster");
    const QString bspPath("BSP/bsp.xml");
    const int functionBlock1Id(1);
    const QString functionBlock1Path("Gauge1/Gauge1.fdf");
    const int functionBlock2Id(2);
    const QString functionBlock2Path("Gauge2/Gauge2.fdf");


    QStringList xml;
    xml.append("<Project Name=\"" + projectName + "\">");
    xml.append("  <BSP SpecificationPath=\"" + bspPath + "\" />");
    xml.append("  <FunctionBlocks>");
    xml.append("    <FunctionBlock ID=\"" + QString::number(functionBlock1Id) + "\" PathInLib=\"" + functionBlock1Path + "\" />");
    xml.append("    <FunctionBlock ID=\"" + QString::number(functionBlock2Id) + "\" PathInLib=\"" + functionBlock2Path + "\" />");
    xml.append("  </FunctionBlocks>");
    xml.append("</Project>");

    // Act
    Project* project;
    bool result;
    QString errorString;
    project = ProjectDeserializer::Deserialize(xml.join("\n"), &result, &errorString);

    // Assert
    QCOMPARE(result, true);
    QCOMPARE(project->GetProjectName(), projectName);
    QCOMPARE(project->GetBSPSpecificationPath(), bspPath);
    QCOMPARE(project->GetFunctionBlocks().List().count(), 2);
    QCOMPARE(project->GetFunctionBlocks().List().at(0)->GetID(), functionBlock1Id);
    QCOMPARE(project->GetFunctionBlocks().List().at(0)->GetPathInFunctionLibrary(), functionBlock1Path);
    QCOMPARE(project->GetFunctionBlocks().List().at(1)->GetID(), functionBlock2Id);
    QCOMPARE(project->GetFunctionBlocks().List().at(1)->GetPathInFunctionLibrary(), functionBlock2Path);

    QVERIFY2(errorString.isEmpty(), "Error text should be empty when no errors occured!");
}

void ProjectDeserializerTests::Deserialize_EmptyXML_ReturnsFalseAndSetsErrorText()
{
    // Arrange
    const QString expectedError("XML string is empty!");
    const QString emptyProjectXML("");

    // Act
    bool result;
    QString errorString;
    Project* project = ProjectDeserializer::Deserialize(emptyProjectXML, &result, &errorString);

    // Assert
    QCOMPARE(result, false);
    QCOMPARE(errorString, expectedError);
    QCOMPARE(project, (Project*)NULL);
}

void ProjectDeserializerTests::Deserialize_NoProjectElement_ReturnsFalseAndSetsErrorText()
{
    // Arrange
    const QString expectedError("No Project element found in XML!");
    const QString unknownXML("<Stuff><OtherStuff>stuffstuff</OtherStuff></Stuff>");

    // Act
    bool result;
    QString errorString;
    Project* project = ProjectDeserializer::Deserialize(unknownXML, &result, &errorString);

    // Assert
    QCOMPARE(result, false);
    QCOMPARE(errorString, expectedError);
    QCOMPARE(project, (Project*)NULL);
}

