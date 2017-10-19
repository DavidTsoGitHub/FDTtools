#include "DynamicValueLabelUIWidgetGenerator.h"

DynamicValueLabelUIWidgetGenerator::DynamicValueLabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project) :
    m_project(project)
{
    m_uiWidgetList = uiWidgetList;

    m_dynamicValueLabelPath = QString(PlatformPaths::CreateUIWidgetPath("DynamicValueLabel/DynamicValueLabelWidgetCfg.c"));
}

DynamicValueLabelUIWidgetGenerator::~DynamicValueLabelUIWidgetGenerator()
{

}

void DynamicValueLabelUIWidgetGenerator::OnGenerateFiles()
{
    generateDynamicValueLabelConfigurationSourceFile();
}

void DynamicValueLabelUIWidgetGenerator::generateDynamicValueLabelConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DynamicValueLabelWidgetCfg.h\"");
    fileLines.append("#include \"fonts.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicValue)
            {
                DynamicValueLabelWidget* dynamicValueLabelWidget = static_cast<DynamicValueLabelWidget*>(uiwidget);
                const GCLSignalSink* sink = dynamicValueLabelWidget->GetSignalSinks().first();

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(dynamicValueLabelWidget->GetUIWidgetID()) + ",\n" \
                                   "        .gclSink = GCL_Read_" +  dynamicValueLabelWidget->GetUIWidgetUniqueName() + "_" + sink->SignalName() +  + ",\n" \
                                   "        .currentString = { 0 },\n" \
                                   "        .currentValue = { .Value = { .U32 = 0 }, .DataType = STD_DATATYPE_UINT32, .Flashing = FALSE, .Valid = FALSE },\n" \
                                   "        .font = &font" + QString::number(dynamicValueLabelWidget->GetFontID()) + " ,\n" \
                                   "        .scaling = " + QString::number(dynamicValueLabelWidget->GetScaling()) + ",\n" \
                                   "        .decimals = " + QString::number(dynamicValueLabelWidget->GetNbrOfDecimals()) + ",\n" \
                                   "        .maximumNbrOfDigits = " + QString::number(dynamicValueLabelWidget->GetNbrOfDigits()) + "\n" \
                                   "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .gclSink = NULL,\n" \
                           "        .currentString = L\"\",\n" \
                           "        .currentValue = { .Value = { 0 }, .DataType = STD_DATATYPE_UINT32, .Flashing = FALSE, .Valid = FALSE },\n" \
                           "        .font = NULL,\n" \
                           "        .scaling = 0,\n" \
                           "        .decimals = 0,\n" \
                           "        .maximumNbrOfDigits = 0\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define DYNAMIC_VALUE_LABEL_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define DYNAMIC_VALUE_LABEL_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");    
    fileLines.append("const uint32 DynamicValueLabelConfigCount = DYNAMIC_VALUE_LABEL_CONFIG_COUNT;");
    fileLines.append("static DynamicValueLabelCfg_t dynamicValueLabelConfigs[DYNAMIC_VALUE_LABEL_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("DynamicValueLabelCfg_t* const DynamicValueLabelConfigs = dynamicValueLabelConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(m_dynamicValueLabelPath, fileLines.join("\n"));
}
