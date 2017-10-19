#include "Logger.h"
#include "Builder.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <assert.h>

void setupLogging(const QCommandLineParser& clParser, const QCommandLineOption& verbosityOption, const QCommandLineOption& logFacilityOption);
void trimStart(QString& string, QList<QChar> charsToTrim);
void trimEnd(QString& string, QList<QChar> charsToTrim);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser clParser;
    clParser.setApplicationDescription("Cluster application builder\nVersion: " + QString(APP_VERSION) + "\nBuild: " + QString(APP_BUILD) + "\nBuild Date: " + QString(APP_BUILD_DATE) + "\n");
    clParser.addHelpOption();
    clParser.addPositionalArgument("output-folder", QCoreApplication::translate("builder", "The path to the output folder in the project to build"));
    clParser.addPositionalArgument("compiler-string", QCoreApplication::translate("builder", "Compiler settings in XML format"));

    QCommandLineOption generateOption(QStringList() << "g" << "generate", QCoreApplication::translate("main", "Generate build environment"));
    clParser.addOption(generateOption);

    QCommandLineOption buildOption(QStringList() << "b" << "build", QCoreApplication::translate("main", "Execute build environment"));
    clParser.addOption(buildOption);

    QCommandLineOption verbosityOption(QStringList() << "v" << "verbosity",QCoreApplication::translate("main", "Log verbosity level (INFO, WARNING, ERROR, DEBUG)"), "ERROR");
    clParser.addOption(verbosityOption);

    QCommandLineOption quickBuildOption(QStringList() << "q" << "quickBuild",QCoreApplication::translate("main", "Do not regenerate MPC5606S.prefix if present"));
    clParser.addOption(quickBuildOption);

    QCommandLineOption logFacilityOption(QStringList() << "l" << "log-facility", QCoreApplication::translate("main", "Log to (none, console, filename)"), "console");
    clParser.addOption(logFacilityOption);

    QCommandLineOption readCompilerStringFromStdIn(QStringList() << "s", QCoreApplication::translate("builder", "If defined, prompts user to input the XML string manually, useful for debugging purposes"));
    clParser.addOption(readCompilerStringFromStdIn);

    QCommandLineOption versionOption(QStringList() << "V" << "version", QCoreApplication::translate("main", "Show version information"));
    clParser.addOption(versionOption);

    clParser.process(a);

    const QStringList args = clParser.positionalArguments();
    QString compilerData("");

    if(!clParser.isSet(buildOption) && !clParser.isSet(generateOption))
    {

        clParser.showHelp(0);
    }

    if (args.length() < 1)
    {
        clParser.showHelp(0);
    }


    if (args.length() != 2 && (args.length() == 1 && !clParser.isSet(readCompilerStringFromStdIn)))
    {
        QSettings settings("SwedSpot AB", "FDT");
        int compilerCount = settings.beginReadArray("Compilers");

        if (compilerCount == 0)
        {
            clParser.showHelp(0);
        }

        QMap<qint32, QString> compilerMapping;

        printf("No compiler specified in command-line, select a compiler from registry settings:\n");
        for (int i=0; i<compilerCount; ++i)
        {
            settings.setArrayIndex(i);
            QString compilerName = settings.value("Name").toString();
            printf("%d - %s\n", i, compilerName.toStdString().c_str());
            QString compilerData = settings.value("Data").toString();
            compilerMapping.insert(i, compilerData);
        }
        settings.endArray();

        printf("Compiler #: ");
        QTextStream stream(stdin);
        bool readOk;
        int answer = stream.readLine().toInt(&readOk);
        if (!readOk)
        {
            printf("Invalid compiler index, aborting\n");
            return 0;
        }
        else
        {
            compilerData = compilerMapping.value(answer);
        }
    }


    setupLogging(clParser, verbosityOption, logFacilityOption);


    QDir outputRootFolder(args[0]);

    Builder::clOptions m_options;
    m_options.generate = false;
    m_options.build = false;
    m_options.quickBuild = false;

    if (clParser.isSet(readCompilerStringFromStdIn))
    {
        printf("Enter compiler XML:\n");
        QFile in;
        in.open(stdin, QIODevice::ReadOnly);
        m_options.compilerString = in.readLine();
    }
    else if (args.count() > 1)
    {
        QString compilerString(args[1]);
        trimStart(compilerString, QList<QChar>() << '\'' << '\"');
        trimEnd(compilerString, QList<QChar>() << '\'' << '\"');
        m_options.compilerString = compilerString;
    }
    else
    {
        m_options.compilerString = compilerData;
    }

    qDebug("its: %s", m_options.compilerString.toStdString().c_str());

    m_options.outputRoot = outputRootFolder;

    if(clParser.isSet(generateOption)) m_options.generate = true;
    if(clParser.isSet(buildOption)) m_options.build = true;
    if(clParser.isSet(quickBuildOption)) m_options.quickBuild = true;

    Builder builder(m_options);

    if (!builder.Run())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void setupLogging(const QCommandLineParser& clParser, const QCommandLineOption& verbosityOption, const QCommandLineOption& logFacilityOption)
{
    if (clParser.isSet(verbosityOption))
    {
        QString logLevel = clParser.value(verbosityOption);

        if (logLevel.compare("info", Qt::CaseInsensitive) == 0)
        {
            Logger::SetLogLevel(Logger::INFO);
        }
        else if (logLevel.compare("warning", Qt::CaseInsensitive) == 0)
        {
            Logger::SetLogLevel(Logger::WARNING);
        }
        else if (logLevel.compare("error", Qt::CaseInsensitive) == 0)
        {
            Logger::SetLogLevel(Logger::ERROR);
        }
        else if (logLevel.compare("debug", Qt::CaseInsensitive) == 0)
        {
            Logger::SetLogLevel(Logger::DEBUG);
        }
        else
        {
            Logger::SetLogLevel(Logger::INFO);
        }
    }
    else
    {
        Logger::SetLogLevel(Logger::INFO);
    }

    if (clParser.isSet(logFacilityOption))
    {
        QString logFacility = clParser.value(logFacilityOption);

        if (logFacility.compare("none", Qt::CaseInsensitive) == 0)
        {
            Logger::TurnOffLogging();
        }
        else if (logFacility.compare("console", Qt::CaseInsensitive) == 0)
        {
            Logger::LogToConsole();
        }
        else
        {
            Logger::LogToFile(logFacility);
        }
    }
    else
    {
        Logger::LogToConsole();
    }
}

void trimStart(QString& string, QList<QChar> charsToTrim)
{
    while (charsToTrim.contains(string.at(0)))
    {
        string.remove(0, 1);
    }
}

void trimEnd(QString& string, QList<QChar> charsToTrim)
{
    while (charsToTrim.contains(string.at(string.length()-1)))
    {
        string.remove(string.length()-1, 1);
    }
}
