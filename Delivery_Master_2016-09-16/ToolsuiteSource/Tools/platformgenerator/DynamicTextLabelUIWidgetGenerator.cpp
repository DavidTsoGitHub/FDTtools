#include "DynamicTextLabelUIWidgetGenerator.h"

DynamicTextLabelUIWidgetGenerator::DynamicTextLabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project) :
    m_project(project)
{
    m_uiWidgetList = uiWidgetList;

    m_dynamicTextLabelPath = QString(PlatformPaths::CreateUIWidgetPath("DynamicTextLabel/DynamicTextLabelWidgetCfg.c"));
}

DynamicTextLabelUIWidgetGenerator::~DynamicTextLabelUIWidgetGenerator()
{

}

void DynamicTextLabelUIWidgetGenerator::OnGenerateFiles()
{
    generateDynamicTextLabelConfigurationSourceFile();
}

void DynamicTextLabelUIWidgetGenerator::generateDynamicTextLabelConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DynamicTextLabelWidgetCfg.h\"");
    fileLines.append("#include \"fonts.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicText)
            {
                DynamicTextLabelWidget* dynamicTextLabelWidget = static_cast<DynamicTextLabelWidget*>(uiwidget);
                const GCLSignalSink* sink = dynamicTextLabelWidget->GetSignalSinks().first();
                quint32 fontId = 0;
                if (dynamicTextLabelWidget->GetFontID() == 0)
                {
                    fontId = m_project.GetLabelDefinition().GetDefaultFont()->ID;
                }
                else
                {
                    fontId = dynamicTextLabelWidget->GetFontID();
                }
                QString fontName = QString("&font%1").arg(fontId);

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(dynamicTextLabelWidget->GetUIWidgetID()) + ",\n" \
                                   "        .gclSink = GCL_Read_" +  dynamicTextLabelWidget->GetUIWidgetUniqueName() + "_" + sink->SignalName() +  + ",\n" \
                                   "        .currentString = (wchar*)0,\n" \
                                   "        .font = " + fontName  + ",\n" \
                                   "        .rightAligned = " + QString::number(dynamicTextLabelWidget->GetRightAligned()) + "U\n" \
                                   "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .gclSink = NULL,\n" \
                           "        .currentString = (wchar*)0,\n" \
                           "        .font = NULL,\n" \
                           "        .rightAligned = 0U\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define DYNAMIC_TEXT_LABEL_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define DYNAMIC_TEXT_LABEL_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");    
    fileLines.append("const uint32 DynamicTextLabelConfigCount = DYNAMIC_TEXT_LABEL_CONFIG_COUNT;");
    fileLines.append("static DynamicTextLabelCfg_t dynamicTextLabelConfigs[DYNAMIC_TEXT_LABEL_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("DynamicTextLabelCfg_t* const DynamicTextLabelConfigs = dynamicTextLabelConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(m_dynamicTextLabelPath, fileLines.join("\n"));
}
