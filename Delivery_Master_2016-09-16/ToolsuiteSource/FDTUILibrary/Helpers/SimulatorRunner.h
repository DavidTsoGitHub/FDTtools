#ifndef SIMULATORRUNNER_H
#define SIMULATORRUNNER_H

#include <QObject>
#include <QProcess>

class QString;

class SimulatorRunner : public QObject
{
    Q_OBJECT
public:
    explicit SimulatorRunner(QObject* parent = 0);
    ~SimulatorRunner(void);

    void RunSimulator(const QString& simulatorFilePath, const QString& bspSpecificationPath);
    void KillSimulatorProcesses(void);
    void SetOutputPath(const QString& outputPath);
    const QDateTime GetPCICLastBuildTime(void) const;

signals:
    void signal_SimulatorStarting(QString, QStringList);
    void signal_SimulatorFinished(int);
    void signal_ReadyReadStandardOutput(const QString& output);
    void signal_ReadyReadStandardError(const QString& error);
    void signal_NoSimulatorFound(void);

private:
    QString m_outputPath;
    QProcess* m_pcicProcess;
    QString m_simulatorFilePath;
    QString m_bspSpecificationPath;
    QProcess* m_simulatorProcess;

    void startSimulator(void);
    const QString getWindowsPath(void) const;

private slots:
    void slot_finished(int exitCode);
};

#endif // SIMULATORRUNNER_H
