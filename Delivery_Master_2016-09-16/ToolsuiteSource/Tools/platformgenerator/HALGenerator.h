#ifndef HALGENERATOR_H
#define HALGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"



class HALGenerator : public FileGeneratorBase
{
public:
    explicit HALGenerator(Project* project);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    const BSPSpecification& m_bspSpecification;

    QString m_HALHeaderPath;
    QString m_HALSourcePath;

    void generateHALSource(void);
    void generateHALHeader(void);
    QString stripPointers(QString& datatypeString);
};

#endif // HALGENERATOR_H
