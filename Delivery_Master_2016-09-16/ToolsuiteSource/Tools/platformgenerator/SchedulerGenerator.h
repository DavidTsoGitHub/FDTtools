#ifndef SCHEDULERGENERATOR_H
#define SCHEDULERGENERATOR_H

#include "Project.h"
#include "FileGeneratorBase.h"

class SchedulerGenerator : public FileGeneratorBase
{

public:
    explicit SchedulerGenerator(Project* project);
    ~SchedulerGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    Project* m_project;

    QString m_SchedulerCallbackHeaderPath;
    QString m_SchedulerConfigurationPath;

    void generateSchedulerConfigurationFile(const QList<const Task*>& tasks);
    void generateSchedulerCallbackHeaderFile(const QList<const Task*>& tasks);
};

#endif // SCHEDULERGENERATOR_H
