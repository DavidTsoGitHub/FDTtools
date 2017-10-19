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

#include "PDU_Router.h"
#include "COM.h"
#include "CAN_IF.h"
#include "Diagnosis.h"
#include "Platform_Types.h"
#include "J1939.h"
// *********** Exposed to lower layers (CAN driver) ***********//

void PDU_Router_CAN_IF_TxConfirmation( uint32 u32PduId)
{
    //Check if the Id belongs to Diagnostic..
    if ( u32PduId == DIAG_RESP_ID)
    {
        Diag_TxConfirmation(u32PduId);
    }
    else
    {
        COM_TxConfirmation(u32PduId);
    }
}

void PDU_Router_CAN_IF_J1939_RxIndication(uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength)
{
    J1939_RxIndication(u32PduId,u8Data, u8DataLength);
}


void PDU_Router_CAN_IF_RxIndication( uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength)
{
    //check routing. Diag or "normal" data
    if ( (u32PduId == DIAG_FUNC_REQ_ID) || (u32PduId == DIAG_REQ_ID))
    {
        Diag_RxIndication(u32PduId,u8Data, u8DataLength);
    }
    else
    {
        //forward indication to COM
        COM_RxIndication(u32PduId,u8Data, u8DataLength);
    }
}


void PDU_Router_ComTransmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength)
{
    CAN_IF_Transmit(u32PduId,Data,u8DataLength);
}

void PDU_Router_J1939Transmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength)
{
    CAN_IF_Transmit(u32PduId,Data,u8DataLength);
}

void PDU_Router_DiagTransmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength)
{
    CAN_IF_Transmit(u32PduId,Data,u8DataLength);
}




