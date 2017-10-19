#include "SignalMappingSerializer.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Logger.h"
#include "Project.h"

#include <QString>
#include <QXmlStreamWriter>

SignalMappingSerializer::SignalMappingSerializer(Project& project) : SerializerBase(NULL),
  m_project(project)
{}

SignalMappingSerializer::~SignalMappingSerializer()
{}

QString SignalMappingSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("SignalMappings");

    QList<GCLSignalSource*> allSources;
    m_project.GetAllGCLSources(allSources);

    foreach (GCLSignalSource* source, allSources)
    {
        if (source->GetConnectedSinks().count() == 0) continue;

        xmlWriter.writeStartElement("SignalMapping");
        xmlWriter.writeStartElement("Source");
        xmlWriter.writeAttribute("ComponentName", source->ComponentName());
        xmlWriter.writeAttribute("Port", source->SignalName());
        xmlWriter.writeEndElement(); // SignalMappings.SignalMapping.Source
        foreach(GCLSignalSink* sink, source->GetConnectedSinks())
        {
            xmlWriter.writeStartElement("Sink");
            xmlWriter.writeAttribute("ComponentName", sink->ComponentName());
            xmlWriter.writeAttribute("Port", sink->SignalName());
            xmlWriter.writeEndElement(); // SignalMappings.SignalMapping.Sink
        }
        xmlWriter.writeEndElement(); // SignalMappings.SignalMapping
    }

    xmlWriter.writeEndElement(); // SignalMappings

    xmlString.append("\n");
    return xmlString;
}
