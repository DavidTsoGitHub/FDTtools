#include <QFile>
#include <QTextStream>
#include <QList>
#include <QSet>
#include <QDirIterator>
#include "BSPGenerator.h"
#include "Logger.h"
#include "PlatformPaths.h"
#include "fileutilities.h"
#include "BSPDisplay.h"
#include "BSPFunction.h"
#include "BSPEnum.h"
#include "BSPSegmentDisplay.h"
#include "BSPMapping.h"
#include "BSPFixedPointFloat.h"

BSPGenerator::BSPGenerator(Project* project, bool generateForSimulation) :
    m_project(*project),
    m_bspSpecification(project->GetBSPSpecification()),
    m_generateForSimulation(generateForSimulation)
{
    m_BSPHeaderPath = QString(PlatformPaths::GetMCALCommonPath()).append("/BSP.h");
    m_BSPTypesHeaderPath = QString(PlatformPaths::GetMCALCommonPath()).append("/BSP_Types.h");

    if (generateForSimulation)
    {
        m_PCICHeaderFilePath = QString(PlatformPaths::GetMCALTargetPath(PlatformPaths::PC)).append("/BSP/inc/PC_BSP.h");
        m_PCICSourceFilePath = QString(PlatformPaths::GetMCALTargetPath(PlatformPaths::PC)).append("/BSP/src/PC_BSP.c");
        setupCustomBSPImplementations();
    }
}

void BSPGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    LOG_INFO("Generating BSP common files...");
    generateBSPHeaderFile();
    generateBSPTypesHeaderFile();

    if (m_generateForSimulation)
    {
        LOG_INFO("Generating BSP for PC simulation!");
        generatePCICHeaderFile();
        generatePCICSourceFile();
        AddFilesFromResourcePath(":/StaticPCICFiles");

        if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
        {
            AddFilesFromResourcePath(":/StaticPCICDCUFiles");
        }
    }
    else
    {
        LOG_INFO("Copying BSP for target hardware ");

        QString source = m_project.GetProjectRoot().absoluteFilePath("TargetSource");
        QString destination = m_project.GetOutputFolder().absolutePath() + "/Source/MCAL/Target";
        LOG_INFO("source: " + source + " destination:" + destination);
        FileUtilities::CopyDir(source, destination, false);
    }

    LOG_INFO("Generating BSP common files...done!");
}

void BSPGenerator::generateBSPHeaderFile()
{
    QStringList fileLines;
    fileLines.append("#ifndef __BSP_H__");
    fileLines.append("#define __BSP_H__");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"BSP_Types.h\"");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"ModeManager.h\"");
    fileLines.append("");

    foreach (const BSPFunction* bspFunction, m_bspSpecification.Functions())
    {
        fileLines.append("/**");
        fileLines.append(bspFunction->Description);
        fileLines.append("**/");

        QString args;
        if(bspFunction->Arguments.count() > 0)
        {
            QStringList arguments;
            foreach (BSPFunctionArgument argument, bspFunction->Arguments)
            {
                arguments << (argument.Type + " " + argument.Name);
            }
            args = (arguments.join(", "));
        }
        else args = "void";
        fileLines.append(bspFunction->ReturnType + " " + bspFunction->Name + "(" + args + ");");
        fileLines.append("");
    }

    fileLines.append("#endif /* __BSP_H__ */");
    fileLines.append("");

    AddGeneratedFile(m_BSPHeaderPath, fileLines.join("\n"));
}

void BSPGenerator::generateBSPTypesHeaderFile()
{
    QStringList fileLines;
    fileLines.append("#ifndef __BSP_TYPES_H__");
    fileLines.append("#define __BSP_TYPES_H__");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"Platform_Types.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");

    foreach (const BSPEnum* bspEnum, m_bspSpecification.Enums())
    {
        fileLines.append("/**");
        fileLines.append(bspEnum->Description);
        fileLines.append("**/");
        fileLines.append("typedef enum");
        fileLines.append("{");

        quint32 lastItemValue = bspEnum->EnumItems.last().Value;
        foreach (BSPEnumItem item, bspEnum->EnumItems)
        {
            fileLines.append("    " + item.Identifier + " = " + QString::number(item.Value) + (item.Value == lastItemValue ? "" : ",") + " // " + item.Description);
        }
        fileLines.append("} " + bspEnum->Name + ";");
        fileLines.append("");
    }

    foreach (const BSPFixedPointFloat* bspFixedPointFloat, m_bspSpecification.FixedPointFloats())
    {
        fileLines.append("/**");
        fileLines.append(" Fixed-point float definition");
        switch (bspFixedPointFloat->Endianess)
        {
        case BSPFixedPointFloat::LittleEndian:
            fileLines.append(" Endianess: LittleEndian");
            break;
        case BSPFixedPointFloat::BigEndian:
            fileLines.append(" Endianess: BigEndian");
            break;
        case BSPFixedPointFloat::UnknownEndian:
        default:
            fileLines.append(" Endianess: UnknownEndian");
            break;
        }

        fileLines.append(" Unit: " + bspFixedPointFloat->Unit);
        fileLines.append(" SignificantBits: " + QString::number(bspFixedPointFloat->SignificantBits));
        fileLines.append(" FractionalBits: " + QString::number(bspFixedPointFloat->FractionalBits));
        fileLines.append("**/");
        fileLines.append("typedef " + bspFixedPointFloat->BaseType + " " + bspFixedPointFloat->Name + ";");
        fileLines.append("");
    }

    fileLines.append("#endif /* __BSP_TYPES_H__ */");
    fileLines.append("");

    AddGeneratedFile(m_BSPTypesHeaderPath, fileLines.join("\n"));
}

void BSPGenerator::generatePCICHeaderFile()
{
    QStringList fileLines;
    fileLines.append("#ifndef __PC_BSP_H__");
    fileLines.append("#define __PC_BSP_H__");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"BSP_Types.h\"");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint16 x;");
    fileLines.append("    uint16 y;");
    fileLines.append("    uint16 width;");
    fileLines.append("    uint16 height;");
    fileLines.append("    uint8 bpp;");
    fileLines.append("    uint8 trans;");
    fileLines.append("    uint8 clutOffset;");
    fileLines.append("    uint8* buffer;");
    fileLines.append("    boolean enabled;");
    fileLines.append("} pcic_layer_t;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("void PCIC_Start(void);");
    fileLines.append("void PCIC_Stop(void);");
    fileLines.append("void PCIC_SetMessageCallback(void(*sendMessageCallback)(const char*,int));");
    fileLines.append("void PCIC_CallFunction(const char* functionName, int index, int value);");
    fileLines.append("void PCIC_Log(const char* logText);");
    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("void PCIC_SendAreaBufferContents(uint8 layerID, uint8* buffer, uint32 size);");
        fileLines.append("void PCIC_SendLayerSetup(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 trans, uint8 clutOffset, boolean enabled);");
        fileLines.append("void PCIC_SetLayerEnabled(uint8 layerID, boolean status);");
        fileLines.append("void PCIC_SetCLUT(uint8 colorCount, uint32* clutData);");
        fileLines.append("void PCIC_SetBackgroundColor(uint8 clutID);");
        fileLines.append("void PCIC_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor);");
        fileLines.append("void PCIC_SetLayerBuffer(uint8 layerID, uint8* frameBuffer);");
        fileLines.append("void PCIC_SetLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 trans, uint8 clutOffset, uint8* buffer, boolean enabled);");
    }
    fileLines.append("void PCIC_SendFramebufferContents(void);");
    fileLines.append("void PCIC_FramebufferUpdateAllowed(void);");
    fileLines.append("void PCIC_DCUUpdateAllowed(void);");

    fileLines.append("");
    fileLines.append("#endif /* __PC_BSP_H__ */");
    fileLines.append("");

    AddGeneratedFile(m_PCICHeaderFilePath, fileLines.join("\n"));
}

