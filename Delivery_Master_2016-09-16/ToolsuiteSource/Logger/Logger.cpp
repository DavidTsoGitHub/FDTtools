#include "Logger.h"

#include <QDateTime>
#include <QFile>
#include <QString>
#include <QTextStream>

volatile bool Logger::m_isReady = false;
Logger::LOG_MODE Logger::m_logMode = Logger::LOG_OFF;
Logger::LOG_LEVEL Logger::m_logLevel = Logger::ERROR;
QFile* Logger::m_logFile = NULL;
QTextStream* Logger::m_logStream = NULL;
QTextStream* Logger::m_errorLogStream = NULL;

void Logger::Log(Logger::LOG_LEVEL level, QString message)
{
    if (!m_isReady || level > m_logLevel)
    {
        return;
    }

    switch (m_logMode)
    {
        case Logger::LOG_OFF:
            return;
        case Logger::LOG_TO_CONSOLE:
            switch (level)
            {
                case Logger::ERROR:
                case Logger::WARNING:
                    if (m_errorLogStream != NULL && m_errorLogStream->device() != NULL)
                    {
                        (*m_errorLogStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << message << endl;
                    }
                return;
                case Logger::DEBUG:
                case Logger::INFO:
                    if (m_logStream != NULL && m_logStream->device() != NULL)
                    {
                        (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << message << endl;
                    }

            }
            return;
        case Logger::LOG_TO_FILE:
            if (m_logStream != NULL && m_logStream->device() != NULL)
            {
                (*m_logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << message << endl;
            }
            return;
    }

}

void Logger::closeLogFile()
{
    if (m_logFile != NULL)
    {
        if (m_logFile->isOpen())
        {
            m_logFile->close();
        }
        delete m_logFile;
    }
}

void Logger::clearLoggingFacilities()
{
    if (m_logStream != NULL)
    {
        delete m_logStream;
        m_logStream = NULL;
    }
    if (m_errorLogStream != NULL)
    {
        delete m_errorLogStream;
        m_errorLogStream = NULL;
    }

    closeLogFile();
}

void Logger::SetLogLevel(LOG_LEVEL logLevel)
{
    m_logLevel = logLevel;
}

void Logger::TurnOffLogging()
{
    m_isReady = false;

    clearLoggingFacilities();
    m_logMode = Logger::LOG_OFF;
    m_isReady = true;
}

void Logger::LogToConsole()
{
    m_isReady = false;

    clearLoggingFacilities();

    m_logStream = new QTextStream(stdout, QIODevice::WriteOnly);
    m_errorLogStream = new QTextStream(stderr, QIODevice::WriteOnly);

    m_logMode = Logger::LOG_TO_CONSOLE;
    m_isReady = true;
}

void Logger::LogToFile(QString filePath)
{
    m_isReady = false;

    clearLoggingFacilities();

    m_logFile = new QFile(filePath);
    if (!m_logFile->exists())
    {
        if (!m_logFile->open(QFile::WriteOnly))
        {
            delete m_logFile;
            m_logFile = NULL;
        }
    }
    else if (!m_logFile->open(QFile::Append))
    {
        delete m_logFile;
        m_logFile = NULL;
    }

    if (m_logFile != NULL && m_logFile->isOpen())
    {
        m_logStream = new QTextStream(m_logFile);
    }

    m_logMode = Logger::LOG_TO_FILE;
    m_isReady = true;
}
