#include "IconDeserializer.h"
#include "Icon.h"
#include "Logger.h"
#include "Project.h"

IconDeserializer::IconDeserializer(IconDefinition& iconDefinition) : DeserializerBase(NULL),
    m_iconDefinition(iconDefinition)
{}

IconDeserializer::~IconDeserializer()
{}

DeserializerBase::DeserializerRespons IconDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    m_missingIconFilesList.clear();

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "IconDefinition")
    {
        m_error = "No IconDefinition root element found in XML!";
        return ErrorTagNotFound;
    }

    Project* project = static_cast<Project*>(m_iconDefinition.GetParent());

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Icons")
            {
                while (!isEndOfElement(xmlReader, "Icons"))
                {
                    if (xmlReader.name() == "Icon" && !xmlReader.isEndElement())
                    {
                        Icon* icon = new Icon();
                        icon->SetID(xmlReader.attributes().value("ID").toInt());
                        icon->SetName(xmlReader.attributes().value("Name").toString());
                        icon->SetPath(xmlReader.attributes().value("Path").toString());
                        icon->SetDescription(xmlReader.attributes().value("Description").toString());

                        if (verifyFileExistence(project->GetProjectRoot().absoluteFilePath(icon->GetPath())))
                        {
                            m_iconDefinition.AddIcon(icon);
                        }
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
                m_error = "Icon XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Icon XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Icon XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    if (!m_missingIconFilesList.empty())
    {
        return ErrorPointsToMissingFile;
    }

    return DeserializationOK;
}

const QList<QString>& IconDeserializer::GetMissingImageFilesList() const
{
    return m_missingIconFilesList;
}

bool IconDeserializer::verifyFileExistence(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        m_missingIconFilesList.append(filename);
        return false;
    }
    else
    {
        return true;
    }
}
