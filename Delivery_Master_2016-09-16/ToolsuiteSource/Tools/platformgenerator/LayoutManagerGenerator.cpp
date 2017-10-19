#include "LayoutManagerGenerator.h"
#include "Logger.h"
#include <QMap>

LayoutManagerGenerator::LayoutManagerGenerator(Project* project) :
    m_layout(project->GetLayoutDefinition())
{
    m_LayoutManagerHeaderFile = QString(PlatformPaths::CreateServicePath("LayoutManager")).append("/LayoutManager.h");
}

LayoutManagerGenerator::~LayoutManagerGenerator()
{}

void LayoutManagerGenerator::OnGenerateFiles()
{
    LOG_INFO("Generating LayoutManager...");
    generateLayoutManagerHeader();
    LOG_INFO("Generating LayoutManager...done!");
}

void LayoutManagerGenerator::generateLayoutManagerHeader()
{
    QStringList fileLines;

    fileLines.append("#ifndef LAYOUTMANAGER");
    fileLines.append("#define LAYOUTMANAGER");
    fileLines.append("");
    fileLines.append("#include \"ContentsManager.h\"");
    if(m_layout.HasMenu())
    {
        fileLines.append("");
        fileLines.append("#define HAS_MENU");
        fileLines.append("");
    }
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("void LayoutManager_Init(void);");
    fileLines.append("void LayoutManager_Run(void);");
    fileLines.append("void LayoutManager_CheckMessageShowing(boolean showing);");
    fileLines.append("dm_state_t LayoutManager_GetState(void);");
    fileLines.append("sint8 LayoutManager_GetCurrentLayoutID(void);");
    fileLines.append("void LayoutManager_SetLayoutFocus(sint8 layoutID, boolean hasFocus);");
    fileLines.append("void LayoutManager_SetWidgetFocus(const ui_widget_t *widget, boolean wantFocus);");
    fileLines.append("void LayoutManager_Up_Indication(void);");
    fileLines.append("void LayoutManager_Down_Indication(void);");
    fileLines.append("void LayoutManager_Set_Indication(void);");
    fileLines.append("void LayoutManager_Clear_Indication(void);");
    fileLines.append("#endif // LAYOUTMANAGER");
    fileLines.append("");

    AddGeneratedFile(m_LayoutManagerHeaderFile, fileLines.join("\n"));
}
