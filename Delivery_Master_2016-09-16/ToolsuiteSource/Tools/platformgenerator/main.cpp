#include <QCoreApplication>
#include <QCommandLineParser>
#include <QStringList>
#include <QTextStream>
#include <QChar>

#include "FunctionBlock.h"
#include "BSPDisplay.h"
#include "BSPGenerator.h"
#include "FDTPlatformGenerator.h"
#include "StaticFilesGenerator.h"
#include "SchedulerGenerator.h"
#include "FunctionBlockGenerator.h"
#include "CALGenerator.h"
#include "GCLGenerator.h"
#include "COMGenerator.h"
#include "DisplayManagerGenerator.h"
#include "SegmentDisplayManagerGenerator.h"
#include "MenuGenerator.h"
#include "HALGenerator.h"
#include "LabelManagerGenerator.h"
#include "LayoutManagerGenerator.h"
#include "CheckMessageManagerGenerator.h"
#include "UIWidgetGenerator.h"
#include "DTCManagerGenerator.h"
#include "DIDCALMappingGenerator.h"

#include "FileWriter.h"
#include "ProjectDeserializer.h"
#include "Project.h"
#include "ProjectValidator.h"
#include "Logger.h"
#include "FileUtilities.h"
#include "IconManagerGenerator.h"
#include "ImageManagerGenerator.h"
#include "ContentsManagerGenerator.h"
#include "AnimationManagerGenerator.h"

#include "Exceptions/ExceptionBase.h"


static void queryUserForGeneratorOptions(bool* generateForSimulation, bool* generateForTargetHW);
static void setupLogging(const QCommandLineParser& clParser, const QCommandLineOption& verbosityOption, const QCommandLineOption& logFacilityOption);
static bool readProjectFile(const QFileInfo projectFileInfo, Project** project);
static void printProject(Project& project);
static void printValidatorIssues(ProjectValidator& validator);
static void generateFilesForSimulation(const QDir& outputPath, Project* project);
static void generateFilesForTargetHW(const QDir& outputPath, Project* project);
static void generateFiles(const QDir outputPath, Project* project, bool generateForSimulation);
static void cleanDir(QDir dir);

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("SwedSpot AB");
    a.setApplicationName("Platformgenerator");
    a.setApplicationVersion(APP_VERSION);

    QCommandLineParser clParser;
    clParser.setApplicationDescription("Cluster platform generator\nVersion: " + QString(APP_VERSION) + "\nBuild: " + QString(APP_BUILD) + "\nBuild Date: " + QString(APP_BUILD_DATE) + "\n");
    clParser.addHelpOption();
    clParser.addPositionalArgument("project-file", QCoreApplication::translate("main", "The FDT project file"));
    clParser.addPositionalArgument("library-path", QCoreApplication::translate("main", "Path to the function library"));

    QCommandLineOption targetOption(QStringList() << "t" << "target", QCoreApplication::translate("platformgenerator", "Generate platform for target hardware"));
    clParser.addOption(targetOption);

    QCommandLineOption simulatorOption(QStringList() << "s" << "simulator", QCoreApplication::translate("platformgenerator", "Generate platform for PC simulation"));
    clParser.addOption(simulatorOption);

    QCommandLineOption verbosityOption(QStringList() << "v" << "verbosity",QCoreApplication::translate("main", "Log verbosity level (INFO, WARNING, ERROR, DEBUG)"), "ERROR");
    clParser.addOption(verbosityOption);

    QCommandLineOption logFacilityOption(QStringList() << "l" << "log-facility", QCoreApplication::translate("main", "Log to (none, console, filename)"), "console");
    clParser.addOption(logFacilityOption);

    clParser.process(a);

    const QStringList args = clParser.positionalArguments();

    if (args.length() != 2)
    {
        clParser.showHelp(0);
    }

    setupLogging(clParser, verbosityOption, logFacilityOption);

    const QFileInfo projectFileInfo(args[0]);
    const QDir functionLibraryDir(args[1]);

    LOG_INFO("Reading project file...");
    Project* project = NULL;

    if (!readProjectFile(projectFileInfo, &project) || project == NULL)
    {
        LOG_ERROR("...reading failed, aborting!");
        return -1;
    }
    LOG_INFO("Reading project file...done!");

    LOG_INFO("Initializing project...");
    try
    {
        project->Initialize(projectFileInfo, functionLibraryDir);
    }
    catch (ExceptionBase& e)
    {
        LOG_ERROR(e.Information() + " " + e.Message() + " " + e.InnerExceptionStrings().join(" "));
        return -1;
    }

    if (!project->IsInitialized())
    {
        LOG_ERROR("...project initialization failed, aborting!");
        return -1;
    }
    LOG_INFO("Initializing project...done!");

    printProject(*project);

    bool generateForTargetHW = clParser.isSet(targetOption);
    bool generateForSimulation = clParser.isSet(simulatorOption);

    if (generateForSimulation && generateForTargetHW)
    {
        LOG_ERROR("Cannot build for both target hardware and simulation! Either supply -t or -s on the command line, not both!");
        return -1;
    }

    if (!generateForTargetHW && !generateForSimulation)
    {
        queryUserForGeneratorOptions(&generateForSimulation, &generateForTargetHW);
    }

    ProjectValidator validator(*project);

    if (generateForSimulation)
    {
        LOG_INFO("Validating project for PC simulation...");
        if (!validator.ProjectIsValidForSimulator())
        {
            printValidatorIssues(validator);
            LOG_ERROR("...project validation failed, aborting!");
            return -1;
        }
        LOG_INFO("Validating project for PC simulation...done!");

        LOG_INFO("Cleaning output directory...");
        QDir outputDir(project->GetOutputFolder());
        cleanDir(outputDir);
        outputDir.mkdir(".");
        LOG_INFO("Cleaning output directory...done!");

        LOG_INFO("Generating platform for PC simulation to " + outputDir.absolutePath() + "...");
        generateFilesForSimulation(QDir(outputDir.path() + "/Source"), project);
        LOG_INFO("...generating platform for PC simulation done!");
    }
    else if (generateForTargetHW)
    {
        LOG_INFO("Validating project for target hardware...");
        if (!validator.ProjectIsValidForTarget())
        {
            printValidatorIssues(validator);
            LOG_ERROR("...project validation failed, aborting!");
            return -1;
        }
        LOG_INFO("Validating project for target hardware...done!");

        LOG_INFO("Cleaning output directory...");
        QDir outputDir(project->GetOutputFolder());
        cleanDir(outputDir);
        outputDir.mkdir(".");
        LOG_INFO("Cleaning output directory...done!");

        LOG_INFO("Generating platform for target hardware to " + outputDir.absolutePath() + "...");
        generateFilesForTargetHW(QDir(outputDir.path() + "/Source"), project);
        LOG_INFO("...generating platform for target hardware done!");
    }

    return 0;
}

