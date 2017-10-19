/**************************
Copyright Swedspot AB 2014
Authors:
Short      Full name
laan       Lars Andersson

Revision history:

Date        author     Changelog
--------------------------------
2015-03-06  laan       Initial version
***************************/

#include "Platform_Types.h"
#include "PDU_Router.h"
#include "CAN_IF_Cfg.h"
#include "CAN_Driver.h"
#include "CAN_IF.h"
#include "Logger.h"



/************* function prototypes *********************/
static boolean isJ1939PDU(uint32 u32PduId);
static boolean checkIDAgainstMask(uint32 u32TestID, uint32 u32AcceptanceID, uint32 u32AcceptanceMask);


//Uses the generated configuration to configure the CAN controller mailboxes.
void CAN_IF_Init(void)
{
    Logger_Info("CAN_IF", "CAN_IF_Init");
    uint8 u8LC;
    CAN_InitController(USED_CAN_CONTROLLER_INDEX, g_CanTimingCtrlRegister);

    for (u8LC = 0u ; u8LC < g_CanMbCount ; u8LC ++ )
    {
        CAN_ConfigMB(USED_CAN_CONTROLLER_INDEX, u8LC, g_CanIfConfig[u8LC].eTxMode, g_CanIfConfig[u8LC].u32CanId, g_CanIfConfig[u8LC].u32Mask);
    }
}

void CAN_IF_Run(void)
{

}

// *********** Modemanagement functions ***********//
void CAN_IF_Start(void)
{
    CAN_Start(USED_CAN_CONTROLLER_INDEX);
}


// *********** Exposed to upper layers (PDUR) ***********//
void CAN_IF_Transmit(uint32 u32PduId, const uint8 *Data, uint8 u8DataLength)
{

    //Check which mailbox the CAN is shall go to//Find mailbox for PDU
    uint8 u8Mb = 0u;
    //Find right mailbox to use.
    while  ( (g_CanIfConfig[u8Mb].u32CanId != u32PduId) && (u8Mb < ((uint8) g_CanMbCount)) )
    {
        u8Mb++;
    }
    //Verify direction and frame length
    if ( (g_CanIfConfig[u8Mb].eTxMode == CAN_MODE_TX )&& (g_CanIfConfig[u8Mb].u8FrameLength == u8DataLength) )
    {
        CAN_Write(USED_CAN_CONTROLLER_INDEX,u8Mb,u32PduId,Data,u8DataLength);
    }

}

// *********** Exposed to lower layers (CAN driver) ***********//
void CAN_IF_TxConfirmation( uint32 u32PduId, uint8 u8Mb, uint8 u8ControllerId)
{
    if ( (u8ControllerId == 0u) && (u8Mb < ((uint8) g_CanMbCount)  ) )
        {
            //TODO: Add check for diagnostic and J1939 messages.
            PDU_Router_CAN_IF_TxConfirmation(u32PduId);
        }
}

void CAN_IF_RxIndication( uint8 u8ControllerId, uint8 u8Mb, uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength)
{
    //Check if the indication is OK.
    if ( (u8ControllerId == 0u) && (u8Mb < ((uint8) g_CanMbCount)) )
    {
        if ( isJ1939PDU(u32PduId) == TRUE )
        {
            PDU_Router_CAN_IF_J1939_RxIndication(u32PduId,u8Data,u8DataLength);
        }
        else
        {
            PDU_Router_CAN_IF_RxIndication(u32PduId,u8Data,u8DataLength);
        }
    }

}

// *********** Private functions ***********//
static boolean checkIDAgainstMask(uint32 u32TestID, uint32 u32AcceptanceID, uint32 u32AcceptanceMask)
{
    boolean ret = FALSE;
    if ( (u32TestID & u32AcceptanceMask) == (u32AcceptanceID & u32AcceptanceMask) )
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}

static boolean isJ1939PDU(uint32 u32PduId)
{
    boolean ret = FALSE;

    if ( checkIDAgainstMask (u32PduId, g_u32J1939SFID, g_u32J1939SFMask) == TRUE)
    {
        ret = TRUE;
    }
    else if ( checkIDAgainstMask (u32PduId, g_u32J1939FFID, g_u32J1939FFMask) == TRUE )
    {
        ret = TRUE;
    }
    else if ( checkIDAgainstMask (u32PduId, g_u32J1939CFID, g_u32J1939CFMask)== TRUE )
    {
        ret = TRUE;
    }
    else
    {
        //nothing to be done here
    }
    return ret;
}

