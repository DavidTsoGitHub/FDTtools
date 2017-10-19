#include "LayoutDefinitionSerializer.h"

#include "Layout.h"
#include "LayoutDefinition.h"
#include "Logger.h"
#include "Project.h"
#include "TransitionCondition.h"
#include "UIWidgetSerializer.h"
#include "ColorPair.h"

#include <QString>
#include <QXmlStreamWriter>

LayoutDefinitionSerializer::LayoutDefinitionSerializer(LayoutDefinition& layoutDefinition) :
    SerializerBase(NULL),
    m_layoutDefinition(layoutDefinition)
{

}

LayoutDefinitionSerializer::~LayoutDefinitionSerializer()
{

}

QString LayoutDefinitionSerializer::Serialize()
{
    LOG_DEBUG("Entering");

    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.writeStartDocument();
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartElement("Layouts");
    xmlWriter.writeAttribute("HighestID", QString::number(m_layoutDefinition.GetHighestID()));
    xmlWriter.writeAttribute("CropLeft", QString::number(m_layoutDefinition.CropLeft()));
    xmlWriter.writeAttribute("CropRight", QString::number(m_layoutDefinition.CropRight()));
    xmlWriter.writeAttribute("CropTop", QString::number(m_layoutDefinition.CropTop()));
    xmlWriter.writeAttribute("CropBottom", QString::number(m_layoutDefinition.CropBottom()));
    Layout* mainLayout = m_layoutDefinition.GetMainLayout();
    quint32 mainLayoutID = 0;
    if (mainLayout != NULL)
    {
        mainLayoutID = mainLayout->GetID();
    }
    xmlWriter.writeAttribute("MainLayout", QString::number(mainLayoutID));
    xmlWriter.writeAttribute("StartupScreenEnabled", m_layoutDefinition.StartupEnabled() ? "true" : "false");
    xmlWriter.writeAttribute("StartupScreenDisplayTime", QString::number(m_layoutDefinition.StartupScreenDisplayTime()));
    xmlWriter.writeAttribute("ChromaStartRed", QString::number(m_layoutDefinition.GetStartChromaKeyColor().red()));
    xmlWriter.writeAttribute("ChromaEndRed", QString::number(m_layoutDefinition.GetEndChromaKeyColor().red()));
    xmlWriter.writeAttribute("ChromaStartGreen", QString::number(m_layoutDefinition.GetStartChromaKeyColor().green()));
    xmlWriter.writeAttribute("ChromaEndGreen", QString::number(m_layoutDefinition.GetEndChromaKeyColor().green()));
    xmlWriter.writeAttribute("ChromaStartBlue", QString::number(m_layoutDefinition.GetStartChromaKeyColor().blue()));
    xmlWriter.writeAttribute("ChromaEndBlue", QString::number(m_layoutDefinition.GetEndChromaKeyColor().blue()));

    foreach (Layout* layout, m_layoutDefinition.Layouts())
    {
        xmlWriter.writeStartElement("Layout");
        xmlWriter.writeAttribute("ID", QString::number(layout->GetID()));
        xmlWriter.writeAttribute("Name", layout->Name());
        xmlWriter.writeAttribute("RGBRed", QString::number(layout->GetBackgroundColor().red()));
        xmlWriter.writeAttribute("RGBGreen", QString::number(layout->GetBackgroundColor().green()));
        xmlWriter.writeAttribute("RGBBlue", QString::number(layout->GetBackgroundColor().blue()));

        if (layout->IsStartupScreen())
        {
            xmlWriter.writeAttribute("IsStartupScreen", "true");

        }
        else
        {
            xmlWriter.writeAttribute("IsStartupScreen", "false");
        }

        if (layout->IsCheckMessageScreen())
        {
            xmlWriter.writeAttribute("IsCheckMessageScreen", "true");
        }
        else
        {
            xmlWriter.writeAttribute("IsCheckMessageScreen", "false");
        }

        if (layout->IsCheckMessagesEnabled())
        {
            xmlWriter.writeAttribute("IsCheckMessagesEnabled", "true");
        }
        else
        {
            xmlWriter.writeAttribute("IsCheckMessagesEnabled", "false");
        }

        xmlWriter.writeStartElement("ButtonTransitions");
        QMap<Layout::LayoutButton, quint8> buttonMap = layout->GetButtonTransitions();
        QMap<Layout::LayoutButton, quint8>::iterator i;
        for (i = buttonMap.begin(); i != buttonMap.end(); ++i)
        {
            xmlWriter.writeStartElement("ButtonTransition");
            xmlWriter.writeAttribute("Button", QString::number(i.key()));
            xmlWriter.writeAttribute("Layout", QString::number(i.value()));
            xmlWriter.writeEndElement(); // ButtonTransition
        }
        xmlWriter.writeEndElement(); // ButtonTransitions

        xmlWriter.writeStartElement("SignalTransitions");

        foreach(TransitionClause* transition, layout->GetSignalTransitions())
        {
            xmlWriter.writeStartElement("Transition");

            TransitionCondition* firstCondition = transition->GetFirstCondition();
            xmlWriter.writeStartElement("FirstCondition");
            xmlWriter.writeAttribute("Value", QString::number(firstCondition->GetConditionValue()));
            xmlWriter.writeAttribute("CompareCondition", QString::number(firstCondition->GetCompareOperator()));
            xmlWriter.writeEndElement(); // FirstCondition

            xmlWriter.writeStartElement("Connective");
            xmlWriter.writeAttribute("Connective", QString::number(transition->GetConnective()));
            xmlWriter.writeEndElement(); // Connective

            if (transition->GetConnective() != LayoutTransitionTypes::NONE)
            {
                TransitionCondition* secondCondition = transition->GetSecondCondition();
                xmlWriter.writeStartElement("SecondCondition");
                xmlWriter.writeAttribute("Value", QString::number(secondCondition->GetConditionValue()));
                xmlWriter.writeAttribute("CompareCondition", QString::number(secondCondition->GetCompareOperator()));
                xmlWriter.writeEndElement(); // SecondCondition
            }


            xmlWriter.writeEndElement(); // Transition
        }
        xmlWriter.writeEndElement(); // SignalTransitions

        foreach (DisplayArea* area, layout->DisplayAreas())
        {
            xmlWriter.writeStartElement("DisplayArea");
            xmlWriter.writeAttribute("Name", area->GetName());
            xmlWriter.writeAttribute("IsCheckMessageArea", (area->IsCheckMessageArea() ? "true" : "false"));
            xmlWriter.writeAttribute("X", QString::number(area->GetX()));
            xmlWriter.writeAttribute("Y", QString::number(area->GetY()));
            xmlWriter.writeAttribute("Width", QString::number(area->GetWidth()));
            xmlWriter.writeAttribute("Height", QString::number(area->GetHeight()));
            xmlWriter.writeAttribute("Border", QString::number(area->GetBorder()));
            xmlWriter.writeAttribute("ForegroundRed", QString::number(area->GetColorPair()->GetFGColor().red()));
            xmlWriter.writeAttribute("ForegroundGreen", QString::number(area->GetColorPair()->GetFGColor().green()));
            xmlWriter.writeAttribute("ForegroundBlue", QString::number(area->GetColorPair()->GetFGColor().blue()));
            xmlWriter.writeAttribute("BackgroundIsTransparent", (area->GetColorPair()->GetBGIsTransparent() ? "true" : "false"));
            xmlWriter.writeAttribute("BackgroundRed", QString::number(area->GetColorPair()->GetBGColor().red()));
            xmlWriter.writeAttribute("BackgroundGreen", QString::number(area->GetColorPair()->GetBGColor().green()));
            xmlWriter.writeAttribute("BackgroundBlue", QString::number(area->GetColorPair()->GetBGColor().blue()));
            xmlWriter.writeStartElement("Widgets");
            if (layout->IsMenuArea(area))
            {
                xmlWriter.writeStartElement("Widget");
                xmlWriter.writeAttribute("Type", "Menu");
                xmlWriter.writeAttribute("ConfigurationFile", Project::MenuDefinitionPath);
                xmlWriter.writeEndElement(); // Display.DisplayAreas.DisplayArea.Widgets.Widget
            }
            foreach (UIWidgetBase* uiWidget, area->UIWidgets())
            {
                xmlWriter.writeStartElement("Widget");
                UIWidgetSerializer::Serialize(xmlWriter, uiWidget);
                xmlWriter.writeEndElement(); // Display.DisplayAreas.DisplayArea.Widgets.Widget
            }
            xmlWriter.writeEndElement(); // Display.DisplayAreas.DisplayArea.Widgets
            xmlWriter.writeEndElement(); // Display.DisplayAreas.DisplayArea
        }
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndDocument(); // Closes all open tags and adds newline
    return xmlString;
}
