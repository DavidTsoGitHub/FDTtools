#include "LabelDefinitionSerializer.h"
#include "Font.h"
#include "Label.h"
#include "Translation.h"
#include "Logger.h"
#include "LabelDefinition.h"

#include <QXmlStreamWriter>

LabelDefinitionSerializer::LabelDefinitionSerializer(LabelDefinition& labelDefinition) : SerializerBase("UTF-16"),
    m_labelDefinition(labelDefinition)
{}

LabelDefinitionSerializer::~LabelDefinitionSerializer()
{}

QString LabelDefinitionSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("LabelDefinition");

    xmlWriter.writeStartElement("Fonts");
    foreach (const Font* font, m_labelDefinition.Fonts())
    {
        xmlWriter.writeStartElement("Font");
        xmlWriter.writeAttribute("ID", QString::number(font->ID));
        xmlWriter.writeAttribute("BDFPath", font->BDFPath);
        xmlWriter.writeAttribute("Description", font->Description);
        xmlWriter.writeAttribute("LineSpacing", QString::number(font->LineSpacing));
        xmlWriter.writeAttribute("StoreInExternalFlash", font->StoreInExternalFlash ? "true" : "false");
        if (isDefaultFont(m_labelDefinition.GetDefaultFont(), font))
        {
            xmlWriter.writeAttribute("Default", "True");
        }
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // Fonts

    xmlWriter.writeStartElement("Languages");
    foreach (QString language, m_labelDefinition.Languages())
    {
        xmlWriter.writeStartElement("Language");
        xmlWriter.writeAttribute("ID", language);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // Languages

    xmlWriter.writeStartElement("MandatoryCharacters");
    xmlWriter.writeAttribute("String", m_labelDefinition.GetMandatoryCharacters());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("Labels");
    xmlWriter.writeAttribute("HighestID", QString::number(m_labelDefinition.GetHighestLabelID()));

    foreach (Label* label, m_labelDefinition.Labels())
    {
        xmlWriter.writeStartElement("Label");
        xmlWriter.writeAttribute("ID", QString::number(label->GetID()));
        xmlWriter.writeAttribute("MasterText", label->GetMasterText());
        xmlWriter.writeAttribute("Description", label->GetDescription());

        foreach(Translation* translation, label->Translations())
        {
            xmlWriter.writeStartElement("Translation");
            xmlWriter.writeAttribute("Language", translation->GetLanguage());
            xmlWriter.writeAttribute("Text", translation->GetText());
            xmlWriter.writeAttribute("FontID", QString::number(translation->GetFontID()));
            xmlWriter.writeEndElement();//Translation
        }
        xmlWriter.writeEndElement(); // Label
    }
    xmlWriter.writeEndElement(); //Labels
    xmlWriter.writeEndElement(); //LabelDefinition

    xmlString.append("\n");
    return xmlString;
}

bool LabelDefinitionSerializer::isDefaultFont(const Font* defaultFont, const Font* currentFont)
{
    return defaultFont->ID == currentFont->ID;
}
