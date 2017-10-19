#include "DTCManagerGenerator.h"
#include "Logger.h"

DTCManagerGenerator::DTCManagerGenerator(Project *project):
m_project(project)
{
    m_dtcManagerHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("Diagnosis")).append("/cfg/DTCManager.h");
    m_dtcManagerSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("Diagnosis")).append("/cfg/DTCManager.c");
}

void DTCManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateDTCManagerHeader();
    generateDTCManagerSource();
}

void DTCManagerGenerator::generateDTCManagerHeader()
{
    QStringList headerLines;

    headerLines.append("#ifndef DTC_MANAGER_H");
    headerLines.append("#define DTC_MANAGER_H");
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"StandardTypes.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("typedef struct");
    headerLines.append("{");
    headerLines.append("    uint32 dtcID;");
    headerLines.append("    uint32 labelID;");
    headerLines.append("} dtc_definition_t;");
    headerLines.append("");
    headerLines.append("/***************************** Exported Functions ****************************/");
    headerLines.append("uint32 DTCManager_GetDTCLabelID(uint32 dtcID);");
    headerLines.append("");
    headerLines.append("#endif // DTC_MANAGER_H");
    headerLines.append("");

    AddGeneratedFile(m_dtcManagerHeaderPath, headerLines.join("\n"));
}

void DTCManagerGenerator::generateDTCManagerSource()
{
    const QList<DTCLabelMapping*>& dtcLabelMappings = m_project->GetDiagDefinition().GetDTCLabelMappings();

    QStringList sourceLines;
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"DTCManager.h\"");
    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("");
    sourceLines.append("/************************* Local Function Prototypes *************************/");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("#define NUMBER_OF_DTCS " + QString::number(dtcLabelMappings.count()) + "u");
    sourceLines.append("");

    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("uint32 DTCManager_GetDTCLabelID(uint32 dtcID)");
    sourceLines.append("{");
    sourceLines.append("    uint32 returnValue = 0;");

    if (dtcLabelMappings.count() > 0)
    {
        sourceLines.append("    static const dtc_definition_t dtcDefinitions[NUMBER_OF_DTCS] = ");
        sourceLines.append("    {");

        foreach (DTCLabelMapping* mapping, dtcLabelMappings)
        {
            sourceLines.append("        {");
            sourceLines.append("            .dtcID = 0x" + QString("%1").arg(mapping->GetDTC(), 6, 16, QChar('0')).toUpper() + ",");
            sourceLines.append("            .labelID = " + QString::number(mapping->GetLabelID()));
            sourceLines.append("        },");
        }
        sourceLines.append("    };");
        sourceLines.append("");
        sourceLines.append("    for (uint32 i = 0; i < NUMBER_OF_DTCS; ++i)");
        sourceLines.append("    {");
        sourceLines.append("        if (dtcDefinitions[i].dtcID == dtcID)");
        sourceLines.append("        {");
        sourceLines.append("            returnValue = dtcDefinitions[i].labelID;");
        sourceLines.append("            break;");
        sourceLines.append("        }");
        sourceLines.append("    }");
        sourceLines.append("");
    }
    else
    {
        sourceLines.append("    UNUSED(dtcID);");
    }
    sourceLines.append("    return returnValue;");
    sourceLines.append("}");
    sourceLines.append("");

    AddGeneratedFile(m_dtcManagerSourcePath, sourceLines.join("\n"));
}
