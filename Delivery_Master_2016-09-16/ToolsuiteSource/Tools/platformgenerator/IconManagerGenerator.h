#ifndef ICONMANAGERGENERATOR_H
#define ICONMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

class IconManagerGenerator : public FileGeneratorBase
{
public:
    explicit IconManagerGenerator(Project* project);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    Project* m_project;
    const IconDefinition& m_iconDefinition;

    QString m_iconManagerHeaderPath;
    QString m_iconManagerSourcePath;
    QString m_iconRoot;
    QStringList m_iconHeaderFiles;

    void generateIconManagerHeader(void);
    void generateIconManagerSource(void);
    void generateIconHeaders(void);
    void generateIconIncludeHeader(void);
};

#endif // ICONMANAGERGENERATOR_H
