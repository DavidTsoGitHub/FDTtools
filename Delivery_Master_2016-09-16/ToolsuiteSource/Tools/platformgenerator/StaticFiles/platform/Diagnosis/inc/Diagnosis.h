/*****************************************************************/
/*                                                               */
/* FILE        : Diagnosis.h                                     */
/* DATE        : 2015-11-20                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : functions relates to  target MCU.               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

extern void Diag_Init(void);
extern void Diag_Run(void);
extern void Diag_RxIndication(uint32 u32CanIfPduId, uint8* u8DataPtr, uint8 u8DataLen);
extern void Diag_TxConfirmation(uint32 u32CanIfPduId);

/* interfaces for UDS */
extern const uint16 u16g_SupportedDID[];
extern void Diag_GenSeed(uint8 u8_Seed[]);
extern void Diag_Encryption(const uint8 u8_Seed[], uint8 u8_Key[]);
extern uint32 Diag_CheckTransitionCondition(uint32 Session);
extern uint32 Diag_CheckClearDTC(void);
#endif
