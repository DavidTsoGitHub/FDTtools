#include "TestCompiler.h"

TestCompiler::TestCompiler() : CompilerBase("TestCompiler")
{
}

bool TestCompiler::Build(QString projectOutputPath)
{
    Q_UNUSED(projectOutputPath);
    return true;
}

bool TestCompiler::HasMandatoryCompilerSpecificData()
{
    return true;
}

bool TestCompiler::GenerateBuildEnvironment(QString projectRootPath)
{
    Q_UNUSED(projectRootPath);
    return true;
}
