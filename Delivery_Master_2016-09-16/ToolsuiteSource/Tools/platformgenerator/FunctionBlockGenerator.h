#ifndef FUNCTIONBLOCKFILECOPIER_H
#define FUNCTIONBLOCKFILECOPIER_H

#include "FileGeneratorBase.h"
#include "Project.h"

#include <QList>
#include <QDir>

class FunctionBlockFileCopier : public FileGeneratorBase
{

public:
    explicit FunctionBlockFileCopier(Project* project, bool generateForSimulation);
    ~FunctionBlockFileCopier(void);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles(void);

private:
    Project* m_project;
    bool m_generateForSimulation;
    QList<GeneratedFile*> m_generatedFiles;
    QString m_currentFunctionBlockRoot;
    QString m_currentFunctionBlockName;

    void copyFunctionBlockFiles(void);
    void generateFunctionBlockConfigurationSourceFiles(void);
    void generateFunctionBlockConfigurationHeaderFiles(void);
    int getFiles(const QDir& currentDir);
    QString getRelativePath(const QFileInfo& functionBlockFile);
    QString toHexString(QString configurationParameterValue);
    QList<QString> getByteValueStringFromValueString(QString value, QString type);
    quint8 getDataLength(const quint8 length, const QString dataType);
};

#endif // FUNCTIONBLOCKFILECOPIER_H
