#include "FunctionDefinition.h"
#include "ProjectObjectVisitor.h"

//Function Definition
FunctionDefinition::FunctionDefinition(ProjectObject* parent) : ProjectObject(QString("FDF"), parent)
{
}

FunctionDefinition::~FunctionDefinition()
{
    ClearTasks();
    ClearCalibrationParameters();
    ClearConfigurationParameters();
    ClearIconResources();
    ClearImageResources();
    ClearLabelMappings();
    ClearSinks();
    ClearSources();
    ClearTasks();
}

QString FunctionDefinition::GetName() const
{
    return m_name;
}

void FunctionDefinition::SetName(const QString& name)
{
    setObjectName(name + " FDF");
    m_name = name;
}

void FunctionDefinition::AddTask(Task* task)
{
    m_tasks.append(task);
}

void FunctionDefinition::AddGCLSignalSink(GCLSignalSink* sink)
{
    sink->SetParent(this);
    m_gclSignalSinks.append(sink);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddGCLSignalSource(GCLSignalSource *source)
{
    source->SetParent(this);
    m_gclSignalSources.append(source);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddConfigurationParameter(ConfigurationParameter* configurationParameter)
{
    m_configurationParameters.append(configurationParameter);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddCalibrationParameter(CalibrationParameter* calibrationParameter)
{
    m_calibrationParameters.append(calibrationParameter);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddLabelMappings(LabelMappings* labelMappings)
{
    labelMappings->SetParent(this);
    m_labelMappingsParameters.append(labelMappings);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddFaultDefinition(QString name, QString description)
{
    FaultDefinition fault;
    fault.Name = name;
    fault.Description = description;
    m_faultDefinitions.append(fault);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddIconResource(QString name)
{
    IconResource iconResource;
    iconResource.Name = name ;
    m_iconResources.append(iconResource);
    NotifyPropertyUpdated();
}

void FunctionDefinition::AddImageResource(QString name)
{
    ImageResource imageResource;
    imageResource.Name = name;
    m_imageResources.append(imageResource);
    NotifyPropertyUpdated();
}

void FunctionDefinition::ClearTasks()
{
    while (!m_tasks.isEmpty())
    {
        delete m_tasks.takeFirst();
    }
}

void FunctionDefinition::ClearSinks()
{
    while (!m_gclSignalSinks.isEmpty())
    {
        m_gclSignalSinks.first()->SetParent(NULL);
        delete m_gclSignalSinks.takeFirst();
    }
}

void FunctionDefinition::ClearSources()
{
    while (!m_gclSignalSources.isEmpty())
    {
        m_gclSignalSources.first()->SetParent(NULL);
        delete m_gclSignalSources.takeFirst();
    }
}

void FunctionDefinition::ClearConfigurationParameters()
{
    while (!m_configurationParameters.isEmpty())
    {
        delete m_configurationParameters.takeFirst();
    }
}

void FunctionDefinition::ClearCalibrationParameters()
{
    while (!m_calibrationParameters.isEmpty())
    {
        delete m_calibrationParameters.takeFirst();
    }
}

void FunctionDefinition::ClearLabelMappings()
{
    while (!m_labelMappingsParameters.isEmpty())
    {
        m_labelMappingsParameters.first()->SetParent(NULL);
        delete m_labelMappingsParameters.takeFirst();
    }
}

void FunctionDefinition::ClearFaultDefinitions()
{
    m_faultDefinitions.clear();
}

void FunctionDefinition::ClearIconResources()
{
    m_iconResources.clear();
}

void FunctionDefinition::ClearImageResources()
{
    m_imageResources.clear();
}

const QList<const Task*>& FunctionDefinition::Tasks() const
{
    return m_tasks;
}

const QList<GCLSignalSource*>& FunctionDefinition::Sources() const
{
    return m_gclSignalSources;
}

const QList<GCLSignalSink*>& FunctionDefinition::Sinks() const
{
    return m_gclSignalSinks;
}

const QList<const ConfigurationParameter*>& FunctionDefinition::ConfigurationParameters() const
{
    return m_configurationParameters;
}

const QList<const CalibrationParameter*>& FunctionDefinition::CalibrationParameters() const
{
    return m_calibrationParameters;
}

const QList<LabelMappings*>& FunctionDefinition::LabelMappingsParameters() const
{
    return m_labelMappingsParameters;
}

const QList<FaultDefinition> &FunctionDefinition::FaultDefinitions() const
{
    return m_faultDefinitions;
}

const QList<IconResource>& FunctionDefinition::IconResources() const
{
    return m_iconResources;
}

const QList<ImageResource>& FunctionDefinition::ImageResources() const
{
    return m_imageResources;
}

void FunctionDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitFunctionDefinition(this);
}
