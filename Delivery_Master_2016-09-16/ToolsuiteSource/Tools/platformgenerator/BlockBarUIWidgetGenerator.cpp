#include "BlockBarUIWidgetGenerator.h"

BlockBarUIWidgetGenerator::BlockBarUIWidgetGenerator(QList<UIWidgetBase*> uiWidgets) :
    m_uiWidgets(uiWidgets)
{
    m_Path = QString(PlatformPaths::CreateUIWidgetPath("BlockBar/BlockBarWidgetCfg.c"));
}


void BlockBarUIWidgetGenerator::OnGenerateFiles()
{
    generateSource();
}

void BlockBarUIWidgetGenerator::generateSource()
{
    QStringList sourceLines;

    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"BlockBarWidgetCfg.h\"");
    sourceLines.append("#include \"GCL_Widgets.h\"");

    QStringList configLines;
    if (m_uiWidgets.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgets)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_BlockBar)
            {
                BlockBarWidget* blockBarWidget = static_cast<BlockBarWidget*>(uiwidget);
                const GCLSignalSink* sink = blockBarWidget->GetSignalSinks().first();
                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(blockBarWidget->GetUIWidgetID()) + "U,\n" \
                                   "        .currentValue = { .Value = { .U32 = 0 }, .DataType = STD_DATATYPE_UINT32, .Flashing = FALSE, .Valid = FALSE },\n" \
                                   "        .lastRenderedValue = 0U,\n" \
                                   "        .spacing = " + QString::number(blockBarWidget->GetSpacing()) + "U ,\n" \
                                   "        .numberOfBlocks = " + QString::number(blockBarWidget->GetNumberOfBlocks()) + "U ,\n" \
                                   "        .min = " + QString::number(blockBarWidget->GetMin()) + "U ,\n" \
                                   "        .max= " + QString::number(blockBarWidget->GetMax()) + "U ,\n" \
                                   "        .gclSink = GCL_Read_"  + blockBarWidget->GetUIWidgetUniqueName() + "_" + sink->SignalName() + "\n"\
                                   "    }");
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999U,\n" \
                           "        .currentValue = { .Value = { .U32 = 0 }, .DataType = STD_DATATYPE_UINT32, .Flashing = FALSE, .Valid = FALSE },\n" \
                           "        .spacing = 0U ,\n" \
                           "        .numberOfBlocks = 0U ,\n" \
                           "        .min = 0U ,\n" \
                           "        .max= 0U ,\n" \
                           "        .gclSink = NULL\n"\
                           "    }");
    }
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgets.count() != 0)
    {
        sourceLines.append("#define BLOCK_BAR_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        sourceLines.append("#define BLOCK_BAR_CONFIG_COUNT    0U");
    }

    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("const uint32 BlockBarWidgetConfigCount = BLOCK_BAR_CONFIG_COUNT;");
    sourceLines.append("static BlockBarWidgetCfg_t blockBarWidgetConfigs[BLOCK_BAR_CONFIG_COUNT] =");
    sourceLines.append("{");
    sourceLines.append(configLines.join(",\n"));
    sourceLines.append("};");
    sourceLines.append("BlockBarWidgetCfg_t* const BlockBarWidgetConfigs = blockBarWidgetConfigs;");
    sourceLines.append("");
    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("");

    AddGeneratedFile(m_Path, sourceLines.join("\n"));
}
