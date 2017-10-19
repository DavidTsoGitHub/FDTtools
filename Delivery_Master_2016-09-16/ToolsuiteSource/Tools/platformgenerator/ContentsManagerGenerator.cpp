#include "ContentsManagerGenerator.h"

#include "AnimationDefinition.h"
#include "BSPDisplay.h"
#include "DynamicImageWidget.h"
#include "GCLSignalSource.h"
#include "IBSP_DCU_BPPModes.h"
#include "ImageWidget.h"
#include "Layout.h"
#include "LayoutDefinition.h"
#include "Logger.h"
#include "Project.h"
#include "TransitionCondition.h"
#include "ColorPair.h"

#include <QMap>

//static QString convertUintToBPP(quint32 value);

ContentsManagerGenerator::ContentsManagerGenerator(Project* project) :
    m_display(project->GetBSPSpecification().GetDisplay()),
    m_layoutDefinition(project->GetLayoutDefinition()),
    m_animationDefinition(project->GetAnimationDefinition())
{
    m_ContentsManagerConfigHeaderFile = QString(PlatformPaths::CreateServicePath("ContentsManager")).append("/ContentsManager_cfg.h");
    m_ContentsManagerConfigSourceFile = QString(PlatformPaths::CreateServicePath("ContentsManager")).append("/ContentsManager_cfg.c");
}

ContentsManagerGenerator::~ContentsManagerGenerator()
{}

void ContentsManagerGenerator::OnGenerateFiles()
{
    LOG_INFO("Generating ContentsManager...");
    generateContentsManagerConfigurationSource();
    generateContentsManagerConfigurationHeader();
    LOG_INFO("Generating ContentsManager...done!");
}

void ContentsManagerGenerator::generateContentsManagerConfigurationHeader()
{
    QStringList fileLines;

    fileLines.append("#ifndef CONTENTSMANAGERCFG_H");
    fileLines.append("#define CONTENTSMANAGERCFG_H");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"UIWidget.h\"");
    fileLines.append("#include \"LayerConfiguration.h\"");
    fileLines.append("#include \"GCL_TransitionManager.h\"");
    fileLines.append("#include \"GCL_LayoutManager.h\"");
    if (m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("#include \"Image.h\"");
        fileLines.append("#include \"AnimationManager.h\"");
        fileLines.append("#include \"Animations_cfg.h\"");
    }
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");

    if (m_display->GetRenderingOrientation() == BSPDisplay::CounterClockWise && m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("#define DISPLAY_ROTATED");
        fileLines.append("");
    }

    int startupLayoutIndex = -1;
    int menuLayoutIndex = -127;
    int menuLayerIndex = -127;
    int index = 0;
    bool startupScreenDefined = false;
    bool checkMessageEnabled = false;
    foreach (Layout* layout, m_layoutDefinition.Layouts())
    {
        if (layout->HasMenu())
        {
            menuLayoutIndex = index;
            menuLayerIndex = layout->GetMenuLayerIndex();
        }

        if (layout->IsStartupScreen())
        {
            startupLayoutIndex = index;
            startupScreenDefined = true;
        }

        if(layout->IsCheckMessageScreen())
        {
            checkMessageEnabled = true;
        }
        ++index;
    }

    if (m_layoutDefinition.StartupEnabled())
    {
        fileLines.append("#define SHOW_STARTUP_LAYOUT");
        fileLines.append("#define STARTUP_LAYOUT                (" + QString::number(startupLayoutIndex) + ")");
        fileLines.append("#define CONTENTS_MANAGER_STARTUP_TIME (" + QString::number(m_layoutDefinition.StartupScreenDisplayTime()*1000) + "u)");
    }

    quint32 layoutCount = m_layoutDefinition.Layouts().count();
    int mainLayoutIndex = m_layoutDefinition.Layouts().indexOf(m_layoutDefinition.GetMainLayout());

    if (m_layoutDefinition.StartupEnabled() == false && startupScreenDefined == true)
    {
        --layoutCount;
        --mainLayoutIndex;
        --menuLayoutIndex;
    }

    if(checkMessageEnabled)
    {
        --layoutCount; //CheckmessageLayout is not a unique layout
    }

    fileLines.append("#define MAIN_LAYOUT                   (" + QString::number(mainLayoutIndex) + ")");
    fileLines.append("#define CONTENTS_MANAGER_LAYOUT_COUNT (" + QString::number(layoutCount) + ")");

    if (m_layoutDefinition.HasMenu())
    {
        fileLines.append("#define MENU_AREA_LAYER_INDEX         (" + QString::number(menuLayerIndex) + ")");
        fileLines.append("#define MENU_LAYOUT_INDEX             (" + QString::number(menuLayoutIndex) + ")");
    }

    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    boolean activeLayout;");
    fileLines.append("    signal_transition_t* signalTransitions;");
    fileLines.append("    uint8 signalTransitionsCount;");
    fileLines.append("    sint8 buttonUpLayout;");
    fileLines.append("    sint8 buttonDownLayout;");
    fileLines.append("    sint8 buttonSetLayout;");
    fileLines.append("    sint8 buttonClearLayout;");
    fileLines.append("    void (*GCLWriteUpButtonEvent)(void);");
    fileLines.append("    void (*GCLWriteDownButtonEvent)(void);");
    fileLines.append("    void (*GCLWriteSetButtonEvent)(void);");
    fileLines.append("    void (*GCLWriteClearButtonEvent)(void);");
    fileLines.append("    uint8 areaCount;");
    fileLines.append("    display_area_definition_t* areas;");
    fileLines.append("    uint8 backgroundColorID;");
    fileLines.append("    const ui_widget_t* focusedWidget;");
    fileLines.append("} display_layout_definition_t;");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("extern display_layout_definition_t ContentsManager_Layouts[" + QString::number(layoutCount) + "];");
    fileLines.append("extern display_area_definition_t* ContentsManager_MenuArea;");
    fileLines.append("extern const display_layout_definition_t* ContentsManager_MenuLayout;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");
    fileLines.append("#endif // CONTENTSMANAGERCFG_H");
    fileLines.append("");

    AddGeneratedFile(m_ContentsManagerConfigHeaderFile, fileLines.join("\n"));
}

