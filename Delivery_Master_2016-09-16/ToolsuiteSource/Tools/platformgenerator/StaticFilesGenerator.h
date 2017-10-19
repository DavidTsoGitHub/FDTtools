#ifndef BASEPLATFORMGENERATOR_H
#define BASEPLATFORMGENERATOR_H

#include "FileGeneratorBase.h"

class StaticFilesGenerator : public FileGeneratorBase
{

public:
    StaticFilesGenerator(void);
    ~StaticFilesGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);
};

#endif // BASEPLATFORMGENERATOR_H
