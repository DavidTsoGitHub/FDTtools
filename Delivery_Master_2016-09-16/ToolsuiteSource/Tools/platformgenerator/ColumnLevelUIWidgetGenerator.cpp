#include "ColumnLevelUIWidgetGenerator.h"
#include "GCLSignalSource.h"
#include "ColorPair.h"
#include "BSPDisplay.h"
#include "DisplayArea.h"
#include "Project.h"

ColumnLevelUIWidgetGenerator::ColumnLevelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgets, Project& project) :
    m_uiWidgets(uiWidgets),
    m_project(project)
{
    m_Path = QString(PlatformPaths::CreateUIWidgetPath("ColumnLevel/ColumnLevelWidgetCfg.c"));
}

void ColumnLevelUIWidgetGenerator::generateSource()
{
    QStringList sourceLines;
    QStringList configLines;
    LayoutDefinition& layoutDefinition = m_project.GetLayoutDefinition();

    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"ColumnLevelWidgetCfg.h\"");
    sourceLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgets.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgets)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_ColumnLevel)
            {
                ColumnLevelWidget* widget = static_cast<ColumnLevelWidget*>(uiwidget);
                const GCLSignalSink* currentColumnLevelSink = widget->GetSignalSink("CurrentValue");
                const GCLSignalSource* selectedColumnLevelSource = widget->GetSignalSource("SelectedValue");

                quint32 fgColor = widget->GetSecondaryColorPair()->GetFGColor().rgb() & 0xffffff;
                quint16 fgColorID = layoutDefinition.appendCLUTGetIndex(fgColor);

                quint32 bgColor = widget->GetSecondaryColorPair()->GetBGColor().rgb() & 0xffffff;

                if (widget->GetSecondaryColorPair()->GetBGIsTransparent())
                {
                    DisplayArea *parent = NULL;
                    if (dynamic_cast<DisplayArea*>(uiwidget->GetParent()) != NULL)
                    {
                        parent = dynamic_cast<DisplayArea*>(uiwidget->GetParent());
                    }
                    else
                    {
                        // If parent isn't a DisplayArea then it is a MenuItem and we get color info from MenuArea
                        parent = layoutDefinition.GetMenuArea();
                    }

                    if (parent != NULL)
                    {
                        if (parent->GetColorPair()->GetBGIsTransparent())
                        {
                            bgColor = layoutDefinition.GetStartChromaKeyColor().rgb() & 0xffffff;
                        }
                        else
                        {
                            bgColor = parent->GetColorPair()->GetBGColor().rgb() & 0xffffff;
                        }
                    }
                }
                quint16 bgColorID = layoutDefinition.appendCLUTGetIndex(bgColor);

                if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::SW)
                {
                    fgColorID = 0;
                    bgColorID = 1;
                }

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(widget->GetUIWidgetID()) + "u,\n" \
                                   "        .currentValue = " + QString::number(widget->GetNumberOfBlocks()) + "u,\n" \
                                   "        .hasFocus = FALSE,\n" \
                                   "        .spacing = " + QString::number(widget->GetSpacing()) + "u ,\n" \
                                   "        .numberOfBlocks = " + QString::number(widget->GetNumberOfBlocks()) + "u ,\n" \
                                   "        .min = " + QString::number(widget->GetMin()) + "u ,\n" \
                                   "        .max = " + QString::number(widget->GetMax()) + "u ,\n" \
                                   "        .minBlockHeight = " + QString::number(widget->GetMinBlockHeight()) + "u ,\n" \
                                   "        .maxBlockHeight = " + QString::number(widget->GetMaxBlockHeight()) + "u ,\n" \
                                   "        .selectedItemBGColor = " + QString::number(bgColorID) + "u ,\n" \
                                   "        .selectedItemFGColor = " + QString::number(fgColorID) + "u ,\n" \
                                   "        .readCurrentColumnLevel = GCL_Read_"  + widget->GetUIWidgetUniqueName() + "_" + currentColumnLevelSink->SignalName() + ",\n"\
                                   "        .writeCurrentColumnLevel = GCL_Write_"  + widget->GetUIWidgetUniqueName() + "_" + selectedColumnLevelSource->SignalName() + "\n"\
                                   "    }");
            }
        }
    }

    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgets.count() != 0)
    {
        sourceLines.append("#define ILLUMINATION_LEVEL_WIDGET_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        sourceLines.append("#define ILLUMINATION_LEVEL_WIDGET_CONFIG_COUNT    0U");
    }
    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("const uint32 ColumnLevelWidgetConfigCount = ILLUMINATION_LEVEL_WIDGET_CONFIG_COUNT;");
    sourceLines.append("static ColumnLevelWidgetCfg_t columnLevelWidgetConfigs[ILLUMINATION_LEVEL_WIDGET_CONFIG_COUNT] =");
    sourceLines.append("{");
    sourceLines.append(configLines.join(",\n"));
    sourceLines.append("};");
    sourceLines.append("ColumnLevelWidgetCfg_t* const ColumnLevelWidgetConfigs = columnLevelWidgetConfigs;");
    sourceLines.append("");
    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("");

    AddGeneratedFile(m_Path, sourceLines.join("\n"));
}

void ColumnLevelUIWidgetGenerator::OnGenerateFiles()
{
    generateSource();
}
