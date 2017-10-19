#include "ImageSerializer.h"
#include "Image.h"
#include "Logger.h"

#include <QXmlStreamWriter>

ImageSerializer::ImageSerializer(ImageDefinition& imageDefinition) : SerializerBase(NULL),
    m_imageDefinition(imageDefinition)
{}

ImageSerializer::~ImageSerializer()
{}

QString ImageSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("ImageDefinition");

    xmlWriter.writeStartElement("Images");
    foreach (const Image* image, m_imageDefinition.Images())
    {
        xmlWriter.writeStartElement("Image");
        xmlWriter.writeAttribute("ID", QString::number(image->GetID()));
        xmlWriter.writeAttribute("Name", image->GetName());
        xmlWriter.writeAttribute("Path", image->GetPath());
        xmlWriter.writeAttribute("Description", image->GetDescription());
        xmlWriter.writeAttribute("StoreInExternalFlash", image->GetStoreOnExternalFlash() ? "true" : "false");
        xmlWriter.writeAttribute("UseForNonAnimations", image->GetUseForNonAnimations() ? "true" : "false");
        xmlWriter.writeAttribute("UseForAnimations", image->GetUseForAnimations() ? "true" : "false");
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // Images

    xmlWriter.writeEndElement(); //ImageDefinition

    xmlString.append("\n");
    return xmlString;
}
