#include "IconUIWidgetGenerator.h"

IconUIWidgetGenerator::IconUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{
}

IconUIWidgetGenerator::~IconUIWidgetGenerator()
{
}

void IconUIWidgetGenerator::generateIconConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"IconWidgetCfg.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach (UIWidgetBase* uiWidget, m_uiWidgetList)
        {
            if (uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Icon)
            {
                IconWidget* iconWidget = static_cast<IconWidget*>(uiWidget);
                configLines.append("    { .widgetID = " + QString::number(iconWidget->GetUIWidgetID()) + "," +
                                   " .iconID = " + QString::number(iconWidget->GetIconID()) + "}");
            }
        }
    }
    else
    {
        configLines.append("    { .widgetID = 999, .iconID = 999}");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define ICON_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define ICON_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 IconConfigCount = ICON_CONFIG_COUNT;");
    fileLines.append("static const IconCfg_t iconConfigs[ICON_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("const IconCfg_t* const IconConfigs = iconConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("Icon/IconWidgetCfg.c")), fileLines.join("\n"));
}

void IconUIWidgetGenerator::OnGenerateFiles()
{
    generateIconConfigurationSourceFile();
}
