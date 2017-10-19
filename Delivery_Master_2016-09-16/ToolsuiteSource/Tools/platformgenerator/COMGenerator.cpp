#include "COMGenerator.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "CANMessage.h"
#include "Logger.h"
#include "PlatformPaths.h"
#include <QStringList>

COMGenerator::COMGenerator(Project* project) :
    m_project(project),
    m_canSpecification(project->GetCANSpecification()),
    m_functionBlocks(project->GetFunctionBlocks())
{
    m_CANConfigSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("COM")).append("/src/CAN_IF_Cfg.c");
    m_CANMessageDataSourcePath = QString(PlatformPaths::CreatePlatformComponentPath("COM")).append("/src/CAN_Message_Data.c");
    m_COMGCLRelaySourcePath = QString(PlatformPaths::CreatePlatformComponentPath("COM")).append("/src/COM_GCL_Relay.c");
    m_COMGCLRelayHeaderPath = QString(PlatformPaths::CreatePlatformComponentPath("COM")).append("/inc/COM_GCL_Relay.h");
    m_CANIFCFGHeaderPath= QString(PlatformPaths::CreatePlatformComponentPath("COM")).append("/inc/CAN_IF_Cfg.h");
}

COMGenerator::~COMGenerator()
{
}

void COMGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    if (!m_project->GetCANSpecificationPath().isEmpty() && m_project->GetCANClusterECUName().isEmpty())
    {
        LOG_ERROR("No cluster CAN node name set in project, COM component will not be in a usable state!");
    }

    generateCANConfig();
    generateCANMessageDataSource();
    generateCOMGCLRelayHeader();
    generateCOMGCLRelaySource();
    generateCANIFConfigHeader();
}


void COMGenerator::generateCANIFConfigHeader(void)
{
    QStringList fileLines;
    fileLines.append("#ifndef CAN_IF_CFG_H_                                ");
    fileLines.append("#define CAN_IF_CFG_H_                                ");
    fileLines.append("                                                     ");
    fileLines.append("#include \"CAN_Driver.h\"                            ");
    fileLines.append("#include \"Platform_Types.h\"                        ");
    fileLines.append("                                                     ");
    fileLines.append("#ifdef SCHEDULER_PROFILING");
    fileLines.append("#define USED_NUMBER_OF_MB  " + QString::number(m_canSpecification.Messages().count() + 14) + "u"); //3 Diag ID's + 3 J1939 and 8 debug mailboxes
    fileLines.append("#else");
    fileLines.append("#define USED_NUMBER_OF_MB  " + QString::number(m_canSpecification.Messages().count() + 6) + "u"); //3 Diag ID's + 3 J1939
    fileLines.append("#endif");
    fileLines.append("#define USED_CAN_CONTROLLER_INDEX	0                  ");
    fileLines.append("                                                     ");
    fileLines.append("typedef struct                                       ");
    fileLines.append("{                                                    ");
    fileLines.append("	CAN_MODES	eTxMode;                               ");
    fileLines.append("	uint32		u32Mask;                               ");
    fileLines.append("	uint32		u32CanId;                              ");
    fileLines.append("	uint8		u8FrameLength;                         ");
    fileLines.append("} tCanMbCfg;                                         ");
    fileLines.append("");
    fileLines.append("extern const uint8 g_CanMbCount;                     ");
    fileLines.append("extern const tCanMbCfg g_CanIfConfig[USED_NUMBER_OF_MB];");
    fileLines.append("extern const uint32 g_CanTimingCtrlRegister;         ");
    fileLines.append("                                                     ");
    fileLines.append("//J1939 configuration                                ");
    fileLines.append("extern const uint32 g_u32J1939SFID;                         ");
    fileLines.append("extern const uint32 g_u32J1939SFMask;                       ");
    fileLines.append("extern const uint32 g_u32J1939FFID;                         ");
    fileLines.append("extern const uint32 g_u32J1939FFMask;                       ");
    fileLines.append("extern const uint32 g_u32J1939CFID;                         ");
    fileLines.append("extern const uint32 g_u32J1939CFMask;                       ");
    fileLines.append("                                                     ");
    fileLines.append("//Diagnostic configuration                                                   ");
    fileLines.append("#define DIAG_FUNC_REQ_ID 0x" + QString::number(m_project->GetCANSpecification().USDTFuncReqID(),16) +"u\t/*Diagnostic Functional Request*/ ");
    fileLines.append("#define DIAG_REQ_ID      0x" + QString::number(m_project->GetCANSpecification().USDTReqID(),16) +"u\t/*Diagnostic Request*/");
    fileLines.append("#define DIAG_RESP_ID     0x" + QString::number(m_project->GetCANSpecification().USDTRespID(),16) +"u\t/*Diagnostic Response*/");
    fileLines.append("                                                     ");
    fileLines.append("#endif /* CAN_IF_CFG_H_ */                           ");
    fileLines.append("");

    AddGeneratedFile(m_CANIFCFGHeaderPath, fileLines.join("\n"));
}

