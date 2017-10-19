/***************************************************************************************/
/*                                                                                     */
/* FILE        : Transport.c                                                           */
/* DATE        : 2015-11-22                                                            */
/* AUTHOR      : Cao chenguang                                                         */
/* DESCRIPTION : transport protocol resolution                                         */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#include "Platform_Types.h"
#include "PDU_Router.h"
#include "CAN_IF_Cfg.h"
#include "Transport.h"
#include "UDSRes.h"

uint8  u8g_TxBuff[TRANSPORT_TX_SIZE];
static uint8  u8g_RxBuff[TRANSPORT_RX_SIZE];
static uint32 u32g_RespFFTxFlg;       /* whether the FF frame has been transmitted when response */
                                      /* 0 - not yet; 1 - transmitted already */
static uint32 u32g_RespCFTxStartFlag; /* consecutive frame starts or not */
static uint32 u32g_STmin;
static uint8  u8g_TxCurSN;
static uint32 u32g_TxTimer;
static uint32 u32g_TxLen;             /* bytes to be sent */
static uint32 u32g_TxCurIndex;


void Trans_Init(void)
{
    u32g_RespFFTxFlg = 0u;
    u32g_RespCFTxStartFlag = 0u;
    u32g_STmin = 0u;
    u8g_TxCurSN = 0u;
    u32g_TxTimer = 0u;
    u32g_TxLen = 0u;
    u32g_TxCurIndex = 0u;
}

void Trans_Receive(const uint8 u8_Data[])
{
    uint32 i;
    uint16 u16t_PCIType;
    uint16 u16t_CurSN;
    static uint32 u32s_UDSDataLen;
    static uint16 u16s_LastSN = 0u;
    static uint32 u32s_RxBuffIn = 0u;

    /* PCI type determination, only normal addressing format is supported */
    u16t_PCIType = (uint16)u8_Data[0] & 0xF0u;
    switch(u16t_PCIType)
    {
    case TRANS_SF:
        u32s_UDSDataLen = (uint32)u8_Data[0] & 0x0Fu;
        for(i = 1u; i <= u32s_UDSDataLen; i++)
        {
            u8g_RxBuff[i - 1u] = u8_Data[i];
        }
        /* byte 0 and byte 1 in u8g_TxBuff are reserved for transport layer */
        UDS_Entry(u8g_RxBuff, u32s_UDSDataLen);
        break;
    case TRANS_FF:
        u32s_UDSDataLen = (uint32)u8_Data[0] & 0x0Fu;
        u32s_UDSDataLen = (u32s_UDSDataLen << 8u) + u8_Data[1];
        if(u32s_UDSDataLen > TRANSPORT_RX_SIZE)
        {
            /* TODO: transport layer error handler */
        }
        else
        {
            for(i = 2u; i < 8u; i++)    /* copy 6 bytes in the first frame */
            {
                u8g_RxBuff[i - 2u] = u8_Data[i];
            }
            u16s_LastSN = 0u;
            u32s_RxBuffIn = 6u;
            /* assemble flow control frame */
            u8g_TxBuff[0] = (uint8)0x30u;
            u8g_TxBuff[1] = (uint8)0x00u;
            u8g_TxBuff[2] = (uint8)RX_STMIN;
            u8g_TxBuff[3] = (uint8)FILL_BYTE; 
            u8g_TxBuff[4] = (uint8)FILL_BYTE;
            u8g_TxBuff[5] = (uint8)FILL_BYTE;
            u8g_TxBuff[6] = (uint8)FILL_BYTE;
            u8g_TxBuff[7] = (uint8)FILL_BYTE;
            PDU_Router_ComTransmit(DIAG_RESP_ID, u8g_TxBuff, 8u);
        }
        break;
    case TRANS_CF:
        u16t_CurSN = (uint16)u8_Data[0] & 0x0Fu;
        if(u16t_CurSN == ((u16s_LastSN + 1u) & 0x0Fu))  /* sequence number is right */
        {
            u16s_LastSN = u16t_CurSN;

            for(i = 0u; i < 7u; i++)
            {
                if(u32s_RxBuffIn < u32s_UDSDataLen)
                {
                    u8g_RxBuff[u32s_RxBuffIn] = u8_Data[1u + i];    /* Data[0] is skipped */
                    u32s_RxBuffIn++;    
                }
                if(u32s_RxBuffIn == u32s_UDSDataLen)    /* all of the data has been received */
                {
                    u32s_RxBuffIn = 0u;
                    UDS_Entry(u8g_RxBuff, u32s_UDSDataLen);
                    break;
                }
            }
        }
        else   /* frame is not in normal sequence */
        {
            /* TODO: transport layer multi-frame receiving error handler */
        	u32s_RxBuffIn = 0u;
          	u16s_LastSN = 0u;
        }
        break;
    case TRANS_FC:
        if(u32g_RespFFTxFlg == 1u)
        {
            u32g_RespFFTxFlg = 0u;
            /* TODO: FC other bytes resolution, STmin units: ms */
            if((u8_Data[0] == 0x30u) && (u8_Data[1] == 0x00u) && (u8_Data[2] < 0x80u))
            {
                u32g_STmin = u8_Data[2];
                u8g_TxCurSN = 0u;
                u32g_TxTimer = 0u;
                u32g_RespCFTxStartFlag = 1u;
            }   
        }
        else
        {
            /* TODO: Error handler: FC received before FF sending */
        }
        break;
    default:
        /* TODO: transport layer error handler */
        break;
    }
}

