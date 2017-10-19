#ifndef CALGENERATOR_H
#define CALGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

class CALGenerator : public FileGeneratorBase
{
public:
    CALGenerator(Project* project, bool generateForSimulation);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    const TargetSettings& m_targetSettings;
    const QList<FunctionBlock*>& m_functionBlocks;
    const BSPSpecification& m_bspSpecification;
    bool m_generateForSimulation;

    QString m_CALConfigSourcePath;
    QString m_CALConfigHeaderPath;
    QList<const CalibrationParameter*> m_ODOParameterList;
    quint32 m_FBCalStart;
    quint32 m_u32TotalParameterLength;

    void generateCALConfigHeader();
    void generateCALConfigSource();

    int addCalibrationParameterEnum(QStringList& fileLines, const CalibrationParameter* cal);

    QString convertToByteString(QString defaultValue, const CalibrationParameter* calParameter, bool littleEndian, quint32 *checksum);
    bool isODOParameter(QString ParameterName);
    int getParameterByteLength(const CalibrationParameter* calParam);
    QStringList toByteArray(quint32 value, bool isLittleEndian);
    QStringList toByteArray(qint32 value, bool isLittleEndian);
    QStringList toByteArray(quint16 value, bool isLittleEndian);
    QStringList toByteArray(qint16 value, bool isLittleEndian);
};

#endif // CALGENERATOR_H
