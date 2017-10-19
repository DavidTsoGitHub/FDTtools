#include "LocalProjectSettingsSerializer.h"

#include "Logger.h"
#include "Project.h"

#include <QXmlStreamReader>

LocalProjectSettingsSerializer::LocalProjectSettingsSerializer(Project& project) :
    SerializerBase(NULL),
    m_project(project)
{}

LocalProjectSettingsSerializer::~LocalProjectSettingsSerializer()
{}

QString LocalProjectSettingsSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("LocalSettings");

    xmlWriter.writeStartElement("Simulator");
    xmlWriter.writeAttribute("Compiler", m_project.GetTargetSettings().GetSimulatorCompilerName());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("TargetHardware");
    xmlWriter.writeAttribute("Compiler", m_project.GetTargetSettings().GetTargetCompilerName());
    xmlWriter.writeEndElement();

    QString activeBuildTarget = "";
    switch (m_project.GetTargetSettings().GetActiveTarget())
    {
        case TargetSettings::Simulator:
            activeBuildTarget = "Simulator";
            break;
        case TargetSettings::TargetHW:
            activeBuildTarget = "TargetHW";
            break;
        default:
            activeBuildTarget = "Simulator";
            break;
    }

    xmlWriter.writeStartElement("ActiveBuildTarget");
    xmlWriter.writeAttribute("Target", activeBuildTarget);
    xmlWriter.writeEndElement();

    xmlWriter.writeEndDocument();
    return xmlString;
}

