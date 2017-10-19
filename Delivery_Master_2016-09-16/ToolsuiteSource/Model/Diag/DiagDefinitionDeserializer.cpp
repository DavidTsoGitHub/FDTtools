#include "DiagDefinitionDeserializer.h"
#include "Logger.h"

DiagDefinitionDeserializer::DiagDefinitionDeserializer(DiagDefinition& diagDefinition) : DeserializerBase("UTF-16"),
    m_diagDefinition(diagDefinition)
{

}

DiagDefinitionDeserializer::~DiagDefinitionDeserializer()
{

}

DeserializerBase::DeserializerRespons DiagDefinitionDeserializer::Deserialize(const QString &xml)
{
    LOG_DEBUG("Starting diag deserializer");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "Diagnostics")
    {
        m_error = "Diagnostics root element not found in XML!";
        return ErrorTagNotFound;
    }

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "DTCDefinition")
            {
                xmlReader.readNext();
                while (!isEndOfElement(xmlReader, "DTCDefinition") && !xmlReader.atEnd())
                {
                    if (xmlReader.name() == "DTC" && !xmlReader.isEndElement())
                    {
                        quint32 dtc = xmlReader.attributes().value("DTC_ID").toUInt();
                        quint32 labelId = xmlReader.attributes().value("Label_ID").toUInt();

                        m_diagDefinition.AddDTCLabelMapping(dtc, labelId);
                    }
                    xmlReader.readNext();
                }
            }
            else if ( xmlReader.name() == "DIDCALMappings")
            {
                xmlReader.readNext();
                while (!isEndOfElement(xmlReader, "DIDCALMappings") && !xmlReader.atEnd())
                {
                    if (xmlReader.name() == "DIDCALMap" && !xmlReader.isEndElement())
                    {
                        quint16 did = xmlReader.attributes().value("DID").toUInt();
                        QString parameterName = xmlReader.attributes().value("CALIBRATIONPARAMETER").toString();
                        m_diagDefinition.AddDIDToCALMapping(did, parameterName);
                    }
                    xmlReader.readNext();
                }
            }
        }
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Diag Definition XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Diag Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Diag Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
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