static bool answerIsValid(const QString& answer)
{
    return answer.compare("s", Qt::CaseInsensitive) == 0 ||
           answer.compare("t", Qt::CaseInsensitive) == 0 ||
           answer.compare("q", Qt::CaseInsensitive) == 0;
}

static void queryUserForGeneratorOptions(bool* generateForSimulation, bool* generateForTargetHW)
{
    QString answer;

    while (!answerIsValid(answer))
    {
        printf("\nNo generator target option defined, please select a target:\n");
        printf("s - PC Simulation\n");
        printf("t - Target Hardware\n");
        printf("q - Abort generator\n");
        printf("Option: ");
        QTextStream stream(stdin);
        answer = stream.readLine();

        if (answer.compare("s", Qt::CaseInsensitive) == 0)
        {
            *generateForSimulation = true;
            *generateForTargetHW = false;
        }
        else if (answer.compare("t", Qt::CaseInsensitive) == 0)
        {
            *generateForSimulation = false;
            *generateForTargetHW = true;
        }
        else if (answer.compare("q", Qt::CaseInsensitive) == 0)
        {
            *generateForSimulation = false;
            *generateForTargetHW = false;
        }
        else
        {
            printf("Please select an option\n");
        }
    }
}

static void setupLogging(const QCommandLineParser& clParser, const QCommandLineOption& verbosityOption, const QCommandLineOption& logFacilityOption)
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

