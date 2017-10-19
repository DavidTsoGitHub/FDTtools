#include <QDirIterator>
#include <QDir>
#include "FDTPlatformGenerator.h"
#include "FileUtilities.h"
#include "MakefileCreator.h"
#include "Logger.h"

FDTPlatformGenerator::FDTPlatformGenerator(const IFileWriter* fileWriter, const QList<FileGeneratorBase*>* fileGenerators)
 : m_fileWriter(fileWriter), m_fileGenerators(fileGenerators)
{
}

FDTPlatformGenerator::~FDTPlatformGenerator()
{
}

void FDTPlatformGenerator::GenerateFiles()
{
    LOG_DEBUG("Entering");
    LOG_INFO("Running file generators");
    QList<GeneratedFile> allFiles;
    foreach(FileGeneratorBase* fileGenerator, *m_fileGenerators)
    {
        QList<GeneratedFile>& generatedFiles = fileGenerator->GenerateFiles();
        foreach (GeneratedFile generatedFile, generatedFiles)
        {
            m_fileWriter->WriteFile(generatedFile.RelativePath(), generatedFile.Contents());
            allFiles.append(generatedFile);
        }
    }
}
