#ifndef GCCCOMPILER_H
#define GCCCOMPILER_H

#include "CompilerBase.h"

class GCCCompiler : public CompilerBase
{
public:
    GCCCompiler();

    // CompilerBase interface
public:
    bool GenerateBuildEnvironment(QString projectOutputPath);
    bool Build(QString projectOutputPath);
protected:
    bool HasMandatoryCompilerSpecificData();

private:
    bool generatePCMakeFile(void);

    QString m_sourcePath;
    QString m_buildFolder;

    SourceTree m_tree;
};

#endif // GCCCOMPILER_H