void BSPGenerator::generatePCICSourceFile()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include <windows.h>");
    fileLines.append("#include <time.h>");
    fileLines.append("#include <process.h>");
    fileLines.append("#include <stdio.h>");
    fileLines.append("#include <string.h>");

    fileLines.append("#include \"jansson.h\"");
    fileLines.append("#include \"BSP.h\"");
    fileLines.append("#include \"PC_BSP.h\"");
    fileLines.append("#include \"CAL.h\"");
    fileLines.append("#include \"Base64.h\"");
    fileLines.append("#include \"osmacro.h\"");
    fileLines.append("#include \"os.h\"");
    fileLines.append("#include \"Logger.h\"");

    if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("#include \"DCU_IF.h\"");
        fileLines.append("#include \"GFX.h\"");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");

    if (m_project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("#define FRAMEBUFFER_SIZE ((" + QString::number(m_project.GetBSPSpecification().GetDisplay()->GetHeight()) + " * " + QString::number(m_project.GetBSPSpecification().GetDisplay()->GetWidth()) + "))");
        fileLines.append("#define DCU_COLORS_SIZE  (256)");
    }
    else
    {
        fileLines.append("#define FRAMEBUFFER_SIZE ((" + QString::number(m_project.GetBSPSpecification().GetDisplay()->GetHeight()) + " * " + QString::number(m_project.GetBSPSpecification().GetDisplay()->GetWidth()) + ") / 8)");
    }

    fileLines.append("#define CAL_STORAGE_SIZE (256)");
    fileLines.append("#define COMPONENTNAME \"PCIC\"");
    fileLines.append("");
    fileLines.append("/************************* Local Function Prototypes *************************/");
    fileLines.append("static void sendMessageToSimulator(const char* message, int messageLength);");
    fileLines.append("static void sendMonadicBSPFunctionMessage(const char* functionName, const char* functionParameter);");
    fileLines.append("static void sendDiadicBSPFunctionMessage(const char* functionName, int indexParameter, const char* valueParameter);");
    fileLines.append("static void reset(void);");
    fileLines.append("");

    QStringList bspFunctionsLines;
    QStringList simFunctionsLines;
    QStringList variablesLines;
    QStringList resetFunctionLines;
    QSet<QString> memberVariableNames;

    /*
    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        memberVariableNames.insert("m_FrameBuffer");
    }
    */

    foreach (const BSPFunction* bspFunction, m_bspSpecification.Functions())
    {
        if (m_customBSPImplementationsByFunctionName.contains(bspFunction->Name))
        {
            bspFunctionsLines.append("/* Custom implementation of " + bspFunction->Name + " */");
            bspFunctionsLines.append(m_customBSPImplementationsByFunctionName.value(bspFunction->Name));
            continue;
        }

        const BSPMapping* mapping = m_bspSpecification.GetMappingFromFunction(bspFunction);

        if (bspFunction->Arguments.count() == 1) // Monadic BSP functions, simple setters/getters
        {
            const BSPFunctionArgument argument = bspFunction->Arguments.first();
            QString memberVariableName = "m_" + argument.Name;
            QString memberVariableType = argument.Type;

            if (!memberVariableNames.contains(memberVariableName))
            {
                memberVariableNames.insert(memberVariableName);

                if (bspFunction->GCLSignalType == BSPFunction::Sink)
                {
                    resetFunctionLines.append("    memset(&" + memberVariableName + ", 0, sizeof(" + memberVariableType + "));");
                    variablesLines.append("static " + memberVariableType + " " + memberVariableName + ";");
                }
                else
                {
                    QString strippedVariableType = QString(memberVariableType).remove("*");
                    resetFunctionLines.append("    memset(&" + memberVariableName + ", 0, sizeof(" + strippedVariableType + "));");
                    variablesLines.append("static " + strippedVariableType + " " + memberVariableName + ";");
                }
            }

            bspFunctionsLines.append(bspFunction->ReturnType + " " + bspFunction->Name + "(" + memberVariableType + " " + argument.Name  + ")");
            bspFunctionsLines.append("{");

            switch (bspFunction->GCLSignalType)
            {
                case BSPFunction::Sink:
                    bspFunctionsLines.append("    /* GCL Sink */");
                    bspFunctionsLines.append("    " + memberVariableName + " = " + argument.Name + ";");
                    bspFunctionsLines.append("    char valueString[16];");
                    bspFunctionsLines.append("    sprintf(valueString, \"%d\", " + memberVariableName + ");");
                    bspFunctionsLines.append("    sendMonadicBSPFunctionMessage(\"" + bspFunction->Name + "\", valueString);");

                    simFunctionsLines.append("void PCIC_Get_" + bspFunction->Name + "()");
                    simFunctionsLines.append("{");
                    simFunctionsLines.append("    Logger_Info(COMPONENTNAME, \"%s() returns %d\", __FUNCTION__, " + memberVariableName + ");");
                    simFunctionsLines.append("    char valueString[16];");
                    simFunctionsLines.append("    sprintf(valueString, \"%d\", " + memberVariableName + ");");
                    simFunctionsLines.append("    sendMonadicBSPFunctionMessage(\"" + bspFunction->Name + "\", valueString);");
                    simFunctionsLines.append("}");
                    simFunctionsLines.append("");
                    break;
                case BSPFunction::Source:
                    bspFunctionsLines.append("    /* GCL Source */");
                    bspFunctionsLines.append("    *" + argument.Name + " = " + memberVariableName + ";");

                    simFunctionsLines.append("void PCIC_Set_" + bspFunction->Name + "(" + QString(argument.Type).replace("*", "") + " " + argument.Name + ")");
                    simFunctionsLines.append("{");
                    simFunctionsLines.append("    Logger_Info(COMPONENTNAME, \"%s(%d)\", __FUNCTION__, " + argument.Name + ");");
                    simFunctionsLines.append("    " + memberVariableName + " = " + argument.Name + ";");
                    simFunctionsLines.append("}");
                    simFunctionsLines.append("");
                    break;
                case BSPFunction::Undefined:
                default:
                    bspFunctionsLines.append("    /* Not GCL connected */");
                    break;
            }
        }
        else if (bspFunction->Arguments.count() == 2 && mapping != NULL) // Indexed BSP functions
        {
            const BSPFunctionArgument indexArgument = bspFunction->Arguments.first();
            const BSPFunctionArgument valueArgument = bspFunction->Arguments.last();

            QString memberVariableName = "m_" + mapping->Type;
            if (!memberVariableNames.contains(memberVariableName))
            {
                memberVariableNames.insert(memberVariableName);

                if (bspFunction->GCLSignalType == BSPFunction::Sink)
                {
                    variablesLines.append("static " + valueArgument.Type + " " + memberVariableName + "[256];");
                    resetFunctionLines.append("    memset(&" + memberVariableName + ", 0, sizeof(" + valueArgument.Type + ") * 256);");
                }
                else
                {
                    QString strippedVariableType = QString(valueArgument.Type).remove("*");
                    variablesLines.append("static " + strippedVariableType + " " + memberVariableName + "[256];");
                    resetFunctionLines.append("    memset(&" + memberVariableName + ", 0, sizeof(" + strippedVariableType + ") * 256);");
                }
            }

            bspFunctionsLines.append(bspFunction->ReturnType + " " + bspFunction->Name + "(" + indexArgument.Type + " " + indexArgument.Name + ", " + valueArgument.Type + " " + valueArgument.Name + ")");
            bspFunctionsLines.append("{");
            switch (bspFunction->GCLSignalType)
            {
                case BSPFunction::Sink:
                    bspFunctionsLines.append("    /* GCL Sink */");
                    bspFunctionsLines.append("    " + memberVariableName + "[" + indexArgument.Name + "] = " + valueArgument.Name + ";");
                    bspFunctionsLines.append("    char valueString[16];");
                    bspFunctionsLines.append("    sprintf(valueString, \"%d\", " + memberVariableName + "[" + indexArgument.Name + "]);");
                    bspFunctionsLines.append("    sendDiadicBSPFunctionMessage(\"" + bspFunction->Name + "\", " + indexArgument.Name + ", valueString);");

                    simFunctionsLines.append("void PCIC_Get_" + bspFunction->Name + "(" + indexArgument.Type + " " + indexArgument.Name + ")");
                    simFunctionsLines.append("{");
                    simFunctionsLines.append("    char valueString[16];");
                    simFunctionsLines.append("    sprintf(valueString, \"%d\", " + memberVariableName + "[" + indexArgument.Name + "]);");
                    simFunctionsLines.append("    sendDiadicBSPFunctionMessage(\"" + bspFunction->Name + "\", " + indexArgument.Name + ", valueString);");
                    simFunctionsLines.append("}");
                    simFunctionsLines.append("");
                    break;
                case BSPFunction::Source:
                    bspFunctionsLines.append("    /* GCL Source */");
                    bspFunctionsLines.append("    *" + valueArgument.Name + " = " + memberVariableName + "[" + indexArgument.Name + "];");

                    simFunctionsLines.append("void PCIC_Set_" + bspFunction->Name + "(" + indexArgument.Type + " " + indexArgument.Name + ", " + QString(valueArgument.Type).replace("*", "") + " " + valueArgument.Name + ")");
                    simFunctionsLines.append("{");
                    simFunctionsLines.append("    Logger_Info(COMPONENTNAME, \"%s(%d, %d)\", __FUNCTION__, " + indexArgument.Name + ", " + valueArgument.Name + ");");
                    simFunctionsLines.append("    " + memberVariableName + "[" + indexArgument.Name + "] = " + valueArgument.Name + ";");
                    simFunctionsLines.append("}");
                    simFunctionsLines.append("");
                    break;
                case BSPFunction::Undefined:
                default:
                    bspFunctionsLines.append("    /* Not GCL connected */");
                    break;
            }

        }
        else    // Other function, merely generate an empty function shell for now
        {
            QStringList argumentsList;
            foreach (BSPFunctionArgument argument, bspFunction->Arguments.values())
            {
                argumentsList.append(argument.Type + " " + argument.Name);
            }

            bspFunctionsLines.append(bspFunction->ReturnType + " " + bspFunction->Name + "(" + argumentsList.join(", ") + ")");
            bspFunctionsLines.append("{");
            bspFunctionsLines.append("    // Unhandled BSP function (GCLType: " + bspFunction->GCLSignalTypeName() + ")");

        }

        if (bspFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
        {
            bspFunctionsLines.append("    return BSP_OK;");
        }
        else if (bspFunction->ReturnType.compare("void") != 0)
        {
            // Best guess, this is some form of integer value, return 0
            bspFunctionsLines.append("    return 0;");
        }
        bspFunctionsLines.append("}");
        bspFunctionsLines.append("");
    }

    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("static void (*m_SendSimulatorMessageCallback)(const char*, int) = NULL;");
    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        fileLines.append("static uint8 m_FrameBuffer[FRAMEBUFFER_SIZE];");
    }
    else
    {
        //fileLines.append("static uint8* m_FrameBuffer;");
        fileLines.append("static pcic_layer_t m_layers[" + QString::number(m_bspSpecification.GetDisplay()->GetNumberOfLayers()) + "];");
    }
    fileLines.append("static dateTime_t m_customDateTime;");
    fileLines.append("static boolean m_useCustomDateTime = FALSE;");
    fileLines.append("static uint8 m_CAL_Storage[CAL_STORAGE_SIZE];");
    fileLines.append("static BSP_ECUModes pcicMode = BSP_MODE_SLEEP;");
    fileLines.append(variablesLines);
    fileLines.append("");
    fileLines.append("/****************************** Local Functions ******************************/");
    fileLines.append("void sendMessageToSimulator(const char* message, int messageLength)");
    fileLines.append("{");
    fileLines.append("    if (NULL == m_SendSimulatorMessageCallback) return;");
    fileLines.append("    m_SendSimulatorMessageCallback(message, messageLength);");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void sendMonadicBSPFunctionMessage(const char* functionName, const char* functionParameter)");
    fileLines.append("{");
    fileLines.append("    json_t *root = json_object();");
    fileLines.append("    json_object_set_new(root, \"function_name\", json_string(functionName));");
    fileLines.append("    json_object_set_new(root, \"index_parameter\", json_integer(0));");
    fileLines.append("    json_object_set_new(root, \"value_parameter\", json_string(functionParameter));");
    fileLines.append("    char* json = json_dumps(root, JSON_INDENT(3) | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER);");
    fileLines.append("    sendMessageToSimulator(json, (strlen(json)));");
    fileLines.append("    json_object_clear(root);");
    fileLines.append("    free(json);");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void sendDiadicBSPFunctionMessage(const char* functionName, int indexParameter, const char* valueParameter)");
    fileLines.append("{");
    fileLines.append("    json_t *root = json_object();");
    fileLines.append("    json_object_set_new(root, \"function_name\", json_string(functionName));");
    fileLines.append("    json_object_set_new(root, \"index_parameter\", json_integer(indexParameter));");
    fileLines.append("    json_object_set_new(root, \"value_parameter\", json_string(valueParameter));");
    fileLines.append("    char* json = json_dumps(root, JSON_INDENT(3) | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER);");
    fileLines.append("    sendMessageToSimulator(json, (strlen(json)));");
    fileLines.append("    json_object_clear(root);");
    fileLines.append("    free(json);");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void reset()");
    fileLines.append("{");

    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        fileLines.append("    memset(&m_FrameBuffer, 0, FRAMEBUFFER_SIZE);");
    }
    else
    {
        fileLines.append("    memset(&m_layers, 0, sizeof(pcic_layer_t) * " + QString::number(m_bspSpecification.GetDisplay()->GetNumberOfLayers()) + ");");
    }

    fileLines.append("    memset(&m_CAL_Storage, 0, sizeof(uint8)*CAL_STORAGE_SIZE);");
    fileLines.append(resetFunctionLines);
    fileLines.append("}");

    fileLines.append("/****************************** BSP.h Functions ******************************/");
    fileLines.append(bspFunctionsLines);
    fileLines.append("/***************************** PC_BSP.h Functions ****************************/");
    fileLines.append("void PCIC_Start(void)");
    fileLines.append("{");
    fileLines.append("    reset();");
    fileLines.append("    OS_Startup();");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void PCIC_Stop(void)");
    fileLines.append("{");
    fileLines.append("    OS_Shutdown();");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void PCIC_SetMessageCallback(void(*sendMessageCallback)(const char*,int))");
    fileLines.append("{");
    fileLines.append("    m_SendSimulatorMessageCallback = sendMessageCallback;");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void PCIC_Log(const char* logText)");
    fileLines.append("{");
    fileLines.append("    Logger_Info(COMPONENTNAME, \"%s\", logText);");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("void PCIC_APPL_WakeupSourceNotify(WakeupSourceType source)");
    fileLines.append("{");
    fileLines.append("    OS_StartTimer();");
    fileLines.append("    APPL_WakeupNotify(source);");
    fileLines.append("}");
    fileLines.append("");
    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        fileLines.append("void PCIC_SendFramebufferContents(void)");
        fileLines.append("{");
        fileLines.append("    // BASE64 encode framebuffer");
        fileLines.append("    uint32 result_length = 0;");
        fileLines.append("    char* result = base64_encode(m_FrameBuffer, FRAMEBUFFER_SIZE, &result_length);");
        fileLines.append("    char encodedString[result_length+1];");
        fileLines.append("    memset(encodedString, 0, result_length+1);");
        fileLines.append("    memcpy(encodedString, result, result_length);");
        fileLines.append("    free(result);");
        fileLines.append("    // Send to Simulator");
        fileLines.append("    sendMonadicBSPFunctionMessage(\"PCIC_FramebufferContents\", encodedString);");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_FramebufferUpdateAllowed(void)");
        fileLines.append("{");
        fileLines.append("");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_DCUUpdateAllowed(void)");
        fileLines.append("{");
        fileLines.append("");
        fileLines.append("}");
        fileLines.append("");
    }
    else
    {
        fileLines.append("void PCIC_SendFramebufferContents(void)");
        fileLines.append("{");
        fileLines.append("");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SendAreaBufferContents(uint8 layerID, uint8* buffer, uint32 size)");
        fileLines.append("{");
        fileLines.append("    // BASE64 encode framebuffer");
        fileLines.append("    uint32 result_length = 0;");
        fileLines.append("    char* result = base64_encode(buffer, size, &result_length);");
        fileLines.append("    char encodedString[result_length+1];");
        fileLines.append("    memset(encodedString, 0, result_length+1);");
        fileLines.append("    memcpy(encodedString, result, result_length);");
        fileLines.append("    free(result);");
        fileLines.append("    // Send to Simulator");
        fileLines.append("    sendDiadicBSPFunctionMessage(\"PCIC_LayerContents\", layerID, encodedString);");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SendLayerSetup(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 trans, uint8 clutOffset, boolean enabled)");
        fileLines.append("{");
        fileLines.append("    json_t *root = json_object();");
        fileLines.append("    json_object_set_new(root, \"layerID\", json_integer(layerID));");
        fileLines.append("    json_object_set_new(root, \"x\", json_integer(x));");
        fileLines.append("    json_object_set_new(root, \"y\", json_integer(y));");
        fileLines.append("    json_object_set_new(root, \"width\", json_integer(width));");
        fileLines.append("    json_object_set_new(root, \"height\", json_integer(height));");
        fileLines.append("    json_object_set_new(root, \"bpp\", json_integer(bpp));");
        fileLines.append("    json_object_set_new(root, \"trans\", json_integer(trans));");
        fileLines.append("    json_object_set_new(root, \"clutOffset\", json_integer(clutOffset));");
        fileLines.append("    json_object_set_new(root, \"enabled\", json_boolean(enabled));");
        fileLines.append("    char* json = json_dumps(root, JSON_INDENT(3) | JSON_ENSURE_ASCII | JSON_PRESERVE_ORDER);");
        fileLines.append("");
        fileLines.append("    sendMonadicBSPFunctionMessage(\"PCIC_LayerSetup\", json);");
        fileLines.append("");
        fileLines.append("    json_object_clear(root);");
        fileLines.append("    free(json);");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetLayerEnabled(uint8 layerID, boolean status)");
        fileLines.append("{");
        fileLines.append("    m_layers[layerID].enabled = status;");
        fileLines.append("    sendDiadicBSPFunctionMessage(\"PCIC_LayerEnabled\", layerID, ((status == TRUE) ? \"1\" : \"0\"));");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetCLUT(uint8 colorCount, uint32* clutData)");
        fileLines.append("{");
        fileLines.append("    for (uint8 i = 0u; i < colorCount; ++i)");
        fileLines.append("    {");
        fileLines.append("        char valueString[16];");
        fileLines.append("        sprintf(valueString, \"0xFF%06X\", clutData[i]);");
        fileLines.append("        sendDiadicBSPFunctionMessage(\"PCIC_ColorData\", i, valueString);");
        fileLines.append("    }");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetBackgroundColor(uint8 clutID)");
        fileLines.append("{");
        fileLines.append("    char valueString[16];");
        fileLines.append("    sprintf(valueString, \"%d\", clutID);");
        fileLines.append("    sendMonadicBSPFunctionMessage(\"PCIC_BackgroundColor\", valueString);");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor)");
        fileLines.append("{");
        fileLines.append("        char chromaKeyString[32];");
        fileLines.append("        sprintf(chromaKeyString, \"0x00%06X;0x00%06X\", startColor, endColor);");
        fileLines.append("        sendDiadicBSPFunctionMessage(\"PCIC_ChromaKey\", layerID, chromaKeyString);");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetLayerBuffer(uint8 layerID, uint8* frameBuffer)");
        fileLines.append("{");
        fileLines.append("    if (frameBuffer != NULL)");
        fileLines.append("    {");
        fileLines.append("        uint8 bppMultiplier = ((m_layers[layerID].bpp == 4u)?2:1);");
        fileLines.append("        uint32 size = m_layers[layerID].width * m_layers[layerID].height * bppMultiplier;");
        fileLines.append("        PCIC_SendAreaBufferContents(layerID, m_layers[layerID].buffer, size);");
        fileLines.append("    }");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_SetLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 trans, uint8 clutOffset, uint8* buffer, boolean enabled)");
        fileLines.append("{");
        fileLines.append("    m_layers[layerID].x = x;");
        fileLines.append("    m_layers[layerID].y = y;");
        fileLines.append("    m_layers[layerID].width = width;");
        fileLines.append("    m_layers[layerID].height = height;");
        fileLines.append("    m_layers[layerID].bpp = bpp;");
        fileLines.append("    m_layers[layerID].trans = trans;");
        fileLines.append("    m_layers[layerID].clutOffset = clutOffset;");
        fileLines.append("    m_layers[layerID].buffer = buffer;");
        fileLines.append("    m_layers[layerID].enabled = enabled;");
        fileLines.append("    PCIC_SendLayerSetup(layerID, x, y, width, height, bpp, trans, clutOffset, enabled);");
        fileLines.append("");
        fileLines.append("    if (enabled)");
        fileLines.append("    {");
        fileLines.append("        PCIC_SetLayerBuffer(layerID, buffer);");
        fileLines.append("    }");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_FramebufferUpdateAllowed(void)");
        fileLines.append("{");
        fileLines.append("    GFX_ReDrawAllowed();");
        fileLines.append("    sendMonadicBSPFunctionMessage(\"PCIC_LayersUpdatedComplete\", \"NORMAL\");");
        fileLines.append("}");
        fileLines.append("");
        fileLines.append("void PCIC_DCUUpdateAllowed(void)");
        fileLines.append("{");
        fileLines.append("    GFX_RequestUpdateSettings();");
        fileLines.append("}");
        fileLines.append("");
    }
    fileLines.append(simFunctionsLines);
    fileLines.append("void PCIC_CallFunction(const char* functionName, int index, int value)");
    fileLines.append("{");
    fileLines.append("    if(NULL != strstr(\"PCIC_Start\", functionName))");
    fileLines.append("    {");
    fileLines.append("        PCIC_Start();");
    fileLines.append("    }");
    fileLines.append("    else if(NULL != strstr(\"PCIC_Stop\", functionName))");
    fileLines.append("    {");
    fileLines.append("        PCIC_Stop();");
    fileLines.append("    }");
    fileLines.append("    else if (NULL != strstr(\"PCIC_Set_BSP_Read_Memory\", functionName))");
    fileLines.append("    {");
    fileLines.append("        BSP_Write_Memory((uint8*)&value, 1, index);");
    fileLines.append("        CAL_Refresh();");
    fileLines.append("    }");
    fileLines.append("    else if (NULL != strstr(\"PCIC_APPL_WakeupSourceNotify\", functionName))");
    fileLines.append("    {");
    fileLines.append("        if (value == 1) //CAN");
    fileLines.append("        {");
    fileLines.append("            PCIC_APPL_WakeupSourceNotify(BSP_WAKEUP_CAN_COMM);");
    fileLines.append("        }");
    fileLines.append("        else if ( value == 2 ) //IGN");
    fileLines.append("        {");
    fileLines.append("            PCIC_APPL_WakeupSourceNotify(BSP_WAKEUP_IGN);");
    fileLines.append("        }");
    fileLines.append("    }");

    foreach (const BSPFunction* bspFunction, m_bspSpecification.Functions())
    {
        // Don't add simulator setters/getters for custom BSP implementations
        if (m_customBSPImplementationsByFunctionName.contains(bspFunction->Name))
        {
            continue;
        }

        const BSPMapping* mapping = m_bspSpecification.GetMappingFromFunction(bspFunction);

        QString simFunc;

        if (bspFunction->Arguments.count() == 1)
        {
            switch (bspFunction->GCLSignalType)
            {
                case BSPFunction::Sink:
                    simFunc = "PCIC_Get_" + bspFunction->Name;
                    fileLines.append("    else if(NULL != strstr(\"" + simFunc + "\", functionName))");
                    fileLines.append("    {");
                    fileLines.append("        " + simFunc + "();");
                    fileLines.append("    }");
                    break;
                case BSPFunction::Source:
                    simFunc = "PCIC_Set_" + bspFunction->Name;
                    fileLines.append("    else if(NULL != strstr(\"" + simFunc + "\", functionName))");
                    fileLines.append("    {");
                    fileLines.append("        " + simFunc + "(value);");
                    fileLines.append("    }");
                    break;
                case BSPFunction::Undefined:
                default:
                    LOG_DEBUG("Ignoring " + bspFunction->Name + " in PCIC_CallFunction as it is not configured as GCL source nor sink");
                    break;
            }
        }
        else if (bspFunction->Arguments.count() == 2 && mapping != NULL)
        {
            switch (bspFunction->GCLSignalType)
            {
                case BSPFunction::Sink:
                    simFunc = "PCIC_Get_" + bspFunction->Name;
                    fileLines.append("    else if(NULL != strstr(\"" + simFunc + "\", functionName))");
                    fileLines.append("    {");
                    fileLines.append("        " + simFunc + "(index);");
                    fileLines.append("    }");
                    break;
                case BSPFunction::Source:
                    simFunc = "PCIC_Set_" + bspFunction->Name;
                    fileLines.append("    else if(NULL != strstr(\"" + simFunc + "\", functionName))");
                    fileLines.append("    {");
                    fileLines.append("        " + simFunc + "(index, value);");
                    fileLines.append("    }");
                    break;
                case BSPFunction::Undefined:
                default:
                    LOG_DEBUG("Ignoring " + bspFunction->Name + " in PCIC_CallFunction as it is not configured as GCL source nor sink");
                    break;
            }

        }
        else
        {
            LOG_DEBUG("Ignoring " + bspFunction->Name + " in PCIC_CallFunction as it is neither a monadic getter/setter nor an indexed BSP call");
        }
    }
    fileLines.append("    else");
    fileLines.append("    {");
    fileLines.append("        Logger_Info(COMPONENTNAME, \"%s Unknown function \\\"%s\\\" called\", __FUNCTION__, functionName);");
    fileLines.append("    }");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("/***************************** Dummy BSP Runnables ****************************/");
    foreach (const BSPRunnable* runnable, m_bspSpecification.Runnables())
    {
        if (!runnable->GetInitFunction().isEmpty())
        {
            fileLines.append("void " + runnable->GetInitFunction() + "() {}");
        }
        if (!runnable->GetRunnableFunction().isEmpty())
        {
            fileLines.append("void " + runnable->GetRunnableFunction() + "() {}");
        }
    }

    AddGeneratedFile(m_PCICSourceFilePath, fileLines.join("\n"));
}