void Trans_Transmit(uint32 u32_AckLen)
{
    uint8 u8t_tmp;
    uint8 u8t_Data[8];
    uint32 i;

    if(u32_AckLen < 8u)    /* Single Frame */
    {
        u8t_Data[0] = (uint8)u32_AckLen;
        for(i = 0u; i < u32_AckLen; i++)
        {
            u8t_Data[i + 1u] = u8g_TxBuff[i];
        }
        for(i = u32_AckLen + 1u; i < 8u; i++)
        {
            u8t_Data[i] = FILL_BYTE;
        }
    }
    else /* First Frame */
    {
        u8t_tmp = (uint8)((u32_AckLen >> 8u) & 0x0Fu);    /* get highest 4 bits */
        u8t_Data[0] = ((uint8)0x10u) | u8t_tmp;
        u8t_Data[1] = (uint8)(u32_AckLen & 0xFFu);
        for(i = 0u; i< 6u; i++)
        {
            u8t_Data[i + 2u] = u8g_TxBuff[i];
        }
        u32g_TxLen = u32_AckLen;
        u32g_TxCurIndex = 6u;
        u32g_RespFFTxFlg = 1u;
    }
    PDU_Router_ComTransmit(DIAG_RESP_ID, u8t_Data, 8u);
}

void Trans_Run(void)
{
    uint8 u8t_Data[8];
    uint32 i;

    if(u32g_RespCFTxStartFlag == 1u)
    {
        if(u32g_TxTimer < u32g_STmin)
        {
            u32g_TxTimer++;
        }
        else
        {
            u32g_TxTimer = 0u;
            u8t_Data[0] = (uint8)(0x20u | u8g_TxCurSN);
            
            for(i = 1u; i < 8u; i++)
            {
            	if(u32g_TxCurIndex < u32g_TxLen)
            	{
            		u8t_Data[i] = u8g_TxBuff[u32g_TxCurIndex];
                    u32g_TxCurIndex++;
                }
            	else
            	{
            		u8t_Data[i] = FILL_BYTE;
            	}
            }
            if(u32g_TxCurIndex == u32g_TxLen)    /* All of the data are sent */
            {
                u32g_RespCFTxStartFlag = 0u;     /* stop multi-frame */
            }
            u8g_TxCurSN = (u8g_TxCurSN + 1u) & 0x0Fu;
            PDU_Router_ComTransmit(DIAG_RESP_ID, &u8t_Data[0], 8u);
        }
    }
}
