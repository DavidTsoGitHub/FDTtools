#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include "ToolSettings.h"

#include <QObject>

class QString;
class QSettings;

class SettingsHandler : public QObject
{
    Q_OBJECT

public:
    explicit SettingsHandler(QObject* parent = 0);
    ~SettingsHandler(void);

    ToolSettings& LoadToolSettings(void);
    ToolSettings& GetToolSettings(void);
    void SaveToolSettings(const ToolSettings& toolSettings);
    void SetToolSettingsToNull(void);
    void SaveFunctionLibraryPath(QSettings& settings, const QString& functionLibraryPath);

    bool ValidatePaths(const ToolSettings& toolSettings);
signals:
    void signal_NoValidFunctionLibraryFound(void);
    void signal_UnavailablePathFound(const QString& message);
    void signal_SettingsLoaded(void);
    void signal_SettingsSaved(void);

private:
    ToolSettings* m_toolSettings;
    bool m_invalidateSettings;

};

#endif // SETTINGSHANDLER_H
