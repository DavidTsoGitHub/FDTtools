#include "FunctionBlock.h"
#include "FunctionDefinitionDeserializer.h"
#include "Logger.h"
#include "Exceptions/CannotOpenFileException.h"
#include "Exceptions/DeserializationExeception.h"
#include "ProjectObjectVisitor.h"

#include <QDir>
#include <QTextStream>

FunctionBlock::FunctionBlock(ProjectObject* parent) : ProjectObject(QString("Function Block"), parent),
    m_id(-1),
    m_functionDefinition(this),
    m_isInitialized(false)
{
}

void FunctionBlock::SetPathInFunctionLibrary(QString pathInFunctionLibrary)
{
    m_pathInFunctionLibrary = pathInFunctionLibrary;
}

/* FunctionBlock interface */
void FunctionBlock::SetID(qint32 id)
{
    m_id = id;
}

qint32 FunctionBlock::GetID() const
{
    return m_id;
}

const QString FunctionBlock::GetPathInFunctionLibrary() const
{
    return m_pathInFunctionLibrary;
}

bool FunctionBlock::Initialize(QDir functionLibraryRoot)
{
    LOG_DEBUG("Entering");
    QFile functionDefinitionFile(functionLibraryRoot.absoluteFilePath(m_pathInFunctionLibrary));
    QFileInfo fileInfo(functionDefinitionFile);

    if(!functionDefinitionFile.open(QFile::ReadOnly))
    {
        throw CannotOpenFileException(fileInfo);
    }

    QTextStream functionDefinitionXml(&functionDefinitionFile);
    FunctionDefinitionDeserializer deserializer(functionDefinitionXml.readAll());
    if (!deserializer.Deserialize(m_functionDefinition))
    {
        throw DeserializationExeception(fileInfo, deserializer.Error());
    }

    m_isInitialized = true;

    setObjectName(m_functionDefinition.GetName());

    return true;
}

const FunctionDefinition& FunctionBlock::GetFunctionDefinition() const
{
    if (!m_isInitialized)
    {
        LOG_WARNING("Accessing Function Definition before initialization!");
    }

    return m_functionDefinition;
}

bool FunctionBlock::HasConfigurationParameter(const QString& parameterName) const
{
    return m_configurationParameters.contains(parameterName);
}

QString FunctionBlock::GetConfigurationParameterValue(const QString& parameterName) const
{
    return m_configurationParameters.value(parameterName);
}

void FunctionBlock::SetConfigurationParameter(const QString& parameterName, const QString& parameterValue)
{
    m_configurationParameters.insert(parameterName, parameterValue);
    NotifyPropertyUpdated();
}

bool FunctionBlock::HasCalibrationParameterDefaultValue(const QString& parameterName) const
{
    return m_calibrationParameterDefaultValues.contains(parameterName);
}

QString FunctionBlock::GetCalibrationParameterDefaultValue(const QString& parameterName) const
{
    return m_calibrationParameterDefaultValues.value(parameterName);
}

void FunctionBlock::SetCalibrationParameterDefaultValue(const QString& parameterName, const QString& parameterValue)
{
    m_calibrationParameterDefaultValues.insert(parameterName, parameterValue);
    NotifyPropertyUpdated();
}

const QMap<QString, QString> FunctionBlock::GetAllCalibrationParameters() const
{
    return m_calibrationParameterDefaultValues;
}

void FunctionBlock::AddLabelMappingsParameter(LabelMappings* labelMappings)
{
    m_functionDefinition.AddLabelMappings(labelMappings);
}

LabelMappings* FunctionBlock::GetLabelMappingsParameter(const QString& labelMappingsName) const
{
    foreach (LabelMappings* labelMappings, m_functionDefinition.LabelMappingsParameters())
    {
        if (labelMappings->GetLabelMappingsName().compare(labelMappingsName) == 0)
        {
            return labelMappings;
        }
    }

    return NULL;
}

bool FunctionBlock::HasLabelMappings(void) const
{
    return (m_functionDefinition.LabelMappingsParameters().isEmpty() ? false : true);
}

bool FunctionBlock::HasDTCParameter(const QString& faultName) const
{
    return m_dtcParameterIDs.contains(faultName);
}

void FunctionBlock::SetDTCParameterID(const QString& faultName, quint32 dtcID)
{
    m_dtcParameterIDs.insert(faultName, dtcID);
    NotifyPropertyUpdated();
}

quint32 FunctionBlock::GetDTCParameterID(const QString& faultName) const
{
    return m_dtcParameterIDs.value(faultName, 0);
}

void FunctionBlock::SetIconResourceID(const QString& resourceName, quint32 iconID)
{
    m_iconResourceIDs.insert(resourceName, iconID);
    NotifyPropertyUpdated();
}

quint32 FunctionBlock::GetIconResourceID(const QString& resourceName) const
{
    return m_iconResourceIDs.value(resourceName, 0);
}

