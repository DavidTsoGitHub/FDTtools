#include "CheckMessagesSerializer.h"
#include "Logger.h"
#include <QXmlStreamWriter>

CheckMessagesSerializer::CheckMessagesSerializer(CheckMessageDefinition& checkMessagesDefinition) : SerializerBase(NULL),
    m_checkMessagesDefinition(checkMessagesDefinition)
{}

CheckMessagesSerializer::~CheckMessagesSerializer()
{}

QString CheckMessagesSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("CheckMessageDefinition");

    xmlWriter.writeStartElement("CheckMessages");
    foreach (CheckMessage* checkMessage, m_checkMessagesDefinition.CheckMessages())
    {
        xmlWriter.writeStartElement("Message");
        xmlWriter.writeAttribute("Name", checkMessage->GetName());
        xmlWriter.writeAttribute("Prio", QString::number(checkMessage->GetPrio()));
        xmlWriter.writeAttribute("GCLTriggerSink", checkMessage->GetTrigger()->SignalName());
        xmlWriter.writeAttribute("Reoccurring", QString(checkMessage->GetReoccurring() == true ? "true":"false"));
        xmlWriter.writeAttribute("ClearCondition", getConditionString(checkMessage->GetClearCondition()));
        xmlWriter.writeAttribute("Timeout", QString::number(checkMessage->GetTimeout()));

        //Items
        xmlWriter.writeStartElement("Items");
        foreach (CheckMessageItem* checkMessageItem, checkMessage->Items())
        {
            xmlWriter.writeStartElement("Item");
            xmlWriter.writeAttribute("Index", QString::number(checkMessageItem->Index));
            xmlWriter.writeAttribute("Type", checkMessageItem->Type);
            xmlWriter.writeAttribute("ResourceID", QString::number(checkMessageItem->ResourceID));
            xmlWriter.writeAttribute("AssociatedWidgetID", QString::number(checkMessageItem->AssociatedWidgetID));
            xmlWriter.writeEndElement(); //Item
        }
        xmlWriter.writeEndElement(); //Items

        xmlWriter.writeStartElement("AllowedIn");
        foreach (quint32 layout, checkMessage->GetAllowedLayouts())
        {
            xmlWriter.writeStartElement("Layout");
            xmlWriter.writeAttribute("ID", QString::number(layout));
            xmlWriter.writeEndElement(); //Item
        }
        xmlWriter.writeEndElement(); //Items

        xmlWriter.writeEndElement(); //Message
    }
    xmlWriter.writeEndElement(); //CheckMessages

    xmlWriter.writeEndElement(); //CheckMessageDefinition

    xmlString.append("\n");
    return xmlString;
}

QString CheckMessagesSerializer::getConditionString(CheckMessage::ClearCondition_e cond)
{
    if (cond == CheckMessage::ClearCondition_e::Trigger)
    {
        return "Trigger";
    }
    else if (cond == CheckMessage::ClearCondition_e::Timeout)
    {
        return "Timeout";
    }
    else
    {
        return "Button";
    }
}
