#ifndef CHECKMESSAGEGENERATOR_H
#define CHECKMESSAGEGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"
#include "CheckMessagesDefinition.h"

class CheckMessageManagerGenerator : public FileGeneratorBase
{
public:
    explicit CheckMessageManagerGenerator(Project* project);
    ~CheckMessageManagerGenerator();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    Project* m_project;
    CheckMessageDefinition& m_checkMessagesDefinition;


    QString m_CheckMessageManagerConfigSourceFilePath;
    QString m_CheckMessageManagerConfigHeaderFilePath;


    quint32 getMaxNumbersOfItemsInLayouts();
    void generateCheckMessageManagerConfigHeaderFile();
    void generateCheckMessageManagerConfigSourceFile();
};

#endif // CHECKMESSAGEGENERATOR_H
