#include "PlatformBuilder.h"
#include "Exceptions/BuildException.h"

#include "CompilerBase.h"
#include "TargetSettings.h"

#include <QFileInfo>
#include <QString>
#include <QList>
#include <QFile>

PlatformBuilder::PlatformBuilder(QObject *parent) : QObject(parent),
    m_buildPlatformProcess(NULL),
    m_killedByUser(false)
{}

PlatformBuilder::~PlatformBuilder()
{
    killProcess();
}

void PlatformBuilder::BuildPlatform(CompilerBase* compiler, QString outputFolder, QString builderFilePath, bool isQuickBuild)
{
    if (builderFilePath.isEmpty() || !QFile(builderFilePath).exists())
    {
        emit signal_NoBuilderFound();
        return;
    }

    m_buildPlatformProcess = new QProcess();
    m_buildPlatformProcess->setProgram(builderFilePath);
    m_buildPlatformProcess->setWorkingDirectory(outputFolder);
    m_buildPlatformProcess->setArguments(QStringList() << outputFolder << compiler->Serialize() << "-b" << "-g" << (isQuickBuild ? "-q" : "") << "-v=DEBUG");
    m_buildPlatformProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_buildPlatformProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=] (int exitCode, QProcess::ExitStatus) { killProcess(); emit signal_PlatformBuildFinished(exitCode); });
    connect(m_buildPlatformProcess, &QProcess::readyReadStandardError, [=] { emit signal_ReadyReadStandardError(m_buildPlatformProcess->readAllStandardError().trimmed()); });
    connect(m_buildPlatformProcess, &QProcess::readyReadStandardOutput, [=] { emit signal_ReadyReadStandardOutput(m_buildPlatformProcess->readAllStandardOutput().trimmed()); });
    connect(m_buildPlatformProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error), [=] { if (!m_killedByUser) emit signal_Error(m_buildPlatformProcess->errorString()); m_killedByUser = false; killProcess();});
    connect(this, &PlatformBuilder::signal_killBuildProcess, this, &PlatformBuilder::slot_killProcess);

    emit signal_PlatformBuildStarting(m_buildPlatformProcess->program(), m_buildPlatformProcess->arguments());

    m_buildPlatformProcess->start();
}

void PlatformBuilder::killProcess()
{
    if(m_buildPlatformProcess != NULL)
    {
        m_buildPlatformProcess->close();
        m_buildPlatformProcess->deleteLater();
        m_buildPlatformProcess = NULL;
    }
}

void PlatformBuilder::slot_killProcess()
{
    m_killedByUser = true;
    killProcess();
}

