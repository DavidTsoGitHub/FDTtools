#include "LabelDefinitionDeserializer.h"
#include "Font.h"
#include "Label.h"
#include "Translation.h"
#include "Logger.h"

#include <QXmlStreamReader>

LabelDefinitionDeserializer::LabelDefinitionDeserializer(LabelDefinition& labelDefinition) : DeserializerBase("UTF-16"),
    m_labelDefinition(labelDefinition)
{}

LabelDefinitionDeserializer::~LabelDefinitionDeserializer()
{}

DeserializerBase::DeserializerRespons LabelDefinitionDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "LabelDefinition")
    {
        m_error = "No LabelDefinition root element found in XML!";
        return ErrorTagNotFound;
    }

    m_labelDefinition.blockSignals(true);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Fonts")
            {
                xmlReader.readNext();
                while (!isEndOfElement(xmlReader, "Fonts"))
                {
                    if (xmlReader.name() == "Font" && !xmlReader.isEndElement())
                    {
                        Font* font = new Font();
                        font->ID = xmlReader.attributes().value("ID").toUInt();
                        font->BDFPath = xmlReader.attributes().value("BDFPath").toString();
                        font->Description = xmlReader.attributes().value("Description").toString();
                        font->LineSpacing = xmlReader.attributes().value("LineSpacing").toUInt();
                        font->StoreInExternalFlash = (xmlReader.attributes().value("StoreInExternalFlash").compare("true") == 0 ? true : false);

                        m_labelDefinition.AddFont(font);

                        if(xmlReader.attributes().hasAttribute("Default"))
                        {
                            m_labelDefinition.SetDefaultFont(font);
                        }
                    }
                    xmlReader.readNext();
                }
            }
            else if (xmlReader.name() == "Languages")
            {
                while (!isEndOfElement(xmlReader, "Languages"))
                {
                    if (xmlReader.name() == "Language" && !xmlReader.isEndElement())
                    {
                        m_labelDefinition.AddLanguage(xmlReader.attributes().value("ID").toString());
                    }

                    xmlReader.readNext();
                }
            }
            else if (xmlReader.name() == "MandatoryCharacters" && !xmlReader.isEndElement())
            {
                m_labelDefinition.SetMandatoryCharacters(xmlReader.attributes().value("String").toString());
            }
            else if (xmlReader.name() == "Labels")
            {
                m_labelDefinition.SetHighestLabelID(xmlReader.attributes().value("HighestID").toUInt());
                while (!isEndOfElement(xmlReader, "Labels"))
                {
                    if (xmlReader.name() == "Label")
                    {
                        Label* label = new Label();
                        label->SetID(xmlReader.attributes().value("ID").toUInt());
                        label->SetMasterText(xmlReader.attributes().value("MasterText").toString());
                        label->SetDescription(xmlReader.attributes().value("Description").toString());

                        while (!isEndOfElement(xmlReader, "Label"))
                        {
                            if (xmlReader.name() == "Translation" && !xmlReader.isEndElement())
                            {
                                Translation* translation = new Translation();
                                translation->SetLanguage(xmlReader.attributes().value("Language").toString());
                                translation->SetText(xmlReader.attributes().value("Text").toString());

                                quint32 fontID = xmlReader.attributes().value("FontID").toInt();
                                const Font* font = m_labelDefinition.GetFontByID(fontID);
                                if (font == NULL)
                                {
                                   m_error = QString("FontID: %1 was nog found in translation (%2) for label ID: %3").arg(fontID).arg(translation->GetLanguage()).arg(label->GetID());
                                   delete translation;
                                   delete label;
                                   return ErrorGeneral;
                                }
                                translation->SetFontID(fontID);

                                label->AddTranslation(translation);
                            }

                            xmlReader.readNext();
                        }
                        m_labelDefinition.AddLabel(label);
                    }

                    xmlReader.readNext();
                }

                xmlReader.readNext();
            }
        }
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Label Definition XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Label Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Label Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    m_labelDefinition.CalculateFontMemoryUsage();
    m_labelDefinition.blockSignals(false);

    return DeserializationOK;
}

bool LabelDefinitionDeserializer::isEndOfElement(QXmlStreamReader& xmlReader, QString elementName)
{
    return (xmlReader.isEndElement() && xmlReader.name() == elementName);
}
