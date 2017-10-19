#include "CompilerBaseTests.h"
#include "TestCompiler.h"

void CompilerBaseTests::Deserialize_EmptyData_ReturnsFalse()
{
    // Arrange
    const QString compilerData = "";
    TestCompiler target;

    // Act
    bool result = target.Deserialize(compilerData);

    // Assert
    QVERIFY2(result == false, "Deserializing empty compiler data should return false!");
}

void CompilerBaseTests::Deserialize_EmptyBaseData_ReturnsTrue()
{
    // Arrange
    const QString compilerData = "<CompilerData></CompilerData>";
    TestCompiler target;

    // Act
    bool result = target.Deserialize(compilerData);


    // Assert
    QVERIFY2(result == true, "Deserializing valid compiler base data should return true!");
}

void CompilerBaseTests::Deserialize_InvalidBaseDataXML_ReturnsFalse()
{
    // Arrange
    const QString compilerData = "<CompilerData><BuildArguments></CompilerData>";
    TestCompiler target;

    // Act
    bool result = target.Deserialize(compilerData);


    // Assert
    QVERIFY2(result == false, "Deserializing invalid compiler base data XML should return false!");
}


void CompilerBaseTests::Deserialize_BaseDataWithEnvironmentVariables_EnvironmentVariablesDeserialized()
{
    // Arrange
    const QString envVarKey = "variable1";
    const QString envVarValue = "value1";
    const QString envVarKey2 = "variable2";
    const QString envVarValue2 = "value2";
    const QString compilerData = "<CompilerData><EnvironmentVariables><EnvironmentVariable Key=\"" + envVarKey + "\" Value=\"" + envVarValue + "\"/><EnvironmentVariable Key=\"" + envVarKey2 + "\" Value=\"" + envVarValue2 + "\"/></EnvironmentVariables></CompilerData>";
    TestCompiler target;

    // Act
    target.Deserialize(compilerData);

    // Assert
    QCOMPARE(target.GetEnvironmentVariables().count(), 2);
    QVERIFY2(target.GetEnvironmentVariables().contains(envVarKey), "Compiler environment variables did not contain the expected variable");
    QVERIFY2(target.GetEnvironmentVariables().value(envVarKey).compare(envVarValue) == 0, "Unexpected environment variable value");
    QVERIFY2(target.GetEnvironmentVariables().contains(envVarKey2), "Compiler environment variables did not contain the expected variable");
    QVERIFY2(target.GetEnvironmentVariables().value(envVarKey2).compare(envVarValue2) == 0, "Unexpected environment variable value");
}

void CompilerBaseTests::Deserialize_CompilerWithCustomAttributes_CustomAttributesDeserialized()
{
    // Arrange
    const QString customAttribute1 = "CustomAttribute1";
    const QString customAttributeValue1 = "value1";
    const QString compilerData = "<CompilerData><CompilerSpecificData><" + customAttribute1 + ">" + customAttributeValue1 + "</" + customAttribute1 + "></CompilerSpecificData></CompilerData>";
    TestCompiler target;

    // Act
    target.Deserialize(compilerData);

    // Assert
    QCOMPARE(target.GetCustomAttribute(customAttribute1), customAttributeValue1);
}

void CompilerBaseTests::Deserialize_SerializedTestCompiler_CompilersAreEqual()
{
    // Arrange
    TestCompiler prototype;
    prototype.AddEnvironmentVariable("envVarName1", "envVarValue1");
    prototype.AddEnvironmentVariable("envVarName2", "envVarValue2");
    QString xml = prototype.Serialize();

    // Act
    TestCompiler target;
    target.Deserialize(xml);

    // Assert
    QCOMPARE(target.GetName(), prototype.GetName());
    QCOMPARE(target.GetEnvironmentVariables().count(), prototype.GetEnvironmentVariables().count());
    foreach (QString key, prototype.GetEnvironmentVariables().keys())
    {
        QVERIFY(target.GetEnvironmentVariables().contains(key));
        QCOMPARE(target.GetEnvironmentVariables().value(key), prototype.GetEnvironmentVariables().value(key));
    }
}

void CompilerBaseTests::Serialize_TestCompiler_ReturnsValidXML()
{
    // Arrange
    TestCompiler target;

    // Act
    QString xml = target.Serialize();

    // Assert
    ValidateXML(xml);
}

void CompilerBaseTests::ValidateXML(QString xml)
{
    QXmlStreamReader reader(xml);
    while (!reader.atEnd())
    {
        reader.readNext();
    }
    QString errorMessage("XML was invalid! ");

    QVERIFY2(!reader.hasError(), errorMessage.append(reader.errorString()).append(xml).toStdString().c_str());
}