void FunctionBlock::SetImageResourceID(const QString& resourceName, quint32 imageID)
{
    m_imageResourceIDs.insert(resourceName, imageID);
    NotifyPropertyUpdated();
}

quint32 FunctionBlock::GetImageResourceID(const QString& resourceName) const
{
    return m_imageResourceIDs.value(resourceName, 0);
}

/* IGCLSignalSink interface */

const QList<GCLSignalSink*>& FunctionBlock::GetSignalSinks() const
{
    return GetFunctionDefinition().Sinks();
}

GCLSignalSink* FunctionBlock::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, GetFunctionDefinition().Sinks())
    {
        if (sink->SignalName() == signalName)
        {
            return sink;
        }
    }
    return NULL;
}

/* IGCLSignalSource interface */

const QList<GCLSignalSource*>& FunctionBlock::GetSignalSources() const
{
    return GetFunctionDefinition().Sources();
}

GCLSignalSource* FunctionBlock::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, GetFunctionDefinition().Sources())
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }
    return NULL;
}

const QString FunctionBlock::GetComponentName() const
{
    return GetFunctionDefinition().GetName();
}

void FunctionBlock::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitFunctionBlock(this);
}

bool FunctionBlock::objectSelfValidation(Project* project, QStringList* errorList)
{
    bool valid = true;

    valid &= validateImageResources(project, errorList);
    valid &= validateConfigurationParameters(errorList);
    valid &= validateCalibrationParameters(errorList);

    return valid;
}

bool FunctionBlock::validateImageResources(Project* project, QStringList* errorList)
{
    bool valid = true;

    foreach (const ImageResource imageResource, GetFunctionDefinition().ImageResources())
    {
        quint32 imageID = GetImageResourceID(imageResource.Name);

        if (imageID == 0)
        {
            errorList->append("Functionblock: '" + GetFunctionDefinition().GetName() + "' -> ImageResource: '" + imageResource.Name + "' has no assosicated image");
            valid = false;
        }

        if (project->GetImageDefinition().GetImageByID(imageID) == &Image::UndefinedImage)
        {
            errorList->append("Functionblock: '" + GetFunctionDefinition().GetName() + "' -> ImageResource: '" + imageResource.Name +
                              "' was mapped to imageID '" + QString::number(imageID) + "'', but this imageID does no longer match any entry in the Image Manager.");
            valid = false;
        }
    }

    return valid;
}

bool FunctionBlock::validateConfigurationParameters(QStringList* errorList)
{
    QStringList allParameterNames;
    bool allParametersAreSet = true;
    bool allParameterNamesAreUnique = true;

    foreach (const ConfigurationParameter* parameter, GetFunctionDefinition().ConfigurationParameters())
    {
        QString parameterName(parameter->GetParameterName());
        if (allParameterNames.contains(parameterName))
        {
            errorList->append("Duplicate configuration parameter name! " + GetFunctionDefinition().GetName() + "." + parameterName);
            allParameterNamesAreUnique = false;
        }
        else
        {
            allParameterNames.append(parameterName);
        }

        if (!HasConfigurationParameter(parameterName))
        {
            errorList->append("Missing value for configuration parameter! " + GetFunctionDefinition().GetName() + "." + parameterName);
            allParametersAreSet = false;
        }
    }

    if (HasLabelMappings())
    {
        foreach (LabelMappings* labelMapping, GetFunctionDefinition().LabelMappingsParameters())
        {
            if (labelMapping->GetMappedLabels().count() == 0)
            {
                errorList->append("Missing label(s) in label mapping: \"" + labelMapping->GetDescriptiveName() + "\" (in function block: " + GetComponentName() + "). Each label mapping must contain atleast one label!");
                allParametersAreSet = false;
            }
        }
    }

    return allParametersAreSet && allParameterNamesAreUnique;
}

bool FunctionBlock::validateCalibrationParameters(QStringList* errorList)
{
    QStringList allParameterNames;
    bool allParametersAreSet = true;
    bool allParameterNamesAreUnique = true;

    foreach (const CalibrationParameter* parameter, GetFunctionDefinition().CalibrationParameters())
    {
        QString parameterName(parameter->GetParameterName());
        if (allParameterNames.contains(parameterName))
        {
            errorList->append("Duplicate calibration parameter name! " + GetFunctionDefinition().GetName() + "." + parameterName);
            allParameterNamesAreUnique = false;
        }
        else
        {
            allParameterNames.append(parameterName);
        }

        if (!HasCalibrationParameterDefaultValue(parameterName))
        {
            errorList->append("Missing default value for calibration parameter! " + GetFunctionDefinition().GetName() + "." + parameterName);
            allParametersAreSet = false;
        }
    }

    return allParametersAreSet && allParameterNamesAreUnique;
}
