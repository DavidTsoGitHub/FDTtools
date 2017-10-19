#ifndef PLATFORMGENERATOR_H
#define PLATFORMGENERATOR_H

class ToolSettings;

#include <QObject>
#include <QProcess>

class PlatformGenerator : public QObject
{
    Q_OBJECT

public:
    explicit PlatformGenerator(QObject* parent = 0);
    ~PlatformGenerator(void);

    void GenerateTargetPlatform(ToolSettings& toolSettings, QString projectFilePath);
    void GenerateSimulatorPlatform(ToolSettings& toolSettings, QString projectFilePath);

public slots:

signals:
    void signal_NoGeneratorFound(void);
    void signal_PlatformGeneratorStarting(QString program, QStringList arguments);
    void signal_PlatformGenertorFinished(int exitCode);
    void signal_ReadyReadStandardOutput(const QString& output);
    void signal_ReadyReadStandardError(const QString& error);
    void signal_Error(QString error);

private:
    QProcess* m_generatePlatformProcess;

    void killProcess(void);
    void generatePlatform(ToolSettings& toolSettings, QString projectFilePath, QString targetArgument);
};

#endif // PLATFORMGENERATOR_H
