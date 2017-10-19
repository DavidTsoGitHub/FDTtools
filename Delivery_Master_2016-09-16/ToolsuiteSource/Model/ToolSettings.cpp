#include "ToolSettings.h"

#include "CompilerBase.h"
#include "CompilerFactory.h"
#include "ToolSettings.h"

#include <QTextStream>

ToolSettings::ToolSettings(void)
{
}

ToolSettings::ToolSettings(const ToolSettings& copyFrom)
{
    FunctionLibraryPath = copyFrom.FunctionLibraryPath;
    Compilers = copyFrom.Compilers;
    PlatformGeneratorFilePath = copyFrom.PlatformGeneratorFilePath;
    SimulatorFilePath = copyFrom.SimulatorFilePath;
    BuilderFilePath = copyFrom.BuilderFilePath;
}

ToolSettings::~ToolSettings()
{
}

void ToolSettings::SetFunctionLibraryPath(QString path)
{
    FunctionLibraryPath = path;
}

void ToolSettings::AddCompiler(QString compilerData)
{
    CompilerBase* compiler = CompilerFactory::GetCompiler(compilerData);
    if (compiler != NULL)
    {
        Compilers.append(compiler);
    }
}

void ToolSettings::UpdateCompiler(QString name, QString compilerData)
{
    int compilerIndex = -1;
    for (int i=0; i<Compilers.count(); ++i)
    {
        if (Compilers.at(i)->GetName() == name)
        {
            compilerIndex = i;
            Compilers.removeAt(i);
        }
    }

    if (compilerIndex >= 0)
    {
        CompilerBase* compiler = CompilerFactory::GetCompiler(compilerData);
        Compilers.insert(compilerIndex, compiler);
    }
}

void ToolSettings::RemoveCompiler(QString name)
{
    for (int i=0; i<Compilers.count(); ++i)
    {
        if (Compilers.at(i)->GetName() == name)
        {
            Compilers.removeAt(i);
            return;
        }
    }
}

CompilerBase* ToolSettings::GetCompiler(QString name)
{
    for (int i=0; i<Compilers.count(); ++i)
    {
        if (Compilers.at(i)->GetName() == name)
        {
            return Compilers.at(i);
        }
    }

    return NULL;
}