void COMGenerator::generateCANConfig()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"CAN_IF_Cfg.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    quint32 timingControlRegister = getTimingControlRegisterValue();
    fileLines.append("const uint32 g_CanTimingCtrlRegister = 0x" + QString::number(timingControlRegister, 16) + ";");
    fileLines.append("");
    fileLines.append("const uint8 g_CanMbCount = USED_NUMBER_OF_MB;");
    fileLines.append("");
    fileLines.append("const uint32 g_u32J1939SFID    = 0x" + QString::number(m_project->GetCANSpecification().J1939SFID(),16)+";");
    fileLines.append("const uint32 g_u32J1939SFMask  = 0x" + QString::number(m_project->GetCANSpecification().J1939SFMask(),16)+";");
    fileLines.append("const uint32 g_u32J1939FFID    = 0x" + QString::number(m_project->GetCANSpecification().J1939FFID(),16)+";");
    fileLines.append("const uint32 g_u32J1939FFMask  = 0x" + QString::number(m_project->GetCANSpecification().J1939FFMask(),16)+";");
    fileLines.append("const uint32 g_u32J1939CFID    = 0x" + QString::number(m_project->GetCANSpecification().J1939CFID(),16)+";");
    fileLines.append("const uint32 g_u32J1939CFMask  = 0x" + QString::number(m_project->GetCANSpecification().J1939CFMask(),16)+";");
    fileLines.append("");
    fileLines.append("const tCanMbCfg g_CanIfConfig[USED_NUMBER_OF_MB] = ");
    fileLines.append("{");
    fileLines.append("    {CAN_MODE_RX, 0x7FF , 0x" + QString::number(m_project->GetCANSpecification().USDTFuncReqID(),16) +", 8}, /*Diagnostic Functional Request*/ ");
    fileLines.append("    {CAN_MODE_RX, 0x7FF , 0x" + QString::number(m_project->GetCANSpecification().USDTReqID(),16) +", 8}, /*Diagnostic Request*/");
    fileLines.append("    {CAN_MODE_TX, 0x7FF , 0x" + QString::number(m_project->GetCANSpecification().USDTRespID(),16) +", 8}, /*Diagnostic Response*/");

    fileLines.append("    {CAN_MODE_RX, 0x" + QString::number(m_project->GetCANSpecification().J1939SFMask(),16) + " , 0x" + QString::number(m_project->GetCANSpecification().J1939SFID(),16) +", 8}, /*J1939 Single frame*/ ");
    fileLines.append("    {CAN_MODE_RX, 0x" + QString::number(m_project->GetCANSpecification().J1939FFMask(),16) + " , 0x" + QString::number(m_project->GetCANSpecification().J1939FFID(),16) +", 8}, /*J1939 First frame*/ ");
    fileLines.append("    {CAN_MODE_RX, 0x" + QString::number(m_project->GetCANSpecification().J1939CFMask(),16) + " , 0x" + QString::number(m_project->GetCANSpecification().J1939CFID(),16) +", 8}, /*J1939 Consecutive frame*/ ");




    QStringList messageMailboxConfigurations;
    QString canClusterECUName(m_project->GetCANClusterECUName());
    foreach (const CANMessage* message, m_canSpecification.Messages())
    {
          if (message->Producer.compare(canClusterECUName) == 0)
          {
              messageMailboxConfigurations.append("    {CAN_MODE_TX, 0x" + QString::number(message->CANID, 16) +", 0x" + QString::number(message->CANID, 16) + ", " + QString::number(message->DLC) + "} /*" + message->Name + "*/");
          }
          else
          {
              foreach (const CANSignal* signal, message->Signals())
              {
                  if (signal->Consumers.contains(canClusterECUName))
                  {
                      messageMailboxConfigurations.append("    {CAN_MODE_RX, 0x1FFFFFFF , 0x" + QString::number(message->CANID, 16) + ", " + QString::number(message->DLC) + "} /*" + message->Name + "*/");
                      break;
                  }
              }
          }
    }

    fileLines.append(messageMailboxConfigurations.join(",\n"));
    fileLines.append("#ifdef SCHEDULER_PROFILING");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append(", { CAN_MODE_TX, 0x1FFFFFFF, 0x1FFF0000, 8 } // Scheduler profiling");
    fileLines.append("#endif //SCHEDULER_PROFILING");
    fileLines.append("};");
    fileLines.append("");

    AddGeneratedFile(m_CANConfigSourcePath, fileLines.join("\n"));
}

