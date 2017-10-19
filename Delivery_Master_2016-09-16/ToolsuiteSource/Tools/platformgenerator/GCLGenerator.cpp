#include "GCLGenerator.h"

#include "FunctionBlock.h"
#include "FunctionDefinition.h"
#include "Layout.h"
#include "Logger.h"
#include "MenuItem.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "TransitionCondition.h"

#include <QStringList>
#include <QMultiMap>
#include <QHashIterator>

GCLGenerator::GCLGenerator(Project* project) :
    m_bspSpecification(project->GetBSPSpecification()),
    m_canSpecification(project->GetCANSpecification()),
    m_functionBlocks(project->GetFunctionBlocks()),
    m_layout(project->GetLayoutDefinition()),
    m_segmentDisplay(project->GetSegmentDisplay()),
    m_icPlatform(project->GetICPlatform()),
    m_animationDefinition(NULL),
    m_menuDefinition(NULL),
    m_checkMessagesDefinition(project->GetCheckMessagesDefinition())

{
    if (m_bspSpecification.HasDisplay())
    {
        foreach (Layout* layout, m_layout.Layouts())
        {
            foreach (DisplayArea* area, layout->DisplayAreas())
            {
                m_uiWidgets.append(area->UIWidgets());
            }

            if (layout->HasMenu())
            {
                m_menuDefinition = layout->GetMenu();
                RootMenuItemContainer& menuRoot = m_menuDefinition->MenuRoot();

                foreach (const MenuItem* menuItem, menuRoot.MenuItems() )
                {
                    parseMenuWidgetsSinks(menuItem);
                }
            }
        }
    }

    project->GetAllGCLSinks(m_allSinks);
    project->GetAllGCLSources(m_allSources);

    m_GCLMenuControllerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_MenuController.h");
    m_GCLContentsManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_LayoutManager.h");
    m_GCLTransitionsManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_TransitionManager.h");
    m_GCLSegmentDisplayManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_SegmentDisplayManager.h");
    m_GCLCheckMessageManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_CheckMessageManager.h");
    m_GCLPlatformHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_Platform.h");
    m_GCLCOMHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_COM.h");
    m_GCLWidgetsHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL"));
    m_GCLAnimationManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/interfaces/GCL_AnimationManager.h");
    m_GCLSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/src/GCL.c");
    m_GCLHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("GCL")).append("/inc/GCL.h");

    if (m_bspSpecification.HasDisplay() && m_layout.StartupEnabled())
    {
        getAllStartupLayoutConditionSinks();
    }
}

void GCLGenerator::parseMenuWidgetsSinks(const MenuItem* menuItem)
{
    m_uiWidgets.append(menuItem->UIWidgets());


    foreach (const MenuItem* subMenuItem, menuItem->SubMenuItems() )
    {
        parseMenuWidgetsSinks(subMenuItem);
    }
}

GCLGenerator::~GCLGenerator()
{
}

void GCLGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    readAllSignals();
    generateGCLFunctionBlockHeaders();
    generateGCLMenuControllerHeader();
    generateGCLWidgetsHeader();
    generateGCLCheckMessageManagerHeader();
    //generateGCLContentsManagerHeader();
    generateGCLLayoutManagerHeader();
    generateGCLTransitionManagerHeader();
    generateGCLSegmentDisplayManagerHeader();
    generateGCLPlatformHeader();
    generateGCLCOMHeader();
    generateAnimationsHeader();
    generateGCLHeader();
    generateGCLSource();
}

/* Preparation */

void GCLGenerator::readAllSignals()
{
    m_sourcesByComponentName.clear();
    m_sinksByComponentName.clear();

    foreach (GCLSignalSink* sink, m_allSinks)
    {
        m_sinksByComponentName.insert(sink->ComponentName(), sink);
    }

    foreach (GCLSignalSource* source, m_allSources)
    {
        m_sourcesByComponentName.insert(source->ComponentName(), source);
    }
}

/* Generators */

