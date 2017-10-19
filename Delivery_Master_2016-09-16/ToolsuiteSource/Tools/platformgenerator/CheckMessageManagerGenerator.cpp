#include "CheckMessageManagerGenerator.h"

#include "CheckMessage.h"
#include "GCLSignalSource.h"
#include "Logger.h"
#include "PlatformPaths.h"
#include "Layout.h"
#include "LayoutDefinition.h"


CheckMessageManagerGenerator::CheckMessageManagerGenerator(Project* project) :
    m_project(project),
    m_checkMessagesDefinition(project->GetCheckMessagesDefinition())
{
    m_CheckMessageManagerConfigSourceFilePath = QString(PlatformPaths::CreateServicePath("CheckMessageManager")).append("/CheckMessageManager_cfg.c");
    m_CheckMessageManagerConfigHeaderFilePath = QString(PlatformPaths::CreateServicePath("CheckMessageManager")).append("/CheckMessageManager_cfg.h");
}

CheckMessageManagerGenerator::~CheckMessageManagerGenerator()
{

}

void CheckMessageManagerGenerator::OnGenerateFiles()
{
    bool hasCheckMessageLayout = false;
    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        if (layout->IsCheckMessageScreen())
        {
            hasCheckMessageLayout = true;
            break;
        }
    }

    if(hasCheckMessageLayout)
    {
        LOG_INFO("Generating CheckMessageManager!");
        AddFilesFromResourcePath(":/CheckMessageManagerFiles");
        generateCheckMessageManagerConfigHeaderFile();
        generateCheckMessageManagerConfigSourceFile();
    }
}

void CheckMessageManagerGenerator::generateCheckMessageManagerConfigHeaderFile()
{
    QStringList fileLines;

    fileLines.append("#ifndef CHECKMESSAGEMANAGER_CFG_H");
    fileLines.append("#define CHECKMESSAGEMANAGER_CFG_H");
    fileLines.append("");
    fileLines.append("#define NUMBER_OF_SOURCES " + QString::number(m_checkMessagesDefinition.GetSignalSources().count()) + "u");
    fileLines.append("#define NUMBER_OF_CHECKMESSAGES " + QString::number(m_checkMessagesDefinition.CheckMessages().count()) + "u");
    //fileLines.append("#define CHECKMESSAGES_ENABLED_GLOBALLY " + (m_project->GetLayoutDefinition().HasCheckMessageLayout() ? QString("1u") : QString("0u")));
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint16 sourceIndex;");
    fileLines.append("    void (*write)(uint32 value);");
    fileLines.append("} check_message_source_t;");
    fileLines.append("");
    fileLines.append("typedef enum");
    fileLines.append("{");
    fileLines.append("    CHECKMESSAGE_INACTIVE = 0u,");
    fileLines.append("    CHECKMESSAGE_ACTIVE = 1u,");
    fileLines.append("    CHECKMESSAGE_BUTTON_CLEARED = 2u,");
    fileLines.append("    CHECKMESSAGE_TIMER_RUNNING = 3u,");
    fileLines.append("    CHECKMESSAGE_TIMER_TIMEDOUT = 4u,");
    fileLines.append("    CHECKMESSAGE_CLEARED = 5u");
    fileLines.append("} check_message_status_e;");
    fileLines.append("");
    fileLines.append("typedef enum");
    fileLines.append("{");
    fileLines.append("    CHECKMESSAGE_CLEAR_BY_TRIGGER,");
    fileLines.append("    CHECKMESSAGE_CLEAR_BY_TIMEOUT,");
    fileLines.append("    CHECKMESSAGE_CLEAR_BY_BUTTON");
    fileLines.append("} check_message_clear_by_e;");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint16 index;");
    fileLines.append("    uint32 value;");
    fileLines.append("} check_message_widget_mapping_t;");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint16 prio;");
    fileLines.append("    check_message_status_e state;");
    fileLines.append("    check_message_clear_by_e clearCondition;");
    fileLines.append("    sint32 timeout;");
    fileLines.append("    sint32 timer;");
    fileLines.append("    boolean reoccuring;");
    fileLines.append("    void (*trigger)(uint8* value, uint8* status);");
    fileLines.append("    check_message_widget_mapping_t mappings[NUMBER_OF_SOURCES];");
    fileLines.append("    uint8 allowedInLayoutCount;");
    fileLines.append("    uint8* allowedInLayout;");
    fileLines.append("} check_message_t;");
    fileLines.append("");
    fileLines.append("extern const check_message_source_t CheckMessageManager_Sources[NUMBER_OF_SOURCES];");
    fileLines.append("extern check_message_t CheckMessageManager_Messages[NUMBER_OF_CHECKMESSAGES];");
    fileLines.append("");
    fileLines.append("#endif //CHECKMESSAGEMANAGER_CFG_H");
    fileLines.append("");

    AddGeneratedFile(m_CheckMessageManagerConfigHeaderFilePath, fileLines.join("\n"));
}

