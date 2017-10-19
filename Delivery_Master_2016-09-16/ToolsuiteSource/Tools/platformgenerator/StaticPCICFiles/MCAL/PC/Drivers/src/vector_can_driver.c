/********************************* Includes **********************************/
#include <windows.h>
#include "vxlapi.h"
#include "CAN_Driver.h"
#include "CAN_IF.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME          "PCIC:CAN"
#define RECEIVE_EVENT_SIZE        1
#define RX_QUEUE_SIZE          4096

#define VECTOR_CONTROLLER_ID      0
#define VECTOR_CAN_CHANNEL        0

/********************************* Variables *********************************/
XLportHandle    g_xlPortHandle              = XL_INVALID_PORTHANDLE;
XLdriverConfig  g_xlDrvConfig;
char            g_AppName[XL_MAX_LENGTH+1]  = "BSPSim";
XLaccess        g_xlChannelMask             = 0;
XLaccess        g_xlPermissionMask          = 0;
unsigned int    g_BaudRate                  = 500000;

static unsigned int g_canWriteErrors;
static boolean bRxOnly;

/********************************* Application ********************************/
void CAN_Init()
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    CAN_InitController(VECTOR_CONTROLLER_ID, 0x00000000);
    CAN_Start(VECTOR_CONTROLLER_ID);
    g_canWriteErrors = 0;
    bRxOnly = FALSE;
}

void CAN_Run()
{
    XLstatus        xlStatus;
    unsigned int    msgsrx = RECEIVE_EVENT_SIZE;
    XLevent         xlEvent;
    static uint8    buffer[8];

    xlStatus = XL_SUCCESS;

    do
    {
        msgsrx = RECEIVE_EVENT_SIZE;
        xlStatus = xlReceive(g_xlPortHandle, &msgsrx, &xlEvent);
        if ( xlStatus != XL_ERR_QUEUE_IS_EMPTY && XL_RECEIVE_MSG == xlEvent.tag)
        {
            int i;
            for(i=0;i<xlEvent.tagData.msg.dlc;i++)
            {
                buffer[i] = xlEvent.tagData.msg.data[i];
            }
            CAN_IF_RxIndication(VECTOR_CONTROLLER_ID, 0, (xlEvent.tagData.msg.id & 0x1FFFFFFF), buffer, xlEvent.tagData.msg.dlc);
            memset(buffer, 0, 8);
        }
    } while (XL_SUCCESS == xlStatus);
}

void CAN_Write(uint8 controllerId, uint8 mailBox, uint32 canID, const uint8* message, uint8 length)
{
    if (bRxOnly == TRUE) return;
    static XLevent       xlEvent;
    XLstatus             xlStatus;
    unsigned int         messageCount = 1;

    memset(&xlEvent, 0, sizeof(xlEvent));

    xlEvent.tag                 = XL_TRANSMIT_MSG;
    xlEvent.tagData.msg.id      = 0x80000000 | canID;
    xlEvent.tagData.msg.dlc     = length;
    xlEvent.tagData.msg.flags   = 0;

    int i;
    for(i=0;i<length;i++)
    {
        xlEvent.tagData.msg.data[i] = message[i];
    }

    xlStatus = xlCanTransmit(g_xlPortHandle, g_xlChannelMask, &messageCount, &xlEvent);
    if(XL_SUCCESS != xlStatus)
    {
        if (g_canWriteErrors++ % 100 == 0)
        {
            Logger_Error(COMPONENTNAME, "%s failed, xlStatus=%s (errorcount:%u)", __FUNCTION__, xlGetErrorString(xlStatus), g_canWriteErrors);
        }
    }
    else
    {
        CAN_IF_TxConfirmation(canID, mailBox, controllerId);
    }
}

void CAN_RxOnly()
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    bRxOnly = TRUE;
}

void CAN_RxAndTx()
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    bRxOnly = FALSE;
}