void ContentsManagerGenerator::generateContentsManagerConfigurationSource()
{
    QStringList fileLines;
    QStringList includeLines;

    includeLines.append("#include \"ContentsManager.h\"");
    includeLines.append("#include \"ContentsManager_cfg.h\"");
    includeLines.append("#include \"LayerConfiguration.h\"");

    QStringList widgetDefinitions;
    QStringList areaDefinitions;
    QStringList transitionsDefinitions;
    QStringList layoutDefinitions;
    QStringList animationMappings;

    bool startupScreenDefined = false;
    bool checkMessageEnabled = false;
    Layout* checkMessageLayout = NULL;
    foreach (Layout* layout, m_layoutDefinition.Layouts())
    {
        if (layout->IsStartupScreen())
        {
            startupScreenDefined = true;
        }

        if(layout->IsCheckMessageScreen())
        {
            checkMessageEnabled = true;
            checkMessageLayout = layout;
        }
    }

    quint32 layoutCount = m_layoutDefinition.Layouts().count();
    int mainLayoutIndex = m_layoutDefinition.Layouts().indexOf(m_layoutDefinition.GetMainLayout());
    int menuLayoutIndex = m_layoutDefinition.Layouts().indexOf(m_layoutDefinition.GetMenuLayout());

    if (m_layoutDefinition.StartupEnabled() == false && startupScreenDefined == true)
    {
        --layoutCount;
        --mainLayoutIndex;
        --menuLayoutIndex;
    }

    if(checkMessageEnabled)
    {
         --layoutCount; //CheckmessageLayout is not a unique layout
    }

    layoutDefinitions.append("display_layout_definition_t ContentsManager_Layouts[" + QString::number(layoutCount) + "] =");
    layoutDefinitions.append("{");

    QStringList layoutDefinition;
    quint8 layoutIndex = 0;
    //quint8 menuLayoutIndex = 0;
    quint32 animationCount = 0;

    foreach (Layout* layout, m_layoutDefinition.Layouts())
    {
        if(layout->IsCheckMessageScreen())
        {
            continue; //CheckmessageScreen is not a unique layout.
        }

        if(layout->IsStartupScreen() && m_layoutDefinition.StartupEnabled() == false)
        {
            continue;
        }

        QStringList areas;
        quint8 areaIndex = 0;
        quint8 menuAreaIndex = 0;
        quint32 bufferOffset = 0;
        QString focusedWidgetString = "NULL";
        quint8 layerIndex = m_display->GetNumberOfLayers() - 1;

        if (!layout->IsStartupScreen())
        {
            if (layout->GetSignalTransitions().count() > 0)
            {
                transitionsDefinitions.append("static signal_transition_t Layout" + QString::number(layoutIndex) + "_SignalTransitions[" + QString::number(layout->GetSignalTransitions().count()) + "] = ");
                transitionsDefinitions.append("{");
                QStringList transitions;

                foreach (TransitionClause* transition, layout->GetSignalTransitions())
                {
                    QStringList transitionString;
                    TransitionCondition* firstCondition = transition->GetFirstCondition();
                    const GCLSignalSink* firstConditionSink = firstCondition->GetSignalSinks().first();
                    transitionString.append("    { .firstConditionGCLSink = GCL_Read_" + firstConditionSink->ComponentName() + "_" + firstConditionSink->SignalName() +
                                       ", .firstConditionCompareOperator = " + getCompareString(firstCondition->GetCompareOperator()) +
                                       ", .firstConditionCompareValue = " + QString::number(firstCondition->GetConditionValue()) +
                                       ", .connective = " + getConnectiveString(transition->GetConnective()));

                    if (transition->GetConnective() != LayoutTransitionTypes::NONE)
                    {
                        TransitionCondition* secondCondition = transition->GetSecondCondition();
                        const GCLSignalSink* secondConditionSink = secondCondition->GetSignalSinks().first();
                        transitionString.append(", .secondConditionGCLSink = GCL_Read_" + secondConditionSink->ComponentName() + "_" + secondConditionSink->SignalName() +
                                           ", .secondConditionCompareOperator = " + getCompareString(secondCondition->GetCompareOperator()) +
                                           ", .secondConditionCompareValue = " + QString::number(secondCondition->GetConditionValue()) + " }");
                    }
                    else
                    {
                        transitionString.append(", .secondConditionGCLSink = NULL" \
                                           ", .secondConditionCompareOperator = EQUALS_TO" \
                                           ", .secondConditionCompareValue = 0 }");
                    }

                    transitions.append(transitionString.join(""));
                }
                transitionsDefinitions.append(transitions.join(",\n"));
                transitionsDefinitions.append("};");
                transitionsDefinitions.append("");
            }

        }

        if(!layout->IsCheckMessageScreen() && !layout->IsStartupScreen() && m_layoutDefinition.HasCheckMessageLayout())
        {
            bool checkMessageAreaNeeded = false;
            Project* project = static_cast<Project*>(m_layoutDefinition.GetParent());
            foreach (CheckMessage* checkMessage, project->GetCheckMessagesDefinition().CheckMessages())
            {
                if (checkMessage->IsAllowedInLayout(layout->GetID()))
                {
                    checkMessageAreaNeeded = true;
                }
            }

            if (checkMessageAreaNeeded)
            {
                layout->AddDisplayArea(checkMessageLayout->DisplayAreas().first()); // CheckMessageScreen only contain one area
            }
        }

        if (layout->HasMenu())
        {
            //menuLayoutIndex = layoutIndex;
            menuAreaIndex = layout->GetMenuAreaIndex();
        }

        areaDefinitions.append("static display_area_definition_t Layout" + QString::number(layoutIndex) + "_Areas[" + QString::number(layout->DisplayAreas().count()) + "] =");
        areaDefinitions.append("{");

        QListIterator<DisplayArea*> reverseAreaIterator(layout->DisplayAreas());
        reverseAreaIterator.toBack();
        while (reverseAreaIterator.hasPrevious())
        {
            DisplayArea* area = reverseAreaIterator.previous();
            quint8 currentAreaLayerID = layerIndex;
            if (menuAreaIndex == areaIndex)
            {
                layout->SetMenuLayerIndex(currentAreaLayerID);
            }

            QString areaWidgets;
            if(area->UIWidgets().count() < 1)
            {
                areaWidgets = "(const ui_widget_t*)0";
            }
            else
            {
                areaWidgets = "Layout" + QString::number(layoutIndex) + "_Area" + QString::number(areaIndex) + "_Widgets";
            }

            if (m_display->GetRenderingType() == BSPDisplay::HW)
            {
                qint8 areaLayerIndex = -1;
                bool indexSet = false;
                if (layout->HasMenu())
                {
                    if (areaIndex == menuAreaIndex)
                    {
                        areaLayerIndex = layerIndex--;
                        indexSet = true;
                    }
                }

                if (!indexSet)
                {
                    if (area->NeedsLayer())
                    {
                        areaLayerIndex = layerIndex--;
                    }
                    else
                    {
                        areaLayerIndex = -1;
                    }
                }
                quint32 areaBufferSize = 0u;
                quint32 areaBufferOffset = 0u;

                if (areaLayerIndex != -1)
                {
                    areaBufferSize = area->GetHeight() * area->GetWidth();
                    areaBufferOffset = bufferOffset;
                }

                quint32 fgColor = area->GetColorPair()->GetFGColor().rgb() & 0xffffff;
                quint16 fgColorID = m_layoutDefinition.appendCLUTGetIndex(fgColor);

                quint32 bgColor = 0;
                if (area->GetColorPair()->GetBGIsTransparent())
                {
                    bgColor = m_layoutDefinition.GetStartChromaKeyColor().rgb() & 0xffffff;
                }
                else
                {
                    bgColor = area->GetColorPair()->GetBGColor().rgb() & 0xffffff;
                }
                quint16 bgColorID = m_layoutDefinition.appendCLUTGetIndex(bgColor);

                areas.append("    { .writableArea = { 0, 0, 0, 0, 0 }" \
                             ", .layerID = " + QString::number(areaLayerIndex) +
                             ", .x = " + QString::number(area->GetX()) +
                             "u, .y = " + QString::number(area->GetY()) +
                             "u, .width = " + QString::number(area->GetWidth()) +
                             "u, .height = " + QString::number(area->GetHeight()) +
                             "u, .border = " + QString::number(area->GetBorder()) +
                             "u, .fgColor = " + QString::number(fgColorID) +
                             "u, .bgColor = " + QString::number(bgColorID) +
                             "u, .widgetCount = " + QString::number(area->UIWidgets().count()) +
                             "u, .bufferOffset = " + QString::number(areaBufferOffset) +
                             "u, .bufferSize = " + QString::number(areaBufferSize) +
                             "u, .isCheckMessageArea = " + (area->IsCheckMessageArea() == true ? "TRUE" : "FALSE") +
                             ", .activeBuffer = NULL, .passiveBuffer = NULL, .widgets = " + areaWidgets + 
                             ", .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0 }");

                bufferOffset += areaBufferSize;
            }
            else
            {
                 areas.append("    { .writableArea = { 0, 0, 0, 0, 0 }" \
                             ", .layerID = 0" \
                             ", .x = " + QString::number(area->GetX()) +
                             "u, .y = " + QString::number(area->GetY()) +
                             "u, .width = " + QString::number(area->GetWidth()) +
                             "u, .height = " + QString::number(area->GetHeight()) +
                             "u, .border = " + QString::number(area->GetBorder()) +
                             "u, .fgColor = 1" +
                             "u, .bgColor = 0" +
                             "u, .widgetCount = " + QString::number(area->UIWidgets().count()) +
                             "u, .bufferOffset = 0" +
                             "u, .bufferSize = 0" +
                             "u, .isCheckMessageArea = " + (area->IsCheckMessageArea() == true ? "TRUE" : "FALSE") +
                             ", .activeBuffer = NULL, .passiveBuffer = NULL, .widgets = " + areaWidgets + 
                             ", .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0 }");
            }

            if(area->UIWidgets().count() > 0)
            {
                widgetDefinitions.append("static const ui_widget_t Layout" + QString::number(layoutIndex) + "_Area" + QString::number(areaIndex) + "_Widgets[" + QString::number(area->UIWidgets().count()) + "] = ");
                widgetDefinitions.append("{");
                QStringList widgetDefinition;

                quint8 widgetIndex = 0;
                QListIterator<UIWidgetBase*> reverseWidgetIterator(area->UIWidgets());
                reverseWidgetIterator.toBack();
                while (reverseWidgetIterator.hasPrevious())
                {
                    UIWidgetBase* uiWidget = reverseWidgetIterator.previous();
                    includeLines.append("#include \"" + uiWidget->GetUIWidgetTypeName() + "Widget.h\"");
                    QString layerID = QString::number((uiWidget->GetNbrOfLayersNeeded() > 0 ) ? layerIndex-- : currentAreaLayerID);

                    if (uiWidget->GetCanHandleFocus() && !layout->HasMenu())
                    {
                        focusedWidgetString = "&Layout" + QString::number(layoutIndex) + "_Area" + QString::number(areaIndex) + "_Widgets[" + QString::number(widgetIndex) + "]";
                    }

                    if (m_display->GetRenderingType() == BSPDisplay::HW)
                    {
                        QString animationCfg = "";
                        if((uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage) || (uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image))
                        {
                            quint32 animation1ID = 0;
                            quint32 animation2ID = 0;
                            if (uiWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
                            {
                                animation1ID = static_cast<DynamicImageWidget*>(uiWidget)->GetAnimationID(1);
                                animation2ID = static_cast<DynamicImageWidget*>(uiWidget)->GetAnimationID(2);
                            }
                            else
                            {
                                animation1ID = static_cast<ImageWidget*>(uiWidget)->GetAnimationID(1);
                                animation2ID = static_cast<ImageWidget*>(uiWidget)->GetAnimationID(2);
                            }

                            if(animation1ID != 0)
                            {
                                quint16 startIndex = 0u;
                                if (uiWidget->GetAnimationOnLoadBehavior(1) == UIWidgetBase::SHOW_DEFAULT)
                                {
                                    startIndex = m_animationDefinition.GetAnimationByID(animation1ID)->GetDefaultStep();
                                }

                                animationMappings.append("    { .animationID = " + QString::number(animation1ID) + ", .layerID = " + layerID + ", .step = 0, .incrementStep = TRUE, .index = 0, .startIndex = " + QString::number(startIndex) + ", .endIndex = 0, .triggerActive = FALSE, .animationUpdated = FALSE, .lastIndication = FALSE, .allreadyTriggered = FALSE, .animationRunning = FALSE, .onLoadRunOneCycleRunning = FALSE }");
                                animationCfg.append(", .animation1 = &AnimationMappings[" + QString::number(animationCount) + "]");
                                ++animationCount;
                            }
                            else
                            {
                                animationCfg.append(", .animation1 = (animation_mapping_t*)0");
                            }

                            if(animation2ID != 0)
                            {
                                quint16 startIndex = 0u;
                                if (uiWidget->GetAnimationOnLoadBehavior(2) == UIWidgetBase::SHOW_DEFAULT)
                                {
                                    startIndex = m_animationDefinition.GetAnimationByID(animation2ID)->GetDefaultStep();
                                }

                                animationMappings.append("    { .animationID = " + QString::number(animation2ID) + ", .layerID = " + layerID + ", .step = 0, .incrementStep = TRUE, .index = 0, .startIndex = " + QString::number(startIndex) + ", .endIndex = 0, .triggerActive = FALSE, .animationUpdated = FALSE, .lastIndication = FALSE, .allreadyTriggered = FALSE, .animationRunning = FALSE, .onLoadRunOneCycleRunning = FALSE }");
                                animationCfg.append(", .animation2 = &AnimationMappings[" + QString::number(animationCount) + "]");
                                ++animationCount;
                            }
                            else
                            {
                                animationCfg.append(", .animation2 = (animation_mapping_t*)0");
                            }
                        }
                        else
                        {
                            animationCfg.append(", .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0");
                        }

                        quint32 fgColor = uiWidget->GetPrimaryColorPair()->GetFGColor().rgb() & 0xffffff;
                        quint16 fgColorID = m_layoutDefinition.appendCLUTGetIndex(fgColor);

                        quint16 bgColorID;
                        if (uiWidget->GetPrimaryColorPair()->GetBGIsTransparent())
                        {
                            quint32 bgColor;

                            if (area->GetColorPair()->GetBGIsTransparent())
                            {
                                bgColor = m_layoutDefinition.GetStartChromaKeyColor().rgb() & 0xffffff;
                            }
                            else
                            {
                                bgColor = area->GetColorPair()->GetBGColor().rgb() & 0xffffff;
                            }

                            bgColorID = m_layoutDefinition.appendCLUTGetIndex(bgColor);
                        }
                        else
                        {
                            quint32 bgColor = uiWidget->GetPrimaryColorPair()->GetBGColor().rgb() & 0xffffff;
                            bgColorID = m_layoutDefinition.appendCLUTGetIndex(bgColor);
                        }

                        widgetDefinition.append("    { .widgetID = " + QString::number(uiWidget->GetUIWidgetID()) +
                                             "u, .layerID = " + layerID +
                                             ", .x = " + QString::number((uiWidget->GetNbrOfLayersNeeded() > 0 ) ? (area->GetX() + uiWidget->GetX()) : uiWidget->GetX()) +
                                             "u, .y = " + QString::number((uiWidget->GetNbrOfLayersNeeded() > 0 ) ? (area->GetY() + uiWidget->GetY()) : uiWidget->GetY()) +
                                             "u, .width = " + QString::number(uiWidget->GetWidth()) +
                                             "u, .height = " + QString::number(uiWidget->GetHeight()) +
                                             "u, .boxed = " + QString::number(uiWidget->GetBoxed()) +
                                             "u, .fgColor = " + QString::number(fgColorID) +
                                             "u, .bgColor = " + QString::number(bgColorID) +
                                             "u, .bpp = " + m_display->GetBSPDCUBPPModes()->GetBitsPerPixelForModeString(uiWidget->GetBPPMode()) +
                                             ", .functions = &" + uiWidget->GetUIWidgetTypeName() + "WidgetFuncs" + animationCfg +
                                             " }");
                    }
                    else
                    {
                        widgetDefinition.append("    { .widgetID = " + QString::number(uiWidget->GetUIWidgetID()) +
                                             "u, .layerID = 0" \
                                             ", .x = " + QString::number(uiWidget->GetX()) +
                                             "u, .y = " + QString::number(uiWidget->GetY()) +
                                             "u, .width = " + QString::number(uiWidget->GetWidth()) +
                                             "u, .height = " + QString::number(uiWidget->GetHeight()) +
                                             "u, .boxed = " + QString::number(uiWidget->GetBoxed()) +
                                             "u, .fgColor = 1" \
                                             "u, .bgColor = 0" \
                                             "u, .functions = &" + uiWidget->GetUIWidgetTypeName() + "WidgetFuncs" \
                                             ", .animation1 = (animation_mapping_t*)0, .animation2 = (animation_mapping_t*)0" \
                                             " }");
                    }
                    ++widgetIndex;
                }
                widgetDefinitions.append(widgetDefinition.join(",\n"));
                widgetDefinitions.append("};");
                widgetDefinitions.append("");

            }
            ++areaIndex;
        }

        areaDefinitions.append(areas.join(",\n"));
        areaDefinitions.append("};");
        areaDefinitions.append("");


        quint32 color = layout->GetBackgroundColor().rgb() & 0xffffff;
        quint16 layoutColorID = m_layoutDefinition.appendCLUTGetIndex(color);

        if (layout->IsStartupScreen())
        {
            layoutDefinition.append("    {\n" \
                                    "        .activeLayout = FALSE,\n" \
                                    "        .signalTransitions = NULL, \n" \
                                    "        .signalTransitionsCount = 0, \n" \
                                    "        .buttonUpLayout = -1, .buttonDownLayout = -1, .buttonSetLayout = -1, .buttonClearLayout = -1,\n" \
                                    "        .GCLWriteUpButtonEvent = NULL, .GCLWriteDownButtonEvent = NULL, .GCLWriteSetButtonEvent = NULL, .GCLWriteClearButtonEvent = NULL,\n" \
                                    "        .areaCount = " + QString::number(layout->DisplayAreas().count()) + "u,\n" \
                                    "        .areas = Layout" + QString::number(layoutIndex) + "_Areas, \n" \
                                    "        .backgroundColorID = " + QString::number(layoutColorID) + "u,\n" \
                                    "        .focusedWidget = NULL\n" \
                                    "    }");
        }
        else
        {
            QString buttonTransitionsRow = getButtonMappingsString(layout);
            QString gclString = getGCLString(layout);
            layoutDefinition.append("    {\n" \
                                    "        .activeLayout = FALSE,\n" \
                                    "        .signalTransitions = " + ((layout->GetSignalTransitions().count() > 0) ? ("Layout" + QString::number(layoutIndex) + "_SignalTransitions,\n") : "NULL,\n") + "" \
                                    "        .signalTransitionsCount = " + QString::number(layout->GetSignalTransitions().count()) + ",\n" \
                                    "        " + buttonTransitionsRow + ",\n" \
                                    "        " + gclString + ",\n" \
                                    "        .areaCount = " + QString::number(layout->DisplayAreas().count()) + "u,\n" \
                                    "        .areas = Layout" + QString::number(layoutIndex) + "_Areas,\n" \
                                    "        .backgroundColorID = " + QString::number(layoutColorID) + "u,\n" \
                                    "        .focusedWidget = " + focusedWidgetString + "\n" \
                                    "    }");
        }
        ++layoutIndex;
    }

    layoutDefinitions.append(layoutDefinition.join(",\n"));
    layoutDefinitions.append("};");
    layoutDefinitions.append("");

    fileLines.append("//lint -save");
    fileLines.append("/********************************* Includes **********************************/");
    includeLines.removeDuplicates();
    fileLines.append(includeLines.join("\n"));
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("");

    if (m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("static animation_mapping_t AnimationMappings[" + QString::number(animationCount) + "u] =");
        fileLines.append("{");
        fileLines.append(animationMappings.join(",\n"));
        fileLines.append("};");
        fileLines.append("");
    }
    fileLines.append(widgetDefinitions.join("\n"));
    fileLines.append(areaDefinitions.join("\n"));
    fileLines.append(transitionsDefinitions.join("\n"));
    fileLines.append(layoutDefinitions.join("\n"));
    if (m_layoutDefinition.HasMenu())
    {
        fileLines.append("display_area_definition_t* ContentsManager_MenuArea = &Layout" + QString::number(menuLayoutIndex) + "_Areas[" + QString::number(m_layoutDefinition.GetMenuLayout()->GetMenuAreaIndex()) + "];");
        fileLines.append("");
    }

    fileLines.append("");

    AddGeneratedFile(m_ContentsManagerConfigSourceFile, fileLines.join("\n"));
}

//static QString convertUintToBPP(quint32 value)
//{
//    switch (value)
//    {
//        case 3:
//            return "BPP8";
//            break;
//        case 4:
//            return "BPP16";
//            break;
//        default:
//            // Do nothing
//            break;
//    }
//    return NULL;
//}

QString ContentsManagerGenerator::getConnectiveString(LayoutTransitionTypes::TransitionConnective connective)
{
    QString connectiveString;

    switch (connective)
    {
        case LayoutTransitionTypes::NONE:
            connectiveString = "NONE";
            break;
        case LayoutTransitionTypes::AND:
            connectiveString = "AND";
            break;
        case LayoutTransitionTypes::XOR:
            connectiveString = "XOR";
            break;
        default:
            connectiveString = "NONE";
            break;
    }

    return connectiveString;
}

QString ContentsManagerGenerator::getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition)
{
    QString compareString;

    switch (compareCondition)
    {
        case LayoutTransitionTypes::EqualTo:
            compareString = "EQUALS_TO";
            break;
        case LayoutTransitionTypes::GreaterThan:
            compareString = "GREATER_THAN";
            break;
        case LayoutTransitionTypes::EqualOrGreaterThan:
            compareString = "EQUALS_OR_GREATER_THAN";
            break;
        case LayoutTransitionTypes::LessThan:
            compareString = "LESS_THAN";
            break;
        case LayoutTransitionTypes::EqualOrLessThan:
            compareString = "EQUALS_OR_LESS_THAN";
            break;
        default:
            compareString = "EQUALS_TO";
            break;
    }

    return compareString;
}

