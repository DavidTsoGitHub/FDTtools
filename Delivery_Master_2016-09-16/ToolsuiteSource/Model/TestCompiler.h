#ifndef TESTCOMPILER_H
#define TESTCOMPILER_H

#include "CompilerBase.h"

class TestCompiler : public CompilerBase
{
public:
    TestCompiler();

    // CompilerBase interface
public:
    bool GenerateBuildEnvironment(QString projectRootPath);
    bool Build(QString projectOutputPath);
protected:
    bool HasMandatoryCompilerSpecificData();
};

#endif // TESTCOMPILER_H
