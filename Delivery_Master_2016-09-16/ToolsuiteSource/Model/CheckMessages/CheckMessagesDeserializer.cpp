#include "CheckMessagesDeserializer.h"

#include "CheckMessage.h"
#include "Layout.h"
#include "Logger.h"

CheckMessagesDeserializer::CheckMessagesDeserializer(LayoutDefinition& layoutDefinition, CheckMessageDefinition& checkMessagesDefinition) : DeserializerBase(NULL),
    m_layoutDefinition(layoutDefinition),
    m_checkMessagesDefinition(checkMessagesDefinition)
{}

CheckMessagesDeserializer::~CheckMessagesDeserializer()
{}

DeserializerBase::DeserializerRespons CheckMessagesDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "CheckMessageDefinition")
    {
        m_error = "No CheckMessages root element found in XML!";
        return ErrorTagNotFound;
    }

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "CheckMessageDefinition")
            {
                while (!isEndOfElement(xmlReader, "CheckMessages"))
                {
                    if (xmlReader.name() == "Message" && !xmlReader.isEndElement())
                    {
                        CheckMessage* checkMessage = new CheckMessage();
                        checkMessage->SetName(xmlReader.attributes().value("Name").toString());
                        checkMessage->SetPrio(xmlReader.attributes().value("Prio").toInt());
                        checkMessage->SetReoccurring((xmlReader.attributes().value("Reoccurring").toString() == "true" ? true : false));
                        QString clearCond = xmlReader.attributes().value("ClearCondition").toString();
                        if(clearCond.compare("Trigger") == 0)
                        {
                            checkMessage->SetClearCondition(CheckMessage::ClearCondition_e::Trigger);
                        }
                        else if (clearCond.compare("Timeout") == 0)
                        {
                            checkMessage->SetClearCondition(CheckMessage::ClearCondition_e::Timeout);
                        }
                        else
                        {
                            checkMessage->SetClearCondition(CheckMessage::ClearCondition_e::Button);
                        }
                        checkMessage->SetTimeout(xmlReader.attributes().value("Timeout").toInt());
                        QString trigger = xmlReader.attributes().value("GCLTriggerSink").toString();
                        GCLSignalSink* gclSignalSink = new GCLSignalSink(trigger, "CheckMessageManager", "uint8");
                        checkMessage->SetTrigger(gclSignalSink);

                        while (!isEndOfElement(xmlReader, "Message"))
                        {
                            if (xmlReader.name() == "Items" && !xmlReader.isEndElement())
                            {
                                while (!isEndOfElement(xmlReader, "Items"))
                                {

                                    if(xmlReader.name() == "Item" && !xmlReader.isEndElement())
                                    {
                                        CheckMessageItem* checkMessageItem = new CheckMessageItem();
                                        checkMessageItem->Index = xmlReader.attributes().value("Index").toInt();
                                        checkMessageItem->Type = xmlReader.attributes().value("Type").toString();
                                        checkMessageItem->ResourceID = xmlReader.attributes().value("ResourceID").toInt();
                                        checkMessageItem->AssociatedWidgetID = xmlReader.attributes().value("AssociatedWidgetID").toInt();
                                        checkMessage->AddItem(checkMessageItem);
                                    }
                                    xmlReader.readNext();
                                }
                            }
                            else if(xmlReader.name() == "AllowedIn" && !xmlReader.isEndElement())
                            {
                                while (!isEndOfElement(xmlReader, "AllowedIn"))
                                {

                                    if(xmlReader.name() == "Layout" && !xmlReader.isEndElement())
                                    {
                                        quint32 layoutId = xmlReader.attributes().value("ID").toUInt();
                                        checkMessage->SetAllowedInLayout(layoutId, true);
                                        foreach (Layout* layout, m_layoutDefinition.Layouts())
                                        {
                                            if (layout->GetID() == layoutId)
                                            {
                                                layout->AddCheckMessage(checkMessage);
                                            }
                                        }
                                    }
                                    xmlReader.readNext();
                                }
                            }
                            xmlReader.readNext();
                        }
                        m_checkMessagesDefinition.AddCheckMessage(checkMessage);
                    } 
                    xmlReader.readNext();
                }
            }
        }

        xmlReader.readNext();
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Check Message XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Check Message XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Check Message XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    return DeserializationOK;
}

