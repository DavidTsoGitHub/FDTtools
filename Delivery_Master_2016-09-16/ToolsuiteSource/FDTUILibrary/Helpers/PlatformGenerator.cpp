#include "PlatformGenerator.h"

#include "ToolSettings.h"

#include <QFile>

PlatformGenerator::PlatformGenerator(QObject *parent) : QObject(parent),
    m_generatePlatformProcess(NULL)
{

}

PlatformGenerator::~PlatformGenerator()
{
    killProcess();
}


void PlatformGenerator::GenerateTargetPlatform(ToolSettings& toolSettings, QString projectFilePath)
{
    generatePlatform(toolSettings, projectFilePath, "-t");
}

void PlatformGenerator::GenerateSimulatorPlatform(ToolSettings& toolSettings, QString projectFilePath)
{
    generatePlatform(toolSettings, projectFilePath, "-s");
}

void PlatformGenerator::generatePlatform(ToolSettings& toolSettings, QString projectFilePath, QString targetArgument)
{
    QString platformGeneratorFilePath(toolSettings.PlatformGeneratorFilePath);
    QString functionLibraryPath(toolSettings.FunctionLibraryPath);

    if (platformGeneratorFilePath.isEmpty() || !QFile(platformGeneratorFilePath).exists())
    {
        emit signal_NoGeneratorFound();
        return;
    }

    QStringList arguments;
    arguments.append(projectFilePath);
    arguments.append(functionLibraryPath);
    arguments.append(targetArgument);

    m_generatePlatformProcess = new QProcess();
    m_generatePlatformProcess->setProgram(platformGeneratorFilePath);
    m_generatePlatformProcess->setArguments(arguments);
    m_generatePlatformProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_generatePlatformProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=] (int exitCode, QProcess::ExitStatus)
    {
        killProcess();
        emit signal_PlatformGenertorFinished(exitCode);
    });

    connect(m_generatePlatformProcess, &QProcess::readyReadStandardError, [=]
    {
        emit signal_ReadyReadStandardError(m_generatePlatformProcess->readAllStandardError().trimmed());
    });
    connect(m_generatePlatformProcess, &QProcess::readyReadStandardOutput, [=]
    {
        emit signal_ReadyReadStandardOutput(m_generatePlatformProcess->readAllStandardOutput().trimmed());
    });
    connect(m_generatePlatformProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error), [=]
    {
        emit signal_Error(m_generatePlatformProcess->errorString());
        killProcess();
    });

    emit signal_PlatformGeneratorStarting(m_generatePlatformProcess->program(), arguments);

    m_generatePlatformProcess->start();
}

void PlatformGenerator::killProcess()
{
    if(m_generatePlatformProcess != NULL)
    {
        m_generatePlatformProcess->close();
        delete m_generatePlatformProcess;
        m_generatePlatformProcess = NULL;
    }
}
