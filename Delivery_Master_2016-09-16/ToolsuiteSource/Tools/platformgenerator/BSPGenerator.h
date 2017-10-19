#ifndef BSPSIMGENERATOR_H
#define BSPSIMGENERATOR_H

#include "FileGeneratorBase.h"
#include "BSPSpecification.h"
#include "Project.h"

class BSPGenerator : public FileGeneratorBase
{
public:
    BSPGenerator(Project* project, bool generateForSimulation);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    Project& m_project;
    const BSPSpecification& m_bspSpecification;
    bool m_generateForSimulation;
    QString m_BSPHeaderPath;
    QString m_BSPTypesHeaderPath;
    QString m_PCICHeaderFilePath;
    QString m_PCICSourceFilePath;

    void generateBSPHeaderFile(void);
    void generateBSPTypesHeaderFile(void);
    void generatePCICHeaderFile(void);
    void generatePCICSourceFile(void);
    void setupCharAsSegmentsArray(quint8 indexSpecialSegment[][2]);
    quint8 getSegmentInMappingPosition(quint8 index, QString character);

    void setupCustomBSPImplementations(void);
    QMap<QString, QStringList> m_customBSPImplementationsByFunctionName;

    quint8 m_CharAsSegments[6][10];
};

#endif // BSPSIMGENERATOR_H
