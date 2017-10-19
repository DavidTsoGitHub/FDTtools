#include "CompilerFactoryTests.h"
#include "CompilerFactory.h"
#include "TestCompiler.h"
#include "GCCCompiler.h"

void CompilerFactoryTests::GetCompiler_EmptyXML_NullCompiler()
{
    // Arrange
    const QString compilerXml("");

    // Act
    CompilerBase* compiler = CompilerFactory::GetCompiler(compilerXml);

    // Assert
    QCOMPARE(compiler, (CompilerBase*)0);
}

void CompilerFactoryTests::GetCompiler_ValidTestCompilerXML_TestCompiler()
{
    // Arrange
    TestCompiler target;
    const QString compilerXml = target.Serialize();

    // Act
    TestCompiler* compiler = dynamic_cast<TestCompiler*>(CompilerFactory::GetCompiler(compilerXml));

    // Assert
    QVERIFY2(compiler != NULL, "Created compiler was not a TestCompiler");
}

void CompilerFactoryTests::GetCompiler_ValidGCCCompilerXML_GCCCompiler()
{
    // Arrange
    GCCCompiler target;

    const QString compilerXml = target.Serialize();

    // Act
    GCCCompiler* compiler = dynamic_cast<GCCCompiler*>(CompilerFactory::GetCompiler(compilerXml));

    // Assert
    QVERIFY2(compiler != NULL, "Created compiler was not a GCCCompiler");
}
