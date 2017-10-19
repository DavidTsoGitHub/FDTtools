#include "MenuGenerator.h"

#include "BSPDisplay.h"
#include "IBSP_DCU_BPPModes.h"
#include "Icon.h"
#include "IconDefinition.h"
#include "Layout.h"
#include "Logger.h"
#include "MenuItem.h"
#include "RegEx/RegExPatterns.h"
#include "ColorPair.h"

MenuGenerator::MenuGenerator(Project* project):
    m_layout(project->GetLayoutDefinition()),
    m_bspSpecification(project->GetBSPSpecification())
{
    if (m_bspSpecification.HasDisplay())
    {
        foreach (Layout* layout, m_layout.Layouts())
        {
            if (layout->HasMenu())
            {
                m_menuDefinition = layout->GetMenu();
            }
        }
    }

    m_MenuConfigFilePath = QString(PlatformPaths::CreateServicePath("Menu")).append("/Menu_cfg.c");
}

MenuGenerator::~MenuGenerator()
{
}

void MenuGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    if (m_menuDefinition == NULL)
    {
        return;
    }

    LOG_INFO("Generating Menu files...");
    m_nextSignalName = m_menuDefinition->GetSignalNameFromCommandID(CommandNext);
    m_previousSignalName = m_menuDefinition->GetSignalNameFromCommandID(CommandPrevious);
    m_enterSignalName = m_menuDefinition->GetSignalNameFromCommandID(CommandEnter);
    m_leaveSignalName = m_menuDefinition->GetSignalNameFromCommandID(CommandLeave);

    generateMenuConfigFile();
    addStaticMenuFiles();
    LOG_INFO("Generating Menu files...done!");
}

void MenuGenerator::addWidgetIncludes(const QList<MenuItem*>& menuItems, QStringList& includeLines)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        foreach (UIWidgetBase* uiWidget, menuItem->UIWidgets())
        {
            includeLines.append("#include \"" + uiWidget->GetUIWidgetTypeName() + "Widget.h\"");
        }

        if (!menuItem->SubMenuItems().isEmpty())
        {
            addWidgetIncludes(menuItem->SubMenuItems(), includeLines);
        }
    }
}

void MenuGenerator::generateMenuConfigFile()
{
    QStringList sourceLines;
    sourceLines.append("//lint -save");
    sourceLines.append("//lint -esym(765,menuItem*) [Supressed as we need to separate generated and static code.]");
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"Menu_cfg.h\"");
    QStringList widgetIncludeLines;
    addWidgetIncludes(m_menuDefinition->MenuRoot().MenuItems(), widgetIncludeLines);
    widgetIncludeLines.removeDuplicates();
    sourceLines.append(widgetIncludeLines.join("\n"));
    sourceLines.append("#include \"ContentsManager_cfg.h\"");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    appendMenuItemDeclarations(sourceLines, m_menuDefinition->MenuRoot().MenuItems());
    sourceLines.append("");
    appendMenuItemWidgetsDefinitions(sourceLines, m_menuDefinition->MenuRoot().MenuItems());
    sourceLines.append("");
    appendMenuItemDefinitions(sourceLines, m_menuDefinition->MenuRoot().MenuItems(), m_menuDefinition->MenuRoot().IsWrapped());
    sourceLines.append("");
    if (m_menuDefinition->MenuRoot().GetStartupMenuItem() != NULL)
    {
        sourceLines.append("const menuItem_t* const StartMenuItem = &" + getUniqueMenuItemName(m_menuDefinition->MenuRoot().GetStartupMenuItem()) + ";");
    }
    else if (m_menuDefinition->MenuRoot().MenuItems().count() > 0)
    {
        sourceLines.append("const menuItem_t* const StartMenuItem = &" + getUniqueMenuItemName(m_menuDefinition->MenuRoot().MenuItems().first()) + ";");
    }
    else
    {
        sourceLines.append("const menuItem_t* const StartMenuItem = NULL;");
    }
    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("/************************* Local Function Prototypes *************************/");
    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("");
    sourceLines.append("//lint -restore");
    sourceLines.append("");

    AddGeneratedFile(m_MenuConfigFilePath, sourceLines.join("\n"));
}

void MenuGenerator::addStaticMenuFiles()
{
    AddFilesFromResourcePath(":/MenuFiles");
}

