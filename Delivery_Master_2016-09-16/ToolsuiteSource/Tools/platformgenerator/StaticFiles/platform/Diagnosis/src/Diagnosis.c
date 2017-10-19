/***************************************************************************************/
/*                                                                                     */
/* FILE        : Diagnosis.c                                                           */
/* DATE        : 2015-11-22                                                            */
/* AUTHOR      : Cao chenguang                                                         */
/* DESCRIPTION : Diagnosis resolution                                                  */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#include "StandardTypes.h"
#include "DiagManager.h"
#include "CAN_IF_Cfg.h"
#include "CAL_Cfg.h"
#include "Diagnosis.h"
#include "Transport.h"
#include "UDSRes.h"
#include "SysTime.h"

static uint8 u8g_CANFrameBuff[8];
static uint8 u8g_DiagReqRx;

void Diag_Init(void)
{       
    u8g_DiagReqRx = 0u;
    Trans_Init();
    UDS_Init();
}

void Diag_Run(void)
{
	if(u8g_DiagReqRx == 1u)
	{
        u8g_DiagReqRx = 0u;
        Trans_Receive(u8g_CANFrameBuff);
	}
	UDS_Run();
	Trans_Run();
}

/*
 * Name        : Diag_RxIndication
 * Description : copy data when a diagnostic frame is received.
 * Arguments   : u32CanIfPduId - CAN ID
 *               u8DataPtr     - received data pointer
 *               u8DataLen     - Data length
 * Return      : none
 */
void Diag_RxIndication(uint32 u32CanIfPduId, uint8* u8DataPtr, uint8 u8DataLen)
{
	uint8 i;

    if(u32CanIfPduId == DIAG_FUNC_REQ_ID)
    {
        u16g_AddressingMode = FUNCTIONAL_ADDRESSING;
    }
    else if(u32CanIfPduId == DIAG_REQ_ID)
    {
        u16g_AddressingMode = PHYTHICAL_ADDRESSING;
    }
    else
    {
        return;
    }

    if(u8DataLen > 0u)
    {
    	/* copy the received data */
    	for(i = 0u; i < u8DataLen; i++)
    	{
    		u8g_CANFrameBuff[i] = *(u8DataPtr + i);
    	}
    	u8g_DiagReqRx = 1u;
    }
}

void Diag_TxConfirmation(uint32 u32CanIfPduId)
{
    u32CanIfPduId = u32CanIfPduId;    /* prevent compiler warning */
}

void DIAG_ReportError(tDiagReport DiagRecord)
{
    /* TODO: to be implemented */
    DiagRecord.u32DTCNumber = 0u;  /* prevent compiler warning */
}

void Diag_Encryption(const uint8 u8_Seed[], uint8 u8_Key[])
{
    uint16 i;
    /* TODO: update encryption algorithm when PC software is OK */
    for(i = 0u; i < 4u; i++)
    {
        u8_Key[i] = u8_Seed[i];
    }
}

void Diag_GenSeed(uint8 u8_Seed[])
{
    uint32 u32t_curTime;
    u32t_curTime = SysTime_GetTimeMs();
    u8_Seed[0] = (uint8)(u32t_curTime >> 24u);
    u8_Seed[1] = (uint8)((u32t_curTime >> 16u) & 0xFFu);
    u8_Seed[2] = (uint8)((u32t_curTime >> 8u) & 0xFFu);
    u8_Seed[3] = (uint8)(u32t_curTime& 0xFFu);
}

uint32 Diag_CheckTransitionCondition(uint32 Session)
{
    Session = Session;  /* prevent compiler warning */
    return 1u;
}

/*
 * TODO: check whether the DTC can be cleared or not.
 */
uint32 Diag_CheckClearDTC(void)
{
    return 1u;
}
