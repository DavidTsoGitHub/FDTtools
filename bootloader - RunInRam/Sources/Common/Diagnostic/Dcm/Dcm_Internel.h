#ifndef DCM_INTERNEL_H
#define DCM_INTERNEL_H

/*=======[I N C L U D E S]====================================================*/
#include "Dcm.h"
#include "Dcm_Cfg.h"

/*=======[E X T E R N A L   D A T A]==========================================*/
/** use static memory for service */
extern uint8	Dcm_MainRxBuff[DCM_RX_BUF_SIZE];

extern uint8	Dcm_MainTxBuff[DCM_TX_BUF_SIZE];

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
/* Interface used for DCM Init */
void Dcm_DspInit(void);

/* Interface used for DCM Dsp */
void Dcm_ServiceFinish(void);

void Dcm_SetSessionMode(const Dcm_SessionType sessMode);

boolean Dcm_CheckSessionSupp(const Dcm_SessionType * sessionTable);

void Dcm_SetSecurityLevel(const Dcm_SecurityType secLev);

boolean Dcm_GetSecurityTimerExpired(void);

boolean Dcm_CheckSecuritySupp(const Dcm_SecurityType * secLevTable);

void  Dcm_StartSecurityTimer(uint32 timeOut);

void Dcm_StartResetTimer(uint16 timeOut);

void Dcm_SendRsp(void);

void Dcm_SendNcr( uint8 nrcCode );

#endif/* endof DCM_INTERNEL_H */