static bool readProjectFile(const QFileInfo projectFileInfo, Project** project)
{
    LOG_DEBUG("Entering");
    QFile file(projectFileInfo.absoluteFilePath());
    if (!file.open(QFile::ReadOnly))
    {
        LOG_ERROR("Couldn't open project file for reading! [" + projectFileInfo.absoluteFilePath() + "] " + file.errorString());
        return false;
    }

    bool result;
    QString errorText;
    QTextStream projectXml(&file);
    *project = ProjectDeserializer::Deserialize(projectXml.readAll(), &result, &errorText);

    if (!result)
    {
        LOG_ERROR("Deserialization of project file failed! Error: " + errorText);
        return false;
    }
    return true;
}

static void printValidatorIssues(ProjectValidator& validator)
{
    LOG_ERROR("Critical project errors found:\n" + validator.Errors());

    if (!validator.Warnings().isEmpty())
    {
        LOG_WARNING("Project validation warnings: \n" + validator.Warnings());
    }
}

static void generateFilesForSimulation(const QDir& outputPath, Project* project)
{
    generateFiles(outputPath, project, true);
}

static void generateFilesForTargetHW(const QDir& outputPath, Project* project)
{
    generateFiles(outputPath, project, false);
}

static void generateFiles(const QDir outputPath, Project* project, bool generateForSimulation)
{

    Q_INIT_RESOURCE(StaticFiles);
    QList<FileGeneratorBase*> generators;
    generators.append(new StaticFilesGenerator());
    generators.append(new SchedulerGenerator(project));
    generators.append(new FunctionBlockFileCopier(project, generateForSimulation));
    generators.append(new GCLGenerator(project));
    generators.append(new COMGenerator(project));
    generators.append(new HALGenerator(project));
    generators.append(new BSPGenerator(project, generateForSimulation));
    generators.append(new CALGenerator(project, generateForSimulation));
    generators.append(new DTCManagerGenerator(project));
    generators.append(new DIDCalMappingGenerator(project));

    if (project->GetBSPSpecification().HasDisplay())
    {
        if (project->GetLayoutDefinition().HasMenu())
        {
            generators.append(new MenuGenerator(project));
        }
        generators.append(new UIWidgetGenerator(*project));
        generators.append(new LabelManagerGenerator(project));
        generators.append(new IconManagerGenerator(project));
        generators.append(new CheckMessageManagerGenerator(project));
        generators.append(new ContentsManagerGenerator(project));
        generators.append(new DisplayManagerGenerator(project));
        generators.append(new LayoutManagerGenerator(project));

        if (project->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
        {
            generators.append(new ImageManagerGenerator(project));
            generators.append(new AnimationManagerGenerator(project));
        }
    }

    SegmentDisplay& segmentDisplay = project->GetSegmentDisplay();

    if (segmentDisplay.GetEnabled())
    {
        const BSPSegmentDisplay* bspSegmentDefinition = project->GetBSPSpecification().SegmentDisplay();
        if (bspSegmentDefinition != NULL)
        {
            generators.append(new SegmentDisplayManagerGenerator(segmentDisplay, *bspSegmentDefinition));
        }
        else
        {
            LOG_ERROR("BSP Specification did not contain a segment display definition, no configuration will be generated for the segment display in the project!");
        }
    }

    FileWriter writer(outputPath.absolutePath());
    FDTPlatformGenerator generator(&writer, &generators);

    generator.GenerateFiles();
}

static void printProject(Project& project)
{
    LOG_INFO("Project name: " + project.GetProjectName());
    LOG_INFO("BSP Specification: " + project.GetBSPSpecificationPath());
    LOG_INFO("CAN Specification: " + (project.GetCANSpecificationPath().isEmpty() ? "No CAN specification in project" : project.GetCANSpecificationPath()));
    LOG_INFO("Function blocks");
    LOG_INFO("\tID | Function Library Path | Configuration File Path");
    foreach (FunctionBlock* block, project.GetFunctionBlocks().List())
    {
        LOG_INFO("\t" + QString::number(block->GetID()) + " " + block->GetPathInFunctionLibrary());
    }
}

static void cleanDir(QDir dir)
{
    if (dir.exists())
    {
        foreach (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir())
            {
                cleanDir(info.absoluteFilePath());
            }
            else
            {
                QFile::remove(info.absoluteFilePath());
            }
        }
        dir.rmdir(".");
    }
}
