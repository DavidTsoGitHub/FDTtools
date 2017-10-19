#include "DynamicIconUIWidgetGenerator.h"

DynamicIconUIWidgetGenerator::DynamicIconUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{
}

DynamicIconUIWidgetGenerator::~DynamicIconUIWidgetGenerator()
{
}

void DynamicIconUIWidgetGenerator::generateConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DynamicIconWidgetCfg.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicIcon)
            {
                DynamicIconWidget* dynamicIconWidget = static_cast<DynamicIconWidget*>(uiwidget);

                const GCLSignalSink* iconIDSink = dynamicIconWidget->GetSignalSink("ResourceIDToDisplay");

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(dynamicIconWidget->GetUIWidgetID()) + ",\n" \
                                   "        .currentIconID = 0U,\n" \
                                   "        .readIconID = GCL_Read_" + dynamicIconWidget->GetUIWidgetUniqueName() + "_" + iconIDSink->SignalName() + "\n" \
                                   "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .currentIconID = 0U,\n" \
                           "        .readIconID = NULL\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define DYNAMIC_ICON_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define DYNAMIC_ICON_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 DynamicIconConfigCount = DYNAMIC_ICON_CONFIG_COUNT;");
    fileLines.append("static DynamicIconCfg_t dynamicIconConfigs[DYNAMIC_ICON_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("DynamicIconCfg_t* const DynamicIconConfigs = dynamicIconConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("DynamicIcon/DynamicIconWidgetCfg.c")), fileLines.join("\n"));
}

void DynamicIconUIWidgetGenerator::OnGenerateFiles()
{
    generateConfigurationSourceFile();
}
