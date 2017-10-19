#include "HALGenerator.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Logger.h"
#include "BSPFunction.h"
#include "BSPMapping.h"

#include <QStringList>

HALGenerator::HALGenerator(Project* project) :
    m_bspSpecification(project->GetBSPSpecification())
{
    m_HALHeaderPath = QString(PlatformPaths::GetMCALCommonPath()).append("/HAL.h");
    m_HALSourcePath = QString(PlatformPaths::GetMCALCommonPath()).append("/HAL.c");
}

void HALGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    generateHALHeader();
    generateHALSource();
}

void HALGenerator::generateHALHeader()
{
    QStringList fileLines;
    fileLines.append("#ifndef MCAL_HAL_H");
    fileLines.append("#define MCAL_HAL_H");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"BSP_Types.h\"");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    foreach (const GCLSignalSink* sink, m_bspSpecification.GetSignalSinks())
    {
        const BSPFunction* bspFunction = m_bspSpecification.GetBSPFunctionFromSignalSink(sink);
        if (bspFunction == NULL)
        {
            LOG_ERROR("GCLSignalSink " + sink->ComponentName() + "." + sink->SignalName() + " has no mapped function in the BSP specification!");
            continue;
        }

        if (sink->GetSignalSource() == NULL)
        {
            continue;
        }

        QString functionParameter = sink->DataType() == "void" ? "void" : sink->DataType() + " value";
        fileLines.append("void HAL_Write_" + sink->SignalName() + "(" + functionParameter + ");");
    }

    foreach (const GCLSignalSource* source, m_bspSpecification.GetSignalSources())
    {
        if (source->GetConnectedSinks().count() == 0)
        {
            continue;
        }

        const BSPFunction* function = m_bspSpecification.GetBSPFunctionFromSignalSource(source);
        if (function == NULL)
        {
            LOG_ERROR("GCLSignalSource " + source->ComponentName() + "." + source->SignalName() + " has no mapped function in the BSP specification!");
            continue;
        }

        QString functionParameter;

        if (source->DataType() == "void")
        {
            functionParameter = "uint8* status";
        }
        else
        {
            functionParameter = source->DataType() + "* value, uint8* status";
        }

        fileLines.append("void HAL_Read_" + source->SignalName() + "(" + functionParameter + ");");
    }

    fileLines.append("#endif /* MCAL_HAL_H */");
    fileLines.append("");

    AddGeneratedFile(m_HALHeaderPath, fileLines.join("\n"));
}