void BSPGenerator::setupCustomBSPImplementations()
{
    QString currentFunctionName;
    /* Custom refresh framebuffer PCIC function */
    QStringList customRefreshFrameBuffer;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::FrameBufferRefreshFunctionName);
    customRefreshFrameBuffer.append("void " + currentFunctionName + "()");
    customRefreshFrameBuffer.append("{");
    customRefreshFrameBuffer.append("    // Send to Simulator");
    customRefreshFrameBuffer.append("    sendMonadicBSPFunctionMessage(\"" + currentFunctionName + "\", \"0\");");
    customRefreshFrameBuffer.append("}");
    customRefreshFrameBuffer.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customRefreshFrameBuffer);

    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        /* Custom get framebuffer PCIC function */
        QStringList customGetFrameBuffer;
        currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::FrameBufferGetFunctionName);
        const BSPFunction* fbGetFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
        customGetFrameBuffer.append(fbGetFunction->ReturnType + " " + currentFunctionName + "(uint8** value)");
        customGetFrameBuffer.append("{");
        customGetFrameBuffer.append("    *value = m_FrameBuffer;");
        if (fbGetFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
        {
            customGetFrameBuffer.append("    return BSP_OK;");
        }
        customGetFrameBuffer.append("}");
        customGetFrameBuffer.append("");
        m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customGetFrameBuffer);
    }

    /* Custom get RTC PCIC function */
    QStringList customGetRTC;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::GetRTCFunctionName);
    const BSPFunction* getRTCFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customGetRTC.append(getRTCFunction->ReturnType + " " + currentFunctionName + "(dateTime_t* value)");
    customGetRTC.append("{");
    customGetRTC.append("    if (m_useCustomDateTime)");
    customGetRTC.append("    {");
    customGetRTC.append("        *value = m_customDateTime;");
    customGetRTC.append("    }");
    customGetRTC.append("    else");
    customGetRTC.append("    {");
    customGetRTC.append("        time_t rawTime;");
    customGetRTC.append("        struct tm* timeInfo;");
    customGetRTC.append("        rawTime = time(NULL);");
    customGetRTC.append("        timeInfo = localtime(&rawTime);");
    customGetRTC.append("        value->year = timeInfo->tm_year + 1900;"); // To get 2015 instead of 115 for display purposes
    customGetRTC.append("        value->month = timeInfo->tm_mon + 1;"); // To get 1-12 instead of 0-11 for display purposes
    customGetRTC.append("        value->day = timeInfo->tm_mday;");
    customGetRTC.append("        value->hour = timeInfo->tm_hour;");
    customGetRTC.append("        value->minute = timeInfo->tm_min;");
    customGetRTC.append("        value->second = timeInfo->tm_sec;");
    customGetRTC.append("    }");
    if (getRTCFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
    {
        customGetRTC.append("    return BSP_OK;");
    }
    customGetRTC.append("}");
    customGetRTC.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customGetRTC);

    /* Custom set RTC PCIC function */
    QStringList customSetRTC;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::SetRTCFunctionName);
    const BSPFunction* setRTCFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customSetRTC.append(setRTCFunction->ReturnType + " " + currentFunctionName + "(dateTime_t value)");
    customSetRTC.append("{");
    customSetRTC.append("    m_customDateTime = value;");
    customSetRTC.append("    m_useCustomDateTime = TRUE;");
    if (setRTCFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
    {
        customSetRTC.append("    return BSP_OK;");
    }
    customSetRTC.append("}");
    customSetRTC.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customSetRTC);

    /* Custom Read Memory PCIC function */
    QStringList customReadMemory;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::ReadMemoryFunctionName);
    const BSPFunction* readMemoryFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customReadMemory.append(readMemoryFunction->ReturnType + " " + currentFunctionName + "(uint8* value, uint16 length, uint16 startAddress)");
    customReadMemory.append("{");
    customReadMemory.append("    for (int i=0; i<length; ++i)");
    customReadMemory.append("    {");
    customReadMemory.append("        value[i] = m_CAL_Storage[startAddress+i];");
    customReadMemory.append("    }");
    customReadMemory.append("");
    if (readMemoryFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
    {
        customReadMemory.append("    return BSP_OK;");
    }
    customReadMemory.append("}");
    customReadMemory.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customReadMemory);

    /* Custom Write Memory PCIC function */
    QStringList customWriteMemory;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::WriteMemoryFunctionName);
    const BSPFunction* writeMemoryFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customWriteMemory.append(writeMemoryFunction->ReturnType + " " + currentFunctionName + "(uint8* value, uint16 length, uint16 startAddress)");
    customWriteMemory.append("{");
    customWriteMemory.append("    uint16 messLength = length + 4;");
    customWriteMemory.append("");
    customWriteMemory.append("    uint8* writeBuffer = (uint8*)malloc(sizeof(uint8) * messLength);");
    customWriteMemory.append("");
    customWriteMemory.append("    writeBuffer[0] = (uint8)((startAddress & 0xFF00) >> 8);");
    customWriteMemory.append("    writeBuffer[1] = (uint8)((startAddress & 0x00FF));");
    customWriteMemory.append("    writeBuffer[2] = (uint8)((length & 0xFF00) >> 8);");
    customWriteMemory.append("    writeBuffer[3] = (uint8)((length & 0x00FF));");
    customWriteMemory.append("");
    customWriteMemory.append("    for (int i=0; i<length; ++i)");
    customWriteMemory.append("    {");
    customWriteMemory.append("        m_CAL_Storage[startAddress+i] = value[i];");
    customWriteMemory.append("        writeBuffer[4+i] = value[i];");
    customWriteMemory.append("    }");
    customWriteMemory.append("");
    customWriteMemory.append("    // BASE64 encode CAL Storage");
    customWriteMemory.append("    uint32 result_length = 0;");
    customWriteMemory.append("    char* result = base64_encode(writeBuffer, messLength, &result_length);");
    customWriteMemory.append("    char encodedString[result_length+1];");
    customWriteMemory.append("    memset(encodedString, 0, result_length+1);");
    customWriteMemory.append("    memcpy(encodedString, result, result_length);");
    customWriteMemory.append("    free(result);");
    customWriteMemory.append("    free(writeBuffer);");
    customWriteMemory.append("    // Send to Simulator");
    customWriteMemory.append("    sendMonadicBSPFunctionMessage(__FUNCTION__, encodedString);");
    customWriteMemory.append("");
    if (writeMemoryFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
    {
        customWriteMemory.append("    return BSP_OK;");
    }
    customWriteMemory.append("}");
    customWriteMemory.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customWriteMemory);

    /* Custom request mode change PCIC function */
    QStringList customRequestModeChange;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::RequestModeChangeFunctionName);
    const BSPFunction* RequestModeChangeFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customRequestModeChange.append(RequestModeChangeFunction->ReturnType + " " + currentFunctionName + "(BSP_ECUModes newMode)");
    customRequestModeChange.append("{");
    customRequestModeChange.append("    pcicMode = newMode;");
    customRequestModeChange.append("    switch (pcicMode)");
    customRequestModeChange.append("    {");
    customRequestModeChange.append("        case BSP_MODE_NORMAL:");
    customRequestModeChange.append("        {");
    customRequestModeChange.append("            sendMonadicBSPFunctionMessage(\"BSP_RequestModeChange\", \"NORMAL\" );");
    customRequestModeChange.append("        }");
    customRequestModeChange.append("        break;");
    customRequestModeChange.append("        case BSP_MODE_LOWPOWER:");
    customRequestModeChange.append("        {");
    customRequestModeChange.append("            sendMonadicBSPFunctionMessage(\"BSP_RequestModeChange\", \"LOWPOWER\" );");
    customRequestModeChange.append("        }");
    customRequestModeChange.append("        break;");
    customRequestModeChange.append("        case BSP_MODE_SLEEP:");
    customRequestModeChange.append("        {");
    customRequestModeChange.append("            sendMonadicBSPFunctionMessage(\"BSP_RequestModeChange\", \"SLEEP\" );");
    customRequestModeChange.append("        }");
    customRequestModeChange.append("        break;");
    customRequestModeChange.append("        default:");
    customRequestModeChange.append("           //should not be here. Dont send anything");
    customRequestModeChange.append("        break;");
    customRequestModeChange.append("    }");
    customRequestModeChange.append("        APPL_NotifyModeChange(pcicMode);");
    customRequestModeChange.append("    if ( pcicMode == BSP_MODE_SLEEP )");
    customRequestModeChange.append("    {");
    customRequestModeChange.append("        OS_StopTimer();");
    customRequestModeChange.append("    }");
    customRequestModeChange.append("}");
    customRequestModeChange.append("");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customRequestModeChange);

    /* Custom set Display Segments PCIC function */
    if (m_bspSpecification.SegmentDisplay() != NULL)
    {
        QStringList customDisplaySegments;
        currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::DisplaySegments);
        const BSPFunction* DisplaySegmentsFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
        customDisplaySegments.append(DisplaySegmentsFunction->ReturnType + " " + currentFunctionName + "(const uint8* segments, uint8 length)");
        customDisplaySegments.append("{");

        quint8 digitCount = m_bspSpecification.SegmentDisplay()->GetSegmentBitMappings().count();
        quint8 specialSegmentCount = m_bspSpecification.SegmentDisplay()->GetSpecialSegments().count();
        quint8 characterCount = m_bspSpecification.SegmentDisplay()->GetCharacterSegmentMapping().count();

        customDisplaySegments.append("    uint8 localSegments[" + QString::number(digitCount) + "];");
        customDisplaySegments.append("    memcpy(localSegments, segments, sizeof(uint8)*" + QString::number(digitCount) + ");");
        customDisplaySegments.append("    uint8 specialSegmentActive = 0;");

        quint8 indexSpecialSegment[specialSegmentCount][2] = { };
        setupCharAsSegmentsArray(indexSpecialSegment);
        customDisplaySegments.append("    uint8 indexSpecialSegment[" + QString::number(specialSegmentCount) + "][2] =");
        customDisplaySegments.append("    {");
        for (int i = 0; i< specialSegmentCount; ++i)
        {
            customDisplaySegments.append("        {" + QString::number(indexSpecialSegment[i][0]) + ", " + QString::number(indexSpecialSegment[i][1]) + "},");
        }
        customDisplaySegments.append("    };");
        customDisplaySegments.append("    char convertedSegments[" + QString::number(digitCount+1) + "];");
        customDisplaySegments.append("    convertedSegments[" + QString::number(digitCount) + "] = '\\0';");
        customDisplaySegments.append("");
        customDisplaySegments.append("    uint8 charAsSegments[" + QString::number(digitCount) + "][" + QString::number(characterCount) + "] = ");
        customDisplaySegments.append("    {");
        QStringList charDefinitionsPerDigit;
        for (int i = 0; i < digitCount; ++i)
        {
            QString charDefinitionForDigit = "        {";
            QStringList chars;
            for (int j = 0; j < characterCount; ++j)
            {
                chars.append(QString("0x" + QString("%1").arg(m_CharAsSegments[i][j], 2, 16, QChar('0')).toUpper()));
            }
            charDefinitionForDigit.append(chars.join(", "));
            charDefinitionForDigit.append("}");
            charDefinitionsPerDigit.append(charDefinitionForDigit);
        }
        customDisplaySegments.append(charDefinitionsPerDigit.join(",\n"));
        customDisplaySegments.append("    };");
        customDisplaySegments.append("");
        customDisplaySegments.append("    for (int i = 0; i < length; ++i)");
        customDisplaySegments.append("    {");
        for (int i=0; i< specialSegmentCount; ++i)
        {
            customDisplaySegments.append("        if(i == indexSpecialSegment[" + QString::number(i) + "][0])");
            customDisplaySegments.append("        {");
            customDisplaySegments.append("            if((localSegments[i] & (0x01 << indexSpecialSegment[" + QString::number(i) + "][1])) > 0)");
            customDisplaySegments.append("            {");
            customDisplaySegments.append("                specialSegmentActive += 1 << " + QString::number(i) + ";");
            customDisplaySegments.append("                localSegments[i] &=  ~(1 << indexSpecialSegment[" + QString::number(i) + "][1]);");
            customDisplaySegments.append("            }");
            customDisplaySegments.append("        }");
        }
        customDisplaySegments.append("     }");
        customDisplaySegments.append("");
        customDisplaySegments.append("    for (int i = 0; i < length; ++i)");
        customDisplaySegments.append("    {");
        customDisplaySegments.append("        if (localSegments[i] == 0x00)");
        customDisplaySegments.append("        {");
        customDisplaySegments.append("            convertedSegments[i] = 0x2D;");
        customDisplaySegments.append("        }");
        customDisplaySegments.append("        else");
        customDisplaySegments.append("        {");
        customDisplaySegments.append("            convertedSegments[i] = 0x45;");
        customDisplaySegments.append("            for (int j = 0; j < " + QString::number(characterCount) + "; ++j)");
        customDisplaySegments.append("            {");
        customDisplaySegments.append("                if (localSegments[i] == charAsSegments[i][j])");
        customDisplaySegments.append("                {");
        customDisplaySegments.append("                    convertedSegments[i] = 0x30 + j;");
        customDisplaySegments.append("                    break;");
        customDisplaySegments.append("                }");
        customDisplaySegments.append("            }");
        customDisplaySegments.append("        }");
        customDisplaySegments.append("    }");
        customDisplaySegments.append("");
        customDisplaySegments.append("    sendDiadicBSPFunctionMessage(__FUNCTION__, specialSegmentActive, convertedSegments);");
        customDisplaySegments.append("}");
        customDisplaySegments.append("");
        m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customDisplaySegments);
    }

    /* Custom BSP Stop Active SoundGeneration */
    QStringList customStopActiveSound;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::StopActiveSound);
    const BSPFunction* stopActiveSoundFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customStopActiveSound.append(stopActiveSoundFunction->ReturnType + " " + currentFunctionName + "(void)");
    customStopActiveSound.append("{");
    customStopActiveSound.append("    m_sound = 0xFF;");
    customStopActiveSound.append("    sendMonadicBSPFunctionMessage(\"" + currentFunctionName + "\", \"0\");");
    customStopActiveSound.append("}");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customStopActiveSound);

    /* Custom BSP Get Active SoundGeneration Status*/
    QStringList customActiveSoundStatus;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::ActiveSoundGenerationStatus);
    const BSPFunction* activeSoundStatusFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customActiveSoundStatus.append(activeSoundStatusFunction->ReturnType + " " + currentFunctionName + "(void)");
    customActiveSoundStatus.append("{");
    customActiveSoundStatus.append("    if (m_sound == 0xFF)");
    customActiveSoundStatus.append("    {");
    customActiveSoundStatus.append("        return BSP_SOUND_GEN_INACTIVE;");
    customActiveSoundStatus.append("    }");
    customActiveSoundStatus.append("    else");
    customActiveSoundStatus.append("    {");
    customActiveSoundStatus.append("        return BSP_SOUND_GEN_ACTIVE;");
    customActiveSoundStatus.append("    }");
    customActiveSoundStatus.append("}");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customActiveSoundStatus);

    /* Custom BSP Stop Passive SoundGeneration */
    QStringList customStopPassiveSound;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::StopPassiveSound);
    const BSPFunction* stopPassiveSoundFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customStopPassiveSound.append(stopPassiveSoundFunction->ReturnType + " " + currentFunctionName + "(void)");
    customStopPassiveSound.append("{");
    customStopPassiveSound.append("    memset(m_Sounds, 0x00, sizeof(m_Sounds));");
    customStopPassiveSound.append("    sendMonadicBSPFunctionMessage(\"" + currentFunctionName + "\", \"0\");");
    customStopPassiveSound.append("}");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customStopPassiveSound);

    /* Custom BSP Get Passive SoundGeneration Status*/
    QStringList customPassiveSoundStatus;
    currentFunctionName = m_bspSpecification.GetConstantName(BSPSpecification::PassiveSoundGenerationStatus);
    const BSPFunction* passiveSoundStatusFunction = m_bspSpecification.GetFunctionByName(currentFunctionName);
    customPassiveSoundStatus.append(passiveSoundStatusFunction->ReturnType + " " + currentFunctionName + "(void)");
    customPassiveSoundStatus.append("{");
    customPassiveSoundStatus.append("    return BSP_SOUND_GEN_INACTIVE;");
    customPassiveSoundStatus.append("}");
    m_customBSPImplementationsByFunctionName.insert(currentFunctionName, customPassiveSoundStatus);
}