void MenuGenerator::appendMenuItemDeclarations(QStringList& sourceLines, const QList<MenuItem*>& menuItems)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        QString uniqueMenuItemName = getUniqueMenuItemName(menuItem);
        sourceLines.append("extern const menuItem_t " + uniqueMenuItemName + ";");

        appendMenuItemDeclarations(sourceLines, menuItem->SubMenuItems());
    }
}

void MenuGenerator::appendMenuItemWidgetsDefinitions(QStringList& sourceLines, const QList<MenuItem*>& menuItems)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        if (menuItem->UIWidgets().count() > 0)
        {
            QString uniqueMenuItemName = getUniqueMenuItemName(menuItem);
            sourceLines.append("static const ui_widget_t " + uniqueMenuItemName + "Widgets[] =");
            sourceLines.append("{");
            QStringList widgetDefinitions;
            foreach (UIWidgetBase* widget, menuItem->UIWidgets())
            {
                if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::HW)
                {
                    quint32 fgColor = widget->GetPrimaryColorPair()->GetFGColor().rgb() & 0xffffff;
                    quint16 fgColorID = m_layout.appendCLUTGetIndex(fgColor);

                    quint16 bgColorID;
                    if (widget->GetPrimaryColorPair()->GetBGIsTransparent())
                    {
                        quint32 bgColor;

                        if (m_layout.GetMenuArea()->GetColorPair()->GetBGIsTransparent())
                        {
                            bgColor = m_layout.GetStartChromaKeyColor().rgb() & 0xffffff;
                        }
                        else
                        {
                            bgColor = m_layout.GetMenuArea()->GetColorPair()->GetBGColor().rgb() & 0xffffff;
                        }

                        bgColorID = m_layout.appendCLUTGetIndex(bgColor);
                    }
                    else
                    {
                        quint32 bgColor = widget->GetPrimaryColorPair()->GetBGColor().rgb() & 0xffffff;
                        bgColorID = m_layout.appendCLUTGetIndex(bgColor);
                    }

                    widgetDefinitions.append("    { .widgetID = " + QString::number(widget->GetUIWidgetID()) +
                                                 "u, .layerID = MENU_AREA_LAYER_INDEX, .x = " + QString::number(widget->GetX()) +
                                                 "u, .y = " + QString::number(widget->GetY()) +
                                                 "u, .width = " + QString::number(widget->GetWidth()) +
                                                 "u, .height = " + QString::number(widget->GetHeight()) +
                                                 "u, .boxed = " + QString::number(widget->GetBoxed()) +
                                                 "u, .fgColor = " + QString::number(fgColorID) +
                                                 "u, .bgColor = " + QString::number(bgColorID) +
                                                 "u, .bpp = " + m_bspSpecification.GetDisplay()->GetBSPDCUBPPModes()->GetBitsPerPixelForModeString(widget->GetBPPMode()) +
                                                 ", .functions = &" + widget->GetUIWidgetTypeName() + 
                                                 "WidgetFuncs, .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0 }");
                }
                else
                {
                    widgetDefinitions.append("    {  .widgetID = " + QString::number(widget->GetUIWidgetID()) +
                                                 "u, .layerID = 0, .x = " + QString::number(widget->GetX()) +
                                                 "u, .y = " + QString::number(widget->GetY()) +
                                                 "u, .width = " + QString::number(widget->GetWidth()) +
                                                 "u, .height = " + QString::number(widget->GetHeight()) +
                                                 "u, .boxed = " + QString::number(widget->GetBoxed()) +
                                                 "u, .fgColor = 1u, .bgColor = 0u, .bpp = DCU_1_BPP, .functions = &" + widget->GetUIWidgetTypeName() + 
                                                 "WidgetFuncs, .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0 }");
                }

            }
            sourceLines.append(widgetDefinitions.join(",\n"));
            sourceLines.append("};");
        }

        appendMenuItemWidgetsDefinitions(sourceLines, menuItem->SubMenuItems());
    }
}


