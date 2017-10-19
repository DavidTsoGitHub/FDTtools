#ifndef CWCOMPILER_H
#define CWCOMPILER_H

#include "CompilerBase.h"

#include <QProcess>

class CWCompiler : public CompilerBase
{
public:
    CWCompiler();
    ~CWCompiler();

    // CompilerBase interface
public:
    bool GenerateBuildEnvironment(QString projectOutputPath);
    bool Build(QString projectOutputPath);
protected:
    bool HasMandatoryCompilerSpecificData();

private:
    bool generateMPC5606SMakeFiles(void);
    bool generateMPC2606SMakeFileLocal(void);
    bool generateSourceArgsFile(void);
    bool generateAsmArgsFile(void);
    bool generateBinaryArgsFile(void);
    bool generatePrefixFile(void);
    bool generateSubDirMakefile(QString subDir);
    bool copyLinkerFile(void);

    QString m_sourcePath;
    QString m_buildPath;

    SourceTree m_tree;
    void GeneratePaddedS19File();
    void AddEmptyLines(quint32 startAddress, quint32 endAddress);
    void PadAndAppendLine(QString line, quint32 rowLength);
    quint8 GetChecksum(QString s19Line);

private slots:
    void slot_buildFinished(int exitCode);
    void slot_readyReadStandardOutput();
    void slot_readyReadStandardError();
    void slot_buildError(QProcess::ProcessError);

    // CompilerBase interface
public:
    bool ValidateBuildOutput(QString buildOutput, QStringList& errors);
};

#endif // CWCOMPILER_H