void COMGenerator::generateCANMessageDataSource()
{
    QStringList fileLines;
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"CAN_Message_Data.h\"");
    fileLines.append("#include \"COM_GCL_Relay.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");

    QStringList rxMessageLines;
    quint16 rxMessageCount = 0;
    QStringList txMessageLines;
    quint16 txMessageCount = 0;
    fileLines.append("//lint -save");
    fileLines.append("//lint -esym(9003, rx_messages) [CAN messages are generated and used in a static file]");
    fileLines.append("//lint -esym(9003, tx_messages) [CAN messages are generated and used in a static file]");
    foreach (const CANMessage* message, m_canSpecification.Messages())
    {
        if (message->Producer.compare(m_project->GetCANClusterECUName()) == 0)
        {
            ++txMessageCount;
            txMessageLines.append("    { .CanId = " + QString::number(message->CANID) + ", " +
                                        ".Data = { 0, 0, 0, 0, 0, 0, 0, 0 }, " +
                                        ".Status = 0, " +
                                        ".Periodicity = " + QString::number(message->CycleTime) + ", " +
                                        ".Delay = " + QString::number(message->Delay) + ", " +
                                        ".Event = FALSE, " +
                                        ".NextTx = 0, " +
                                        ".LastTx = 0, " +
                                        ".Dlc = " + QString::number(message->DLC) + ", " +
                                        ".SendType = " + CANMessage::SendTypeString(message->CANMessageSendType) + "}");
        }
        else
        {
            bool isConsumer = false;

            foreach (const CANSignal* signal, message->Signals())
            {
                if (signal->Consumers.contains(m_project->GetCANClusterECUName()))
                {
                    isConsumer = true;
                    break;
                }
            }


            if (isConsumer)
            {
                bool hasNotification = false;
                foreach (const CANSignal* signal, message->Signals())
                {
                    if (signalHasNotification(signal))
                    {
                        hasNotification = true;
                        break;
                    }
                }

                ++rxMessageCount;
                QString notificationCallback = hasNotification ? "COM_" + message->Name + "_Notification"  : "(void(*)(void))0";
                rxMessageLines.append("    { .CanId = " + QString::number(message->CANID) + ", " +
                                            ".Data = { 0, 0, 0, 0, 0, 0, 0, 0 }, " +
                                            ".Status = 0, " +
                                            ".TimeOut = " + QString::number(message->Timeout) + ", " +
                                            ".LastRx = 0, " +
                                            ".NotificationCallback = " + notificationCallback +  "}");
            }
        }

    }
    fileLines.append("static rx_message_t rx_messages[" + QString::number(rxMessageCount) + "] =");
    fileLines.append("{");
    fileLines.append(rxMessageLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("");
    fileLines.append("static tx_message_t tx_messages[" + QString::number(txMessageCount) + "] =");
    fileLines.append("{");
    fileLines.append(txMessageLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("");
    fileLines.append("/************************* Local Function Prototypes *************************/");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("rx_message_t* Get_CAN_Rx_Messages(uint16* count)");
    fileLines.append("{");
    fileLines.append("    *count = " + QString::number(rxMessageCount) + ";");
    fileLines.append("    return rx_messages;");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("tx_message_t* Get_CAN_Tx_Messages(uint16* count)");
    fileLines.append("{");
    fileLines.append("    *count = " + QString::number(txMessageCount) + ";");
    fileLines.append("    return tx_messages;");
    fileLines.append("}");
    fileLines.append("");
    fileLines.append("//lint -restore");
    fileLines.append("");

    AddGeneratedFile(m_CANMessageDataSourcePath, fileLines.join("\n"));
}


void COMGenerator::generateCOMGCLRelayHeader()
{
    QStringList includeLines;
    includeLines.append("#ifndef COM_GCL_RELAY_H");
    includeLines.append("#define COM_GCL_RELAY_H");
    includeLines.append("");
    includeLines.append("/********************************* Includes **********************************/");
    includeLines.append("#include \"StandardTypes.h\"");

    QStringList headerFileLines;

    headerFileLines.append("");

    QStringList headerNotificationLines;

    foreach (const CANMessage* message, m_canSpecification.Messages())
    {
        bool messageHasSignalsWithNotification;
        headerFileLines.append("/***** Begin Message " + message->Name + " *****/");

        foreach (const CANSignal* signal, message->Signals())
        {
            if (signal->Consumers.contains(m_project->GetCANClusterECUName()))
            {
                headerFileLines.append("void COM_Read_" + signal->Name + "(uint32* value, uint8* status);");

                if (signalHasNotification(signal))
                {
                    messageHasSignalsWithNotification = true;
                }
            }
            else if (message->Producer.compare(m_project->GetCANClusterECUName()) == 0)
            {
                headerFileLines.append("void COM_Write_" + signal->Name + "(uint32 value);");
            }
            else
            {
                LOG_DEBUG("GCL_COM_Relay skipping signal " + signal->Name + " in message " + message->Name + " as the cluster neither produces nor consumes this signal");
                continue;
            }
        }

        if (messageHasSignalsWithNotification)
        {
            includeLines.append("#include \"GCL_COM.h\"");
            headerNotificationLines.append("void COM_" + message->Name + "_Notification(void);");
        }

        headerFileLines.append("/***** End Message " + message->Name + " *****/");
        headerFileLines.append("");
    }

    headerFileLines.append("/***** Notifications *****/");
    headerFileLines.append(headerNotificationLines);
    headerFileLines.append("");
    headerFileLines.append("#endif //  COM_GCL_RELAY_H");
    headerFileLines.append("");

    includeLines.removeDuplicates();
    includeLines.append(headerFileLines);
    AddGeneratedFile(m_COMGCLRelayHeaderPath, includeLines.join("\n"));
}


void COMGenerator::generateCOMGCLRelaySource()
{
    QStringList sourceFileLines;
    sourceFileLines.append("/********************************* Includes **********************************/");
    sourceFileLines.append("#include \"COM_GCL_Relay.h\"");
    sourceFileLines.append("#include \"COM.h\"");

    QStringList notificationFunctions;
    QStringList exportedFunctions;
    QStringList notificationLastValueMembers;

    foreach (const CANMessage* message, m_canSpecification.Messages())
    {
        QStringList signalNotificationsInMessage;
        exportedFunctions.append("/***** Begin Message " + message->Name + " *****/");

        foreach (const CANSignal* signal, message->Signals())
        {
            if (signal->Consumers.contains(m_project->GetCANClusterECUName()))
            {
                exportedFunctions.append("void COM_Read_" + signal->Name + "(uint32* value, uint8* status)");
                exportedFunctions.append("{");
                exportedFunctions.append("    COM_Read_Signal(" + QString::number(message->CANID) + ", " + QString::number(signal->StartBit) + ", " + QString::number(signal->BitLength) + ", value, status);");
                exportedFunctions.append("}");

                if (signalHasNotification(signal))
                {
                    const GCLSignalSource* source = m_project->GetCANSpecification().GetSignalSource(signal->Name);
                    signalNotificationsInMessage.append(source->SignalName());

                    notificationLastValueMembers.append("static uint32 last_" + source->SignalName() + "_value = 0;");
                }
            }
            else if (message->Producer.compare(m_project->GetCANClusterECUName()) == 0)
            {
                exportedFunctions.append("void COM_Write_" + signal->Name + "(uint32 value)");
                exportedFunctions.append("{");
                if(signal->IsEventTrigger)
                {

                }
                exportedFunctions.append("    COM_Write_Signal(" + QString::number(message->CANID) + ", " + QString::number(signal->StartBit) + ", " + QString::number(signal->BitLength) + ", value," + QString(signal->IsEventTrigger ? "TRUE" : "FALSE") + ");");
                exportedFunctions.append("}");
            }
            else
            {
                LOG_DEBUG("GCL_COM_Relay skipping signal " + signal->Name + " in message " + message->Name + " as the cluster neither produces nor consumes this signal");
                continue;
            }
        }

        if (signalNotificationsInMessage.count() > 0)
        {
            notificationFunctions.append("void COM_" + message->Name + "_Notification()");
            notificationFunctions.append("{");
            notificationFunctions.append("    uint8 status;");
            notificationFunctions.append("    uint32 currentValue;");

            foreach (QString signal, signalNotificationsInMessage)
            {
                notificationFunctions.append("");
                notificationFunctions.append("    static uint32 last_" + signal + "_value = 0;");
                notificationFunctions.append("    COM_Read_" + signal + "(&currentValue, &status);");
                notificationFunctions.append("    if (currentValue != last_" + signal + "_value)");
                notificationFunctions.append("    {");
                notificationFunctions.append("        GCL_CAN_" + signal + "_Indication();");
                notificationFunctions.append("        last_" + signal + "_value = currentValue;");
                notificationFunctions.append("    }");
            }
            notificationFunctions.append("}");
            notificationFunctions.append("");
        }
        exportedFunctions.append("/***** End Message " + message->Name + " *****/");
        exportedFunctions.append("");
    }

    sourceFileLines.append("/**************************** Constants and Types ****************************/");
    sourceFileLines.append("");
    sourceFileLines.append("/********************************* Variables *********************************/");
    sourceFileLines.append("");
    sourceFileLines.append("/************************* Local Function Prototypes *************************/");
    sourceFileLines.append("");
    sourceFileLines.append("/***************************** Exported Functions ****************************/");
    sourceFileLines.append(exportedFunctions);
    sourceFileLines.append("/***** Notifications *****/");
    sourceFileLines.append(notificationFunctions);
    sourceFileLines.append("");

    AddGeneratedFile(m_COMGCLRelaySourcePath, sourceFileLines.join("\n"));
}

bool COMGenerator::signalHasNotification(const CANSignal* signal)
{
    const GCLSignalSource* source = m_project->GetCANSpecification().GetSignalSource(signal->Name);

    foreach (const GCLSignalSink* sink, source->GetConnectedSinks())
    {
        if (sink->HasUpdateNotification())
        {
            return true;
        }
    }

    return false;
}

quint32 COMGenerator::getTimingControlRegisterValue()
{
    CANSpecification::DriverParameters driverParameters = m_canSpecification.GetDriverParameters();
    quint32 timingControlRegister = 0;

    timingControlRegister |= driverParameters.PRESDIV << 24;
    timingControlRegister |= driverParameters.PSEG1 << 19;
    timingControlRegister |= driverParameters.PSEG2 << 16;
    timingControlRegister |= driverParameters.PROPSEG;

    return timingControlRegister;
}
