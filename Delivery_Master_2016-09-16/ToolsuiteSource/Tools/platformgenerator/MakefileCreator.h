#ifndef MAKEFILEGENERATOR_H
#define MAKEFILEGENERATOR_H

#include "FileGeneratorBase.h"
#include <QStringList>

class MakefileCreator
{

public:
    QString GenerateMakefile(const QList<GeneratedFile>& generatedFiles, const QList<QString> staticFiles);
    QString GenerateMakefileLocal();
    QString GenerateSubDirMakefile(QString dir);
    QStringList GetSourceDirs();

private:
    QStringList m_includeDirectories;
    QStringList m_libraryFiles;
    QStringList m_libraryDirectories;
    QStringList m_sourceFiles;
    QStringList m_sourceDirectories;

    void ParseGeneratedFiles(const QList<GeneratedFile>& files);
    void ParseStaticFiles(const QList<QString> files);

    void GeneratePCMakeFile();
    void GenerateMPC5606SMakeFiles();

};

#endif // MAKEFILEGENERATOR_H
