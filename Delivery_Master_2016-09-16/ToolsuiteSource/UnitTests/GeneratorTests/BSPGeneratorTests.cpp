#include "BSPGeneratorTests.h"
#include "BSPGenerator.h"

void BSPGeneratorTests::GenerateFiles_NumberOfFiles()
{
    // Arrange
    Project project;
    project.SetBSPSpecificationPath("bsp.xml");
    project.Initialize(QFileInfo(":/Project/project.fdt"), QDir("."));
    BSPGenerator target(&project, false);

    // Act
    QList<GeneratedFile>& result = target.GenerateFiles();

    // Assert
    QCOMPARE(result.count(), 4);
}
