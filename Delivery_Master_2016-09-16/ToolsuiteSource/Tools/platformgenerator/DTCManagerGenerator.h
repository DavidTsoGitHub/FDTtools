#ifndef DTCMANAGERGENERATOR
#define DTCMANAGERGENERATOR

#include "FileGeneratorBase.h"
#include "Project.h"
#include <QString>


class DTCManagerGenerator : public FileGeneratorBase
{
public:
    explicit DTCManagerGenerator(Project* project);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    void generateDTCManagerHeader(void);
    void generateDTCManagerSource(void);

    QString m_dtcManagerHeaderPath;
    QString m_dtcManagerSourcePath;
    Project* m_project;
};

#endif // DTCMANAGERGENERATOR

