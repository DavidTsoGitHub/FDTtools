#include "StaticFilesGenerator.h"
#include "Logger.h"

StaticFilesGenerator::StaticFilesGenerator(void)
{
}

StaticFilesGenerator::~StaticFilesGenerator(void)
{
}

void StaticFilesGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    AddFilesFromResourcePath(":/StaticFiles");
}
