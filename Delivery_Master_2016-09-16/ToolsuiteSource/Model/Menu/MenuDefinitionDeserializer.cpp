#include "MenuDefinitionDeserializer.h"

#include "FunctionDefinition.h"
#include "IconDefinition.h"
#include "Logger.h"
#include "MenuDefinition.h"
#include "MenuItem.h"
#include "Project.h"
#include "UIWidgetDeserializer.h"

#include <QFileInfo>
#include <QXmlStreamReader>

MenuDefinitionDeserializer::MenuDefinitionDeserializer(const QString& xml) : m_xml(xml)
{
}

MenuDefinitionDeserializer::~MenuDefinitionDeserializer()
{
}

bool MenuDefinitionDeserializer::Deserialize(MenuDefinition& menuDefinition, Project& project)
{
    LOG_DEBUG("Entering");

    if (m_xml.isNull() || m_xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return false;
    }

    QXmlStreamReader xmlReader(m_xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "Menu")
    {
        m_error = "No Menu root element found in XML!";
        return false;
    }

    while(!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "InputToCommandMappings")
            {
                while (!(xmlReader.isEndElement() && xmlReader.name() == "InputToCommandMappings"))
                {
                    xmlReader.readNext();

                    if (xmlReader.isStartElement() && xmlReader.name() == "Mapping")
                    {
                        bool inputOk, commandOk;
                        MenuInput input = static_cast<MenuInput>(xmlReader.attributes().value("MenuInputID").toInt(&inputOk));
                        MenuCommandID command = static_cast<MenuCommandID>(xmlReader.attributes().value("CommandID").toInt(&commandOk));

                        if (!inputOk || !commandOk)
                        {
                            continue;
                        }

                        menuDefinition.ModifyInputToCommandMapping(input, command);
                    }
                }
            }
            else if (xmlReader.name() == "MenuRoot")
            {
                bool wrapRootMenu = xmlReader.attributes().value("WrapRootMenu").compare("true", Qt::CaseInsensitive) == 0;
                menuDefinition.MenuRoot().SetWrapped(wrapRootMenu);

                m_startupMenuID = xmlReader.attributes().value("StartupMenuItemID").toInt();

                while(!(xmlReader.isEndElement() && xmlReader.name() == "MenuRoot"))
                {
                    xmlReader.readNext();

                    if (xmlReader.isStartElement() && xmlReader.name() == "MenuItem")
                    {
                        MenuItem* menuItem = parseMenuItem(xmlReader, project, menuDefinition);

                        if (menuItem != NULL)
                        {
                            menuDefinition.MenuRoot().AddMenuItem(menuItem);
                        }
                    }
                }
            }
        }
    }

    if(xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Menu definition XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Menu definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Menu definition XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
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

const QString MenuDefinitionDeserializer::Error() const
{
    return m_error;
}

MenuItem* MenuDefinitionDeserializer::parseMenuItem(QXmlStreamReader& xmlReader, Project& project, MenuDefinition& menuDefinition)
{
    quint32 id = xmlReader.attributes().value("ID").toInt();
    QString name = xmlReader.attributes().value("Name").toString();
    quint16 index = xmlReader.attributes().value("Index").toInt();
    QString wrapSubMenuString = xmlReader.attributes().value("WrapSubMenu").toString();
    quint16 labelID = xmlReader.attributes().value("LabelID").toUInt();
    quint32 imageID = xmlReader.attributes().value("ImageID").toUInt();

    bool wrapSubMenu = false;

    if (wrapSubMenuString.compare("True") == 0)
    {
        wrapSubMenu = true;
    }
    else
    {
        wrapSubMenu = false;
    }

    if (name.isEmpty())
    {
        name = "--UnNamed Item--";
    }

    MenuItem* menuItem = new MenuItem(name);
    if (id > 0)
    {
        menuItem->SetID(id);
    }

    if (m_startupMenuID == menuItem->GetID())
    {
        menuDefinition.MenuRoot().SetStartupMenuItem(menuItem);
    }

    menuItem->SetIndex(index);
    menuItem->SetSubMenuWrapped(wrapSubMenu);
    menuItem->SetLabelID(labelID);
    menuItem->setImageID(imageID);

    while (!(xmlReader.isEndElement() && xmlReader.name() == "MenuItem"))
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Widgets")
            {
                while (!(xmlReader.isEndElement() && xmlReader.name() == "Widgets"))
                {
                    xmlReader.readNext();
                    if (xmlReader.isStartElement() && xmlReader.name() == "Widget")
                    {
                        UIWidgetBase* uiWidget = UIWidgetDeserializer::Deserialize(xmlReader, project);

                        if (uiWidget != NULL)
                        {
                            menuItem->AddUIWidget(uiWidget);
                        }
                    }
                }
            }
            else if (xmlReader.name() == "SubMenuItems")
            {
                while (!(xmlReader.isEndElement() && xmlReader.name() == "SubMenuItems"))
                {
                    xmlReader.readNext();
                    if (xmlReader.isStartElement() && xmlReader.name() == "MenuItem")
                    {
                        MenuItem* subMenuItem = parseMenuItem(xmlReader, project, menuDefinition);
                        if (subMenuItem != NULL)
                        {
                            menuItem->AddSubMenuItem(subMenuItem);
                        }
                    }
                }
            }
        }
    }

    return menuItem;
}


