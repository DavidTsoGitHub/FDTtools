/***************************************************************************************/
/*                                                                                     */
/* FILE        : UDSRes.c                                                              */
/* DATE        : 2015-11-30                                                            */
/* AUTHOR      : Cao Chenguang                                                         */
/* DESCRIPTION : UDS resolution                                                        */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#include "Platform_Types.h"
#include "CAL.h"
#include "Diagnosis.h"
#include "Transport.h"
#include "UDSPorts.h"
#include "UDSRes.h"
#include "DIDCALMapping.h"

#if  TRANSPORT_RX_SIZE < (DID_MAX_NUM * 2u + 1u)
#error "Transport: Receive  Buffer is NOT sufficent for DID!"
#endif
#if  TRANSPORT_TX_SIZE < (DID_MAX_NUM * 2u + 1u)
#error "Transport: Transmit Buffer is NOT sufficent for DID!"
#endif
#if TRANSPORT_TX_SIZE < (DTC_NUM * 3u + 3u)
#error "Transport: Transmit Buffer is NOT sufficent for DTC!"
#endif

uint16 u16g_AddressingMode;

static uint16 u16g_SaftyMode;
static uint16 u16g_CurSession;
static uint16 u16g_SeedTxFlg;
static uint8  u8g_Key[SEED_LEN];
static uint16 u16g_testPresentCnt;
static uint16 u16g_SecurityAccessFailureCnt;
static uint16 u16g_SecurityAccessDelayCnt;
static uint16 u16g_SecurityAccessDelayFlag;

static const uint16 u16g_BasicDTC[DTC_NUM] =
{
    0xC111u, 0xC222u, 0xC333u, 0xC444u, 0xC555u, 0xC666u,
    0xC777u, 0xC888u, 0xC999u, 0xCAAAu, 0xCBBBu, 0xCCCCu
};
static uint8 u8g_FTB[DTC_NUM];       /* u8g_FTB shall be stored into NVRAM in case of power disturbing */
static uint8 u8g_DTCStatus[DTC_NUM]; /* u8g_DTCStatus shall be stored into NVRAM in case of power disturbing */

static void UDS_Response(const uint16 u16_NRC, const uint16 u16_SuppressPosRspMsgIndBit, uint32 u32_AckLen);
static void UDS_DiagSessionCtl(const uint8 u8_Data[]);
static void UDS_SecurityAccess(const uint8 u8_Data[], uint32 u32_DataLen);
static void UDS_ECUReset(const uint8 u8_Data[]);
static void UDS_TesterPresent(const uint8 u8_Data[]);
static void UDS_ReadDataById(const uint8 u8_Data[], uint32 u32_DIDLen);
static void UDS_WriteDataById(const uint8 u8_Data[], uint32 u32_DataLen);
static void UDS_ReadDTCInfo(const uint8 u8_Data[],uint32 u32_DataLen);
static void UDS_ClearDiagnosticInfo(const uint8 u8_Data[]);

void UDS_Init(void)
{
    uint16 i;

    u16g_SaftyMode      = DEFAULT_SECURITY_LEVEL;
    u16g_CurSession     = DEFAULT_SESSION;
    u16g_AddressingMode = PHYTHICAL_ADDRESSING;
    u16g_testPresentCnt = 0u;
    u16g_SeedTxFlg      = 0u;

    u16g_SecurityAccessDelayCnt = 0u;
    u16g_SecurityAccessDelayFlag = 0u;
    u16g_SecurityAccessFailureCnt = 0u;

    for(i = 0u; i < DTC_NUM; i++)
    {
        u8g_FTB[i]= 0x55u;    /* TODO: update when FTB is agreed with swedspot */
        u8g_DTCStatus[i] = 0x55u;
    }
}

