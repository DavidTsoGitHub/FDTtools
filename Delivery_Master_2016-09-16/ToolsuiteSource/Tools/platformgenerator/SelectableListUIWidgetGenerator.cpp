#include "SelectableListUIWidgetGenerator.h"

#include "ColorPair.h"
#include "GCLSignalSource.h"
#include "LayoutDefinition.h"
#include "Project.h"
#include "BSPDisplay.h"
#include "DisplayArea.h"
#include "ProjectObject.h"

SelectableListUIWidgetGenerator::SelectableListUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project) :
    m_uiWidgetList(uiWidgetList),
    m_project(project),
    m_layoutDefinition(m_project.GetLayoutDefinition())
{
}

SelectableListUIWidgetGenerator::~SelectableListUIWidgetGenerator()
{
}

void SelectableListUIWidgetGenerator::generateConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"SelectableListWidgetCfg.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");
    fileLines.append("#include \"TextRenderer.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        foreach(UIWidgetBase* uiwidget, m_uiWidgetList)
        {
            if (uiwidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_SelectableList)
            {
                SelectableListWidget* selectableListWidget = static_cast<SelectableListWidget*>(uiwidget);

                const GCLSignalSink* mappingsSink = selectableListWidget->GetSignalSink("LabelToValueMappings");
                const GCLSignalSink* currentValueSink = selectableListWidget->GetSignalSink("CurrentValue");
                const GCLSignalSource* selectedValueSource = selectableListWidget->GetSignalSource("SelectedValue");

                quint32 fgColor = selectableListWidget->GetSecondaryColorPair()->GetFGColor().rgb() & 0xffffff;
                quint16 fgColorID = m_layoutDefinition.appendCLUTGetIndex(fgColor);

                quint32 bgColor = selectableListWidget->GetSecondaryColorPair()->GetBGColor().rgb() & 0xffffff;

                if (selectableListWidget->GetSecondaryColorPair()->GetBGIsTransparent())
                {
                    DisplayArea *parent = NULL;
                    if (dynamic_cast<DisplayArea*>(uiwidget->GetParent()) != NULL)
                    {
                        parent = dynamic_cast<DisplayArea*>(uiwidget->GetParent());
                    }
                    else
                    {
                        // If parent isn't a DisplayArea then it is a MenuItem and we get color info from MenuArea
                        parent = m_layoutDefinition.GetMenuArea();
                    }

                    if (parent != NULL)
                    {
                        if (parent->GetColorPair()->GetBGIsTransparent())
                        {
                            bgColor = m_layoutDefinition.GetStartChromaKeyColor().rgb() & 0xffffff;
                        }
                        else
                        {
                            bgColor = parent->GetColorPair()->GetBGColor().rgb() & 0xffffff;
                        }
                    }
                }
                quint16 bgColorID = m_layoutDefinition.appendCLUTGetIndex(bgColor);

                if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::SW)
                {
                    fgColorID = 0;
                    bgColorID = 1;
                }

                if (selectableListWidget->GetSelectable())
                {
                    configLines.append("    {\n" \
                                       "        .widgetID = " + QString::number(selectableListWidget->GetUIWidgetID()) + ",\n" \
                                       "        .currentValue = 0U,\n" \
                                       "        .hasFocus = FALSE,\n" \
                                       "        .isSelectable = " + QString::number(selectableListWidget->GetSelectable()) + ",\n" \
                                       "        .readLabelMappings = GCL_Read_" +  selectableListWidget->GetUIWidgetUniqueName() + "_" + mappingsSink->SignalName() + ",\n" \
                                       "        .readCurrentValue = GCL_Read_" +  selectableListWidget->GetUIWidgetUniqueName() + "_" + currentValueSink->SignalName() + ",\n" \
                                       "        .writeSelectedValue = GCL_Write_" +  selectableListWidget->GetUIWidgetUniqueName() + "_" + selectedValueSource->SignalName() + ",\n" \
                                       "        .showNumbering = " + (selectableListWidget->GetShowNumbering() ? "TRUE" : "FALSE") + ",\n" \
                                       "        .displayItemCount = " + QString::number(selectableListWidget->GetDisplayItemCount()) + "u,\n" \
                                       "        .selectedItemFgColor = " + QString::number(fgColorID) + "u,\n" \
                                       "        .selectedItemBgColor = " + QString::number(bgColorID) + "u,\n" \
                                       "        .textAlignment = " + selectableListWidget->GetTextAlignmentEnumString() + ",\n" \
                                       "        .mappings = (LabelMappings*)0U\n" \
                                       "    }");
                }
                else
                {
                    configLines.append("    {\n" \
                                       "        .widgetID = " + QString::number(selectableListWidget->GetUIWidgetID()) + ",\n" \
                                       "        .currentValue = 0U,\n" \
                                       "        .hasFocus = FALSE,\n" \
                                       "        .isSelectable = " + QString::number(selectableListWidget->GetSelectable()) + ",\n" \
                                       "        .readLabelMappings = GCL_Read_" +  selectableListWidget->GetUIWidgetUniqueName() + "_" + mappingsSink->SignalName() + ",\n" \
                                       "        .readCurrentValue = NULL,\n" \
                                       "        .writeSelectedValue = NULL,\n" \
                                       "        .showNumbering = " + (selectableListWidget->GetShowNumbering() ? "TRUE" : "FALSE") + ",\n" \
                                       "        .displayItemCount = " + QString::number(selectableListWidget->GetDisplayItemCount()) + "u,\n" \
                                       "        .selectedItemFgColor = " + QString::number(fgColorID) + "u,\n" \
                                       "        .selectedItemBgColor = " + QString::number(bgColorID) + "u,\n" \
                                       "        .textAlignment = " + selectableListWidget->GetTextAlignmentEnumString() + ",\n" \
                                       "        .mappings = (LabelMappings*)0U\n" \
                                       "    }");
                }
            }
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999,\n" \
                           "        .currentValue = 0U,\n" \
                           "        .hasFocus = FALSE,\n" \
                           "        .readLabelMappings = NULL,\n" \
                           "        .readCurrentValue = NULL,\n" \
                           "        .writeSelectedValue = NULL,\n" \
                           "        .mappings = (LabelMappings*)0U\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define SELECTABLE_LIST_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define SELECTABLE_LIST_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 SelectableListConfigCount = SELECTABLE_LIST_CONFIG_COUNT;");
    fileLines.append("static SelectableListCfg_t selectableListConfigs[SELECTABLE_LIST_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("SelectableListCfg_t* const SelectableListConfigs = selectableListConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("SelectableList/SelectableListWidgetCfg.c")), fileLines.join("\n"));
}

void SelectableListUIWidgetGenerator::OnGenerateFiles()
{
    generateConfigurationSourceFile();
}