void BSPGenerator::setupCharAsSegmentsArray(quint8 indexSpecialSegment[][2])
{
    const BSPSegmentDisplay* segmentDisplay = m_bspSpecification.SegmentDisplay();
    QStringList specialSegments = segmentDisplay->GetSpecialSegments();


    const QMap <QChar, const BSPSegmentDisplay::SegmentStatus*> charSegmentMappings = segmentDisplay->GetCharacterSegmentMapping();
    QMapIterator <quint8, const BSPSegmentDisplay::SegmentBitMapping*>itBitMapping(segmentDisplay->GetSegmentBitMappings());

    quint8 index = 0;
    quint8 indexSpecialSegments = 0;

    while (itBitMapping.hasNext())
    {
        itBitMapping.next();
        for(int i = 0; i < specialSegments.count(); ++i)
        {
            if(itBitMapping.value()->Bit0 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 0;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit1 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 1;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit2 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 2;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit3 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 3;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit4 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 4;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit5 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 5;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit6 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 6;
                ++indexSpecialSegments;
            }
            else if(itBitMapping.value()->Bit7 == specialSegments[i])
            {
                indexSpecialSegment[indexSpecialSegments][0] = itBitMapping.key();
                indexSpecialSegment[indexSpecialSegments][1] = 7;
                ++indexSpecialSegments;
            }
        }

        index = 0;
        QMapIterator <QChar, const BSPSegmentDisplay::SegmentStatus*> it(charSegmentMappings);
        while (it.hasNext())
        {
            quint8 value = 0x00;
            it.next();

            if (it.value()->A)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "A");
            }
            if (it.value()->B)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "B");
            }
            if (it.value()->C)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "C");
            }
            if (it.value()->D)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "D");
            }
            if (it.value()->E)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "E");
            }
            if (it.value()->F)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "F");
            }
            if (it.value()->G)
            {
                value += getSegmentInMappingPosition(itBitMapping.key(), "G");
            }
            m_CharAsSegments[itBitMapping.key()][index] = value;
            ++index;
        }
    }
}

quint8 BSPGenerator::getSegmentInMappingPosition(const quint8 index, QString character)
{
    const BSPSegmentDisplay* segmentDisplay = m_bspSpecification.SegmentDisplay();
    QMap<quint8, const BSPSegmentDisplay::SegmentBitMapping*> segmentBitMappings = segmentDisplay->GetSegmentBitMappings();


    const BSPSegmentDisplay::SegmentBitMapping* segmentBitMapping = segmentBitMappings.value(index);

    if (segmentBitMapping->Bit0 == character)
    {
        return 1 << 0;
    }
    else if (segmentBitMapping->Bit1 == character)
    {
        return 1 << 1;
    }
    else if (segmentBitMapping->Bit2 == character)
    {
        return 1 << 2;
    }
    else if (segmentBitMapping->Bit3 == character)
    {
        return 1 << 3;
    }
    else if (segmentBitMapping->Bit4 == character)
    {
        return 1 << 4;
    }
    else if (segmentBitMapping->Bit5 == character)
    {
        return 1 << 5;
    }
    else if (segmentBitMapping->Bit6 == character)
    {
        return 1 << 6;
    }
    else if (segmentBitMapping->Bit7 == character)
    {
        return 1 << 7;
    }
    return 0;
}

