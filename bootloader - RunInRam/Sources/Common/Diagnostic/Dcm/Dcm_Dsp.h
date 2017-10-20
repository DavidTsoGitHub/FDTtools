#ifndef DCM_DSP_H
#define DCM_DSP_H

/*=======[I N C L U D E S]====================================================*/
#include "Dcm.h"
#include "FlashLoader.h"

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
void Dcm_RecvMsg10(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg11(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg22(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg2E(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg27(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg28(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg34(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg36(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg37(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg31(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_CheckProgPreCondition(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_CheckProgDependencies(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_Enter_Tsw(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_CheckMemory(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_EraseMemory(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg3E(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_RecvMsg85(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);

void Dcm_Pending2E(const FL_ResultType errorCode,
                   const Dcm_BuffType * rxBuff,
                   Dcm_BuffType * txBuff);
                   
void Dcm_Pending36(const FL_ResultType errorCode,
                   const Dcm_BuffType * rxBuff,
                   Dcm_BuffType * txBuff);
                   
void Dcm_Pending31(const FL_ResultType errorCode,
                   const Dcm_BuffType * rxBuff,
                   Dcm_BuffType * txBuff);

#endif/* endof DCM_DSP_H */
