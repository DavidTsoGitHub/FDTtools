#include "SimulatorRunner.h"

#include "Exceptions/SimulatorGeneralException.h"
#include "Exceptions/FileNotExistException.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>

SimulatorRunner::SimulatorRunner(QObject *parent) : QObject(parent),
    m_outputPath(""),
    m_pcicProcess(NULL),
    m_simulatorFilePath(""),
    m_simulatorProcess(NULL)
{}

SimulatorRunner::~SimulatorRunner()
{
    if (m_pcicProcess != NULL)
    {
        m_pcicProcess->close();
        m_pcicProcess->deleteLater();
        m_pcicProcess = NULL;
    }

    if (m_simulatorProcess != NULL)
    {
        m_simulatorProcess->close();
        m_simulatorProcess->deleteLater();
        m_simulatorProcess = NULL;
    }
}

void SimulatorRunner::RunSimulator(const QString& simulatorFilePath, const QString& bspSpecificationPath)
{
    m_simulatorFilePath = simulatorFilePath;
    m_bspSpecificationPath = bspSpecificationPath;

    if (m_simulatorFilePath.isEmpty() || !QFile(m_simulatorFilePath).exists())
    {
        emit signal_NoSimulatorFound();
        return;
    }

    if (m_outputPath.isEmpty())
    {
        throw SimulatorGeneralException("No output path set!", false);
    }

#ifdef _WIN32
    QString pcicPath = QDir(m_outputPath).absoluteFilePath("Build/PCIC.exe");
#else
    QString pcicPath = QDir(m_outputPath).absoluteFilePath("Build/PCIC");
#endif

    QFile file(pcicPath);

    if (!file.exists())
    {
        QFileInfo fileInfo(pcicPath);
        throw FileNotExistException(fileInfo);
    }

    KillSimulatorProcesses();

    m_pcicProcess = new QProcess();
    m_pcicProcess->setProgram(pcicPath);
    m_pcicProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_pcicProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &SimulatorRunner::slot_finished);
    connect(m_pcicProcess, &QProcess::readyReadStandardError, [=] { emit signal_ReadyReadStandardError(m_pcicProcess->readAllStandardError().trimmed()); });
    connect(m_pcicProcess, &QProcess::readyReadStandardOutput, [=] { emit signal_ReadyReadStandardOutput(m_pcicProcess->readAllStandardOutput().trimmed()); });

    signal_SimulatorStarting(m_pcicProcess->program(), m_pcicProcess->arguments());

    startSimulator();
    m_pcicProcess->start();
}


void SimulatorRunner::KillSimulatorProcesses()
{
    if (m_simulatorProcess != NULL && m_simulatorProcess->state() == QProcess::Running)
    {
        m_simulatorProcess->close();
        m_simulatorProcess->deleteLater();
        m_simulatorProcess = NULL;
    }

    if (m_pcicProcess != NULL && m_pcicProcess->state() > QProcess::NotRunning)
    {
        m_pcicProcess->close();
        m_pcicProcess->deleteLater();
        m_pcicProcess = NULL;
    }

#if defined (Q_OS_WIN32)
    QProcess* qCmd = new QProcess();
    qCmd->setProgram(getWindowsPath() + "\\System32\\TaskKill.exe");

    QStringList qsListArgs = (QStringList() << "/F" << "/IM" << "simulator.exe");
    qCmd->setArguments(qsListArgs);
    qCmd->start();
    qCmd->waitForFinished(3000);

    delete qCmd;

    qCmd = new QProcess();
    qCmd->setProgram(getWindowsPath() + "\\System32\\TaskKill.exe");

    qsListArgs = (QStringList() << "/F" << "/IM" << "PCIC.exe");
    qCmd->setArguments(qsListArgs);
    qCmd->start();
    qCmd->waitForFinished(3000);

    delete qCmd;
#else

#endif
}

void SimulatorRunner::SetOutputPath(const QString& outputPath)
{
    m_outputPath = outputPath;
}

const QDateTime SimulatorRunner::GetPCICLastBuildTime() const
{
    if (m_outputPath.isEmpty())
    {
        throw SimulatorGeneralException("No output path set!", false);
    }

#ifdef _WIN32
    QFile pcicFile(QDir(m_outputPath).absoluteFilePath("Build/PCIC.exe"));
#else
    QFile pcicFile(QDir(m_outputPath).absoluteFilePath("Build/PCIC"));
#endif

    if (!pcicFile.exists())
    {
        return QDateTime::currentDateTime().addYears(-2000);
    }

    QFileInfo pcicFileInfo(pcicFile);
    return pcicFileInfo.lastModified();
}

void SimulatorRunner::startSimulator()
{
    if (m_simulatorFilePath.isEmpty())
    {
        throw SimulatorGeneralException("Simulator not found!\n\nThere is no path specified for the simulator.", false);
    }
    else if (!QFile(m_simulatorFilePath).exists())
    {
        throw SimulatorGeneralException("Simulator not found!\n\nThe simulator could not be found in the path " + m_simulatorFilePath, false);
    }

    KillSimulatorProcesses();

    m_simulatorProcess = new QProcess();
    m_simulatorProcess->setProgram(m_simulatorFilePath);
    m_simulatorProcess->setWorkingDirectory(QFileInfo(m_simulatorFilePath).absolutePath());

    connect(m_simulatorProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=] { KillSimulatorProcesses(); });

    QStringList args;
    args.append(m_bspSpecificationPath);
    m_simulatorProcess->setArguments(args);
    m_simulatorProcess->start();
}

const QString SimulatorRunner::getWindowsPath() const
{
    QStringList systemEnvironmentPaths(QProcess::systemEnvironment());

    int windowsDirectoryEnvironmentPathIndex = systemEnvironmentPaths.indexOf(QRegExp("^WINDIR=.*", Qt::CaseInsensitive));
    if (windowsDirectoryEnvironmentPathIndex > -1)
    {
        return systemEnvironmentPaths[windowsDirectoryEnvironmentPathIndex].split('=')[1];
    }
    else
    {
        return "C:\\Windows";
    }
}

void SimulatorRunner::slot_finished(int exitCode)
{
    if (exitCode != 0 && m_simulatorProcess->state() > QProcess::NotRunning)
    {
        emit signal_SimulatorFinished(exitCode);
        KillSimulatorProcesses();
    }
}