void CheckMessageManagerGenerator::generateCheckMessageManagerConfigSourceFile()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"GCL_CheckMessageManager.h\"");
    fileLines.append("#include \"CheckMessageManager_cfg.h\"");
    fileLines.append("");
    fileLines.append("const check_message_source_t CheckMessageManager_Sources[NUMBER_OF_SOURCES] =");
    fileLines.append("{");

    quint32 count = 0;
    foreach (GCLSignalSource* source, m_checkMessagesDefinition.GetSignalSources()) {
        fileLines.append("    { .sourceIndex = " + QString::number(count) + "u, .write = " + source->GetGCLFunctionName() + " },");
        ++count;
    }
    fileLines.append("};");
    fileLines.append("");

    quint32 messageCount = 0;
    QStringList checkLines;

    checkLines.append("check_message_t CheckMessageManager_Messages[NUMBER_OF_CHECKMESSAGES] =");
    checkLines.append("{");
    foreach (CheckMessage* message, m_checkMessagesDefinition.CheckMessages())
    {
        checkLines.append("    {");
        checkLines.append("        .prio = " + QString::number(message->GetPrio()) + "u,");
        checkLines.append("        .state = CHECKMESSAGE_INACTIVE,");
        checkLines.append("        .clearCondition = " + message->GetConditionString() + ",");
        checkLines.append("        .timeout = " + QString::number(message->GetTimeout()) + ",");
        checkLines.append("        .timer = 0,");
        checkLines.append("        .reoccuring = " + (message->GetReoccurring() == true ? QString("TRUE") : QString("FALSE")) + ",");
        checkLines.append("        .trigger = " + message->GetTrigger()->GetGCLFunctionName() + ",");
        QStringList items;
        items.append("        .mappings = { ");
        foreach (CheckMessageItem* item, message->Items())
        {
            items.append("{ .index = " + QString::number(item->Index) + "u, .value = " + QString::number(item->ResourceID) + "u }, ");
        }
        items.append(" },");
        checkLines.append(items.join(""));
        checkLines.append("        .allowedInLayoutCount = " + QString::number(message->GetAllowedLayouts().count()) + "u,");
        if (message->GetAllowedLayouts().count() > 0)
        {
            checkLines.append("        .allowedInLayout = AllowedLayouts" + QString::number(messageCount));
        }
        else
        {
            checkLines.append("        .allowedInLayout = NULL");
        }
        checkLines.append("    },");

        QString allowedLine;
        QStringList allowedLayouts;

        if (message->GetAllowedLayouts().count() > 0)
        {
            allowedLine.append("static uint8 AllowedLayouts" + QString::number(messageCount) + "[" + QString::number(message->GetAllowedLayouts().count()) + "] = { ");

            foreach(quint32 id, message->GetAllowedLayouts())
            {

                Layout* layout = m_project->GetLayoutDefinition().GetLayoutFromID(id);
                qint32 index = m_project->GetLayoutDefinition().Layouts().indexOf(layout);
                allowedLayouts.append(QString::number(index));
            }
            allowedLine.append(allowedLayouts.join(", "));
            allowedLine.append(" } ;");
        }

        fileLines.append(allowedLine);
        ++messageCount;
    }
    checkLines.append("};");
    fileLines.append("");
    fileLines.append(checkLines);
    fileLines.append("");


    AddGeneratedFile(m_CheckMessageManagerConfigSourceFilePath, fileLines.join("\n"));
}
