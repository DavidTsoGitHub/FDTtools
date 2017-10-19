#include "SettingsHandler.h"

#include "CompilerBase.h"

#include <QDir>
#include <QSettings>
#include <QString>

SettingsHandler::SettingsHandler(QObject* parent) : QObject(parent),
    m_toolSettings(NULL),
    m_invalidateSettings(false)
{}

SettingsHandler::~SettingsHandler()
{
    delete m_toolSettings;
}

ToolSettings& SettingsHandler::LoadToolSettings()
{
    if (m_toolSettings != NULL)
    {
        m_toolSettings = NULL;
    }

    m_toolSettings = new ToolSettings();

    QSettings settings;

    m_toolSettings->FunctionLibraryPath = settings.value("FunctionLibraryPath").toString();
    m_toolSettings->PlatformGeneratorFilePath = settings.value("PlatformGeneratorPath").toString();
    m_toolSettings->SimulatorFilePath = settings.value("SimulatorFilePath").toString();
    m_toolSettings->BuilderFilePath = settings.value("BuilderFilePath").toString();

    int compilerCount = settings.beginReadArray("Compilers");
    for (int i=0; i<compilerCount; ++i)
    {
        settings.setArrayIndex(i);
        QString compilerData = settings.value("Data").toString();
        m_toolSettings->AddCompiler(compilerData);
    }

    settings.endArray();

    ValidatePaths(*m_toolSettings);

    m_invalidateSettings = false;
    emit signal_SettingsLoaded();

    return *m_toolSettings;
}

ToolSettings& SettingsHandler::GetToolSettings()
{
    if (m_toolSettings == NULL || m_invalidateSettings)
    {
        LoadToolSettings();
    }

    return *m_toolSettings;
}

void SettingsHandler::SaveToolSettings(const ToolSettings& toolSettings)
{
    QSettings settings;
    settings.clear();
    SaveFunctionLibraryPath(settings, toolSettings.FunctionLibraryPath);
    settings.setValue("PlatformGeneratorPath", toolSettings.PlatformGeneratorFilePath);
    settings.setValue("SimulatorFilePath", toolSettings.SimulatorFilePath);
    settings.setValue("BuilderFilePath", toolSettings.BuilderFilePath);

    settings.beginWriteArray("Compilers");
    for (int i = 0; i < toolSettings.Compilers.count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("Name", toolSettings.Compilers.at(i)->GetName());
        settings.setValue("Data", toolSettings.Compilers.at(i)->Serialize());
    }
    settings.endArray();

    m_invalidateSettings = true;
    emit signal_SettingsSaved();
}

void SettingsHandler::SetToolSettingsToNull()
{
    m_toolSettings = NULL;
}

void SettingsHandler::SaveFunctionLibraryPath(QSettings& settings, const QString& functionLibraryPath)
{
    settings.setValue("FunctionLibraryPath", functionLibraryPath);
}

bool SettingsHandler::ValidatePaths(const ToolSettings& toolSettings)
{
    QStringList warnings;
    bool result = true;

    if (toolSettings.FunctionLibraryPath.isEmpty() || !QDir(toolSettings.FunctionLibraryPath).exists())
    {
        warnings.append("Function Library Path path has been moved or deleted.");
    }

    if (toolSettings.PlatformGeneratorFilePath.isEmpty() || !QFileInfo(toolSettings.PlatformGeneratorFilePath).exists())
    {
        warnings.append("Platform Generator path has been moved or deleted.");
    }

    if (toolSettings.SimulatorFilePath.isEmpty() || !QFileInfo(toolSettings.SimulatorFilePath).exists())
    {
        warnings.append("Simulator path has been moved or deleted.");
    }

    if (toolSettings.BuilderFilePath.isEmpty() || !QFileInfo(toolSettings.BuilderFilePath).exists())
    {
        warnings.append("Builder path has been moved or deleted.");
    }

    if (warnings.count() > 0)
    {
        emit signal_UnavailablePathFound(warnings.join("\n") + "\n\nPlease update paths in settings.");
        result = false;
    }

    return result;
}
