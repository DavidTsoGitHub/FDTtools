#include "DIDCALMappingGenerator.h"
#include "Logger.h"

DIDCalMappingGenerator::DIDCalMappingGenerator(Project* project):
m_project(project)
{
    m_didCALMappingHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("Diagnosis")).append("/cfg/DIDCALMapping.h");
    m_didCALMappingSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("Diagnosis")).append("/cfg/DIDCALMapping.c");
}

DIDCalMappingGenerator::~DIDCalMappingGenerator()
{

}

void DIDCalMappingGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateDIDCALMappingHeader();
    generateDIDCALMappingSource();
}

void DIDCalMappingGenerator::generateDIDCALMappingHeader()
{
    QStringList headerLines;

    headerLines.append("#ifndef DID_CAL_MAPPING_H");
    headerLines.append("#define DID_CAL_MAPPING_H");
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"StandardTypes.h\"");
    headerLines.append("#include \"CAL_cfg.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("typedef struct");
    headerLines.append("{");
    headerLines.append("    uint16 did;");
    headerLines.append("    calibration_parameter_id_t calID;");
    headerLines.append("} did_mapping_t;");
    headerLines.append("");
    headerLines.append("/***************************** Exported Functions ****************************/");
    headerLines.append("boolean DIDCALMapping_GetCALForDID(uint16 did, calibration_parameter_id_t* calID);");
    headerLines.append("");
    headerLines.append("#endif // DID_CAL_MAPPING_H");
    headerLines.append("");

    AddGeneratedFile(m_didCALMappingHeaderPath, headerLines.join("\n"));
}

void DIDCalMappingGenerator::generateDIDCALMappingSource()
{
    const QList<DIDToCALMapping*> mappings = m_project->GetDiagDefinition().GetDIDToCALMappings();

    QStringList sourceLines;
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"DIDCALMapping.h\"");
    sourceLines.append("#include \"CAL_Cfg.h\"");
    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("");
    sourceLines.append("/************************* Local Function Prototypes *************************/");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("#define NUMBER_OF_DIDS " + QString::number(mappings.count()) + "u");

    sourceLines.append("");

    sourceLines.append("boolean DIDCALMapping_GetCALForDID(uint16 did, calibration_parameter_id_t* calID)");
    sourceLines.append("{");
    sourceLines.append("    boolean returnValue = FALSE;");

    if (mappings.count() > 0)
    {
        sourceLines.append("    static const did_mapping_t didMapping[NUMBER_OF_DIDS] = ");
        sourceLines.append("    {");

        foreach (DIDToCALMapping* mapping ,mappings)
        {
            sourceLines.append("        {");
            sourceLines.append("            .did = 0x" + QString("%1").arg(mapping->GetDID(), 4, 16, QChar('0')).toUpper() + ",");
            sourceLines.append("            .calID = " + mapping->GetCALParameter());
            sourceLines.append("        },");
        }

        sourceLines.append("    };");
        sourceLines.append("");
        sourceLines.append("    for (uint16 i = 0; i < NUMBER_OF_DIDS; ++i)");
        sourceLines.append("    {");
        sourceLines.append("        if (didMapping[i].did == did)");
        sourceLines.append("        {");
        sourceLines.append("            *calID = didMapping[i].calID;");
        sourceLines.append("            returnValue = TRUE;");
        sourceLines.append("            break;");
        sourceLines.append("        }");
        sourceLines.append("    }");
    }
    sourceLines.append("    return returnValue;");
    sourceLines.append("}");
    sourceLines.append("");

    AddGeneratedFile(m_didCALMappingSourcePath, sourceLines.join("\n"));
}
