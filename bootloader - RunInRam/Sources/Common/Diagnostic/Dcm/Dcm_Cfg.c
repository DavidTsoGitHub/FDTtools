/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Dcm_Types.h"
#include "Dcm_Dsp.h"
#include "Dcm_Internel.h"

extern void Board_ActivateStayInBoot(const Dcm_BuffType * rxBuff,Dcm_BuffType * txBuff);
extern SecM_StatusType Board_GenerateSeed(SecM_SeedType *seed);

/*=======[E X T E R N A L   D A T A]==========================================*/
const Dcm_ServiceTableType Dcm_ServiceTable[DCM_SERVICE_NUM] = 
{
    /* SID, funcAddrSupport,  sessionSupp,      ServiceFct,  pendingFct */
    /* @type:uint8 range:0x00~0xFF note:select unique service Id */
             /* @type:boolean range:TRUE,FALSE note:NONE */
                               /* @type:uint8 range:session value note:select defined session value */
                                                    /* @type:API range:NONE note:auto generate */
                                                                   /* @type:API range:NONE note:auto generate */
    {0x10u,  TRUE,             {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND}, &Dcm_RecvMsg10,NULLPTR},
    {0x11u,  TRUE,             {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND}, &Dcm_RecvMsg11,NULLPTR},
    {0x22u,  FALSE,            {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND}, &Dcm_RecvMsg22,NULLPTR},
    {0x27u,  FALSE,            {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg27,NULLPTR},
    {0x28u,  TRUE,             {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg28,NULLPTR},
    {0x2Eu,  FALSE,            {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg2E,&Dcm_Pending2E},
    {0x31u,  TRUE,             {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND}, &Dcm_RecvMsg31,&Dcm_Pending31},
    {0x34u,  FALSE,            {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg34,NULLPTR},
    {0x36u,  FALSE,            {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg36,&Dcm_Pending36},
    {0x37u,  FALSE,            {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg37,NULLPTR},
    {0x3Eu,  TRUE,             {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND}, &Dcm_RecvMsg3E,NULLPTR},
    {0x85u,  TRUE,             {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND}, &Dcm_RecvMsg85,NULLPTR},
};

const Dcm_SessionRowType Dcm_SessionRow[DCM_SESSION_NUM] =
{
    /* sessionType, sessionSupp,     securitySupp */ 
    /* @type:uint8 range:0x00~0xFF note:select unique session value */
                     /* @type:uint8 range:session value note:select defined session value */
                                        /* @type:uint8 range:security value note:select defined security value */
    {DCM_SESSION_DEFAULT,    {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
    {DCM_SESSION_PROGRAMMING,{DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
    {DCM_SESSION_EXTEND,     {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
};

const Dcm_ResetRowType Dcm_ResetRow[DCM_RESET_NUM] =
{
    /* resetType, sessionSupp,     securitySupp */
    /* @type:uint8 range:0x00~0xFF note:NONE */
                   /* @type:uint8 range:session value note:select defined session value */
                                       /* @type:uint8 range:security value note:select defined security value */
    {0x01u,        {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV2}},
    {0x02u,        {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV2}},
};

#if(DCM_READDID_NUM > 0)
const Dcm_ReadDidRowType Dcm_ReadDidRow[DCM_READDID_NUM] = 
{
    /* DID, securitySupp,readDataFct */  
			/* @type:uint16 range:0x0000~0xFFFF note:NONE */
					/* @type:uint8 range:security value note:select defined security value */
                            /* @type:API range:NONE note:input APIname */
    {0xF15Bu,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF15B},
    {0xF180u,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF180},
    {0xF18Bu,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF18B},
    {0xF18Cu,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF18C},
    {0xF192u,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF192},
    {0xF1FAu,{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}, &FL_ReadDidF1FA}

};
#endif

const Dcm_SecurityRowType Dcm_SecurityRow[DCM_SECURITY_NUM] = 
{
    /* seedId,keyId,secAccessLevel,   sessionSupp,     securitySupp */
    /* @type:uint8 range:0x00~0xFF note:NONE */
               /* @type:uint8 range:0x00~0xFF note:NONE */
                      /* @type:define range:DCM_SECURITY_LEV1~DCM_SECURITY_LEV2 note:NONE */
                                        /* @type:uint8 range:session value note:select defined session value */
                                                            /* @type:uint8 range:security value note:select defined security value */
                                                                          /* @type:API range:NONE note:input APIname */
                                                                                             /* @type:API range:NONE note:input APIname */
    {0x01u,    0x02u, DCM_SECURITY_LEV1,{DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Board_GenerateSeed,&SecM_CompareKey},
};

#if (DCM_COM_CONTROL_NUM > 0)
const Dcm_ComControlRowType Dcm_ComControlRow[DCM_COM_CONTROL_NUM] = 
{
    /* controlType,sessionSupp,     securitySupp */ 
    /* @type:uint8 range:0x00~0xFF note:NONE */
                   /* @type:uint8 range:session value note:select defined session value */
                                       /* @type:uint8 range:security value note:select defined security value */
    {0x00u,        {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
    {0x01u,        {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
};
#endif

const Dcm_WriteDidRowType Dcm_WriteDidRow[DCM_WRITEDID_NUM] = 
{
    /* DID,  dataLength,securitySupp,writeDataFct */  
    /* @type:uint16 range:0x0000~0xFFFF note:NONE */
              /* @type:uint16 range:0x0001~0xFFFF note:NONE */
                          /* @type:uint8 range:security value note:select defined security value */
                                        /* @type:API range:NONE note:input APIname */
    {0xF15Au, 0x11u,      {DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&FL_WriteFingerPrint},
};

const Dcm_RoutineControlRowType Dcm_RoutineControlRow[DCM_ROUTINE_CONTROL_NUM] = 
{
    /* routineId,optionLength,funcAddrSupp,sessionSupp,    securitySupp,routineControl */ 
    /* @type:uint16 range:0x0000~0xFFFF note:select unique routine Id */
                 /* @type:uint16 range:0x0000~0xFFFF note:auto generate */
                                /* @type:boolean range:TRUE,FALSE note:auto generate */
                                             /* @type:uint8 range:session value note:select defined session value */
                                                                 /* @type:uint8 range:security value note:select defined security value */
                                                                               /* @type:API range:NONE note:auto generate */
    {0x0203u,    0x00u,         TRUE,        {DCM_SESSION_DEFAULT,DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Dcm_CheckProgPreCondition},
    {0xFF00u,    0x02u,         FALSE,       {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Dcm_EraseMemory},
    {0x0202u,    0xCu,          FALSE,       {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Dcm_CheckMemory},
    {0xFF01u,    0x00u,         FALSE,       {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Dcm_CheckProgDependencies},
    {0xF000u,    0x00u,         FALSE,       {DCM_SESSION_DEFAULT,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Dcm_Enter_Tsw},
    {0xDEADu,    0x00u,         FALSE,       {DCM_SESSION_DEFAULT,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},&Board_ActivateStayInBoot}
};

const Dcm_DownloadRowType Dcm_DownloadRow = 
{
    /* securitySupp */
    /* @type:uint8 range:security value note:select defined security value */
    {DCM_SECURITY_LEV1,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1},
};

#if (DCM_DTC_SET_NUM > 0)
const Dcm_DTCSettingRowType Dcm_DTCSettingRow[DCM_DTC_SET_NUM] = 
{
    /* DTCSet,sessionSupp,     securitySupp */ 
    /* @type:uint8 range:0x00~0xFF note:NONE */
               /* @type:uint8 range:session value note:select defined session value */
                                   /* @type:uint8 range:security value note:select defined security value */
    {0x01u,    {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
    {0x02u,    {DCM_SESSION_PROGRAMMING,DCM_SESSION_EXTEND,DCM_SESSION_EXTEND},{DCM_SECURITY_LOCKED,DCM_SECURITY_LEV1,DCM_SECURITY_LEV1}},
};
#endif
