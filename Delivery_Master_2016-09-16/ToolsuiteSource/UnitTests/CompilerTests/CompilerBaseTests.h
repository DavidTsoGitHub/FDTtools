#ifndef COMPILERBASETESTS_H
#define COMPILERBASETESTS_H

#include "autotest.h"

class CompilerBaseTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void Deserialize_EmptyData_ReturnsFalse(void);
    void Deserialize_EmptyBaseData_ReturnsTrue(void);
    void Deserialize_InvalidBaseDataXML_ReturnsFalse(void);
    void Deserialize_BaseDataWithEnvironmentVariables_EnvironmentVariablesDeserialized(void);
    void Deserialize_CompilerWithCustomAttributes_CustomAttributesDeserialized(void);

    void Serialize_TestCompiler_ReturnsValidXML(void);
    void Deserialize_SerializedTestCompiler_CompilersAreEqual();

private:
    void ValidateXML(QString xml);
};

DECLARE_TEST(CompilerBaseTests)

#endif // COMPILERBASETESTS_H
