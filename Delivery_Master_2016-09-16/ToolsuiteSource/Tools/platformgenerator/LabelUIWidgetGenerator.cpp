#include "LabelUIWidgetGenerator.h"

LabelUIWidgetGenerator::LabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{
}

LabelUIWidgetGenerator::~LabelUIWidgetGenerator()
{
}

void LabelUIWidgetGenerator::generateLabelConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"LabelWidgetCfg.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach (UIWidgetBase* uiWidget, m_uiWidgetList)
        {
            if (uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Label)
            {
                LabelWidget* labelWidget = static_cast<LabelWidget*>(uiWidget);
                configLines.append("    { .widgetID = " + QString::number(labelWidget->GetUIWidgetID()) + "," +
                                   " .labelID = " + QString::number(labelWidget->GetLabelID()) + "}");
            }
        }
    }
    else
    {
        configLines.append("    { .widgetID = 999, .labelID = 999}");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define LABEL_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define LABEL_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 LabelConfigCount = LABEL_CONFIG_COUNT;");
    fileLines.append("static const LabelCfg_t labelConfigs[LABEL_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("const LabelCfg_t* const LabelConfigs = labelConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("Label/LabelWidgetCfg.c")), fileLines.join("\n"));
}

void LabelUIWidgetGenerator::OnGenerateFiles()
{
    generateLabelConfigurationSourceFile();
}
