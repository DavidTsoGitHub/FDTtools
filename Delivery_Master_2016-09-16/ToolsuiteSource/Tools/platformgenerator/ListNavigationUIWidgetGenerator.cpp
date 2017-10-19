#include "ListNavigationUIWidgetGenerator.h"

#include "ColorPair.h"
#include "BSPDisplay.h"
#include "LayoutDefinition.h"
#include "Project.h"

ListNavigationUIWidgetGenerator::ListNavigationUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project) :
    m_uiWidgetList(uiWidgetList),
    m_project(project),
    m_layout(m_project.GetLayoutDefinition())
{
}

ListNavigationUIWidgetGenerator::~ListNavigationUIWidgetGenerator()
{
}

void ListNavigationUIWidgetGenerator::generateListNavigationConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"ListNavigationWidgetCfg.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach (UIWidgetBase* uiWidget, m_uiWidgetList)
        {
            if (uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_ListNavigation)
            {
                ListNavigationWidget* widget = static_cast<ListNavigationWidget*>(uiWidget);

                quint32 fgColor = widget->GetSecondaryColorPair()->GetFGColor().rgb() & 0xffffff;
                quint16 fgColorID = m_layout.appendCLUTGetIndex(fgColor);

                quint32 bgColor = widget->GetSecondaryColorPair()->GetBGColor().rgb() & 0xffffff;
                if (widget->GetSecondaryColorPair()->GetBGIsTransparent())
                {
                    bgColor = m_layout.GetStartChromaKeyColor().rgb() & 0xffffff;
                }
                quint16 bgColorID = m_layout.appendCLUTGetIndex(bgColor);

                if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::SW)
                {
                    fgColorID = 0;
                    bgColorID = 1;
                }

                configLines.append("    { .widgetID = " + QString::number(widget->GetUIWidgetID()) + "," +
                                   " .displayItemCount = " + QString::number(widget->GetDisplayItemCount()) + "," +
                                   " .firstIndexInView = 0," +
                                   " .lastDrawnMenuItem = NULL," +
                                   " .selectedItemFgColor = " + QString::number(fgColorID) + "u," +
                                   " .selectedItemBgColor = " + QString::number(bgColorID) + "u }");
            }
        }
    }
    else
    {
        configLines.append("    { .widgetID = 999, .displayItemCount = 999, .firstIndexInView = 0, .lastDrawnMenuItem = NULL, .selectedItemFgColor = 0, .selectedItemBgColor = 1 }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() == 0)
    {
        fileLines.append("#define LIST_NAVIGATION_CONFIG_COUNT    1U");
    }
    else
    {
        fileLines.append("#define LIST_NAVIGATION_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 ListNavigationConfigCount = LIST_NAVIGATION_CONFIG_COUNT;");
    fileLines.append("static ListNavigationCfg_t listNavigationConfigs[LIST_NAVIGATION_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("ListNavigationCfg_t* const ListNavigationConfigs = listNavigationConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("ListNavigation/ListNavigationWidgetCfg.c")), fileLines.join("\n"));
}

void ListNavigationUIWidgetGenerator::OnGenerateFiles()
{
    generateListNavigationConfigurationSourceFile();
}
