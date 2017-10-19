#include "SignalTriggerUIWidgetGenerator.h"

#include "GCLSignalSource.h"

SignalTriggerUIWidgetGenerator::SignalTriggerUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{
}

SignalTriggerUIWidgetGenerator::~SignalTriggerUIWidgetGenerator()
{
}

void SignalTriggerUIWidgetGenerator::generateConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"SignalTriggerWidgetCfg.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_SignalTrigger)
            {
                SignalTriggerWidget* signalTriggerWidget = static_cast<SignalTriggerWidget*>(uiwidget);

                const GCLSignalSource* signalTriggerSource = signalTriggerWidget->GetSignalSource("SignalTriggerSource");

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(signalTriggerWidget->GetUIWidgetID()) + ",\n" \
                                   "        .writeSignal = GCL_Write_" +  signalTriggerWidget->GetUIWidgetUniqueName() + "_" + signalTriggerSource->SignalName() + ",\n" \
                                   "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .writeSignal = NULL,\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define SIGNALTRIGGER_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define SIGNALTRIGGER_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 SignalTriggerConfigCount = SIGNALTRIGGER_CONFIG_COUNT;");
    fileLines.append("static const SignalTriggerCfg_t signalTriggerConfigs[SIGNALTRIGGER_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("const SignalTriggerCfg_t* const SignalTriggerConfigs = signalTriggerConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("SignalTrigger/SignalTriggerWidgetCfg.c")), fileLines.join("\n"));
}

void SignalTriggerUIWidgetGenerator::OnGenerateFiles()
{
    generateConfigurationSourceFile();
}
