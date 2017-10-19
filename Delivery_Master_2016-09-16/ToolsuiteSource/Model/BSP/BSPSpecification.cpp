#include "BSPSpecification.h"

#include "BSPEnum.h"
#include "BSPDisplay.h"
#include "BSPFixedPointFloat.h"
#include "BSPFunction.h"
#include "BSPMapping.h"
#include "BSPMemory.h"
#include "BSPRunnable.h"
#include "BSPSegmentDisplay.h"
#include "CalibrationParameter.h"
#include "Logger.h"
#include "ProjectObject.h"
#include "ProjectObjectVisitor.h"

QString BSPSpecification::StdBSPReturnType("BSP_StdReturnType");

BSPSpecification::BSPSpecification(ProjectObject* parent) : ProjectObject(QString("BSP"), parent),
  m_display(NULL),
  m_memory(new BSPMemory()),
  m_segmentDisplay(NULL)
{
    initializeConstants();
}

BSPSpecification::~BSPSpecification()
{
    clearConstants();
    ClearCalibrationParameters();
    ClearEnums();
    ClearFixedPointFloats();
    ClearFunctions();
    ClearMappings();
    ClearRunnables();
}

void BSPSpecification::AddCalibrationParameter(const CalibrationParameter* calibrationParameter, QString defaultValue)
{
    m_calibrationParameters.append(calibrationParameter);
    m_defaultCalibrationValues.insert(calibrationParameter->GetParameterName(), defaultValue);
}

void BSPSpecification::ClearCalibrationParameters()
{
    while (!m_calibrationParameters.isEmpty())
    {
        delete m_calibrationParameters.takeFirst();
    }
}

const QList<const CalibrationParameter*>& BSPSpecification::CalibrationParameters() const
{
    return m_calibrationParameters;
}

QString BSPSpecification::GetCalibrationParameterDefaultValue(QString parameterName) const
{
    if (m_defaultCalibrationValues.contains(parameterName))
    {
        return m_defaultCalibrationValues.value(parameterName);
    }

    LOG_WARNING("No default value found for BSP calibration parameter " + parameterName);

    return 0;
}

void BSPSpecification::AddEnum(const BSPEnum* bspEnum)
{
    m_enums.append(bspEnum);
}

void BSPSpecification::ClearEnums()
{
    while (!m_enums.isEmpty())
    {
        delete m_enums.takeFirst();
    }
}

const QList<const BSPEnum*>& BSPSpecification::Enums() const
{
    return m_enums;
}

bool BSPSpecification::IsEnum(QString dataType) const
{
    foreach (const BSPEnum* bspEnum, m_enums)
    {
        if (bspEnum->Name == dataType.remove("*"))
        {
            return true;
        }
    }
    return false;
}

const BSPEnum* BSPSpecification::GetBSPDCUBPPModes() const
{
    if(this->GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        foreach (const BSPEnum* bspenum, m_enums)
        {
            if (bspenum->Name.compare("BSP_DCU_BPPModes") == 0)
            {
                return bspenum;
            }
        }
    }
    return NULL;
}

void BSPSpecification::SetBSPDCUBPPModes(IBSP_DCU_BPPModes* bspDCUBPPModes)
{
    m_display->SetBSPDCUBPPModes(bspDCUBPPModes);
}

void BSPSpecification::AddFixedPointFloat(const BSPFixedPointFloat* fpf)
{
    m_fixedPointFloats.append(fpf);
}

void BSPSpecification::ClearFixedPointFloats()
{
    while (!m_fixedPointFloats.isEmpty())
    {
        delete m_fixedPointFloats.takeFirst();
    }
}

const QList<const BSPFixedPointFloat*>& BSPSpecification::FixedPointFloats() const
{
    return m_fixedPointFloats;
}

void BSPSpecification::AddFunction(const BSPFunction* function)
{
    if (!function->GCLSignalName.isEmpty())
    {
        QString signalDataType;
        if (function->Arguments.count() > 0)
        {
            signalDataType = getGCLDataType(function, 0);
        }
        else
        {
            signalDataType = "void";
        }

        if (function->GCLSignalType == BSPFunction::Sink)
        {
            GCLSignalSink* sink = new GCLSignalSink(function->GCLSignalName, "HAL", signalDataType, false, false);
            AddGCLSignalSink(sink, function);
        }
        else if (function->GCLSignalType == BSPFunction::Source)
        {
            // When BSP is the source, the datatype is suffixed with a * because we always
            // aquire values from the BSP with a pointer, but this is not necessary in the GCL signal mapping
            // so it is removed.
            int indexOfPointer = signalDataType.indexOf('*');
            QString dataType = indexOfPointer >= 0 ? signalDataType.replace(indexOfPointer, 1, "") : signalDataType;

            GCLSignalSource* source = new GCLSignalSource(function->GCLSignalName, "HAL", dataType);
            AddGCLSignalSource(source, function);
        }
    }

    m_functions.append(function);
}

