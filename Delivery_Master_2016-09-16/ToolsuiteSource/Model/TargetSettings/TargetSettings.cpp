#include "TargetSettings.h"

#include "ConfigFileWriter.h"
#include "LocalProjectSettingsSerializer.h"
#include "Project.h"
#include "ProjectObject.h"

#include <QString>
#include <QXmlStreamReader>

TargetSettings::TargetSettings() :
    m_targetEndianess(LittleEndian)
{}

TargetSettings::TargetSettings(const TargetSettings& other) :
    m_activeTarget(other.m_activeTarget),
    m_simulatorCompilerName(other.m_simulatorCompilerName),
    m_targetCompilerName(other.m_targetCompilerName),
    m_targetEndianess(other.m_targetEndianess)
{
}

void TargetSettings::SetActiveTarget(TargetSettings::Target target)
{
    m_activeTarget = target;
}

TargetSettings::Target TargetSettings::GetActiveTarget()
{
    return m_activeTarget;
}

void TargetSettings::SetSimulatorCompilerName(const QString& compiler)
{
    m_simulatorCompilerName = compiler;
}

const QString& TargetSettings::GetSimulatorCompilerName() const
{
    return m_simulatorCompilerName;
}

void TargetSettings::SetTargetCompilerName(const QString& compiler)
{
    m_targetCompilerName = compiler;
}

const QString& TargetSettings::GetTargetCompilerName() const
{
    return m_targetCompilerName;
}

void TargetSettings::SetTargetEndianess(TargetEndianess endianess)
{
    m_targetEndianess = endianess;
}

TargetSettings::TargetEndianess TargetSettings::GetTargetEndianess() const
{
    return m_targetEndianess;
}

void TargetSettings::SaveSettings(ProjectObject* project)
{
    Project* currentProject = static_cast<Project*>(project);
    QString localProjectSettingsFileName = currentProject->GetProjectFile().absoluteFilePath().append(".local");
    LocalProjectSettingsSerializer localProjectSettingsSerializer(*currentProject);

    QString error;
    ConfigFileWriter::WriteToFile(localProjectSettingsSerializer, localProjectSettingsFileName, error);
}
