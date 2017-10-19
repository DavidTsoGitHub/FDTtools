#include "SegmentDisplaySerializer.h"

#include "Logger.h"
#include "SegmentDisplay.h"

#include <QString>
#include <QXmlStreamWriter>

SegmentDisplaySerializer::SegmentDisplaySerializer(SegmentDisplay& displayDefinition) :
    SerializerBase(NULL),
    m_displayDefinition(displayDefinition)
{
}

SegmentDisplaySerializer::~SegmentDisplaySerializer()
{

}

QString SegmentDisplaySerializer::Serialize()
{
    LOG_DEBUG("Entering");

    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.writeStartDocument();
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartElement("SegmentDisplay");
    xmlWriter.writeAttribute("Enabled", (m_displayDefinition.GetEnabled() ? "true" : "false"));
    xmlWriter.writeEndDocument(); // Closes all open tags and adds newline
    return xmlString;
}