void CAN_InitController(uint8 u8ControllerId, uint32 u32TimingConfig)
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    XLstatus          xlStatus;
    uint8             i;

    xlStatus = xlOpenDriver();
    if(XL_SUCCESS == xlStatus)
    {
        xlStatus = xlGetDriverConfig(&g_xlDrvConfig);
    }
    else
    {
        Logger_Error(COMPONENTNAME, "%s  xlOpenDriver() failed! xlStatus=%s", __FUNCTION__, xlGetErrorString(xlStatus));
    }

    if(XL_SUCCESS == xlStatus)
    {
        g_xlChannelMask = 0;
        for (i=0; i < g_xlDrvConfig.channelCount; i++)
        {
            XLchannelConfig* config = &g_xlDrvConfig.channel[i];
            if (config->channelBusCapabilities & XL_BUS_ACTIVE_CAP_CAN && i == VECTOR_CAN_CHANNEL && config->hwType == XL_HWTYPE_VIRTUAL)
            {
                Logger_Info(COMPONENTNAME, "Using Channel:%d (%s) hwType:%u, hwChannel:%u channelMask:%lu", i, config->name, config->hwType, config->hwChannel, config->channelMask);
                g_xlChannelMask |= config->channelMask;
            }
        }

        if (!g_xlChannelMask)
        {
            xlStatus = XL_ERROR;
            Logger_Error(COMPONENTNAME, "No CAN channel selected!");
            return;
        }
    }

    g_xlPermissionMask = g_xlChannelMask;

    if(XL_SUCCESS == xlStatus)
    {
        xlStatus = xlOpenPort(&g_xlPortHandle, g_AppName, g_xlChannelMask, &g_xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
    }

    if ((XL_SUCCESS == xlStatus) && (XL_INVALID_PORTHANDLE != g_xlPortHandle))
    {
        if (g_xlChannelMask == g_xlPermissionMask)
        {
            xlStatus = xlCanSetChannelBitrate(g_xlPortHandle, g_xlChannelMask, g_BaudRate);
            if(XL_SUCCESS != xlStatus)
            {
                Logger_Error(COMPONENTNAME, "%s  xlCanSetChannelBitrate(PortHandle:%d, ChannelMask:%d, BaudRate:%d) failed! xlStatus=%s", __FUNCTION__, g_xlPortHandle, g_xlChannelMask, g_BaudRate, xlGetErrorString(xlStatus));
            }
        }
    }
    else
    {
        Logger_Error(COMPONENTNAME, "%s  xlOpenPort(PortHandle:%d, AppName:%d, ChannelMask:%d, PermissionMask:%d) failed! xlStatus=%s", __FUNCTION__, g_xlPortHandle, g_AppName, g_xlChannelMask, g_xlPermissionMask, xlGetErrorString(xlStatus));
        xlClosePort(g_xlPortHandle);
        g_xlPortHandle = XL_INVALID_PORTHANDLE;
        xlStatus = XL_ERROR;
    }

    xlCanResetAcceptance(g_xlPortHandle, g_xlChannelMask, XL_CAN_EXT);

    xlStatus = xlDeactivateChannel(g_xlPortHandle, g_xlChannelMask);
    if(XL_SUCCESS != xlStatus)
    {
        Logger_Error(COMPONENTNAME, "%s  xlDeactivateChannel(PortHandle:%d, ChannelMask:%d) failed! xlStatus=%s", __FUNCTION__, g_xlPortHandle, g_xlChannelMask, xlGetErrorString(xlStatus));
    }
}

void CAN_Start(uint8 controllerId)
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    XLstatus xlStatus;
    xlStatus = xlActivateChannel(g_xlPortHandle, g_xlChannelMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_RESET_CLOCK);
    if(XL_SUCCESS != xlStatus)
    {
        Logger_Error(COMPONENTNAME, "%s xlActivateChannel(PortHandle:%d, ChannelMask%d) failed, xlStatus=%s", __FUNCTION__, g_xlPortHandle, g_xlChannelMask, xlGetErrorString(xlStatus));
    }
}

void CAN_Stop(uint8 controllerId)
{
    Logger_Info(COMPONENTNAME, __FUNCTION__);
    XLstatus xlStatus;
    xlStatus = xlDeactivateChannel(g_xlPortHandle, g_xlChannelMask);
    if(XL_SUCCESS != xlStatus)
    {
        Logger_Error(COMPONENTNAME, "%s failed, xlStatus=%s", __FUNCTION__, xlGetErrorString(xlStatus));
    }
}


void CAN_ConfigMB(uint8 controllerId, uint8 mailBox, CAN_MODES mode, uint32 canID, uint32 canMask)
{
    //Unimplemented
}

void CAN_DisableControllerInterrupts(uint8 controllerId)
{
    //Unimplemented
}

void CAN_EnableControllerInterrupts(uint8 controllerId)
{
    //Unimplemented
}

void CAN_BusOff(uint8 controllerId)
{
    //Unimplemented
}
