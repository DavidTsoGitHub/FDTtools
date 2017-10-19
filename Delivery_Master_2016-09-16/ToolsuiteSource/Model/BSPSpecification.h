#ifndef BSPSPECIFICATION_H
#define BSPSPECIFICATION_H

#include "IGCLSignalSource.h"
#include "IGCLSignalSink.h"
#include "ProjectObject.h"

class BSPEnum;
class BSPDisplay;
class BSPFixedPointFloat;
class BSPFunction;
class BSPMapping;
class BSPMemory;
class BSPRunnable;
class BSPSegmentDisplay;
class CalibrationParameter;
class ProjectObjectVisitor;
class GCLSignalSink;
class GCLSignalSource;
class GCLSignalEndpoint;
class IBSP_DCU_BPPModes;

#include <qmap.h>

class BSPSpecification : public ProjectObject, public IGCLSignalSource, public IGCLSignalSink
{
public:

    static QString StdBSPReturnType;

    enum BSPConstants
    {
        FrameBufferGetFunctionName,
        FrameBufferRefreshFunctionName,
        GetRTCFunctionName,
        SetRTCFunctionName,
        WriteMemoryFunctionName,
        ReadMemoryFunctionName,
        RequestModeChangeFunctionName,
        DisplaySegments,
        StopActiveSound,
        ActiveSoundGenerationStatus,
        StopPassiveSound,
        PassiveSoundGenerationStatus
    };

    explicit BSPSpecification(ProjectObject* parent = 0);
    ~BSPSpecification();

    void AddCalibrationParameter(const CalibrationParameter* calibrationParameter, QString defaultValue);
    void ClearCalibrationParameters();
    const QList<const CalibrationParameter*>& CalibrationParameters() const;
    QString GetCalibrationParameterDefaultValue(QString parameterName) const;

    void AddEnum(const BSPEnum* bspEnum);
    void ClearEnums();
    const QList<const BSPEnum*>& Enums() const;
    bool IsEnum(QString dataType) const;

    const BSPEnum *GetBSPDCUBPPModes() const;
    void SetBSPDCUBPPModes(IBSP_DCU_BPPModes* bspDCUBPPModes);

    void AddFixedPointFloat(const BSPFixedPointFloat* fpf);
    void ClearFixedPointFloats();
    const QList<const BSPFixedPointFloat*>& FixedPointFloats() const;

    void AddFunction(const BSPFunction* function);
    void ClearFunctions();
    const QList<const BSPFunction*>& Functions() const;
    const BSPFunction* GetBSPFunctionFromSignalSource(const GCLSignalSource* signalName) const;
    const BSPFunction* GetBSPFunctionFromSignalSink(const GCLSignalSink* signalName) const;
    const BSPFunction* GetFunctionByName(const QString& functionName) const;

    void AddMapping(const BSPMapping* mapping);
    void ClearMappings();
    const QList<const BSPMapping*>& Mappings() const;
    const BSPMapping* GetMappingFromFunction(const BSPFunction* function) const;

    void AddRunnable(const BSPRunnable* runnable);
    void ClearRunnables();
    const QList<const BSPRunnable*>& Runnables() const;

    void SetDisplay(BSPDisplay* display);
    const BSPDisplay* GetDisplay() const;
    bool HasDisplay() const;

    void SetMemory(BSPMemory *memory);
    const BSPMemory *GetMemory() const;
    bool HasExternalMemory() const;

    void SetSegmentDisplay(BSPSegmentDisplay* segmentDisplay);
    const BSPSegmentDisplay* SegmentDisplay() const;
    bool HasSegmentDisplay() const;

    void ClearSignals();
    void ParseGCLSignals();
    void AddGCLSignalSource(GCLSignalSource* source, const BSPFunction* function);
    void AddGCLSignalSink(GCLSignalSink* sink, const BSPFunction* function);
    const QList<const GCLSignalEndpoint*> GetSignalsForFunction(const BSPFunction* function) const;

    const QString GetConstantName(BSPSpecification::BSPConstants constantEnumuration) const;
    const QStringList GetConstants() const;

private:
    QList<const BSPRunnable*> m_runnables;
    QList<const BSPEnum*> m_enums;
    QList<const BSPFunction*> m_functions;
    QList<const BSPFixedPointFloat*> m_fixedPointFloats;
    QList<const BSPMapping*> m_mappings;
    BSPDisplay* m_display;
    BSPMemory* m_memory;
    BSPSegmentDisplay* m_segmentDisplay;
    QList<const CalibrationParameter*> m_calibrationParameters;
    QList<GCLSignalSource*> m_gclSignalSources;
    QList<GCLSignalSink*> m_gclSignalSinks;

    QMap<BSPSpecification::BSPConstants, QString> m_constants;

    QMap<QString, QString> m_defaultCalibrationValues;

    QMap<const GCLSignalSink*, const BSPFunction*> m_sinkToFunctionMapping;
    QMap<const GCLSignalSource*, const BSPFunction*> m_sourceToFunctionMapping;
    QMultiMap<const BSPFunction*, const GCLSignalEndpoint*> m_endpointsByFunction;

    QString getGCLDataType(const BSPFunction* function, quint8 argumentIndex);

    void initializeConstants();
    void clearConstants();

    // IGCLSignalSource interface
public:
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;
    const QString GetComponentName() const;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // BSPSPECIFICATION_H
