#ifndef FDTPLATFORMGENERATOR_H
#define FDTPLATFORMGENERATOR_H

#include "IFileWriter.h"
#include "FileGeneratorBase.h"
#include <QList>

class FDTPlatformGenerator
{

public:
    FDTPlatformGenerator(const IFileWriter* fileWriter, const QList<FileGeneratorBase*>* fileGenerators);
    ~FDTPlatformGenerator(void);

    void GenerateFiles(void);

private:
    const IFileWriter* m_fileWriter;
    const QList<FileGeneratorBase*>* m_fileGenerators;
};

#endif // FDTPLATFORMGENERATOR_H
