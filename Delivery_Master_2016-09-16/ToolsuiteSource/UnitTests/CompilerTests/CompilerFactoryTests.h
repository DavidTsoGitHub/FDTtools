#ifndef COMPILERFACTORYTESTS_H
#define COMPILERFACTORYTESTS_H

#include "autotest.h"

class CompilerFactoryTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void GetCompiler_EmptyXML_NullCompiler(void);
    void GetCompiler_ValidTestCompilerXML_TestCompiler(void);
    void GetCompiler_ValidGCCCompilerXML_GCCCompiler(void);

};

DECLARE_TEST(CompilerFactoryTests)

#endif // COMPILERFACTORYTESTS_H
