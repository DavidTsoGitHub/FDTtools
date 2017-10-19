#ifndef PLATFORMBUILDER_H
#define PLATFORMBUILDER_H

#include <QObject>
#include <QProcess>

class CompilerBase;
class TargetSettings;

class PlatformBuilder : public QObject
{
    Q_OBJECT

public:
    explicit PlatformBuilder(QObject *parent = 0);
    ~PlatformBuilder(void);

    void BuildPlatform(CompilerBase* compiler, QString outputFolder, QString builderFilePath, bool isQuickBuild);

signals:
    void signal_PlatformBuildStarting(QString, QStringList);
    void signal_PlatformBuildFinished(int);
    void signal_ReadyReadStandardOutput(const QString& output);
    void signal_ReadyReadStandardError(const QString& error);
    void signal_Error(const QString& error);
    void signal_NoBuilderFound(void);
    void signal_killBuildProcess();

private:
    QProcess* m_buildPlatformProcess;
    bool m_killedByUser;

    void killProcess();

private slots:
    void slot_killProcess();
};

#endif // PLATFORMBUILDER_H
