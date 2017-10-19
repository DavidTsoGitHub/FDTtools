#include "DiagDefinitionSerializer.h"
#include "Logger.h"

#include <QMap>
#include <QXmlStreamWriter>

DiagDefinitionSerializer::DiagDefinitionSerializer(DiagDefinition& diagDefinition) : SerializerBase("UTF-16"),
    m_diagDefinition(diagDefinition)
{
}

DiagDefinitionSerializer::~DiagDefinitionSerializer()
{
}

QString DiagDefinitionSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Diagnostics");

    xmlWriter.writeStartElement("DTCDefinition");

    foreach (const DTCLabelMapping* dtc, m_diagDefinition.GetDTCLabelMappings())
    {
        xmlWriter.writeStartElement("DTC");
        xmlWriter.writeAttribute("DTC_ID", QString::number(dtc->GetDTC()));
        xmlWriter.writeAttribute("Label_ID", QString::number(dtc->GetLabelID()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); //DTCDefinition

    xmlWriter.writeStartElement("DIDCALMappings");

    foreach (const DIDToCALMapping* mapping, m_diagDefinition.GetDIDToCALMappings())
    {
        xmlWriter.writeStartElement("DIDCALMap");
        xmlWriter.writeAttribute("DID", QString::number(mapping->GetDID()));
        xmlWriter.writeAttribute("CALIBRATIONPARAMETER", mapping->GetCALParameter());
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement(); //DIDCALMappings
    xmlWriter.writeEndElement(); //Diagnostics

    xmlString.append("\n");
    return xmlString;
}
