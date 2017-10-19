#ifndef TOOL_SETTINGS_H
#define TOOL_SETTINGS_H

#include <QString>
#include <QList>

class ToolSettings;
class CompilerBase;

class ToolSettings
{
public:
    ToolSettings(void);
    explicit ToolSettings(const ToolSettings& copyFrom);
    ~ToolSettings(void);

    QString FunctionLibraryPath;
    QString PlatformGeneratorFilePath;
    QString SimulatorFilePath;
    QString BuilderFilePath;
    QList<CompilerBase*> Compilers;

    void SetFunctionLibraryPath(QString path);
    void AddCompiler(QString compilerData);
    void UpdateCompiler(QString name, QString compilerData);
    void RemoveCompiler(QString name);
    CompilerBase* GetCompiler(QString name);
};

#endif // PROJECT