QString ContentsManagerGenerator::getButtonMappingsString(Layout* layout)
{
    qint8 buttonUpLayout = -1;
    qint8 buttonDownLayout = -1;
    qint8 buttonSetLayout = -1;
    qint8 buttonClearLayout = -1;

    QMap<Layout::LayoutButton, quint8> buttonMap = layout->GetButtonTransitions();
    QMap<Layout::LayoutButton, quint8>::iterator iterator;

    for (iterator = buttonMap.begin(); iterator != buttonMap.end(); ++iterator)
    {
        Layout::LayoutButton button = static_cast<Layout::LayoutButton>(iterator.key());

        quint8 buttonValue = -1;

        if (m_layoutDefinition.HasStartupLayout() && !m_layoutDefinition.StartupEnabled() && (qint8)iterator.value() > 0)
        {
            buttonValue = (qint8)iterator.value() - 1;
        }
        else
        {
            buttonValue = (qint8)iterator.value();
        }

        switch (button)
        {
            case Layout::LAYOUT_UP:
                buttonUpLayout = buttonValue;
                break;
            case Layout::LAYOUT_DOWN:
                buttonDownLayout = buttonValue;
                break;
            case Layout::LAYOUT_SET:
                buttonSetLayout = buttonValue;
                break;
            case Layout::LAYOUT_CLEAR:
                buttonClearLayout = buttonValue;
            default:
                // Do nothing
                break;
        }
    }

    QString returnString = ".buttonUpLayout = " + QString::number(buttonUpLayout) \
                       + ", .buttonDownLayout = " + QString::number(buttonDownLayout) \
                       + ", .buttonSetLayout = " + QString::number(buttonSetLayout) \
                       + ", .buttonClearLayout = " + QString::number(buttonClearLayout);

    return returnString;
}

QString ContentsManagerGenerator::getGCLString(Layout* layout)
{
    QList<GCLSignalSource*> sources = layout->GetSignalSources();

    QString returnString = ".GCLWriteUpButtonEvent = GCL_Write_" + sources.at(0)->ComponentName() + "_" + sources.at(0)->SignalName() +
                         ", .GCLWriteDownButtonEvent = GCL_Write_" + sources.at(1)->ComponentName() + "_" + sources.at(1)->SignalName() +
                         ", .GCLWriteSetButtonEvent = GCL_Write_" + sources.at(2)->ComponentName() + "_" + sources.at(2)->SignalName() +
                         ", .GCLWriteClearButtonEvent = GCL_Write_" + sources.at(3)->ComponentName() + "_" + sources.at(3)->SignalName();

    return returnString;
}
