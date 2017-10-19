#include "SignalMappingDeserializer.h"

#include "Logger.h"
#include "Project.h"
#include "SignalMapping.h"

#include <QList>
#include <QXmlStreamReader>

SignalMappingDeserializer::SignalMappingDeserializer(const QString& mappingXml) :
    m_mappingXml(mappingXml)
{}

bool SignalMappingDeserializer::Deserialize(QList<SignalMapping*>& signalMappings)
{
    LOG_DEBUG("Entering");
    if (m_mappingXml.isNull() || m_mappingXml.isEmpty())
    {
        m_error = "XML string is empty!";
        return false;
    }

    QXmlStreamReader xml(m_mappingXml);

    if (!getNextElement(xml, "SignalMappings"))
    {
        m_error = "No SignalMappings element found in XML!";
        return false;
    }

    while (getNextElement(xml, "SignalMapping"))
    {
        if (!parseSignalMapping(xml, signalMappings))
        {
            return false;
        }
    }

    if (xml.hasError())
    {
        switch (xml.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "SignalMappings XML not well formed! Line:" +  QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of SignalMappings XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in SignalMappings XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xml.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return false;
    }

    return true;
}

bool SignalMappingDeserializer::parseSignalMapping(QXmlStreamReader& xml, QList<SignalMapping*>& signalMappings)
{
    if (xml.name() == "SignalMapping")
    {
        SignalMapping* signalMapping = new SignalMapping();

        while (!(xml.name() == "SignalMapping" && xml.isEndElement()))
        {
            xml.readNext();
            if (xml.isStartElement() && xml.name() == "Source")
            {
                QString sourceComponentName = xml.attributes().value("ComponentName").toString();
                QString sourceSignalName = xml.attributes().value("Port").toString();
                signalMapping->AddSource(sourceComponentName, sourceSignalName);
            }
            else if (xml.isStartElement() && xml.name() == "Sink")
            {
                QString sinkComponentName = xml.attributes().value("ComponentName").toString();
                QString sinkSignalName = xml.attributes().value("Port").toString();
                signalMapping->AddSink(sinkComponentName, sinkSignalName);
            }
        }

        if (signalMapping->GetSource().Name.isEmpty() || signalMapping->GetSource().Signal.isEmpty())
        {
            m_error = "Source component name or signal name was empty!";
            return false;
        }

        if (signalMapping->GetSinks().count() == 0)
        {
            m_error = "No sinks defined for source: " + signalMapping->GetSource().Name + " " + signalMapping->GetSource().Signal;
            return false;
        }

        signalMappings.append(signalMapping);
    }

    return true;
}

const QString SignalMappingDeserializer::Error() const
{
    return m_error;
}

bool SignalMappingDeserializer::getNextElement(QXmlStreamReader& sr, QString elementName)
{
    while (!sr.atEnd())
    {
        sr.readNext();
        if (sr.isStartElement() && sr.name() == elementName)
        {
            return true;
        }
    }

    return false;
}