void UDS_Run(void)
{
    u16g_testPresentCnt++;
    if(u16g_testPresentCnt > TEST_PRESENT_TIMEOUT)
    {
        u16g_testPresentCnt = TEST_PRESENT_TIMEOUT + 1u;    /* prevent overflow */
#if 0
        /* TODO: for test, recovery when Tester is OK */
        u16g_SaftyMode  = DEFAULT_SECURITY_LEVEL;
        u16g_CurSession = DEFAULT_SESSION;
#endif
    }
    if(u16g_SecurityAccessDelayFlag == 1u)
    {
        u16g_SecurityAccessDelayCnt++;
    }
    if(u16g_SecurityAccessDelayCnt > SECURITY_ACCESS_DELAY_TIMEOUT) /* delay time expired, stop counting */
    {
        u16g_SecurityAccessDelayFlag = 0u;
        u16g_SecurityAccessDelayCnt = 0u;
    }
}
void UDS_Entry(const uint8 u8_RxBuff[], uint32 u32_UDSDataLen)
{
    /* dispatch service */
    switch(u8_RxBuff[0])
    {
    case 0x10u:
            if(u32_UDSDataLen == 2u)
            {
                UDS_DiagSessionCtl(&u8_RxBuff[1]);
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x10u;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;
    case 0x14u:
            if(u32_UDSDataLen == 4u)
            {
                UDS_ClearDiagnosticInfo(&u8_RxBuff[1]);
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x14u;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;

    case 0x19u: /* length various from different sub-function */
            UDS_ReadDTCInfo(&u8_RxBuff[1], u32_UDSDataLen);
            break;

    case 0x22u:
            u32_UDSDataLen--;
            /* Amount of DID must be an even number */
            if((u32_UDSDataLen > 0u) && ((u32_UDSDataLen & 0x01u) == 0u))
            {
                if(u32_UDSDataLen > (DID_MAX_NUM * 2u))  /* client exceeds the maximum number of DIDs */
                {
                    u8g_TxBuff[0] = (uint8)0x7Fu;
                    u8g_TxBuff[1] = (uint8)0x22u;
                    u8g_TxBuff[2] = (uint8)ROOR;
                    Trans_Transmit(3u);
                }
                else
                {
                    UDS_ReadDataById(&u8_RxBuff[1], u32_UDSDataLen);
                }
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x22u;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;
    case 0x2Eu:
            u32_UDSDataLen--;
            if(u32_UDSDataLen >= 3u)    /* 2 bytes DID + 1 byte data*/
            {
                UDS_WriteDataById(&u8_RxBuff[1], u32_UDSDataLen);
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x2Eu;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;
    case 0x11u:
            if(u32_UDSDataLen == 2u)
            {
                UDS_ECUReset(&u8_RxBuff[1]);
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x11u;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;
    case 0x27u:
            UDS_SecurityAccess(&u8_RxBuff[1], u32_UDSDataLen);
            break;
    case 0x3Eu:
            if(u32_UDSDataLen == 2u)
            {
                UDS_TesterPresent(&u8_RxBuff[1]);
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = (uint8)0x3Eu;
                u8g_TxBuff[2] = (uint8)IMLOIF;
                Trans_Transmit(3u);
            }
            break;
    case 0x3Fu:
            /* Nothing to do */
            break;
    default: /* unsupported service */
            if(u16g_AddressingMode == PHYTHICAL_ADDRESSING)
            {
                u8g_TxBuff[0] = (uint8)0x7Fu;
                u8g_TxBuff[1] = u8_RxBuff[0];
                u8g_TxBuff[2] = (uint8)SNS;
                Trans_Transmit(3u);
            }
            else
            {
                ; /* No response */
            }
            break;
    }
}

/*
 * Name        : UDS_voDiagSessionCtl
 * Description : 0x10 service handler
 * Arguments   : u8_Data     - data after dispatching
 * Return      : none
 */
static void UDS_DiagSessionCtl(const uint8 u8_Data[])
{
    uint16 u16t_SubFunc;
    uint16 u16t_SuppressPosRspMsgIndBit;
    uint16 u16t_NRC;
    uint32 u32t_AckLen;
    uint32 u32t_ConditionCheck;

    u16t_SuppressPosRspMsgIndBit = ((uint16)u8_Data[0]) & 0x80u;
    u16t_SubFunc                 = ((uint16)u8_Data[0]) & 0x7Fu;             /* get sub-function */

    if((u16t_SubFunc == DEFAULT_SESSION) || (u16t_SubFunc == PROGRAM_SESSION) ||(u16t_SubFunc == EXTEND_SESSION))
    {
        u32t_ConditionCheck = TRANSITION_SESS(DEFAULT_SESSION);    /* additional operations for specific ECU */
        if(u32t_ConditionCheck == 1u)
        {
            u16g_CurSession = u16t_SubFunc;
            if(u16t_SubFunc == 0x02u)
            {
                SET_REBOOT_FLAG();
                RESET_ECU();
            }
        }
        else
        {
            u16t_NRC = CNC;
        }
    }
    else
    {
        u16t_NRC = SFNS;   /* sub-functin not supported */
    }

    /* assemble response */
    if(u16t_NRC == PR)
    {
        u8g_TxBuff[0] = (uint8)0x50u;
        u8g_TxBuff[1] = (uint8)u16t_SubFunc;
        u32t_AckLen = 2u;
    }
    else
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x10u;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }

    UDS_Response(u16t_NRC, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
}

/*
 * Name        : UDS_ClearDiagnosticInfo
 * Description : 0x14 service handler
 * Arguments   : u8_Data - data after dispatching
 * Return      : none
 */
static void UDS_ClearDiagnosticInfo(const uint8 u8_Data[])
{
    uint16 u16t_NRC;
    uint16 i;
    uint32 ret;
    uint32 u32t_AckLen;

    if((u8_Data[0] == 0xFFu) && (u8_Data[1] == 0xFFu) && (u8_Data[2] == 0xFFu))
    {
        ret = CLEAR_DTC_CHECK();
        if(ret == 1u)
        {
            /* TODO: update when FTB is agreed with swedspot */
            for(i = 0u; i < DTC_NUM; i++)
            {
                u8g_FTB[i]= 0x55u;
            }
            u16t_NRC = PR;
        }
        else
        {
            u16t_NRC = CNC;
        }
    }
    else
    {
        u16t_NRC = IMLOIF;
    }

    /* assemble response */
    if(u16t_NRC == PR)
    {
        u8g_TxBuff[0] = (uint8)0x54u;
        u32t_AckLen = 1u;
    }
    else
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x14u;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }

    UDS_Response(u16t_NRC, 0u, u32t_AckLen);
}

/*
 * Name        : UDS_ReadDTCInfo
 * Description : 0x19 service handler
 * Arguments   : u8_Data[]   - data after dispatching
                 u32_DataLen - data length
 * Return      : none
 */
static void UDS_ReadDTCInfo(const uint8 u8_Data[],uint32 u32_DataLen)
{
    uint16 u16t_SubFunc;
    uint16 u16t_SuppressPosRspMsgIndBit;
    uint16 u16t_NRC;
    uint8  i;
    uint32 u32t_AckLen;

    u16t_SuppressPosRspMsgIndBit = ((uint16)u8_Data[0]) & 0x80u;
    u16t_SubFunc                 = ((uint16)u8_Data[0]) & 0x7Fu;

    switch(u16t_SubFunc)
    {
    case 0x01u:
            if(u32_DataLen == 0x03u)
            {
                u8g_TxBuff[0] = (uint8)0x59u;
                u8g_TxBuff[1] = (uint8)0x01u;
                u8g_TxBuff[2] = AVAILABILITY_MASK;
                u8g_TxBuff[3] = ISO15031_6;
                u8g_TxBuff[4] = 0u;
                u32t_AckLen = 5u;
                for(i = 0u; i < DTC_NUM; i++)
                {
                    if((u8g_DTCStatus[i] & u8_Data[1] & AVAILABILITY_MASK) != 0u)
                    {
                        u8g_TxBuff[4]++;
                    }
                }
                u16t_NRC = PR;
            }
            else
            {
                u16t_NRC = IMLOIF;
            }
            break;
    case 0x02u:
            if(u32_DataLen == 0x03u)
            {
                u8g_TxBuff[0] = (uint8)0x59u;
                u8g_TxBuff[1] = (uint8)0x02u;
                u8g_TxBuff[2] = AVAILABILITY_MASK;
                u32t_AckLen = 3u;
                for(i = 0u; i < DTC_NUM; i++)
                {
                    if((u8g_DTCStatus[i] & u8_Data[1] & AVAILABILITY_MASK) != 0u)
                    {
                        u8g_TxBuff[u32t_AckLen] = (uint8)((u16g_BasicDTC[i] >> 8u));
                        u32t_AckLen++;
                        u8g_TxBuff[u32t_AckLen] = (uint8)(u16g_BasicDTC[i] & 0xFFu);
                        u32t_AckLen++;
                        u8g_TxBuff[u32t_AckLen] = u8g_FTB[i];
                        u32t_AckLen++;
                    }
                }
                u16t_NRC = PR;
            }
            else
            {
                u16t_NRC = IMLOIF;
            }
            break;
    default:
            u16t_NRC = SFNS;   /* sub-functin not supported */
            break;
    }

    /* assemble response */
    if(u16t_NRC != PR)
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x19u;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }

    UDS_Response(u16t_NRC, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
}

/*
 * Name        : UDS_ReadDataById
 * Description : 0x22 service handler
 * Arguments   : u8_Data[]   - data after dispatching
 *               u32_DIDLen  - amount of DIDs
 * Return      : none
 */
static void UDS_ReadDataById(const uint8 u8_Data[], uint32 u32_DIDLen)
{
    uint16 u16t_NRC;
    uint16 i, j;
    uint16 u16t_ReqID;
    uint16 u16t_CalIDLen;
    CAL_STATUS ret;
    boolean searchCALIDRet;
    uint32 u32t_AckLen;
    calibration_parameter_id_t  CalID;

    u16t_NRC = PR;
    j = 1u;
    u8g_TxBuff[0] = 0x62u;

    for(i = 0u; i < (u32_DIDLen - 1u); i = i + 2u)
    {
        /* assemble DID */
        u16t_ReqID = u8_Data[i];
        u16t_ReqID = u16t_ReqID << 8u;
        u16t_ReqID = u16t_ReqID + u8_Data[i + 1u];

        /* check whether the requested ID is supported */
        searchCALIDRet = DIDCALMapping_GetCALForDID(u16t_ReqID, &CalID);
        if(searchCALIDRet == FALSE)
        {
            u16t_NRC = ROOR;
            break;
        }
        else                                  /* DID is found */
        {
            u8g_TxBuff[j] = u8_Data[i];
            j++;
            u8g_TxBuff[j] = u8_Data[i + 1u];
            j++;
            u16t_CalIDLen = CAL_CalibrationParameters[CalID].Length;
            ret = CAL_Read(CalID, &u8g_TxBuff[j], u16t_CalIDLen);
            if(ret != CAL_OK)
            {
                u16t_NRC = CNC;
                break;
            }
            else
            {
                j = j + u16t_CalIDLen;
            }
        }
    }

    /* assemble negative response, all of the previously assembled bytes are cleard */
    if(u16t_NRC != PR)
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x22u;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }
    else
    {
        u32t_AckLen = j;
    }
    UDS_Response(u16t_NRC, 0u, u32t_AckLen);
}

/*
 * Name        : UDS_WriteDataById
 * Description : 0x2E service handler
 * Arguments   : u8_Data[], data after dispatching
 * Return      : u32_DataLen
 * Notes       : Only Single Frame is supported
 */
static void UDS_WriteDataById(const uint8 u8_Data[], uint32 u32_DataLen)
{
    uint16 u16t_NRC;
    uint16 u16t_ReqID;
    uint16 i, j;
    calibration_parameter_id_t CalID;
    uint16 u16t_CalIDLen;
    CAL_STATUS ret;
    boolean searchCALIDRet;
    uint32 u32t_AckLen;

    if((u16g_AddressingMode == PHYTHICAL_ADDRESSING) && (u16g_SaftyMode == DIAGNOSIS_SECURITY_LEVEL))
    {
        u16t_NRC = PR;
        i = 0u;
        j = 1u;    /* NOTE: j starts with 1 */
        u8g_TxBuff[0] = 0x6Eu;

        while(i < (u32_DataLen - 1u)) /* 1 MUST be subtracted to prevent bound violation */
        {
            /* assemble DID, and response DID */
            u16t_ReqID = u8_Data[i];
            u8g_TxBuff[j] = u8_Data[i];
            u16t_ReqID = u16t_ReqID << 8u;
            u16t_ReqID = u16t_ReqID + u8_Data[i + 1u];
            u8g_TxBuff[j + 1u] = u8_Data[i + 1u];
            i = i + 2u;
            j = j + 2u;
            if(j > ((DID_MAX_NUM * 2u) + 1u))    /* j exceedes the limitation of DIDs */
            {
                u16t_NRC = ROOR;
                break;
            }

            /* check whether the requested ID is supported */
            searchCALIDRet = DIDCALMapping_GetCALForDID(u16t_ReqID, &CalID);
            if(searchCALIDRet == FALSE)
            {
                u16t_NRC = ROOR;
                break;
            }
            else   /* DID is found */
            {
                u16t_CalIDLen = CAL_CalibrationParameters[CalID].Length;
                ret = CAL_Write(CalID, &u8_Data[i], u16t_CalIDLen);
                if(ret == CAL_OK)
                {
                    i = i + u16t_CalIDLen;
                }
                else
                {
                    u16t_NRC = ROOR;
                    break;
                }
            }
        }
    }
    else
    {
        u16t_NRC = CNC;
    }
    /* assemble negative response, all of the previously assembled bytes are cleard */
    if(u16t_NRC != PR)
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x2Eu;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }
    else
    {
        u32t_AckLen = j;
    }
    UDS_Response(u16t_NRC, 0u, u32t_AckLen);
}

/*
 * Name        : UDS_ECUReset
 * Description : 0x11 service handler
 * Arguments   : u8_Data[]   - data after dispatching
 *               u32_DataLen - data length
 * Return      : none
 */
static void UDS_ECUReset(const uint8 u8_Data[])
{
    uint16 u16t_SubFunc;
    uint16 u16t_SuppressPosRspMsgIndBit;
    uint32 u32t_AckLen;

    u16t_SuppressPosRspMsgIndBit = ((uint16)u8_Data[0]) & 0x80u;
    u16t_SubFunc                 = ((uint16)u8_Data[0]) & 0x7Fu;

    if(u16t_SubFunc == (uint16)0x01u)    /* only hard reset is supported */
    {
        u8g_TxBuff[0] = (uint8)0x51u;
        u8g_TxBuff[1] = (uint8)0x01u;
        u32t_AckLen = 2u;
        UDS_Response(PR, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
        RESET_ECU();
    }
    else
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x11u;
        u8g_TxBuff[2] = (uint8)SFNS;
        u32t_AckLen = 3u;
        UDS_Response(SFNS, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
    }
}

/*
 * Name        : UDS_SecurityAccess
 * Description : 0x27 service handler
 * Arguments   : u8_Data[] - data after dispatching
 * Return      : none
 */
static void UDS_SecurityAccess(const uint8 u8_Data[], uint32 u32_DataLen)
{
    uint16 u16t_VerifiedFlag;
    uint16 u16t_SubFunc;
    uint16 u16t_SuppressPosRspMsgIndBit;
    uint16 u16t_NRC;
    uint16 i;
    uint32 u32t_AckLen;

    u16t_SuppressPosRspMsgIndBit = ((uint16)u8_Data[0]) & 0x80u;
    u16t_SubFunc                 = ((uint16)u8_Data[0]) & 0x7Fu;     /* get sub-function */

    if((u16g_AddressingMode == FUNCTIONAL_ADDRESSING) || (u16g_CurSession == DEFAULT_SESSION))
    {
        u16t_NRC = CNC;
    }
    else
    {
        if(u16t_SubFunc == 0x01u)   /* request seed */
        {
            if(u32_DataLen != 2u)
            {
              u16t_NRC = IMLOIF;
            }
            else if(u16g_SecurityAccessDelayCnt > 0u)
            {
                u16t_NRC = RTDNE;
            }
            else
            {
                u8g_TxBuff[0] = (uint8)0x67u;
                u8g_TxBuff[1] = (uint8)0x01u;
                /* generate seed and corresponding key */
                if(u16g_SaftyMode == DEFAULT_SECURITY_LEVEL)
                {
                    GEN_SEED(&u8g_TxBuff[2]);
                }
                else
                {
                    u8g_TxBuff[2] = 0u;
                    u8g_TxBuff[3] = 0u;
                    u8g_TxBuff[4] = 0u;
                    u8g_TxBuff[5] = 0u;
                }
                ENCRYPTION(&u8g_TxBuff[2], u8g_Key);
                u32t_AckLen = 6u;
                u16t_NRC = PR;
                u16g_SeedTxFlg = 1u;
            }
        }
        else if(u16t_SubFunc == 0x02u) /* key validation*/
        {
            if(u32_DataLen != (0x02u + SEED_LEN))
            {
                u16t_NRC = IMLOIF;
            }
            else if(u16g_SeedTxFlg != 1u) /* sequence error */
            {
                u16t_NRC = RSE;
            }
            else
            {
                u16g_SeedTxFlg = 0u;
                /* comparation between key and recevied key */
                u16t_VerifiedFlag = 1u;
                for(i = 0u; i < SEED_LEN; i++)
                {
                    if(u8g_Key[i] != u8_Data[1u + i])
                    {
                        u16t_VerifiedFlag = 0u;
                        break;
                    }
                }

                if(u16t_VerifiedFlag == 1u)    /* verification passed */
                {
                    u16t_NRC = PR;
                    u16g_SecurityAccessFailureCnt = 0u;
                    u16g_SaftyMode = DIAGNOSIS_SECURITY_LEVEL;
                    u8g_TxBuff[0] = (uint8)0x67u;
                    u8g_TxBuff[1] = (uint8)0x02u;
                    u32t_AckLen = 2u;
                }
                else
                {
                    u16g_SecurityAccessFailureCnt++;
                    u16g_SaftyMode = DEFAULT_SECURITY_LEVEL;
                    if(u16g_SecurityAccessFailureCnt > MAX_ACCESS_FAILURE_NUM)
                    {
                        u16t_NRC = ENOA;
                        u16g_SecurityAccessFailureCnt = MAX_ACCESS_FAILURE_NUM + 1u; /* prevent overflow */
                        u16g_SecurityAccessDelayFlag = 1u;    /* start delay counting */
                    }
                    else
                    {
                        u16t_NRC = IK;
                    }
                }
            }
        }
        else
        {
            u16t_NRC = SFNS;
        }
    }

    /* assemble response */
    if(u16t_NRC != PR)
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x27u;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }

    UDS_Response(u16t_NRC, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
}

/*
 * Name        : UDS_ECUReset
 * Description : 0x11 service handler
 * Arguments   : u8_Data     - data after dispatching
 *               u32_DataLen - data length
 * Return      : none
 */
static void UDS_TesterPresent(const uint8 u8_Data[])
{
    uint16 u16t_SubFunc;
    uint16 u16t_SuppressPosRspMsgIndBit;
    uint16 u16t_NRC;
    uint32 u32t_AckLen;

    u16t_SuppressPosRspMsgIndBit = ((uint16)u8_Data[0]) & 0x80u;
    u16t_SubFunc                 = ((uint16)u8_Data[0]) & 0x7Fu;

    if(u16t_SubFunc == 0x00u)
    {
        u16g_testPresentCnt = 0u;
        u16t_NRC = PR;
    }
    else
    {
        u16t_NRC = SFNS;
    }

    /* assemble response */
    if(u16t_NRC == PR)
    {
        u8g_TxBuff[0] = (uint8)0x7Eu;
        u8g_TxBuff[1] = (uint8)0x00u;
        u32t_AckLen = 2u;
    }
    else
    {
        u8g_TxBuff[0] = (uint8)0x7Fu;
        u8g_TxBuff[1] = (uint8)0x3Eu;
        u8g_TxBuff[2] = (uint8)u16t_NRC;
        u32t_AckLen = 3u;
    }
    UDS_Response(u16t_NRC, u16t_SuppressPosRspMsgIndBit, u32t_AckLen);
}

/*
 * Name        : UDS_voResponse
 * Description : Server response behavior
 * Arguments   : U16_NRC                     : negative response code
                 U16_SuppressPosRspMsgIndBit : suppress positive Response Message Indcation Bit
                 u32_AckLen                  : Response length
 * Return      : none
 */
static void UDS_Response(const uint16 u16_NRC, const uint16 u16_SuppressPosRspMsgIndBit, uint32 u32_AckLen)
{

    /* response decision */
    if((u16g_AddressingMode == FUNCTIONAL_ADDRESSING) && ((u16_NRC == SNS) || (u16_NRC == SFNS) || (u16_NRC == ROOR)))
    {
        ; /* NO Response */
    }
    else
    {
        if((u16_NRC == PR) && (u16_SuppressPosRspMsgIndBit == 0x80u))
        {
            ; /* NO Response */
        }
        else
        {
            Trans_Transmit(u32_AckLen);
        }
    }
}
