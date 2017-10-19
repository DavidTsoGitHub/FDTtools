#ifndef LOGGER_H
#define LOGGER_H

class QFile;
class QString;
class QTextStream;



class Logger
{
public:
    typedef enum
    {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    } LOG_LEVEL;

public:

    static void Log(LOG_LEVEL level, QString message);
    static void SetLogLevel(LOG_LEVEL logLevel);
    static void TurnOffLogging();
    static void LogToConsole();
    static void LogToFile(QString filePath);

private:
    typedef enum
    {
        LOG_OFF,
        LOG_TO_CONSOLE,
        LOG_TO_FILE
    }LOG_MODE;

    static volatile bool m_isReady;
    static LOG_MODE m_logMode;
    static LOG_LEVEL m_logLevel;

    static QFile* m_logFile;
    static QTextStream* m_logStream;
    static QTextStream* m_errorLogStream;

    static void clearLoggingFacilities();
    static void closeLogFile();
};

#define LOG_INFO(M) Logger::Log(Logger::INFO, QString("[INFO] ").append(M))
#define LOG_WARNING(M) Logger::Log(Logger::WARNING, QString("[WARNING] ").append(M))
#define LOG_ERROR(M) Logger::Log(Logger::ERROR, QString("[ERROR] ").append(QString().sprintf(" (%s:%d %s) ", __FILE__, __LINE__, __FUNCTION__)).append(M))
#define LOG_DEBUG(M) Logger::Log(Logger::DEBUG, QString("[DEBUG] ").append(QString().sprintf(" (%s:%d %s) ", __FILE__, __LINE__, __FUNCTION__)).append(M))

#endif // LOGUTILS_H
