#include "IconSerializer.h"
#include "Icon.h"
#include "Logger.h"

#include <QXmlStreamWriter>

IconSerializer::IconSerializer(IconDefinition& iconDefinition) : SerializerBase(NULL),
    m_iconDefinition(iconDefinition)
{}

IconSerializer::~IconSerializer()
{}

QString IconSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("IconDefinition");

    xmlWriter.writeStartElement("Icons");
    foreach (const Icon* icon, m_iconDefinition.Icons())
    {
        xmlWriter.writeStartElement("Icon");
        xmlWriter.writeAttribute("ID", QString::number(icon->GetID()));
        xmlWriter.writeAttribute("Name", icon->GetName());
        xmlWriter.writeAttribute("Path", icon->GetPath());
        xmlWriter.writeAttribute("Description", icon->GetDescription());

        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // Icons

    xmlWriter.writeEndElement(); //IconDefinition

    xmlString.append("\n");
    return xmlString;
}
