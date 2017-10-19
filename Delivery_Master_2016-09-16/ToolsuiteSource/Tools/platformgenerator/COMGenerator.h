#ifndef COMGENERATOR_H
#define COMGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

#include <QStringList>

class COMGenerator : public FileGeneratorBase
{
public:
    explicit COMGenerator(Project* project);
    ~COMGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    Project* m_project;
    const CANSpecification& m_canSpecification;
    const FunctionBlockContainer& m_functionBlocks;

    QString m_CANConfigSourcePath;
    QString m_CANMessageDataSourcePath;
    QString m_COMGCLRelaySourcePath;
    QString m_COMGCLRelayHeaderPath;
    QString m_CANIFCFGHeaderPath;

    bool signalHasNotification(const CANSignal* signal);
    void generateCANMessageDataSource(void);
    void generateCOMGCLRelayHeader(void);
    void generateCOMGCLRelaySource(void);
    void generateCANConfig(void);
    void generateCANIFConfigHeader(void);
    quint32 getTimingControlRegisterValue();
};

#endif // COMGENERATOR_H
