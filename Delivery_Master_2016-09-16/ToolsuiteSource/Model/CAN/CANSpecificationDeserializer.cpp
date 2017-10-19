#include "CANSpecificationDeserializer.h"

#include "CANMessage.h"
#include "Logger.h"

CANSpecificationDeserializer::CANSpecificationDeserializer(CANSpecification& canSpecification) : DeserializerBase(NULL),
    m_canSpecification(canSpecification)
{
}

CANSpecificationDeserializer::~CANSpecificationDeserializer()
{

}

DeserializerBase::DeserializerRespons CANSpecificationDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "CAN")
    {
        m_error = "No CAN root element found in XML!";
        return ErrorTagNotFound;
    }

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Messages")
            {
                xmlReader.readNext();
                while (!(xmlReader.isEndElement() && xmlReader.name() == "Messages"))
                {
                    if (xmlReader.name() == "Message")
                    {
                        CANMessage* message = parseMessage(xmlReader);
                        if (message->CANID > 0)
                        {
                            m_canSpecification.AddMessage(message);
                        }
                    }
                    xmlReader.readNext();
                }
            }
            if (xmlReader.name() == "Modules")
            {
                xmlReader.readNext();
                while (!(xmlReader.isEndElement() && xmlReader.name() == "Modules"))
                {
                    if (xmlReader.isStartElement() && xmlReader.name() == "Module")
                    {
                        m_canSpecification.Modules.append(xmlReader.readElementText());
                    }
                    xmlReader.readNext();
                }
            }
        }
    }

    if(xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "CAN XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of CAN XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in CAN XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
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

CANMessage* CANSpecificationDeserializer::parseMessage(QXmlStreamReader& xml)
{
    CANMessage* message = new CANMessage();

    QXmlStreamAttributes attributes = xml.attributes();
    if (attributes.hasAttribute("ID"))
    {
        message->CANID = attributes.value("ID").toInt();
    }
    else
    {
        message->CANID = -1;
    }

    while (!xml.atEnd() && xml.readNextStartElement())
    {
        if (xml.name() == "Name")
        {
            message->Name = xml.readElementText();
        }
        else if (xml.name() == "Producer")
        {
            message->Producer = xml.readElementText();
        }
        else if (xml.name() == "DLC")
        {
            message->DLC = xml.readElementText().toInt();
        }
        else if (xml.name() == "Timing")
        {
            QXmlStreamAttributes attributes = xml.attributes();
            if (attributes.hasAttribute("Type"))
            {
                message->CANMessageSendType = CANMessage::SendTypeFromString(attributes.value("Type").toString());
            }
            else
            {
                message->CANMessageSendType = CANMessage::Undefined;
            }

            while (!xml.atEnd() && xml.readNextStartElement())
            {
                if (xml.name() == "Cycle")
                {
                    message->CycleTime = xml.readElementText().toInt();
                }
                else if (xml.name() == "Delay")
                {
                    message->Delay = xml.readElementText().toInt();
                }
                else if (xml.name() == "InitialDelay")
                {
                    message->InitialDelay = xml.readElementText().toInt();
                }
                else if (xml.name() == "Timeout")
                {
                    message->Timeout = xml.readElementText().toInt();
                }
            }
        }
        else if (xml.name() == "Signals")
        {
            xml.readNext();
            while (!(xml.name() == "Signals" && xml.isEndElement()))
            {
                if (xml.name() == "Signal")
                {
                    CANSignal* signal = parseSignal(xml);
                    if (!signal->Name.isEmpty())
                    {
                        message->AddSignal(signal);

                    }
                }

                xml.readNext();
            }
        }
    }

    return message;
}

CANSignal* CANSpecificationDeserializer::parseSignal(QXmlStreamReader& xml)
{
    CANSignal* signal = new CANSignal();

    signal->Name = xml.attributes().value("Name").toString();
    signal->TimeoutNotification = xml.attributes().value("TimeoutNotification").compare("yes", Qt::CaseInsensitive) == 0;
    signal->IsEventTrigger = xml.attributes().value("IsEventTrigger").compare("True", Qt::CaseInsensitive) == 0;

    while (!xml.atEnd() && xml.readNextStartElement())
    {
        if (xml.name() == "Position")
        {
            while (!(xml.name() == "Position" && xml.isEndElement()))
            {
                if  (xml.name() == "StartBit")
                {
                    signal->StartBit = xml.readElementText().toInt();
                }
                else if (xml.name() == "Length")
                {
                    signal->BitLength = xml.readElementText().toInt();
                }
                xml.readNext();
            }
        }
        else if (xml.name() == "Value")
        {
            while (!(xml.name() == "Value" && xml.isEndElement()))
            {
                if (xml.name() == "Offset")
                {
                    signal->Offset = xml.readElementText().toDouble();
                }
                else if (xml.name() == "Min")
                {
                    signal->Min = xml.readElementText().toDouble();
                }
                else if (xml.name() == "Max")
                {
                    signal->Max = xml.readElementText().toDouble();
                }
                else if (xml.name() == "Resolution")
                {
                    signal->Scaling = xml.readElementText().toDouble();
                }
                else if (xml.name() == "UOM")
                {
                    signal->UOM = xml.readElementText();
                }

                xml.readNext();
            }
        }
        else if (xml.name() == "Consumers")
        {
            while (!(xml.name() == "Consumers" && xml.isEndElement()))
            {
                if (xml.name() == "Consumer" && xml.isStartElement())
                {
                    signal->Consumers.append(xml.readElementText());
                }
                xml.readNext();
            }
        }
    }
    return signal;
}