void MenuGenerator::appendMenuItemDefinitions(QStringList& sourceLines, const QList<MenuItem*>& menuItems, bool wrapMenu)
{
    foreach (MenuItem* menuItem, menuItems)
    {
        QString uniqueMenuItemName = getUniqueMenuItemName(menuItem);

        sourceLines.append("const menuItem_t " + uniqueMenuItemName + " =");
        sourceLines.append("{");

        QString previousMenuItemString("NULLMENUITEM");
        QString nextMenuItemString("NULLMENUITEM");
        QString enterMenuItemString("NULLMENUITEM");
        QString leaveMenuItemString("NULLMENUITEM");

        // previousMenuItemString
        if (menuItem->IsTopLevelMenuItem())
        {
            if (menuItem != menuItems.first() || wrapMenu)
            {
                MenuItem* previousMenuItem = getPreviousMenuItem(menuItem, menuItems);
                previousMenuItemString = "&" + getUniqueMenuItemName(previousMenuItem);
            }
        }
        else
        {
            if (menuItem != menuItems.first() || wrapMenu)
            {
                MenuItem* previousMenuItem = getPreviousMenuItem(menuItem, menuItems);
                previousMenuItemString = "&" + getUniqueMenuItemName(previousMenuItem);
            }
        }

        // nextMenuItemString
        if (menuItem->IsTopLevelMenuItem())
        {
            if (menuItem != menuItems.last() || wrapMenu)
            {
                MenuItem* nextMenuItem = getNextMenuItem(menuItem, menuItems);
                nextMenuItemString = "&" +getUniqueMenuItemName(nextMenuItem);
            }
        }
        else
        {
            if (menuItem != menuItems.last() || wrapMenu)
            {
                MenuItem* nextMenuItem = getNextMenuItem(menuItem, menuItems);
                nextMenuItemString = "&" +getUniqueMenuItemName(nextMenuItem);
            }
        }

        // enterMenuItemString
        if (menuItem->SubMenuItems().count() > 0)
        {
            MenuItem* firstChild = menuItem->SubMenuItems().first();
            enterMenuItemString = "&" +getUniqueMenuItemName(firstChild);
        }

        // leaveMenuItemString
        if (!menuItem->IsTopLevelMenuItem())
        {
            MenuItem* parent = static_cast<MenuItem*>(menuItem->GetParent());
            leaveMenuItemString = "&" +getUniqueMenuItemName(parent);
        }

        sourceLines.append("    .labelID = " + QString::number(menuItem->GetLabelID()) + "u,");
        sourceLines.append("    .imageID = " + ((menuItem->GetImageID()==0)? QString("0u,") : QString::number(menuItem->GetImageID()) + "u,"));
        sourceLines.append("    .actionNext = " + nextMenuItemString + ", .actionPrevious = " + previousMenuItemString + ", .actionEnter = " + enterMenuItemString + ", .actionLeave = " + leaveMenuItemString + ",");
        if (menuItem->UIWidgets().count() == 0)
        {
            sourceLines.append("    .uiWidgets = NULL,");
        }
        else
        {
            sourceLines.append("    .uiWidgets = " + uniqueMenuItemName + "Widgets,");
        }
        int uiWidgetCount = menuItem->UIWidgets().count();
        sourceLines.append("    .uiWidgetCount = " + QString::number(uiWidgetCount) + "u,");
        sourceLines.append("    .menuItemIndex = " + QString::number(menuItem->GetIndex()) + "u");
        sourceLines.append("};");

        appendMenuItemDefinitions(sourceLines, menuItem->SubMenuItems(), menuItem->IsSubMenuWrapped());
    }
}

MenuItem* MenuGenerator::getPreviousMenuItem(MenuItem* currentItem, const QList<MenuItem*>& container)
{
    if (container.count() == 1)
    {
        return currentItem;
    }

    if (container.count() == 2)
    {
        return (currentItem == container.first() ? container.last() : container.first());
    }

    qint32 indexOfCurrentItem = container.indexOf(currentItem);
    qint32 indexOfPreviousItem = indexOfCurrentItem - 1;

    if (indexOfPreviousItem < 0)
    {
        return container.last();
    }

    return container.at(indexOfPreviousItem);
}

MenuItem* MenuGenerator::getNextMenuItem(MenuItem* currentItem, const QList<MenuItem*>& container)
{
    if (container.count() == 1)
    {
        return currentItem;
    }

    if (container.count() == 2)
    {
        return (currentItem == container.first() ? container.last() : container.first());
    }

    qint32 indexOfCurrentItem = container.indexOf(currentItem);
    qint32 indexOfNextItem = indexOfCurrentItem + 1;

    if (indexOfNextItem > container.indexOf(container.last()))
    {
        return container.first();
    }

    return container.at(indexOfNextItem);
}

QString MenuGenerator::getUniqueMenuItemName(const MenuItem* menuItem)
{
    if (!m_uniqueMenuItemNames.contains(menuItem))
    {
        qint32 count = m_uniqueMenuItemNames.count();
        m_uniqueMenuItemNames.insert(menuItem, "menuItem" + menuItem->GetName() + QString::number(count));
    }

    return m_uniqueMenuItemNames.value(menuItem);
}
