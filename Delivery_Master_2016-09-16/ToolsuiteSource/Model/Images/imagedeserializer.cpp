#include "ImageDeserializer.h"
#include "Image.h"
#include "Logger.h"
#include "Project.h"

#include <QImage>

ImageDeserializer::ImageDeserializer(ImageDefinition &imageDefinition) : DeserializerBase(NULL),
    m_imageDefinition(imageDefinition)
{}

ImageDeserializer::~ImageDeserializer()
{}

DeserializerBase::DeserializerRespons ImageDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    m_missingImageFilesList.clear();

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "ImageDefinition")
    {
        m_error = "No ImageDefinition root element found in XML!";
        return ErrorTagNotFound;
    }

    Project* project = static_cast<Project*>(m_imageDefinition.GetParent());

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Images")
            {
                while (!isEndOfElement(xmlReader, "Images"))
                {
                    if (xmlReader.name() == "Image" && !xmlReader.isEndElement())
                    {
                        Image* image = new Image();
                        image->SetID(xmlReader.attributes().value("ID").toInt());
                        image->SetName(xmlReader.attributes().value("Name").toString());
                        image->SetPath(xmlReader.attributes().value("Path").toString());
                        image->SetDescription(xmlReader.attributes().value("Description").toString());
                        image->SetStoreOnExternalFlash((xmlReader.attributes().value("StoreInExternalFlash").compare("true", Qt::CaseInsensitive) == 0) ? true : false);
                        image->SetUseForNonAnimations((xmlReader.attributes().value("UseForNonAnimations").compare("false", Qt::CaseInsensitive) == 0) ? false : true);
                        image->SetUseForAnimations((xmlReader.attributes().value("UseForAnimations").compare("true", Qt::CaseInsensitive) == 0) ? true : false);

                        QImage qimage;
                        qimage.load(project->GetProjectRoot().absoluteFilePath(image->GetPath()));

                        image->SetWidth(qimage.width());
                        image->SetHeight(qimage.height());

                        if (verifyFileExistence(project->GetProjectRoot().absoluteFilePath(image->GetPath())))
                        {
                            m_imageDefinition.AddImage(image);
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
                m_error = "Image XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Image XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Image XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    if (!m_missingImageFilesList.empty())
    {
        return ErrorPointsToMissingFile;
    }

    return DeserializationOK;
}

const QList<QString>& ImageDeserializer::GetMissingImageFilesList() const
{
    return m_missingImageFilesList;
}

bool ImageDeserializer::verifyFileExistence(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        m_missingImageFilesList.append(filename);
        return false;
    }
    else
    {
        return true;
    }
}
