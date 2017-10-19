#ifndef COMPILERFACTORY
#define COMPILERFACTORY

#include "CompilerBase.h"

class CompilerFactory
{
public:
    static QList<CompilerBase*> GetAvailableCompilers();
    static CompilerBase* GetCompiler(QString compilerXml);
    static CompilerBase* GetCompilerSkeleton(QString compilerType);
    static QString PeekForType(QString compilerXml);
    static QString PeekForName(QString compilerXml);
};

#endif // COMPILERFACTORY