void HALGenerator::generateHALSource()
{
    QStringList fileLines;
    fileLines.append("//lint -save");
    fileLines.append("//lint -e9030 [BSP in/outputs and return types are stored as enum]");
    fileLines.append("//lint -e9034 [BSP in/outputs and return types are stored as enum]");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"HAL.h\"");
    fileLines.append("#include \"BSP.h\"");
    fileLines.append("#include \"GCL_Types.h\"");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");

    foreach (const GCLSignalSink* sink, m_bspSpecification.GetSignalSinks())
    {
        const BSPFunction* bspFunction = m_bspSpecification.GetBSPFunctionFromSignalSink(sink);
        if (bspFunction == NULL)
        {
            LOG_ERROR("GCLSignalSink " + sink->ComponentName() + "." + sink->SignalName() + " has no mapped function in the BSP specification!");
            continue;
        }

        if (sink->GetSignalSource() == NULL)
        {
            continue;
        }

        const BSPMapping* mapping = m_bspSpecification.GetMappingFromFunction(bspFunction);

        QString functionParameter = sink->DataType() == "void" ? "void" : sink->DataType() + " value";
        fileLines.append("void HAL_Write_" + sink->SignalName() + "(" + functionParameter + ")");
        fileLines.append("{");
        QString bspFunctionCall;

        if (bspFunction->Arguments.count() == 1)
        {
            QString bspDataType = bspFunction->Arguments.values().at(0).Type;
            bspFunctionCall = bspFunction->Name + "(" + (functionParameter == "void" ? "" : "(" + bspDataType + ")value") + ")";
        }
        else if (bspFunction->Arguments.count() == 2 && mapping != NULL)
        {
            QString bspDataType = bspFunction->Arguments.values().at(1).Type;
            quint8 index = mapping->GetIndexFromSignalName(sink->SignalName());
            bspFunctionCall = bspFunction->Name + "(" + QString::number(index) + "U" + (functionParameter == "void" ? "" : ",(" + bspDataType + ")value") + ")";
        }
        else
        {
            fileLines.append("    // HAL does not know how to handle signals with this signature (argument count: " + QString::number(bspFunction->Arguments.count()) + ")");
        }

        if (!bspFunctionCall.isEmpty())
        {
            if (bspFunction->ReturnType.compare("void") != 0)
            {
                fileLines.append("    (void)" + bspFunctionCall + ";");
            }
            else
            {
                fileLines.append("    " + bspFunctionCall + ";");
            }    
        }
        else if (functionParameter != "void")
        {
            fileLines.append("    UNUSED(value);");
        }

        fileLines.append("}");
        fileLines.append("");
    }

    foreach (const GCLSignalSource* source, m_bspSpecification.GetSignalSources())
    {
        if (source->GetConnectedSinks().count() == 0)
        {
            continue;
        }

        const BSPFunction* bspFunction = m_bspSpecification.GetBSPFunctionFromSignalSource(source);
        if (bspFunction == NULL)
        {
            LOG_ERROR("GCLSignalSource " + source->ComponentName() + "." + source->SignalName() + " has no mapped function in the BSP specification!");
            continue;
        }

        const BSPMapping* mapping = m_bspSpecification.GetMappingFromFunction(bspFunction);


        QString functionParameter = source->DataType() == "void" ? "void" : source->DataType() + "* value";
        if (source->DataType() == "void")
        {
            functionParameter = "uint8* status";
        }
        else
        {
            functionParameter = source->DataType() + "* value, uint8* status";
        }

        fileLines.append("void HAL_Read_" + source->SignalName() + "(" + functionParameter + ")");
        fileLines.append("{");
        QString bspDataType = "";
        QString functionCall;
        bool appendTrailerConversion = false;

        if (bspFunction->Arguments.count() == 0)
        {
            functionCall = bspFunction->Name + "()";
            appendTrailerConversion = false;
        }
        else if (bspFunction->Arguments.count() == 1)
        {
            bspDataType = bspFunction->Arguments.values().at(0).Type;
            bspDataType = stripPointers(bspDataType);
            fileLines.append("    " + bspDataType + " tempValue;");
            functionCall = bspFunction->Name + "(" + (functionParameter == "void" ? "" : "&tempValue") + ")";
            appendTrailerConversion = true;
        }
        else if (bspFunction->Arguments.count() == 2 && mapping != NULL)
        {
            bspDataType = bspFunction->Arguments.values().at(1).Type;
            bspDataType = stripPointers(bspDataType);
            fileLines.append("    " + bspDataType + " tempValue;");
            quint8 index = mapping->GetIndexFromSignalName(source->SignalName());
            functionCall = bspFunction->Name + "(" + QString::number(index) + "U" + (functionParameter == "void" ? "" : ",&tempValue") + ")";
            appendTrailerConversion = true;
        }
        else
        {
            fileLines.append("    // HAL does not know how to handle signals with this signature (argument count: " + QString::number(bspFunction->Arguments.count()) + ")");
            appendTrailerConversion = false;
        }

        if (!functionCall.isEmpty())
        {
            if (bspFunction->ReturnType.compare(BSPSpecification::StdBSPReturnType) == 0)
            {
                fileLines.append("    if (" + functionCall + " != BSP_OK)");
                fileLines.append("    {");
                fileLines.append("        *status |= GCL_SIGNAL_ERROR;");
                fileLines.append("    }");
            }
            else if (bspFunction->ReturnType.compare("void") == 0)
            {
                fileLines.append("    " + functionCall + ";");
                fileLines.append("    *status = GCL_SIGNAL_OK;");
            }
            else
            {
                fileLines.append("    (void)" + functionCall + ";");
                fileLines.append("    *status = GCL_SIGNAL_OK;");
            }

        }
        else
        {
            if (source->DataType() != "void")
            {
                fileLines.append("    UNUSED(value);");
            }
            fileLines.append("    UNUSED(status);");
        }

        if (appendTrailerConversion)
        {
            fileLines.append("    *value = (" + source->DataType() + ")tempValue;");
        }

        fileLines.append("}");
        fileLines.append("");
    }

    fileLines.append("//lint -restore");
    fileLines.append("");

    AddGeneratedFile(m_HALSourcePath, fileLines.join("\n"));
}


QString HALGenerator::stripPointers(QString &datatypeString)
{
    return datatypeString.replace("*", "");
}
