#include "DynamicLabelUIWidgetGenerator.h"

DynamicLabelUIWidgetGenerator::DynamicLabelUIWidgetGenerator(QList<UIWidgetBase *> uiWidgetList) :
     m_uiWidgetList(uiWidgetList)
{
}

DynamicLabelUIWidgetGenerator::~DynamicLabelUIWidgetGenerator()
{
}


void DynamicLabelUIWidgetGenerator::generateConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;
    quint32 dynamicLabelCount = 0;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DynamicLabelWidgetCfg.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0 )
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWIdget_Type_DynamicLabel)
            {
                 dynamicLabelCount++;

                 DynamicLabelWidget* dynamicLabelWidget = static_cast<DynamicLabelWidget*>(uiwidget);
                 const GCLSignalSink* labelSink = dynamicLabelWidget->GetSignalSink("ResourceIDToDisplay");

                 configLines.append("    {\n" \
                                    "        .widgetID = " + QString::number(dynamicLabelWidget->GetUIWidgetID()) + ",\n" \
                                    "        .currentLabelID = 0U,\n" \
                                    "        .alignment = " + dynamicLabelWidget->GetTextAlignmentEnumString() + ",\n" \
                                    "        .readLabelID = GCL_Read_" +  dynamicLabelWidget->GetUIWidgetUniqueName() + "_" + labelSink->SignalName() + "\n" \
                                    "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .currentLabelID = 0U,\n" \
                           "        .alignment = TEXT_LEFT_ALIGNED,\n" \
                           "        .readLabelID = NULL\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define DYNAMIC_LABEL_CONFIG_COUNT    " + QString::number(dynamicLabelCount) + "U");
    }
    else
    {
        fileLines.append("#define DYNAMIC_LABEL_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 DynamicLabelConfigCount = DYNAMIC_LABEL_CONFIG_COUNT;");
    fileLines.append("static DynamicLabelCfg_t dynamicLabelConfigs[DYNAMIC_LABEL_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("DynamicLabelCfg_t* const DynamicLabelConfigs = dynamicLabelConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("DynamicLabel/DynamicLabelWidgetCfg.c")), fileLines.join("\n"));


}


void DynamicLabelUIWidgetGenerator::OnGenerateFiles()
{

   generateConfigurationSourceFile();
}
