#include "MenuDefinitionSerializer.h"

#include "MenuDefinition.h"
#include "MenuItem.h"
#include "Logger.h"
#include "UIWidgetSerializer.h"

#include <QXmlStreamWriter>


MenuDefinitionSerializer::MenuDefinitionSerializer(MenuDefinition& menuDefinition) : SerializerBase(NULL),
  m_menuDefinition(menuDefinition)
{}

MenuDefinitionSerializer::~MenuDefinitionSerializer()
{}

QString MenuDefinitionSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Menu");
    xmlWriter.writeStartElement("InputToCommandMappings");

    if (&m_menuDefinition != NULL)
    {
        foreach (InputToCommandMapping* inputToCommandMapping, m_menuDefinition.InputToCommandMappings())
        {
            xmlWriter.writeStartElement("Mapping");
            xmlWriter.writeAttribute("MenuInputID", QString::number(inputToCommandMapping->Input));
            xmlWriter.writeAttribute("CommandID", QString::number(inputToCommandMapping->CommandID));
            xmlWriter.writeEndElement(); // Menu.InputToCommandMappings.Mapping
        }
        xmlWriter.writeEndElement(); // Menu.InputToCommandMappings

        xmlWriter.writeStartElement("MenuRoot");
        QString wrapValue = m_menuDefinition.MenuRoot().IsWrapped() ? "True" : "False";
        xmlWriter.writeAttribute("WrapRootMenu", wrapValue);

        const MenuItem* startupMenuItem = m_menuDefinition.MenuRoot().GetStartupMenuItem();
        if (startupMenuItem != NULL)
        {
            xmlWriter.writeAttribute("StartupMenuItemID", QString::number(startupMenuItem->GetID()));
        }

        qint16 index = 0;
        foreach (const MenuItem* menuItem, m_menuDefinition.MenuRoot().MenuItems())
        {
            writeMenuItem(xmlWriter, menuItem, index);
            ++index;
        }
    }
    xmlWriter.writeEndElement(); // Menu.MenuRoot
    xmlWriter.writeEndElement(); // Menu

    xmlString.append("\n");
    return xmlString;
}

void MenuDefinitionSerializer::writeMenuItem(QXmlStreamWriter& xmlWriter, const MenuItem* menuItem, qint16 index)
{
    xmlWriter.writeStartElement("MenuItem");
    xmlWriter.writeAttribute("Name", menuItem->GetName());
    xmlWriter.writeAttribute("ID", QString::number(menuItem->GetID()));
    xmlWriter.writeAttribute("Index", QString::number(index));
    QString wrapValue = menuItem->IsSubMenuWrapped() ? "True" : "False";
    xmlWriter.writeAttribute("WrapSubMenu", wrapValue);

    quint16 labelID = menuItem->GetLabelID();
    xmlWriter.writeAttribute("LabelID", QString::number(labelID));

    quint32 imageID = menuItem->GetImageID();
    xmlWriter.writeAttribute("ImageID", QString::number(imageID));

    xmlWriter.writeStartElement("Widgets");
    foreach (UIWidgetBase* uiWidget, menuItem->UIWidgets())
    {
        xmlWriter.writeStartElement("Widget");
        UIWidgetSerializer::Serialize(xmlWriter, uiWidget);
        xmlWriter.writeEndElement(); // Menu.MenuRoot.MenuItem.Widgets.Widget
    }
    xmlWriter.writeEndElement(); // Menu.MenuRoot.MenuItem.Widgets

    if (menuItem->SubMenuItems().count() > 0)
    {
        xmlWriter.writeStartElement("SubMenuItems");
        quint16 subMenuIndex = 0;
        foreach (MenuItem* subMenuItem, menuItem->SubMenuItems())
        {
            writeMenuItem(xmlWriter, subMenuItem, subMenuIndex);
            ++subMenuIndex;
        }

        xmlWriter.writeEndElement(); // Menu.MenuRoot.MenuItem.SubMenuItems
    }

    xmlWriter.writeEndElement(); // Menu.MenuRoot.MenuItem
}
