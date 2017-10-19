#include "SegmentDisplayDeserializer.h"

#include "SegmentDisplay.h"

#include <QXmlStreamReader>
#include <QTextStream>

SegmentDisplayDeserializer::SegmentDisplayDeserializer(const QString& displayXml) :
    m_displayXml(displayXml)
{
}

SegmentDisplayDeserializer::~SegmentDisplayDeserializer()
{
}

bool SegmentDisplayDeserializer::Deserialize(SegmentDisplay& displayDefinition)
{
    if (m_displayXml.isEmpty())
    {
        m_error = "XML string is empty!";
        return false;
    }

    QXmlStreamReader xmlReader(m_displayXml);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (!xmlReader.isStartElement()) continue;
        else if (xmlReader.name() == "SegmentDisplay")
        {
            bool enabled = xmlReader.attributes().value("Enabled").toString() == "true";
            displayDefinition.SetEnabled(enabled);
        }
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Segment Display Definition XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Segment Display Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Segment Display Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return false;
    }

    return true;
}

const QString SegmentDisplayDeserializer::Error() const
{
    return m_error;
}
