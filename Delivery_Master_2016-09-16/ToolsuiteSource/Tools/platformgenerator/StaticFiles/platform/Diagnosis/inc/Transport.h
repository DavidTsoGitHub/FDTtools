/*****************************************************************/
/*                                                               */
/* FILE        : Transport.h                                     */
/* DATE        : 2015-11-20                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : Transport protocol definition                   */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#ifndef TRANSPORT_H
#define TRANSPORT_H

#define N_BS		   150u
#define N_CR		   150u
#define CTS			     0u
#define WT			     1u
#define OVFLW		     2u
#define TIME_WAITFORCF 200u


#define TRANS_SF      0x00u      /* Single Frame */
#define TRANS_FF      0x10u      /* First Frame */
#define TRANS_CF      0x20u      /* Consecutive Frame */
#define TRANS_FC      0x30u      /* Flow Control Frame */
#define FILL_BYTE     0xFFu      /* fill byte */

/* Minimum interval of receiving consecutive frames, MUST be greater than diag_Run period */
#define RX_STMIN        60u

#define TRANSPORT_RX_SIZE            402uL
#define TRANSPORT_TX_SIZE            402uL

extern uint8 u8g_TxBuff[TRANSPORT_TX_SIZE];

extern void Trans_Init(void);
extern void Trans_Receive(const uint8 u8_Data[]);
extern void Trans_Transmit(uint32 u32_AckLen);
extern void Trans_Run(void);
#endif