void BSPSpecification::ClearFunctions()
{
    while (!m_functions.isEmpty())
    {
        delete m_functions.takeFirst();
    }
}

const QList<const BSPFunction*>& BSPSpecification::Functions() const
{
    return m_functions;
}

const BSPFunction* BSPSpecification::GetBSPFunctionFromSignalSource(const GCLSignalSource* source) const
{
    if (m_sourceToFunctionMapping.contains(source))
    {
        return m_sourceToFunctionMapping.value(source, NULL);
    }

    LOG_ERROR("No matching function found for GCLSignalSource " + source->LongName());
    return NULL;
}

const BSPFunction* BSPSpecification::GetBSPFunctionFromSignalSink(const GCLSignalSink* sink) const
{
    if (m_sinkToFunctionMapping.contains(sink))
    {
        return m_sinkToFunctionMapping.value(sink, NULL);
    }

    LOG_ERROR("No matching function found for GCLSignalSink " + sink->LongName());
    return NULL;
}

const BSPFunction* BSPSpecification::GetFunctionByName(const QString& functionName) const
{
    foreach (const BSPFunction* function, m_functions)
    {
        if (function->Name == functionName)
        {
            return function;
        }
    }
    return NULL;
}

void BSPSpecification::AddMapping(const BSPMapping* mapping)
{
    m_mappings.append(mapping);
}

void BSPSpecification::ClearMappings()
{
    while (!m_mappings.isEmpty())
    {
        delete m_mappings.takeFirst();
    }
}

const QList<const BSPMapping*>& BSPSpecification::Mappings() const
{
    return m_mappings;
}

const BSPMapping* BSPSpecification::GetMappingFromFunction(const BSPFunction* function) const
{
    foreach (const BSPMapping* mapping, m_mappings)
    {
        if (mapping->FunctionName == function->Name)
        {
            return mapping;
        }
    }

    return NULL;
}

void BSPSpecification::AddRunnable(const BSPRunnable* runnable)
{
    m_runnables.append(runnable);
}

void BSPSpecification::ClearRunnables()
{
    while (!m_runnables.isEmpty())
    {
        delete m_runnables.takeFirst();
    }
}

const QList<const BSPRunnable*>& BSPSpecification::Runnables() const
{
    return m_runnables;
}

void BSPSpecification::SetDisplay(BSPDisplay* display)
{
    m_display = display;
}

const BSPDisplay* BSPSpecification::GetDisplay() const
{
    return m_display;
}

bool BSPSpecification::HasDisplay() const
{
    return m_display != NULL;
}

void BSPSpecification::SetMemory(BSPMemory* memory)
{
    m_memory = memory;
}

const BSPMemory* BSPSpecification::GetMemory() const
{
    return m_memory;
}

bool BSPSpecification::HasExternalMemory() const
{
    if (m_memory != NULL)
    {
        return (m_memory->GetExternalMemorySize() > 0);
    }

    return false;
}

void BSPSpecification::SetSegmentDisplay(BSPSegmentDisplay* segmentDisplay)
{
    m_segmentDisplay = segmentDisplay;
}

const BSPSegmentDisplay* BSPSpecification::SegmentDisplay() const
{
    return m_segmentDisplay;
}

bool BSPSpecification::HasSegmentDisplay() const
{
    return m_segmentDisplay != NULL;
}

void BSPSpecification::ClearSignals()
{
    while (!m_gclSignalSinks.isEmpty())
    {
        GCLSignalSink* sink = m_gclSignalSinks.takeFirst();
        delete sink;
    }

    while (!m_gclSignalSources.isEmpty())
    {
        GCLSignalSource* source = m_gclSignalSources.takeFirst();
        delete source;
    }

    m_sinkToFunctionMapping.clear();
    m_sourceToFunctionMapping.clear();
    m_endpointsByFunction.clear();
}

void BSPSpecification::ParseGCLSignals()
{
    foreach (const BSPMapping* mapping, m_mappings)
    {
        const BSPFunction* function = GetFunctionByName(mapping->FunctionName);
        if (function == NULL)
        {
            LOG_WARNING("Could not find BSP function (" + mapping->FunctionName + ") for mapping type " + mapping->Type);
            continue;
        }

        foreach (IndexToSignalMapping* indexToSignalMapping, mapping->IndexToSignalMappings)
        {
            QString dataType = getGCLDataType(function, 1);

            if (function->GCLSignalType == BSPFunction::Sink)
            {
                GCLSignalSink* sink = new GCLSignalSink(indexToSignalMapping->GCLSignal, "HAL", dataType, false, false);
                AddGCLSignalSink(sink, function);
            }
            else if (function->GCLSignalType == BSPFunction::Source)
            {
                // When BSP is the source, the datatype is suffixed with a * because we always
                // aquire values from the BSP with a pointer, but this is not necessary in the GCL signal mapping
                // so it is removed.
                int indexOfPointer = dataType.indexOf('*');
                QString cleanDataType = indexOfPointer >= 0 ? dataType.replace(indexOfPointer, 1, "") : dataType;
                GCLSignalSource* source = new GCLSignalSource(indexToSignalMapping->GCLSignal, "HAL", cleanDataType);
                AddGCLSignalSource(source, function);
            }
        }
    }
}

