#include "LayoutDefinitionDeserializer.h"

#include "Layout.h"
#include "LayoutDefinition.h"
#include "Logger.h"
#include "MenuDefinition.h"
#include "MenuDefinitionDeserializer.h"
#include "Project.h"
#include "TransitionCondition.h"
#include "UIWidgetDeserializer.h"
#include "UIWidgetsRepository.h"
#include "ColorPair.h"

#include <QXmlStreamReader>
#include <QTextStream>

LayoutDefinitionDeserializer::LayoutDefinitionDeserializer(LayoutDefinition& layoutDefinition, Project& project) : DeserializerBase("UTF-8"),
    m_layoutDefinition(layoutDefinition),
    m_project(project)
{
}

LayoutDefinitionDeserializer::~LayoutDefinitionDeserializer()
{
}

DeserializerBase::DeserializerRespons LayoutDefinitionDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Starting layout deserializer");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (!xmlReader.isStartElement()) continue;
        else if (xmlReader.name() == "Layouts")
        {
            m_layoutDefinition.SetHighestID(xmlReader.attributes().value("HighestID").toUInt());
            m_layoutDefinition.SetCropLeft(xmlReader.attributes().value("CropLeft").toInt());
            m_layoutDefinition.SetCropRight(xmlReader.attributes().value("CropRight").toInt());
            m_layoutDefinition.SetCropTop(xmlReader.attributes().value("CropTop").toInt());
            m_layoutDefinition.SetCropBottom(xmlReader.attributes().value("CropBottom").toInt());
            int mainLayoutID = (xmlReader.attributes().value("MainLayout").toInt());

            bool startupScreenEnabled = (xmlReader.attributes().value("StartupScreenEnabled").toString().compare("true", Qt::CaseInsensitive) == 0) ? true : false;
            m_layoutDefinition.SetStartupScreenDisplayTime(xmlReader.attributes().value("StartupScreenDisplayTime").toUShort());
            m_layoutDefinition.SetStartupEnabled(startupScreenEnabled);

            m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::RED, xmlReader.attributes().value("ChromaStartRed").toInt());
            m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::RED, xmlReader.attributes().value("ChromaEndRed").toInt());
            m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::GREEN, xmlReader.attributes().value("ChromaStartGreen").toInt());
            m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::GREEN, xmlReader.attributes().value("ChromaEndGreen").toInt());
            m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::BLUE, xmlReader.attributes().value("ChromaStartBlue").toInt());
            m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::BLUE, xmlReader.attributes().value("ChromaEndBlue").toInt());

            while (!xmlReader.atEnd() && !(xmlReader.name() == "Layouts" && xmlReader.isEndElement()))
            {
                xmlReader.readNext();

                if (!xmlReader.isStartElement()) continue;
                else if (xmlReader.name() == "Layout")
                {
                    Layout* layout = new Layout(xmlReader.attributes().value("Name").toString(), &m_layoutDefinition);
                    layout->SetID(xmlReader.attributes().value("ID").toUInt());
                    layout->SetBackgroundColorComponent(ColorComponent::RED, xmlReader.attributes().value("RGBRed").toInt());
                    layout->SetBackgroundColorComponent(ColorComponent::GREEN, xmlReader.attributes().value("RGBGreen").toInt());
                    layout->SetBackgroundColorComponent(ColorComponent::BLUE, xmlReader.attributes().value("RGBBlue").toInt());

                    if (xmlReader.attributes().value("IsStartupScreen").toString().compare("true", Qt::CaseInsensitive) == 0)
                    {
                        layout->SetStartupScreen(true);
                    }
                    else
                    {
                        layout->SetStartupScreen(false);
                    }

                    if (xmlReader.attributes().value("IsCheckMessageScreen").toString().compare("true", Qt::CaseInsensitive) == 0)
                    {
                        layout->SetCheckMessageScreen(true);
                    }
                    else
                    {
                        layout->SetCheckMessageScreen(false);
                    }

                    int transitionCounter = 0;
                    while (!xmlReader.atEnd() && !(xmlReader.name() == "Layout" && xmlReader.isEndElement()))
                    {
                        xmlReader.readNext();

                        if (!xmlReader.isStartElement()) continue;
                        else if (xmlReader.name() == "ButtonTransitions")
                        {
                            while (!xmlReader.atEnd() && !(xmlReader.name() == "ButtonTransitions" && xmlReader.isEndElement()))
                            {
                                xmlReader.readNext();
                                if (!xmlReader.isStartElement()) continue;

                                else if (xmlReader.name() == "ButtonTransition")
                                {
                                    Layout::LayoutButton button = static_cast<Layout::LayoutButton>(xmlReader.attributes().value("Button").toUInt());
                                    qint8 layoutID = xmlReader.attributes().value("Layout").toInt();
                                    layout->AddButtonTransition(button, layoutID);
                                }
                            }
                        }
                        else if (xmlReader.name() == "SignalTransitions")
                        {
                            while (!xmlReader.atEnd() && !(xmlReader.name() == "SignalTransitions" && xmlReader.isEndElement()))
                            {
                                xmlReader.readNext();
                                if (!xmlReader.isStartElement()) continue;

                                else if (xmlReader.name() == "Transition")
                                {
                                    TransitionClause* transition = new TransitionClause(transitionCounter++, layout);
                                    while (!xmlReader.atEnd() && !(xmlReader.name() == "Transition" && xmlReader.isEndElement()))
                                    {
                                        xmlReader.readNext();
                                        if (!xmlReader.isStartElement()) continue;
                                        else if (xmlReader.name() == "FirstCondition")
                                        {
                                            transition->GetFirstCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                            transition->GetFirstCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toUInt()));
                                        }
                                        else if (xmlReader.name() == "Connective")
                                        {
                                            transition->SetConnective(static_cast<LayoutTransitionTypes::TransitionConnective>(xmlReader.attributes().value("Connective").toInt()));
                                        }
                                        else if (xmlReader.name() == "SecondCondition")
                                        {
                                            transition->AddSecondCondition();
                                            transition->GetSecondCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                            transition->GetSecondCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toInt()));
                                        }
                                    }
                                    layout->AddSignalTransition(transition);
                                }
                            }
                        }

                        else if (xmlReader.name() == "DisplayArea")
                        {
                            DisplayArea* area = new DisplayArea();

                            area->SetName(xmlReader.attributes().value("Name").toString());
                            area->SetCheckMessageArea(xmlReader.attributes().value("IsCheckMessageArea").toString().compare("true", Qt::CaseInsensitive) == 0);
                            area->SetX(xmlReader.attributes().value("X").toInt());
                            area->SetY(xmlReader.attributes().value("Y").toInt());
                            area->SetHeight(xmlReader.attributes().value("Height").toInt());
                            area->SetWidth(xmlReader.attributes().value("Width").toInt());
                            area->SetBorder(xmlReader.attributes().value("Border").toInt());
                            area->GetColorPair()->SetFGColorComponent(ColorComponent::RED, xmlReader.attributes().value("ForegroundRed").toInt());
                            area->GetColorPair()->SetFGColorComponent(ColorComponent::GREEN, xmlReader.attributes().value("ForegroundGreen").toInt());
                            area->GetColorPair()->SetFGColorComponent(ColorComponent::BLUE, xmlReader.attributes().value("ForegroundBlue").toInt());
                            area->GetColorPair()->SetBGIsTransparent(xmlReader.attributes().value("BackgroundIsTransparent").toString().compare("true", Qt::CaseInsensitive) == 0);
                            area->GetColorPair()->SetBGColorComponent(ColorComponent::RED, xmlReader.attributes().value("BackgroundRed").toInt());
                            area->GetColorPair()->SetBGColorComponent(ColorComponent::GREEN, xmlReader.attributes().value("BackgroundGreen").toInt());
                            area->GetColorPair()->SetBGColorComponent(ColorComponent::BLUE, xmlReader.attributes().value("BackgroundBlue").toInt());
                            while ((!xmlReader.atEnd()) && (!(xmlReader.name() == "DisplayArea" && xmlReader.isEndElement())))
                            {
                                xmlReader.readNext();

                                if (!xmlReader.isStartElement()) continue;
                                else if (xmlReader.name() == "Widgets")
                                {
                                    UIWidgetBase* uiWidget = NULL;
                                    while (!xmlReader.atEnd() && !(xmlReader.name() == "Widgets" && xmlReader.isEndElement()))
                                    {
                                        quint32 nbrOfAnimation1Triggers = 0;
                                        quint32 nbrOfAnimation2Triggers = 0;
                                        xmlReader.readNext();

                                        if (!xmlReader.isStartElement()) continue;
                                        else if (xmlReader.name() == "Widget")
                                        {
                                            QString widgetType = xmlReader.attributes().value("Type").toString();

                                            if (widgetType.compare("Menu", Qt::CaseInsensitive) == 0)
                                            {
                                                MenuDefinition* menu = NULL;
                                                QString menuDefinitionPath = xmlReader.attributes().value("ConfigurationFile").toString();
                                                menu = parseMenuDefinition(menuDefinitionPath);
                                                if (menu == NULL)
                                                {
                                                    m_error = "Failed to parse menu definition (" + menuDefinitionPath + ")";
                                                    return ErrorGeneral;
                                                }
                                                layout->SetMenu(menu, area);
                                            }
                                            else
                                            {
                                                uiWidget = UIWidgetDeserializer::Deserialize(xmlReader, m_project);
                                                if (uiWidget != NULL)
                                                {
                                                    area->AppendUIWidget(uiWidget);
                                                    GCLSignalSink* sink = uiWidget->GetSignalSink("ResourceIDToDisplay");
                                                    if(layout->IsCheckMessageScreen() && (sink != NULL))
                                                    {
                                                        //VERIFY
                                                        m_project.GetCheckMessagesDefinition().AddWidgetSink(sink);
                                                        foreach (CheckMessage* checkMessage, m_project.GetCheckMessagesDefinition().CheckMessages())
                                                        {
                                                            checkMessage->AddItem(new CheckMessageItem(m_project.GetCheckMessagesDefinition().GetIndexOfSource(sink->SignalName(), sink->ComponentName()), uiWidget->GetUIWidgetTypeName(), 1, uiWidget->GetUIWidgetID()));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        else if (xmlReader.name() == "Animation1_Properties")
                                        {
                                            uiWidget->SetAnimationOnLoadBehavior(1, static_cast<UIWidgetBase::AnimationOnLoadBehavior>(xmlReader.attributes().value("BehaviorOnLoad").toInt()));
                                            uiWidget->SetAnimationOnTriggerActiveBehavior(1, static_cast<UIWidgetBase::AnimationOnTriggerActiveBehavior>(xmlReader.attributes().value("BehaviorOnTriggerActive").toInt()));
                                            uiWidget->SetAnimationOnTriggerInactiveBehavior(1, static_cast<UIWidgetBase::AnimationOnTriggerInactiveBehavior>(xmlReader.attributes().value("BehaviorOnTriggerInactive").toInt()));

                                            xmlReader.readNext();

                                            while (!xmlReader.atEnd() && !(xmlReader.name() == "Animation1_Triggers" && xmlReader.isEndElement()))
                                            {
                                                xmlReader.readNext();
                                                if (!xmlReader.isStartElement()) continue;

                                                else if (xmlReader.name() == "Trigger")
                                                {

                                                    QString componentName = uiWidget->GetUIWidgetTypeName() + "_" + QString::number(uiWidget->GetUIWidgetID()) + "_Animation1";
                                                    TransitionClause* trigger = new TransitionClause(nbrOfAnimation1Triggers++, uiWidget, componentName);
                                                    while (!xmlReader.atEnd() && !(xmlReader.name() == "Trigger" && xmlReader.isEndElement()))
                                                    {
                                                        xmlReader.readNext();
                                                        if (!xmlReader.isStartElement()) continue;
                                                        else if (xmlReader.name() == "FirstCondition")
                                                        {
                                                            trigger->GetFirstCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                                            trigger->GetFirstCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toUInt()));
                                                        }
                                                        else if (xmlReader.name() == "Connective")
                                                        {
                                                            trigger->SetConnective(static_cast<LayoutTransitionTypes::TransitionConnective>(xmlReader.attributes().value("Connective").toInt()));
                                                        }
                                                        else if (xmlReader.name() == "SecondCondition")
                                                        {
                                                            trigger->AddSecondCondition();
                                                            trigger->GetSecondCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                                            trigger->GetSecondCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toInt()));
                                                        }
                                                    }
                                                    uiWidget->AddAnimationTrigger(1, trigger);
                                                }
                                            }
                                        }
                                        else if (xmlReader.name() == "Animation2_Properties")
                                        {
                                            uiWidget->SetAnimationOnLoadBehavior(2, static_cast<UIWidgetBase::AnimationOnLoadBehavior>(xmlReader.attributes().value("BehaviorOnLoad").toInt()));
                                            uiWidget->SetAnimationOnTriggerActiveBehavior(2, static_cast<UIWidgetBase::AnimationOnTriggerActiveBehavior>(xmlReader.attributes().value("BehaviorOnTriggerActive").toInt()));
                                            uiWidget->SetAnimationOnTriggerInactiveBehavior(2, static_cast<UIWidgetBase::AnimationOnTriggerInactiveBehavior>(xmlReader.attributes().value("BehaviorOnTriggerInactive").toInt()));

                                            xmlReader.readNext();

                                            while (!xmlReader.atEnd() && !(xmlReader.name() == "Animation2_Triggers" && xmlReader.isEndElement()))
                                            {
                                                xmlReader.readNext();
                                                if (!xmlReader.isStartElement()) continue;

                                                else if (xmlReader.name() == "Trigger")
                                                {

                                                    QString componentName = uiWidget->GetUIWidgetTypeName() + "_" + QString::number(uiWidget->GetUIWidgetID()) + "_Animation2";
                                                    TransitionClause* trigger = new TransitionClause(nbrOfAnimation2Triggers++, uiWidget, componentName);
                                                    while (!xmlReader.atEnd() && !(xmlReader.name() == "Trigger" && xmlReader.isEndElement()))
                                                    {
                                                        xmlReader.readNext();
                                                        if (!xmlReader.isStartElement()) continue;
                                                        else if (xmlReader.name() == "FirstCondition")
                                                        {
                                                            trigger->GetFirstCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                                            trigger->GetFirstCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toUInt()));
                                                        }
                                                        else if (xmlReader.name() == "Connective")
                                                        {
                                                            trigger->SetConnective(static_cast<LayoutTransitionTypes::TransitionConnective>(xmlReader.attributes().value("Connective").toInt()));
                                                        }
                                                        else if (xmlReader.name() == "SecondCondition")
                                                        {
                                                            trigger->AddSecondCondition();
                                                            trigger->GetSecondCondition()->SetConditionValue(xmlReader.attributes().value("Value").toInt());
                                                            trigger->GetSecondCondition()->SetCompareOperator(static_cast<LayoutTransitionTypes::TransitionCompare>(xmlReader.attributes().value("CompareCondition").toInt()));
                                                        }
                                                    }
                                                    uiWidget->AddAnimationTrigger(2, trigger);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            layout->AppendDisplayArea(area);
                        }
                    }
                    m_layoutDefinition.AddLayout(layout);
                }
            }
            m_layoutDefinition.SetMainLayout(m_layoutDefinition.GetLayoutFromID(mainLayoutID));
        }
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Layout Definition XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Layout Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Layout Definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
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

MenuDefinition* LayoutDefinitionDeserializer::parseMenuDefinition(QString menuDefinitionPath)
{
    QFile menuDefinitionFile(m_project.GetProjectRoot().absoluteFilePath(menuDefinitionPath));
    if (!menuDefinitionFile.open(QFile::ReadOnly))
    {
        QString errorMessage("Error opening MenuDefinition XML file! (" + menuDefinitionFile.fileName() + ") " + menuDefinitionFile.errorString());
        m_error.append(errorMessage + "\n");
        return NULL;
    }

    QTextStream menuXml(&menuDefinitionFile);
    MenuDefinitionDeserializer deserializer(menuXml.readAll());
    menuDefinitionFile.close();

    MenuDefinition* menuDefinition = MenuDefinition::GetEmptyMenuDefinition();
    if (!deserializer.Deserialize(*menuDefinition, m_project))
    {
        QString errorMessage("Error deserializing MenuDefinition XML file! (" + menuDefinitionFile.fileName() + ") " + deserializer.Error());
        m_error.append(errorMessage + "\n");

        delete menuDefinition;
        return NULL;
    }

    return menuDefinition;
}
