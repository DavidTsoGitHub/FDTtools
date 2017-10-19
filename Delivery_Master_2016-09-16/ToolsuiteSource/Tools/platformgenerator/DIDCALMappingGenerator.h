#ifndef DIDCALMAPPINGGENERATOR_H
#define DIDCALMAPPINGGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

#include <QString>


class DIDCalMappingGenerator : public FileGeneratorBase
{
public:
    explicit DIDCalMappingGenerator(Project* project);
    ~DIDCalMappingGenerator();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    void generateDIDCALMappingHeader();
    void generateDIDCALMappingSource();

    QString m_didCALMappingHeaderPath;
    QString m_didCALMappingSourcePath;

    Project* m_project;
};

#endif // DIDCALMAPPINGGENERATOR_H
