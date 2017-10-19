#include "DisplayManagerGenerator.h"
#include "Logger.h"
#include "LayoutDefinition.h"

#include <QColor>

DisplayManagerGenerator::DisplayManagerGenerator(Project* project) :
    m_display(project->GetBSPSpecification().GetDisplay()),
    m_layout(project->GetLayoutDefinition())
{
    m_DisplayManagerConfigSourceFile = QString(PlatformPaths::CreateServicePath("DisplayManager")).append("/DisplayManager_cfg.c");
    m_DisplayManagerConfigHeaderFile = QString(PlatformPaths::CreateServicePath("DisplayManager")).append("/DisplayManager_cfg.h");
}

DisplayManagerGenerator::~DisplayManagerGenerator()
{
}

void DisplayManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    LOG_INFO("Generating DisplayManager...");
    generateDisplayManagerConfigurationSource();
    generateDisplayManagerConfigurationHeader();
    createStaticDisplayManagerFiles();
    LOG_INFO("Generating DisplayManager...done!");
}

void DisplayManagerGenerator::generateDisplayManagerConfigurationSource()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DisplayManager_cfg.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if(m_display->GetRenderingType() == BSPDisplay::SW)
    {
        fileLines.append("const uint16 DisplayManager_BytesPerLine = " + QString::number(m_display->GetWidth() / 8) + "u;");
    }
    fileLines.append("const uint8 DisplayManager_LayerCount = " + QString::number(m_display->GetNumberOfLayers()) + "u;");
    fileLines.append("const uint16 DisplayManager_DisplayHeight = " + QString::number(m_display->GetHeight()) + "u;");
    fileLines.append("const uint16 DisplayManager_DisplayWidth = " + QString::number(m_display->GetWidth()) + "u;");
    if(m_display->GetRenderingType() == BSPDisplay::HW)
    {
        quint32 chromaKeyStartColor = m_layout.GetStartChromaKeyColor().rgb() & 0xffffff;
        quint32 chromaKeyEndColor = m_layout.GetEndChromaKeyColor().rgb() & 0xffffff;

        quint16 chromaKeyStartID = m_layout.appendCLUTGetIndex(chromaKeyStartColor);
        quint16 chromaKeyEndID = m_layout.appendCLUTGetIndex(chromaKeyEndColor);

        fileLines.append("const uint8 DisplayManager_ChromaKeyStartColorID = " + QString::number(chromaKeyStartID) + "u;");
        fileLines.append("const uint8 DisplayManager_ChromaKeyEndColorID = " + QString::number(chromaKeyEndID) + "u;");
        fileLines.append("const uint8 DisplayManager_ColorCount = " + QString::number(m_layout.GetCLUT().count()) + "u;");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("layer_metadata_t DisplayManager_ActiveLayerConfig[" + QString::number(m_display->GetNumberOfLayers()) + "] = ");
    fileLines.append("{");
    for (quint8 i = 0; i < m_display->GetNumberOfLayers(); ++i)
    {
        fileLines.append("    { .configuration = { .x = 0u, .y = 0u, .width = 0u, .height = 0u, .bpp = DCU_8_BPP, .isEnabled = FALSE, .isFrameBufferContents = FALSE,  .frameBufferOffset = 0u, .dataSize = 0u, .data = NULL, .opacity = 0u }, .isUpdated = FALSE },");
    }
    fileLines.append("};");
    fileLines.append("");
    fileLines.append("layer_metadata_t DisplayManager_PassiveLayerConfig[" + QString::number(m_display->GetNumberOfLayers()) + "] = ");
    fileLines.append("{");
    for (quint8 i = 0; i < m_display->GetNumberOfLayers(); ++i)
    {
        fileLines.append("    { .configuration = { .x = 0u, .y = 0u, .width = 0u, .height = 0u, .bpp = DCU_8_BPP, .isEnabled = FALSE, .isFrameBufferContents = FALSE,  .frameBufferOffset = 0u, .dataSize = 0u, .data = NULL, .opacity = 0u }, .isUpdated = FALSE },");
    }
    fileLines.append("};");
    fileLines.append("");

    if (m_display->GetRenderingType() == BSPDisplay::HW && m_layout.GetCLUT().count() > 1)
    {
        QStringList cluts;
        foreach (quint32 clut, m_layout.GetCLUT())
        {
            cluts.append("    0x" + QString("%1").arg(clut, 8, 16, QChar('0')).toUpper());
        }

        fileLines.append("uint32 DisplayManager_CLUT[" + QString::number(m_layout.GetCLUT().count()) + "] = ");
        fileLines.append("{");
        fileLines.append(cluts.join(",\n"));
        fileLines.append("};");
        fileLines.append("");
    }

    AddGeneratedFile(m_DisplayManagerConfigSourceFile, fileLines.join("\n"));
}

void DisplayManagerGenerator::generateDisplayManagerConfigurationHeader()
{
    QStringList fileLines;

    fileLines.append("#ifndef DISPLAYMANAGERCFG_H");
    fileLines.append("#define DISPLAYMANAGERCFG_H");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("#include \"LayerConfiguration.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");

    if (m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("#define BUFFER_SIZE (" + QString::number(static_cast<quint32>(m_display->GetAvailableMemory() / 2)) + "u)");
    }
    else
    {
        fileLines.append("#define BUFFER_SIZE (" + QString::number(static_cast<quint32>((m_display->GetWidth() * m_display->GetHeight()) / 8)) + "u)");
    }
    fileLines.append("");
    fileLines.append("extern const uint8  DisplayManager_LayerCount;");
    fileLines.append("extern const uint16 DisplayManager_DisplayHeight;");
    fileLines.append("extern const uint16 DisplayManager_DisplayWidth;");
    if(m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("extern const uint8 DisplayManager_ColorCount;");
        fileLines.append("extern const uint8 DisplayManager_ChromaKeyStartColorID;");
        fileLines.append("extern const uint8 DisplayManager_ChromaKeyEndColorID;");
    }
    else
    {
        fileLines.append("extern const uint16 DisplayManager_BytesPerLine;");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");

    if(m_display->GetRenderingType() == BSPDisplay::HW)
    {
        fileLines.append("extern layer_metadata_t DisplayManager_ActiveLayerConfig[" + QString::number(m_display->GetNumberOfLayers()) + "];");
        fileLines.append("extern layer_metadata_t DisplayManager_PassiveLayerConfig[" + QString::number(m_display->GetNumberOfLayers()) + "];");
        fileLines.append("extern uint32 DisplayManager_CLUT[];");
    }
    else
    {
        fileLines.append("extern layer_metadata_t DisplayManager_ActiveLayerConfig[1];");
        fileLines.append("extern layer_metadata_t DisplayManager_PassiveLayerConfig[1];");
    }
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");
    fileLines.append("#endif // DISPLAYMANAGERCFG_H");
    fileLines.append("");

    AddGeneratedFile(m_DisplayManagerConfigHeaderFile, fileLines.join("\n"));
}

void DisplayManagerGenerator::createStaticDisplayManagerFiles()
{
    AddFilesFromResourcePath(":/DisplayFiles/DisplayManagerFiles");

    if (m_display->GetRenderingType() == BSPDisplay::SW)
    {
        AddFilesFromResourcePath(":/DisplayFiles/MonochromeDisplayFiles");
    }
    else if (m_display->GetRenderingType() == BSPDisplay::HW)
    {
        AddFilesFromResourcePath(":/DisplayFiles/ColorDisplayFiles");
        AddFilesFromResourcePath(":/DisplayFiles/GFX");
    }
    else
    {
        // Maybe introduce some error handling
    }
}
