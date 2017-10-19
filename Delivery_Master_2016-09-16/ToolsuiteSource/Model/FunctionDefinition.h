#ifndef FUNCTIONDEFINITION
#define FUNCTIONDEFINITION

#include "ProjectObject.h"
#include "Task.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "IRunnable.h"
#include "ConfigurationParameter.h"
#include "CalibrationParameter.h"
#include "LabelMappings.h"
#include "FaultDefinition.h"
#include "IconResource.h"
#include "ImageResource.h"

class FunctionDefinition : public ProjectObject, public IRunnable
{
public:
    explicit FunctionDefinition(ProjectObject* parent = 0);
    ~FunctionDefinition(void);

    QString GetName() const;
    void SetName(const QString& name);

    void AddTask(Task* task);
    void AddGCLSignalSink(GCLSignalSink* sink);
    void AddGCLSignalSource(GCLSignalSource* source);
    void AddConfigurationParameter(ConfigurationParameter* configurationParameter);
    void AddCalibrationParameter(CalibrationParameter* calibrationParameter);
    void AddLabelMappings(LabelMappings* labelMappings);
    void AddFaultDefinition(QString name, QString description);
    void AddIconResource(QString name);
    void AddImageResource(QString name);

    void ClearTasks(void);
    void ClearSinks(void);
    void ClearSources(void);
    void ClearConfigurationParameters(void);
    void ClearCalibrationParameters(void);
    void ClearLabelMappings(void);
    void ClearFaultDefinitions(void);
    void ClearIconResources(void);
    void ClearImageResources(void);

    const QList<GCLSignalSource*>& Sources() const;
    const QList<GCLSignalSink*>& Sinks() const;
    const QList<const ConfigurationParameter*>& ConfigurationParameters() const;
    const QList<const CalibrationParameter*>& CalibrationParameters() const;
    const QList<LabelMappings*>& LabelMappingsParameters() const;
    const QList<FaultDefinition> &FaultDefinitions() const;
    const QList<IconResource>& IconResources() const;
    const QList<ImageResource>& ImageResources() const;

private:
    QString m_name;
    QList<const Task*> m_tasks;
    QList<GCLSignalSink*> m_gclSignalSinks;
    QList<GCLSignalSource*> m_gclSignalSources;
    QList<const ConfigurationParameter*> m_configurationParameters;
    QList<const CalibrationParameter*> m_calibrationParameters;
    QList<LabelMappings*> m_labelMappingsParameters;
    QList<FaultDefinition> m_faultDefinitions;
    QList<IconResource> m_iconResources;
    QList<ImageResource> m_imageResources;

    // IRunnable interface
public:
    const QList<const Task*>& Tasks() const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // FUNCTIONDEFINITION