void GCLGenerator::generateGCLCOMHeader()
{
    QString fileNameAsDefine = "GCL_COM_H";
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for COM stack ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    // Since COM only use the GCL for Indicating incoming messages and not actively call any Write or Read
    // functions, we only export the indicator functions in this header.
    fileLines.append("/***************************** Exported Functions ****************************/");
    foreach (GCLSignalSink* sink, m_allSinks)
    {
        GCLSignalSource* source = sink->GetSignalSource();
        if (source != NULL && source->ComponentName() == "CAN" && sink->HasUpdateNotification())
        {
            fileLines.append("void GCL_" + source->ComponentName() + "_" + source->SignalName() + "_Indication(void);");
        }
    }
    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLCOMHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLFunctionBlockHeaders()
{
    foreach (FunctionBlock* block, m_functionBlocks.List())
    {
        const FunctionDefinition& functionDefinition = block->GetFunctionDefinition();
        QString fileName = "GCL_" + functionDefinition.GetName() + ".h";
        QString fileNameAsDefine = "GCL_" + QString(functionDefinition.GetName()).toUpper() + "_H";

        QStringList fileLines;
        fileLines.append("#ifndef " + fileNameAsDefine);
        fileLines.append("#define " + fileNameAsDefine);
        fileLines.append("/*** Generated GCL header file for " + functionDefinition.GetName() + " ***/");
        fileLines.append("");
        fileLines.append("#include \"StandardTypes.h\"");
        fileLines.append("#include \"GCL_Types.h\"");
        fileLines.append("");

        QStringList notifications;
        fileLines.append("/*** Input signals ***/");
        foreach (const GCLSignalSink* sink, functionDefinition.Sinks())
        {
            fileLines.append(sink->GetGCLFunctionSignature() + ";");

            if (sink->HasUpdateNotification())
            {
                notifications.append(sink->GetNotificationFunctionSignature() + ";");
            }
        }
        fileLines.append("");
        fileLines.append("/*** Output signals ***/");
        foreach (const GCLSignalSource* source, functionDefinition.Sources())
        {
            fileLines.append(source->GetGCLFunctionSignature() + ";");
        }
        fileLines.append("");
        fileLines.append("/*** Signal indication callbacks ***/");
        fileLines.append(notifications);
        fileLines.append("");
        fileLines.append("#endif // " + fileNameAsDefine);
        fileLines.append("");

        QString path(PlatformPaths::CreatePlatformComponentPath("GCL"));
        path.append("/interfaces/").append(fileName);
        AddGeneratedFile(path, fileLines.join("\n"));
    }
}

void GCLGenerator::generateGCLMenuControllerHeader()
{
    if (m_bspSpecification.HasDisplay())
    {
        foreach (Layout* layout, m_layout.Layouts())
        {
            if (layout->HasMenu())
            {
                m_menuDefinition = layout->GetMenu();
            }
        }
    }

    if (m_menuDefinition == NULL)
    {
        return;
    }

    QString fileNameAsDefine = "GCL_MENUCONTROLLER_H";

    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for MenuController ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");

    QStringList sinkIndicationPrototypes;
    QStringList sinkFunctionPrototypes;

    foreach (const GCLSignalSink* sink, m_menuDefinition->GetSignalSinks())
    {
        sinkFunctionPrototypes.append(sink->GetGCLFunctionSignature() + ";");

        if (sink->HasUpdateNotification())
        {
            sinkIndicationPrototypes.append(sink->GetNotificationFunctionSignature() + ";");
        }
    }

    fileLines.append("/*** Input signals ***/");
    fileLines.append(sinkFunctionPrototypes);

    QStringList sourceFunctionPrototypes;

    foreach (const GCLSignalSource* source, m_menuDefinition->GetSignalSources())
    {

        sourceFunctionPrototypes.append(source->GetGCLFunctionSignature() + ";");
    }

    fileLines.append("");
    fileLines.append("/*** Output signals ***/");
    fileLines.append(sourceFunctionPrototypes);
    fileLines.append("");
    fileLines.append("/*** Signal indication callbacks ***/");
    fileLines.append(sinkIndicationPrototypes);
    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLMenuControllerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLCheckMessageManagerHeader()
{
    QString fileNameAsDefine = "GCL_CHECKMESSAGEMANAGER_H";
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for CheckMessageManager ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/*** Input signals ***/");
    foreach(const GCLSignalSink* sink, m_checkMessagesDefinition.GetSignalSinks())
    {
        fileLines.append(sink->GetGCLFunctionSignature() + ";");
    }
    fileLines.append("");
    fileLines.append("/*** Output signals ***/");
    foreach(const GCLSignalSource* source, m_checkMessagesDefinition.GetSignalSources())
    {
        fileLines.append(source->GetGCLFunctionSignature() + ";");
    }
    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLCheckMessageManagerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLLayoutManagerHeader()
{
    QString fileNameAsDefine = "GCL_LAYOUTHANDLER_H";
    QStringList fileLines;
    QStringList sinkIndicationPrototypes;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for LayoutManager ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/*** Input signals ***/");
    if(m_bspSpecification.HasDisplay())
    {
        foreach(const GCLSignalSink* sink, m_layout.GetSignalSinks())
        {
            fileLines.append(sink->GetGCLFunctionSignature() + ";");
            if (sink->HasUpdateNotification())
            {
                sinkIndicationPrototypes.append(sink->GetNotificationFunctionSignature() + ";");
            }
        }
    }

    fileLines.append("");
    QStringList sourceFunctionPrototypes;

    foreach (Layout* layout, m_layout.Layouts())
    {
        if (!layout->IsStartupScreen())
        {
            foreach (const GCLSignalSource* source, layout->GetSignalSources())
            {

                sourceFunctionPrototypes.append(source->GetGCLFunctionSignature() + ";");
            }
        }
    }

    fileLines.append("");
    fileLines.append("/*** Output signals ***/");
    fileLines.append(sourceFunctionPrototypes);
    fileLines.append("");
    fileLines.append("/*** Signal indication callbacks ***/");
    fileLines.append(sinkIndicationPrototypes);
    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLContentsManagerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLTransitionManagerHeader()
{
    QString fileNameAsDefine = "GCL_TRANSITIONSHANDLER_H";
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for TransitionManager ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/*** Input signals ***/");

    if (m_bspSpecification.HasDisplay())
    {
        foreach (GCLSignalSink* sink, m_allSinks)
        {
            if (sink->ComponentName().startsWith("Transition_"))
            {
                if (!m_startupLayoutSinks.contains(sink))
                {
                    fileLines.append(sink->GetGCLFunctionSignature() + ";");
                }
            }
        }
    }

    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLTransitionsManagerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLSegmentDisplayManagerHeader()
{
    QString fileNameAsDefine = "GCL_SEGMENTDISPLAYHANDLER_H";
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("/*** Generated GCL header file for SegmentDisplayManager ***/");
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/*** Input signals ***/");
    if(m_segmentDisplay.GetEnabled())
    {
        foreach(const GCLSignalSink* sink, m_segmentDisplay.GetSignalSinks())
        {
            fileLines.append(sink->GetGCLFunctionSignature() + ";");
        }
    }
    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLSegmentDisplayManagerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLWidgetsHeader()
{
    QString fileNameAsDefine = "GCL_WIDGETS_H";
    QStringList fileLines;

    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");

    QStringList indicationCallbacks;

    fileLines.append("/*** Input signals ***/");
    foreach(const GCLSignalSink* sink, m_allSinks)
    {
        if (sink->ComponentName().startsWith("Widgets_"))
        {
            fileLines.append(sink->GetGCLFunctionSignature() + ";");
            if (sink->HasUpdateNotification())
            {
                indicationCallbacks.append(sink->GetNotificationFunctionSignature());
            }
        }
    }

    fileLines.append("");
    fileLines.append("/*** Output signals ***/");
    foreach(const GCLSignalSource* source, m_allSources)
    {
        if (source->ComponentName().startsWith("Widgets_"))
        {
            fileLines.append(source->GetGCLFunctionSignature() + ";" );
        }
    }

    fileLines.append("");
    fileLines.append("/*** Signal indication callbacks ***/");
    foreach (QString indicationCallback, indicationCallbacks)
    {
        fileLines.append(indicationCallback + ";");
    }

    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLWidgetsHeaderPath + "/interfaces/GCL_Widgets.h", fileLines.join("\n"));
}

void GCLGenerator::generateAnimationsHeader()
{
    QString fileNameAsDefine = "GCL_ANIMATIONMANAGER_H";
    QStringList fileLines;

    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/*** Output signals ***/");
    foreach(const GCLSignalSource* source, m_allSources)
    {
        if (source->ComponentName().startsWith("Animation"))
        {
            fileLines.append(source->GetGCLFunctionSignature() + ";" );
        }
    }

    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLAnimationManagerHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::generateGCLPlatformHeader()
{

    QString fileNameAsDefine = "GCL_PLATFORM_H";
    QStringList fileLines;
    fileLines.append("#ifndef " + fileNameAsDefine);
    fileLines.append("#define " + fileNameAsDefine);
    fileLines.append("");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("");
    foreach (const GCLSignalSource* source, m_icPlatform.GetSignalSources())
    {
        fileLines.append(source->GetGCLFunctionSignature() + ";" );
    }

    fileLines.append("");
    fileLines.append("#endif // " + fileNameAsDefine);
    fileLines.append("");

    AddGeneratedFile(m_GCLPlatformHeaderPath, fileLines.join("\n"));
}

void GCLGenerator::addFunctionBlocksSignalMappingMembers(QStringList& sourceFileLines)
{
    foreach (GCLSignalSource* source, m_allSources)
    {
        if (source->NeedsGCLMemberVariable())
        {
            if (source->HasDynamicWidgetValueSink())
            {
                sourceFileLines.append("static DynamicWidgetValue m_" + source->GCLMemberVariableName() + ";");
            }
            else
            {
                sourceFileLines.append("static " + source->DataType() + " m_" + source->GCLMemberVariableName() + ";");
            }
        }
    }
}

void GCLGenerator::generateGCLHeader()
{
    QStringList headerFileLines;

    headerFileLines.append("#ifndef GCL_H");
    headerFileLines.append("#define GCL_H");
    headerFileLines.append("");
    headerFileLines.append("/********************************* Includes **********************************/");

    foreach (QString sinkComponentName, m_sinksByComponentName.uniqueKeys())
    {
        // HAL and COM are always included in the GCL.c, no processing needed
        if(sinkComponentName == "HAL" || sinkComponentName == "CAN")
        {
            continue;
        }

        QString includeDirective;
        if (sinkComponentName.startsWith("Widgets_"))
        {
            includeDirective = "#include \"GCL_Widgets.h\"";
        }
        else if (sinkComponentName.startsWith("Transition_"))
        {
            includeDirective = "#include \"GCL_TransitionManager.h\"";
        }
        else if (sinkComponentName.startsWith("AnimationManager_"))
        {
            includeDirective = "#include \"GCL_AnimationManager.h\"";
        }
        else
        {
            includeDirective = "#include \"GCL_" + sinkComponentName + ".h\"";
        }

        if (!headerFileLines.contains(includeDirective))
        {
            headerFileLines.append(includeDirective);
        }
    }

    foreach (QString sourceComponentName, m_sourcesByComponentName.uniqueKeys())
    {
        // HAL and COM are always included in the GCL.c, no processing needed
        if(sourceComponentName == "HAL" || sourceComponentName == "CAN")
        {
            continue;
        }

        QString includeDirective;
        if (sourceComponentName.startsWith("Widgets_"))
        {
            includeDirective = "#include \"GCL_Widgets.h\"";
        }
        else if (sourceComponentName.startsWith("AnimationManager_"))
        {
            includeDirective = "#include \"GCL_AnimationManager.h\"";
        }
        else if (sourceComponentName.startsWith("LayoutManager_"))
        {
            includeDirective = "#include \"GCL_LayoutManager.h\"";
        }
        else
        {
            includeDirective = "#include \"GCL_" + sourceComponentName + ".h\"";
        }

        if (!headerFileLines.contains(includeDirective))
        {
            headerFileLines.append(includeDirective);
        }
    }

    headerFileLines.append("");
    headerFileLines.append("#endif //GCL_H");
    headerFileLines.append("");

    AddGeneratedFile(m_GCLHeaderPath, headerFileLines.join("\n"));
}

void GCLGenerator::generateGCLSource()
{
    QStringList sourceFileLines;

    sourceFileLines.append("/********************************* Includes **********************************/");
    sourceFileLines.append("#include \"GCL.h\"");
    sourceFileLines.append("#include \"GCL_COM.h\"");
    sourceFileLines.append("#include \"HAL.h\"");
    sourceFileLines.append("#include \"COM_GCL_Relay.h\"");
    
    sourceFileLines.append("/**************************** Constants and Types ****************************/");
    sourceFileLines.append("");
    sourceFileLines.append("/********************************* Variables *********************************/");
    addFunctionBlocksSignalMappingMembers(sourceFileLines);
    sourceFileLines.append("");
    sourceFileLines.append("/************************* Local Function Prototypes *************************/");
    sourceFileLines.append("");
    sourceFileLines.append("/***************************** Exported Functions ****************************/");

    // We need to make sure that all sinks and sources defined in the project have a read/write function in GCL
    // Even if a source doesn't have a mapped sink, it still expects there to be a function call available.

    foreach (GCLSignalSource* source, m_allSources)
    {
        // First make a nice printout summary of the mapping
        sourceFileLines.append("/* Source: " + source->DataType() + " " + source->LongName());
        sourceFileLines.append(" * Sinks: ");
        foreach (GCLSignalSink* sink, source->GetConnectedSinks())
        {
            if (m_startupLayoutSinks.contains(sink)) continue;

            if (sink->HasUpdateNotification())
            {
                sourceFileLines.append(" *   " + sink->DataType() + " " + sink->LongName() + " (Notification)");
            }
            else
            {
                sourceFileLines.append(" *   " + sink->DataType() + " " + sink->LongName());
            }
        }
        sourceFileLines.append(" */");

        /**** GCL_Write_<ComponentName>_<SignalName> functions are only generated for non HAL/CAN sources ****/
        if (source->ComponentName() != "HAL" && source->ComponentName() != "CAN")
        {
            // Generate GCL_Write
            sourceFileLines.append(source->GetGCLFunctionSignature());
            sourceFileLines.append("{");

            QStringList updateNotifications;
            if(source->GetConnectedSinks().count() > 0)
            {
                bool memberHasBeenSet = false;
                // Process each sink that this source is mapped to
                foreach (GCLSignalSink* sink, source->GetConnectedSinks())
                {
                    if (m_startupLayoutSinks.contains(sink)) continue;

                    if (sink->ComponentName() == "HAL")
                    {
                        if (source->DataType() == "void")
                        {
                            sourceFileLines.append("    HAL_Write_" + sink->SignalName() + "();");
                        }
                        else
                        {
                            sourceFileLines.append("    HAL_Write_" + sink->SignalName() + "(value);");
                        }
                    }
                    else if (sink->ComponentName() == "CAN")
                    {
                        // There are no void signal sinks in CAN so no check for it here
                        sourceFileLines.append("    COM_Write_" + sink->SignalName() + "(value);");
                    }
                    else
                    {
                        if (!memberHasBeenSet)
                        {
                            if (source->HasDynamicWidgetValueSink())
                            {
                                if (source->DataType() == "uint8")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.U8 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_UINT8;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else if (source->DataType() == "uint16")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.U16 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_UINT16;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else if (source->DataType() == "uint32")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.U32 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_UINT32;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else if (source->DataType() == "sint8")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.S8 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_SINT8;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else if (source->DataType() == "sint16")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.S16 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_SINT16;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else if (source->DataType() == "sint32")
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Value.S32 = value;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".DataType = STD_DATATYPE_SINT32;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Valid = TRUE;");
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + ".Flashing = FALSE;");
                                }
                                else // we assume that the source datatype is also DynamicWidgetValue
                                {
                                    sourceFileLines.append("    m_" + source->GCLMemberVariableName() + " = value;");
                                }
                            }
                            else if (source->DataType() != "void")
                            {
                                sourceFileLines.append("    m_" + source->GCLMemberVariableName() + " = value;");
                            }

                            memberHasBeenSet = true;
                        }
                    }


                    if (sink->HasUpdateNotification())
                    {
                        updateNotifications.append(sink->GetNotificationFunctionCall());
                    }
                }

                foreach (QString updateNotification, updateNotifications)
                {
                    sourceFileLines.append("    " + updateNotification + ";");
                }
            }
            else
            {
                sourceFileLines.append("    // This signal is not mapped to any sink!");
                if (source->DataType() != "void")
                {
                    sourceFileLines.append("    UNUSED(value);");
                }
            }

            sourceFileLines.append("}");
            sourceFileLines.append("");
        }

        QStringList notificationsForCANSource;
        /**** GCL_Read_<ComponentName>_<SignalName> functions ****/
        foreach (GCLSignalSink* sink, source->GetConnectedSinks())
        {
            if (m_startupLayoutSinks.contains(sink)) continue;

            if (sink->ComponentName() != "HAL" && sink->ComponentName() != "CAN" && sink->DataType() != "void")
            {
                // Generate GCL_Read
                sourceFileLines.append(sink->GetGCLFunctionSignature());
                sourceFileLines.append("{");
                sourceFileLines.append("    *status = GCL_SIGNAL_OK;");
                if (source->HasDynamicWidgetValueSink())
                    {
                        // If the source is CAN or HAL, read the value into a temporary variable
                        if (source->ComponentName() == "HAL" || source->ComponentName() == "CAN")
                        {
                            // Read the value from HAL or CAN into a temporary variable.
                            if (source->ComponentName() == "HAL")
                            {
                                sourceFileLines.append("    " + source->DataType() + " tempValue = 0;");
                                sourceFileLines.append("    HAL_Read_" + source->SignalName() + "(&tempValue, status);");
                            }
                            else if (source->ComponentName() == "CAN")
                            {
                                // All CAN signals are read as uint32 from COM stack
                                sourceFileLines.append("    uint32 tempValue = 0;");
                                sourceFileLines.append("    COM_Read_" + source->SignalName() + "(&tempValue, status);");
                            }

                            // If this is the DynamicWidgetValue sink place the value of the temporary variable in the proper union member
                            if (sink->DataType() == "DynamicWidgetValue")
                            {
                                sourceFileLines.append("    value->Valid = (*status == GCL_SIGNAL_OK) ? TRUE : FALSE;");
                                if (source->DataType() == "uint8")
                                {
                                    sourceFileLines.append("    value->Value.U8 = (uint8)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_UINT8;");
                                }
                                else if (source->DataType() == "uint16")
                                {
                                    sourceFileLines.append("    value->Value.U16 = (uint16)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_UINT16;");
                                }
                                else if (source->DataType() == "uint32")
                                {
                                    sourceFileLines.append("    value->Value.U32 = (uint32)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_UINT32;");
                                }
                                else if (source->DataType() == "sint8")
                                {
                                    sourceFileLines.append("    value->Value.S8 = (sint8)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_INT8;");
                                }
                                else if (source->DataType() == "sint16")
                                {
                                    sourceFileLines.append("    value->Value.S16 = (sint16)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_INT16;");
                                }
                                else if (source->DataType() == "sint32")
                                {
                                    sourceFileLines.append("    value->Value.S32 = (sint32)tempValue;");
                                    sourceFileLines.append("    value->DataType = STD_DATATYPE_INT32;");
                                }
                            }
                            else
                            {
                                sourceFileLines.append("    *value = (" + source->DataType() + ")tempValue;");
                            }
                        }
                        else
                        {
                            // Use the member variable to populate the incoming value pointer
                            if (sink->DataType() == "DynamicWidgetValue")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ";");
                            }
                            else if (sink->DataType() == "uint8")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.U8;");
                            }
                            else if (sink->DataType() == "uint16")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.U16;");
                            }
                            else if (sink->DataType() == "uint32")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.U32;");
                            }
                            else if (sink->DataType() == "sint8")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.S8;");
                            }
                            else if (sink->DataType() == "sint16")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.S16;");
                            }
                            else if (sink->DataType() == "sint32")
                            {
                                sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ".Value.S32;");
                            }
                            else
                            {
                                sourceFileLines.append("#error A mapping was found where one of the sinks was a DynamicWidgetValue but the source was neither DynamicWidgetValue nor a basic datatype.");
                            }
                        }
                    }
                    else
                    {
                        if (source->ComponentName() == "HAL")
                        {
                            sourceFileLines.append("    HAL_Read_" + source->SignalName() + "(value, status);");
                        }
                        else if (source->ComponentName() == "CAN")
                        {
                            sourceFileLines.append("    uint32 tempValue = 0;");
                            sourceFileLines.append("    COM_Read_" + source->SignalName() + "(&tempValue, status);");
                            sourceFileLines.append("    *value = (" + source->DataType() +")tempValue;");
                        }
                        else
                        {
                            sourceFileLines.append("    *value = m_" + source->GCLMemberVariableName() + ";");
                        }
                    }

                sourceFileLines.append("}");
                sourceFileLines.append("");
            }

            if (sink->HasUpdateNotification() && source->ComponentName() == "CAN")
            {
                notificationsForCANSource.append(sink->GetNotificationFunctionCall());
            }
        }

        if (notificationsForCANSource.count() > 0)
        {
            sourceFileLines.append("void GCL_" + source->GCLMemberVariableName() + "_Indication()");
            sourceFileLines.append("{");
            foreach (QString notificationForCANSource, notificationsForCANSource)
            {
                sourceFileLines.append("    " + notificationForCANSource + ";");
            }
            sourceFileLines.append("}");
        }

        sourceFileLines.append("");
    }

    // Handle all unmapped sinks
    sourceFileLines.append("/*** Unmapped Sinks ***/");
    foreach (GCLSignalSink* sink, m_allSinks)
    {
        if (m_startupLayoutSinks.contains(sink)) continue;

        // Since CAN and HAL don't use GCL_Read functions, we don't need to make sure that they exist here either
        if (sink->ComponentName() == "CAN" || sink->ComponentName() == "HAL") continue;

        if (sink->GetSignalSource() == NULL)
        {
            if (!sink->IsMandatory())
            {
                if (sink->DataType() == "void")
                {
                    sourceFileLines.append(sink->GetGCLFunctionSignature());
                    sourceFileLines.append("{");
                    sourceFileLines.append("    *status = GCL_SIGNAL_UNMAPPED;");
                    sourceFileLines.append("}");
                    sourceFileLines.append("");
                }
                else
                {
                    sourceFileLines.append(sink->GetGCLFunctionSignature());
                    sourceFileLines.append("{");
                    sourceFileLines.append("    *status = GCL_SIGNAL_UNMAPPED;");
                    sourceFileLines.append("    *value = 0;");
                    sourceFileLines.append("}");
                    sourceFileLines.append("");
                }
            }
            else
            {
                LOG_ERROR("Signal source for signal " + sink->LongName() + " was NULL!");
            }
        }
    }

    sourceFileLines.append("");

    AddGeneratedFile(m_GCLSourcePath, sourceFileLines.join("\n"));
}

void GCLGenerator::getAllStartupLayoutConditionSinks()
{
    Layout* startupLayout = m_layout.GetStartupLayout();
    QList<TransitionCondition*> conditions;

    foreach (TransitionClause* transition, startupLayout->GetSignalTransitions())
    {
        conditions.append(transition->GetFirstCondition());
        conditions.append(transition->GetSecondCondition());
    }
    conditions.removeAll(NULL);

    foreach (TransitionCondition* condition, conditions)
    {
        m_startupLayoutSinks.append(condition->GetSignalSinks().first());
    }
}
