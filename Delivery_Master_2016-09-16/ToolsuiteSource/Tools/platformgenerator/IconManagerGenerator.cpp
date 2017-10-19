#include "IconManagerGenerator.h"

#include "Icon.h"
#include "IconConverter.h"
#include "Logger.h"
#include "RegEx/RegExPatterns.h"

#include <IconDefinition.h>
#include <QTextStream>

IconManagerGenerator::IconManagerGenerator(Project* project) :
    m_project(project),
    m_iconDefinition(project->GetIconDefinition())
{
    m_iconManagerHeaderPath = QString(PlatformPaths::CreateServicePath("IconManager")).append("/IconManager.h");
    m_iconManagerSourcePath = QString(PlatformPaths::CreateServicePath("IconManager")).append("/IconManager.c");
    m_iconRoot = QString(PlatformPaths::CreateServicePath("Icons"));
}

void IconManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateIconManagerHeader();
    generateIconManagerSource();
    generateIconHeaders();
    generateIconIncludeHeader();
}

void IconManagerGenerator::generateIconManagerHeader()
{
    QStringList headerLines;

    headerLines.append("#ifndef ICON_MANAGER_H");
    headerLines.append("#define ICON_MANAGER_H");
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("#include \"StandardTypes.h\"");
    headerLines.append("#include \"Icon.h\"");
    headerLines.append("");
    headerLines.append("/**************************** Constants and Types ****************************/");
    headerLines.append("");
    headerLines.append("/***************************** Exported Functions ****************************/");
    headerLines.append("const icon_t* IconManager_GetIcon(uint32 iconID);");
    headerLines.append("");
    headerLines.append("#endif");
    headerLines.append("");

    AddGeneratedFile(m_iconManagerHeaderPath, headerLines.join("\n"));
}

void IconManagerGenerator::generateIconManagerSource()
{
    QStringList sourceLines;
    sourceLines.append("//lint -save");
    sourceLines.append("/********************************* Includes **********************************/");
    sourceLines.append("#include \"IconManager.h\"");
    sourceLines.append("#include \"Icons.h\"");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("typedef struct");
    sourceLines.append("{");
    sourceLines.append("    uint32 iconID;");
    sourceLines.append("    const icon_t* icon;");
    sourceLines.append("} icon_definition_t;");
    sourceLines.append("");
    sourceLines.append("/********************************* Variables *********************************/");
    sourceLines.append("");
    sourceLines.append("/************************* Local Function Prototypes *************************/");
    sourceLines.append("");
    sourceLines.append("/**************************** Constants and Types ****************************/");
    sourceLines.append("#define NUMBER_OF_ICONS " + QString::number(m_iconDefinition.Icons().count()) + "u");
    sourceLines.append("");

    if (m_iconDefinition.Icons().count() > 0)
    {
        sourceLines.append("//lint -esym(9003,iconDefinitions) [Supressed as we need to separate generated and static code.]");
        sourceLines.append("static const icon_definition_t iconDefinitions[NUMBER_OF_ICONS] = ");
        sourceLines.append("{");
        QStringList iconMappings;
        foreach (Icon* icon, m_iconDefinition.Icons())
        {
            QString iconMapping("    { .iconID = %1, .icon=&Icon%1 }");
            iconMappings.append(iconMapping.arg(icon->GetID()));
        }
        sourceLines.append(iconMappings.join(",\n"));
        sourceLines.append("};");
        sourceLines.append("");
    }

    sourceLines.append("/***************************** Exported Functions ****************************/");
    sourceLines.append("const icon_t* IconManager_GetIcon(uint32 iconID)");
    sourceLines.append("{");
    sourceLines.append("    const icon_t* returnValue = NULL;");

    if (m_iconDefinition.Icons().count() > 0)
    {
        sourceLines.append("    for (uint32 i = 0; i < NUMBER_OF_ICONS; ++i)");
        sourceLines.append("    {");
        sourceLines.append("        if (iconDefinitions[i].iconID == iconID)");
        sourceLines.append("        {");
        sourceLines.append("            returnValue = iconDefinitions[i].icon;");
        sourceLines.append("            break;");
        sourceLines.append("        }");
        sourceLines.append("    }");
        sourceLines.append("");
    }
    sourceLines.append("    return returnValue;");
    sourceLines.append("}");
    sourceLines.append("");
    sourceLines.append("//lint -restore");
    sourceLines.append("");

    AddGeneratedFile(m_iconManagerSourcePath, sourceLines.join("\n"));
}

void IconManagerGenerator::generateIconHeaders()
{
    foreach (Icon* icon, m_iconDefinition.Icons())
    {
       QString iconName = "Icon" + QString::number(icon->GetID());
       iconName = iconName.replace(RegExPatterns::NotAcceptedCodeCharacters(), "");

       QString iconFilePath(m_project->GetProjectRoot().absoluteFilePath(icon->GetPath()));

       QString iconHeader = IconConverter::GenerateIconHeader(iconFilePath, icon->GetID());

       QString path(m_iconRoot + "/" + iconName + ".h");
       AddGeneratedFile(path, iconHeader);

       m_iconHeaderFiles.append(iconName + ".h");
    }
}

void IconManagerGenerator::generateIconIncludeHeader()
{
    QStringList headerLines;
    QString inclusionGuardName("ICONS_H");

    headerLines.append("#ifndef " + inclusionGuardName);
    headerLines.append("#define " + inclusionGuardName);
    headerLines.append("");
    headerLines.append("/********************************* Includes **********************************/");
    headerLines.append("");

    foreach (QString iconHeader, m_iconHeaderFiles)
    {
        headerLines.append("#include \"" + iconHeader + "\"");
    }

    headerLines.append("");
    headerLines.append("#endif");
    headerLines.append("");

    AddGeneratedFile(m_iconRoot + "/Icons.h", headerLines.join("\n"));
}



