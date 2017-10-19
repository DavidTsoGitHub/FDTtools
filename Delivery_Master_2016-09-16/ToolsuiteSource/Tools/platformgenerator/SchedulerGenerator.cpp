#include "SchedulerGenerator.h"
#include "Logger.h"
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QDirIterator>

SchedulerGenerator::SchedulerGenerator(Project* project) :
    m_project(project)
{
    m_SchedulerCallbackHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("Scheduler")).append("/cfg/Scheduler_Callbacks.h");
    m_SchedulerConfigurationPath = QString(PlatformPaths::CreatePlatformComponentPath("Scheduler")).append("/cfg/Scheduler_Cfg.h");
}

SchedulerGenerator::~SchedulerGenerator()
{
}

void SchedulerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    QList<const Task*> tasks;
    m_project->GetAllTasks(tasks);

    generateSchedulerConfigurationFile(tasks);
    generateSchedulerCallbackHeaderFile(tasks);
}

void SchedulerGenerator::generateSchedulerCallbackHeaderFile(const QList<const Task*>& tasks)
{
    QStringList fileLines;
    fileLines.append("#ifndef SCHEDULER_CALLBACKS_H");
    fileLines.append("#define SCHEDULER_CALLBACKS_H");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("");
    foreach (const Task* task, tasks)
    {
        if ((!task->InitFunction.isEmpty()) && (!task->Runnable.isNull()))
        {
            fileLines.append(task->GetInitFunctionPrototype() + ";");
        }

        if ((!task->Runnable.isEmpty()) && (!task->Runnable.isNull()))
        {
            fileLines.append(task->GetRunnableFunctionPrototype() + ";");
        }
    }

    fileLines.append("#endif //SCHEDULER_CALLBACKS_H");
    fileLines.append("");

    AddGeneratedFile(m_SchedulerCallbackHeaderPath, fileLines.join("\n"));
}

void SchedulerGenerator::generateSchedulerConfigurationFile(const QList<const Task*>& tasks)
{
    QStringList fileLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"Scheduler.h\"");
    fileLines.append("#include \"Scheduler_Callbacks.h\"");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("#define  NUMBER_OF_SCHEDULER_TASKS " + QString::number(tasks.count()) + "u");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("tTimers TaskTable[NUMBER_OF_SCHEDULER_TASKS] = {");
    fileLines.append("    //fnpInit                       fpnTask                     u16InitialDelay         u16PeriodTick       u16TimeToActivation");

    QStringList taskSchedulerConfigurations;

    foreach (const Task* task, tasks)
    {
        taskSchedulerConfigurations.append("    " + task->GetSchedulerEntry());
    }

    fileLines.append(taskSchedulerConfigurations.join(",\n"));

    fileLines.append("};");
    fileLines.append("");

    AddGeneratedFile(m_SchedulerConfigurationPath, fileLines.join("\n"));
}