QString BSPSpecification::getGCLDataType(const BSPFunction* function, quint8 argumentIndex)
{
    if (function->Arguments.values().count() <= argumentIndex)
    {
        LOG_WARNING("Attempting to get data type with index " + QString::number(argumentIndex) + " from function with " + QString::number(function->Arguments.values().count()) + " arguments");
        return "";
    }

    QString dataType = function->Arguments.values().at(argumentIndex).Type;

    if (IsEnum(dataType))
    {
       return "uint8";
    }

    foreach (const BSPFixedPointFloat* fpf, m_fixedPointFloats)
    {
        if (fpf->Name == QString(dataType).remove("*"))
        {
            return fpf->BaseType;
        }
    }

    return dataType;
}

void BSPSpecification::initializeConstants()
{
    m_constants.insert(BSPSpecification::FrameBufferGetFunctionName, "BSP_Get_Framebuffer");
    m_constants.insert(BSPSpecification::FrameBufferRefreshFunctionName, "BSP_Service_FramebufferUpdated");
    m_constants.insert(BSPSpecification::GetRTCFunctionName, "BSP_Get_RTC");
    m_constants.insert(BSPSpecification::SetRTCFunctionName, "BSP_Set_RTC");
    m_constants.insert(BSPSpecification::WriteMemoryFunctionName, "BSP_Write_Memory");
    m_constants.insert(BSPSpecification::ReadMemoryFunctionName, "BSP_Read_Memory");
    m_constants.insert(BSPSpecification::RequestModeChangeFunctionName, "BSP_RequestModeChange");
    m_constants.insert(BSPSpecification::DisplaySegments, "BSP_Set_DisplaySegments");
    m_constants.insert(BSPSpecification::StopActiveSound, "BSP_StopActiveSoundGeneration");
    m_constants.insert(BSPSpecification::StopPassiveSound, "BSP_Service_StopSoundGeneration");
    m_constants.insert(BSPSpecification::ActiveSoundGenerationStatus, "BSP_Get_ActiveSoundGenerationStatus");
    m_constants.insert(BSPSpecification::PassiveSoundGenerationStatus, "BSP_Get_SoundGeneratorStatus");
}

void BSPSpecification::clearConstants()
{
    m_constants.clear();
}

const QStringList BSPSpecification::GetConstants() const
{
    return m_constants.values();
}

const QString BSPSpecification::GetConstantName(BSPSpecification::BSPConstants constantEnumuration) const
{
    return m_constants.find(constantEnumuration).value();
}

void BSPSpecification::AddGCLSignalSink(GCLSignalSink* sink, const BSPFunction* function)
{
    if (function->GCLSignalType != BSPFunction::Sink)
    {
        LOG_WARNING("Attempting to add a GCLSignalSink to a function that is not defined as Sink");
        return;
    }

    m_gclSignalSinks.append(sink);
    m_sinkToFunctionMapping.insert(sink, function);
    m_endpointsByFunction.insert(function, sink);
}

void BSPSpecification::AddGCLSignalSource(GCLSignalSource* source, const BSPFunction* function)
{
    if (function->GCLSignalType != BSPFunction::Source)
    {
        LOG_WARNING("Attempting to add a GCLSignalSource to a function that is not defined as Source");
        return;
    }

    m_gclSignalSources.append(source);
    m_sourceToFunctionMapping.insert(source, function);
    m_endpointsByFunction.insert(function, source);
}

const QList<const GCLSignalEndpoint*> BSPSpecification::GetSignalsForFunction(const BSPFunction* function) const
{
    return m_endpointsByFunction.values(function);
}

const QList<GCLSignalSource*>& BSPSpecification::GetSignalSources() const
{
    return m_gclSignalSources;
}

GCLSignalSource* BSPSpecification::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_gclSignalSources)
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }
    return NULL;
}

const QList<GCLSignalSink*>& BSPSpecification::GetSignalSinks() const
{
    return m_gclSignalSinks;
}

GCLSignalSink* BSPSpecification::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, m_gclSignalSinks)
    {
        if (sink->SignalName() == signalName)
        {
            return sink;
        }
    }
    return NULL;
}

const QString BSPSpecification::GetComponentName() const
{
    return "HAL";
}

void BSPSpecification::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitBSPSpecification(this);
}
