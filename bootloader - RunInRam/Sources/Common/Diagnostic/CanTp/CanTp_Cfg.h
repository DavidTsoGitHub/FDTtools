#ifndef CANTP_CFG_H
#define CANTP_CFG_H

/* to get the GPT_PERIOD_TIME */
#include "Gpt_Cfg.h"

/*=======[M A C R O S]========================================================*/

/** Minimum time the sender shall wait between transmissions of two N-PDU */ 
/* @type:uint8 range:0~255 note:uint-ms */
#define ISO15765_ST_MIN     ((uint8)0u)	

/** N_As timeout for transmission of any CAN frame */
/* @type:uint8 range:0~255 note:uint-ms */
#define ISO15765_N_As       ((uint8)250u)	

/** Timeout for transmission of a CAN frame (ms) */	
/* @type:uint8 range:0~255 note:uint-ms */
#define ISO15765_N_Ar       ((uint8)250u)

/** N_Bs timeout */	
/* @type:uint8 range:0~255 note:uint-ms */
#define ISO15765_N_Bs       ((uint8)250u)

/** Time out for consecutive frames (ms) */	
/* @type:uint8 range:0~255 note:uint-ms */
#define ISO15765_N_Cr       ((uint8)250u)

/* @type:define range:GPT_PERIOD_TIME note:auto generate */
#define CANTP_MAIN_TICK GPT_PERIOD_TIME

/** padding value */	
/* @type:uint8 range:0x00~0xFF note:NONE */
#define CANTP_PADDING_VALUE (0x55u)	

/* @type:NONE range:NONE note:auto generate */
#define PADDING_ON

/* @type:uint8 range:1~255 note:reference to num of CanTp_TxSdu */
#define CANTP_TX_CHANNEL_NUM 	1u

/* @type:uint8 range:1~255 note:reference to num of CanTp_RxSdu */
#define CANTP_RX_CHANNEL_NUM 	1u

#endif


