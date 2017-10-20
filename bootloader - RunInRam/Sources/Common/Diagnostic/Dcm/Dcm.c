/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Dcm.h"
#include "Dcm_Types.h"
#include "Dcm_Internel.h"
#include "Appl.h"

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** Dcm communicate status */
typedef struct
{
    /* DCM main rx buffer */
    Dcm_BuffType	rxBuff;
    
    /* DCM main tx buffer */
    Dcm_BuffType	txBuff;

    /* DCM current service */
    const Dcm_ServiceTableType * curServicePtr;

    /* if there is a new service request is not processed */
    boolean reqNew;

    /* if there is response is finished */
    boolean respFinished;

    uint16 pendingCount;
        
    /* DCM P3C timer */
    uint16 p3cTimer;

    /* DCM P2e timer */
    uint16 p2eTimer;
    
} Dcm_ComType;

/** Session runtime status */
typedef struct
{
    /* DCM current session type */
    Dcm_SessionType curSession;

    /* ECU reset Timer */
    uint16 resetTimer;
    
    /* if security access is unlocked */
    Dcm_SecurityType securityLevel;
    
    /* DCM security access timer */
    uint32 securityTimer;

} Dcm_RunTimeType;

/*=======[E X T E R N A L   D A T A]==========================================*/
/** use static memory for service */
uint8	Dcm_MainRxBuff[DCM_RX_BUF_SIZE];

uint8	Dcm_MainTxBuff[DCM_TX_BUF_SIZE];

/*=======[I N T E R N A L   D A T A]==========================================*/

/** dcm communication status about service */
STATIC Dcm_ComType dcmComStatus;

/** dcm runtime status about session and security access */
STATIC Dcm_RunTimeType dcmRunTime;

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
STATIC void Dcm_ServiceStart(void);
STATIC void Dcm_ServiceHandle(void);
STATIC void Dcm_ServiceProcess(void);
STATIC void	Dcm_StartP3cTimer(void);
STATIC void	Dcm_StopP3cTimer(void);
STATIC void Dcm_P3cTimerCheck(void);
STATIC void	Dcm_StartP2eTimer(uint16 timeout);
STATIC void	Dcm_StopP2eTimer(void);
STATIC void Dcm_P2eTimerCheck(void);
STATIC void Dcm_StartTransmit( void );
STATIC void Dcm_SecurityTimerCheck(void);
STATIC void Dcm_StopResetTimer(void);
STATIC void Dcm_ResetTimerCheck(void);
STATIC void Dcm_TimerProc(void);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/**
 * @brief               <DCM module initialize>
 * 
 * <DCM module initialize> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_Init(void)
{
    /* set session to default session */
    Dcm_SetSessionMode(DCM_SESSION_DEFAULT);
    
    /* reset to security access level */
    Dcm_SetSecurityLevel(DCM_SECURITY_LOCKED);

    /* start security access timer */
    Dcm_StartSecurityTimer(DCM_SECURITY_TIME_ON_BOOT);

    /* stop ECU reset timer */
    Dcm_StopResetTimer();
    
    /* reset service process */
    Dcm_ServiceFinish();

	/* initialize dsp */
    Dcm_DspInit();
    
    return;
}

/******************************************************************************/
/**
 * @brief               <DCM module program initialize>
 * 
 * <when program boot request is equal to FL_EXT_PROG_REQUEST_RECEIVED,
 *  this API will be called in Appl_FlStartup function, session is initialized 
 *  to programming session,and simulate an 10 03 session control service is 
 *  received> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_ProgramInit(void)
{
    /* set session to extended session */
    Dcm_SetSessionMode(DCM_SESSION_EXTEND);
    
    /* simulate receive service 10 */
    dcmComStatus.rxBuff.buffStatus = DCM_BUFF_FOR_TP;
    
    dcmComStatus.rxBuff.pduId = DCM_RX_PHY_PDU_ID;
    
	/* set received data */
    dcmComStatus.rxBuff.pduInfo.SduDataPtr[0] = 0x11u;
    dcmComStatus.rxBuff.pduInfo.SduDataPtr[1] = DCM_SESSION_PROGRAMMING;

    dcmComStatus.rxBuff.pduInfo.SduLength = 0x02u;
    
	Dcm_StartSecurityTimer(0x00u);
    
	/* start process service 0x10 */
    Dcm_ServiceStart();
    
    return;
}

/******************************************************************************/
/**
 * @brief               <DCM Send a ECU reset response>
 *
 * <If there was a ECU reset and we endup in the boot with an erroneous application
 *  then we send the response from here> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Dcm_ResetResponse(void)
{

   dcmComStatus.txBuff.pduInfo.SduDataPtr[0] = 0x51;
   dcmComStatus.txBuff.pduInfo.SduDataPtr[1] = DCM_HARD_RESET;
   dcmComStatus.txBuff.pduInfo.SduDataPtr[2] = 0;

   dcmComStatus.txBuff.pduInfo.SduLength = 0x03u;

   Dcm_SendRsp();

   return;
}

/******************************************************************************/
/**
 * @brief               <DCM Send a default session response>
 *
 * <If there was a default session request then the boot will have performed a reset
 *  if there is an erroneous application(applicaiton will not respond to the request)
 *  then the boot will send the response from here> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Dcm_DefaultSessionResponse(void)
{
    /* set response message */
	dcmComStatus.txBuff.pduInfo.SduDataPtr[0] = 0x50u;
	dcmComStatus.txBuff.pduInfo.SduDataPtr[1] = DCM_SESSION_DEFAULT;
	dcmComStatus.txBuff.pduInfo.SduDataPtr[2] = (uint8)DCM_P2MAX_TIME;
	dcmComStatus.txBuff.pduInfo.SduDataPtr[3] = (uint8)(((uint16) DCM_P2MAX_TIME) >> 0x08u);
	dcmComStatus.txBuff.pduInfo.SduDataPtr[4] = (uint8)DCM_P2SMAX_TIME;
	dcmComStatus.txBuff.pduInfo.SduDataPtr[5] = (uint8)(((uint16) DCM_P2SMAX_TIME) >> 0x08u);
	dcmComStatus.txBuff.pduInfo.SduLength = 0x06u;

	Dcm_SendRsp();
}

/******************************************************************************/
/**
 * @brief               <DCM main task function>
 * 
 * <DCM main task function> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_MainFunction(void)
{
    /* process P2C, P3C, security timer */
    Dcm_TimerProc();
    
    /* process service */
    Dcm_ServiceProcess();

    return;
}

/******************************************************************************/
/**
 * @brief               <DCM provide rx buffer for CanTp>
 * 
 * <DCM provide rx buffer for CanTp,mean an request is receiving> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <pduId (IN), pduLen (IN)>
 * @param[out]          <sduInfo (OUT)>
 * @param[in/out]       <NONE>
 * @return              <CanTp_BufReq_RetType>    
 */
/******************************************************************************/
CanTp_BufReq_RetType Dcm_ProvideRxBuffer(PduIdType pduId,
                                         PduLengthType pduLen,
											                   PduInfoType **sduInfo )
{
    CanTp_BufReq_RetType ret = BUFREQ_OK;
    
    /* Rx buffer is free state and pduid is correct */
    if ((DCM_BUFF_FREE == dcmComStatus.rxBuff.buffStatus) &&
        ((DCM_RX_PHY_PDU_ID == pduId) ||
         (DCM_RX_FUNC_PDU_ID == pduId)))
    {
	    /* Main Buffer should be free */
	    if (pduLen <= DCM_RX_BUF_SIZE)
	    {
	        /* allocate MainRxBuff */
		    dcmComStatus.rxBuff.buffStatus = DCM_BUFF_FOR_TP;
            dcmComStatus.rxBuff.pduId = pduId;
		    dcmComStatus.rxBuff.pduInfo.SduLength = pduLen;
	      	*sduInfo = &dcmComStatus.rxBuff.pduInfo;
	    }
	    else
	    {
	        /* main buffer provided fail */
		    ret = BUFREQ_E_OVFL;
	    }
    }
    else
    {
	      /* No buffer available */
		  ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/**
 * @brief               <DCM rx indication from CanTp>
 * 
 * <DCM rx indication from CanTp,mean an request is received> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <pduId (IN), result (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_RxIndication(PduIdType pduId,NotifResultType result)
{
	/* check if Rx buffer is provide for CanTp */
    if (DCM_BUFF_FOR_TP == dcmComStatus.rxBuff.buffStatus)
    {
        /* Indication for MainBuff */
	    if ((NTFRSLT_OK == result) &&
            (pduId == dcmComStatus.rxBuff.pduId))
	    {
	        /* receive Ok */
            Dcm_ServiceStart();
	    }
	    else
	    {
	        /* receive failed */
            Dcm_ServiceFinish();
	    }
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <DCM provide tx buffer for CanTp>
 * 
 * <DCM provide rx buffer for CanTp,mean a response is sending> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <pduId (IN)>
 * @param[out]          <sduInfo (OUT)>
 * @param[in/out]       <NONE>
 * @return              <CanTp_BufReq_RetType>    
 */
/******************************************************************************/
CanTp_BufReq_RetType Dcm_ProvideTxBuffer(PduIdType pduId,PduInfoType **sduInfo)
{
    CanTp_BufReq_RetType ret = BUFREQ_OK;  
	
	/* check if Tx Buffer is provide for service process */
	if ((DCM_BUFF_FOR_SERVICE == dcmComStatus.txBuff.buffStatus) &&
        (pduId == dcmComStatus.txBuff.pduId))
	{
	        /* send MainTXBuff */
	        *sduInfo = &dcmComStatus.txBuff.pduInfo;
	        dcmComStatus.txBuff.buffStatus = DCM_BUFF_FOR_TP;
    }
    else
    {
        Dcm_ServiceFinish();
        ret = BUFREQ_E_NOT_OK;
    }

    return ret;
}

/******************************************************************************/
/**
 * @brief               <DCM tx confirmation from CanTp>
 * 
 * <DCM rx indication from CanTp,mean a response is sended> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <pduId (IN), result (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_TxConfirmation(PduIdType pduId,NotifResultType result)
{

    /* service response finished */
    if((NTFRSLT_OK == result) && 
        (pduId == dcmComStatus.txBuff.pduId) &&
        (FALSE == dcmComStatus.respFinished))
    {
    	/* 
    	** this case is only appear when send pending message and service is 
    	** not finished 
    	*/
        dcmComStatus.txBuff.buffStatus = DCM_BUFF_FREE;
    }
    else
    {
        Dcm_ServiceFinish();
    }
 
    return;
}

/******************************************************************************/
/**
 * @brief               <set service process finish>
 * 
 * <set service process finish> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_ServiceFinish(void)
{
    /* set current service */
    dcmComStatus.curServicePtr = NULLPTR;

    /* initialize tx and rx buffer */
    dcmComStatus.rxBuff.pduInfo.SduDataPtr = Dcm_MainRxBuff;
    dcmComStatus.txBuff.pduInfo.SduDataPtr = Dcm_MainTxBuff;
    
    /* initialize tx and rx buffer status */
    dcmComStatus.rxBuff.buffStatus = DCM_BUFF_FREE;
    dcmComStatus.txBuff.buffStatus = DCM_BUFF_FREE;
    
    /* initialize no new service request */
    dcmComStatus.reqNew = FALSE;
    dcmComStatus.respFinished = TRUE;

    /* clear pending count num */
    dcmComStatus.pendingCount = 0x00u;
    
    /* stop P2E timer */
    Dcm_StopP2eTimer();

    /* process P3C timer in extended and programming session */
    if (DCM_SESSION_DEFAULT == dcmRunTime.curSession)
    {
        Dcm_StopP3cTimer();
    }
    else
    {
        Dcm_StartP3cTimer();	
    }

    return;
}

/******************************************************************************/
/**
 * @brief               <set session mode>
 * 
 * <set session mode> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <sessMode (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_SetSessionMode(const Dcm_SessionType sessMode)
{

    /* set current session */
    dcmRunTime.curSession = sessMode;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <get session mode>
 * 
 * <get session mode> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
Dcm_SessionType Dcm_GetSessionMode(void)
{
    /* get current session */
    return dcmRunTime.curSession;
}

/******************************************************************************/
/**
 * @brief               <check if session mode is support>
 * 
 * <check if session mode is support> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <sessionTable (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <boolean>    
 */
/******************************************************************************/
boolean Dcm_CheckSessionSupp(const Dcm_SessionType * sessionTable)
{
    boolean ret = FALSE;
    uint8 sessionIndex = DCM_SESSION_SUPPORT_NUM;
    
    for (sessionIndex = 0; 
         (sessionIndex < DCM_SESSION_SUPPORT_NUM) && (FALSE == ret); 
         sessionIndex ++)
    {
        if (dcmRunTime.curSession == sessionTable[sessionIndex])
        {
            ret = TRUE;
        }
    }

    return ret;
}

/******************************************************************************/
/**
 * @brief               <set security level>
 * 
 * <set security level> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <secLev (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_SetSecurityLevel(const Dcm_SecurityType secLev)
{
    /* set current security level */
    dcmRunTime.securityLevel = secLev;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <get security level>
 * 
 * <get security level> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <Dcm_SecurityType>    
 */
/******************************************************************************/
Dcm_SecurityType Dcm_GetSecurityLevel(void)
{
    /* get current security level */
    return dcmRunTime.securityLevel;
}

/******************************************************************************/
/**
 * @brief               <check if security timer is expired>
 * 
 * <check if security timer is expired> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <boolean>    
 */
/******************************************************************************/
boolean Dcm_GetSecurityTimerExpired(void)
{
    
    boolean ret = TRUE;

    /* check if security timer expired */
    if (dcmRunTime.securityTimer > 0)
    {
        ret = FALSE;
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <check if security level if supported>
 * 
 * <check if security level if supported> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <secLevTable (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <boolean>    
 */
/******************************************************************************/
boolean Dcm_CheckSecuritySupp(const Dcm_SecurityType * secLevTable)
{
    boolean ret = FALSE;
    uint8 secIndex;
    
    for (secIndex = 0; 
         (secIndex < DCM_SECURITY_SUPPORT_NUM) && (FALSE == ret); 
         secIndex ++)
    {
        if (dcmRunTime.securityLevel == secLevTable[secIndex])
        {
            ret = TRUE;
        }
    }

    return ret;
}

/******************************************************************************/
/**
 * @brief               <start security timer>
 * 
 * <start security timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <timeOut (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void  Dcm_StartSecurityTimer(uint32 timeOut)
{  
    /* set to config value */
    dcmRunTime.securityTimer = timeOut / DCM_MAIN_TICK;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <start reset timer>
 * 
 * <start reset timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <timeOut (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_StartResetTimer(uint16 timeOut)
{    
    /* set reset timer value */
    dcmRunTime.resetTimer = timeOut / DCM_MAIN_TICK;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <send response>
 * 
 * <send response> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_SendRsp(void) 
{
	/* service process is finished */
    dcmComStatus.respFinished = TRUE;
    
    /* transmit message */
    Dcm_StartTransmit();
    
    return;
}

/******************************************************************************/
/**
 * @brief               <transmit negative response>
 * 
 * <transmit negative response> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <nrcCode (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Dcm_SendNcr( uint8 nrcCode ) 
{
    uint8* pduData = (uint8*)&(dcmComStatus.txBuff.pduInfo.SduDataPtr[0]);

	/* check if send pending */
    if (nrcCode != DCM_E_PENDING)
    {
    	/* if send NRC, service process is finished */
        dcmComStatus.respFinished = TRUE;
    }
    else
    {
    	/* if send pending , increat count */
        dcmComStatus.pendingCount ++;
    }
    
    pduData[0] = DCM_RSP_SID_NRC;
    pduData[1] = dcmComStatus.rxBuff.pduInfo.SduDataPtr[0];
    pduData[2] = nrcCode;
    dcmComStatus.txBuff.pduInfo.SduLength = 0x03u;
    
    Dcm_StartTransmit();
    
    return;
}

/******************************************************************************/
/**
 * @brief               <start service process when received message>
 * 
 * <start service process when received message> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_ServiceStart(void)
{
    /* Rx buffer in service status */
	dcmComStatus.rxBuff.buffStatus = DCM_BUFF_FOR_SERVICE;

    /* start P2E timer */
    Dcm_StartP2eTimer(DCM_P2MAX_TIME);

    /* stop P3C timer */
	Dcm_StopP3cTimer();    	
    
    /* a new service request is received */
    dcmComStatus.reqNew = TRUE;
    
    dcmComStatus.respFinished = FALSE;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <handle requested service>
 * 
 * <handle requested service> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_ServiceHandle(void)
{
    boolean SIDFind = FALSE;
    uint8 tableIndex = DCM_SERVICE_NUM;
    
    /* record current service table */
    dcmComStatus.curServicePtr = Dcm_ServiceTable;
    
    /* find service table */
    while ((tableIndex > 0) && (FALSE == SIDFind))
    {
        tableIndex --;
        if ((dcmComStatus.curServicePtr->SID == dcmComStatus.rxBuff.pduInfo.SduDataPtr[0]) &&
            ((DCM_RX_PHY_PDU_ID == dcmComStatus.rxBuff.pduId) ||
             ((DCM_RX_FUNC_PDU_ID == dcmComStatus.rxBuff.pduId) && 
              (TRUE == dcmComStatus.curServicePtr->funcAddrSupportted))))
        {
            SIDFind = TRUE;
        }
        else
        {
            dcmComStatus.curServicePtr ++;
        }
    }

	/* check if service table is finded */
    if (TRUE == SIDFind)
    {
    	/* check if service is supportted in current session */
        if (TRUE == Dcm_CheckSessionSupp(dcmComStatus.curServicePtr->sessionSupp))
        {
        	/* execute service process */
            dcmComStatus.curServicePtr->serviceFct(&dcmComStatus.rxBuff,&dcmComStatus.txBuff);
        }
        else
        {
            /* service is not supportted in active session */
            Dcm_SendNcr(DCM_E_SERVICE_NOT_SUPPORTED_INACTIVE_SESSION);
        } 
    }
    else
    {
    	/* has not find service table */
        if (DCM_RX_FUNC_PDU_ID == dcmComStatus.rxBuff.pduId)
        {
            /* if received PDU ID is function address ,reset service process */
            Dcm_ServiceFinish();            
        }
        else
        {
            /* if received PDU ID is phycial address ,service is not supportted */
            Dcm_SendNcr(DCM_E_SERVICE_NOT_SUPPORTED);
        }
    }
		
    return;
}


/******************************************************************************/
/**
 * @brief               <process new requested service>
 * 
 * <process new requested service> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_ServiceProcess(void)
{
    /* check if tx buffer is free */
    if (DCM_BUFF_FREE == dcmComStatus.txBuff.buffStatus)
    {
    	/* check if there if a new request service */
        if (TRUE == dcmComStatus.reqNew)
        {
            dcmComStatus.reqNew = FALSE;
            
            Dcm_ServiceHandle();
        }
        else
        {
        	/* check service process for pending */
            if (FALSE == dcmComStatus.respFinished)
            {
                FL_ResultType errorCode;
                boolean serviceFinished = FL_ServiceFinished(&errorCode);

                /* check if service busy is finished,then can send response. */
                if ((TRUE == serviceFinished) && 
                    (dcmComStatus.curServicePtr->pendingFct != NULLPTR))
                {
                    dcmComStatus.curServicePtr->pendingFct(errorCode,
                                                           &dcmComStatus.rxBuff,
                                                           &dcmComStatus.txBuff);
                }
            }
        }
    }

    return;
}

/******************************************************************************/
/**
 * @brief               <start P3C timer by config value>
 * 
 * <start P3C timer by config value> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void	Dcm_StartP3cTimer(void)
{
    dcmComStatus.p3cTimer = DCM_P3MAX_TIME / DCM_MAIN_TICK;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <stop P3C timer>
 * 
 * <stop P3C timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void	Dcm_StopP3cTimer(void)
{
    dcmComStatus.p3cTimer = 0;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <process P3C timer>
 * 
 * <process P3C timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_P3cTimerCheck(void)
{
    if (dcmComStatus.p3cTimer > 0x00u)
    {
        dcmComStatus.p3cTimer --;
        
        if (0x00u == dcmComStatus.p3cTimer)
        {
            /* if P3C timeout, set ECU reset to default session */
            Appl_EcuReset();
        }
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <start P2E timer by given value>
 * 
 * <start P2E timer by given value> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <timeout (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void	Dcm_StartP2eTimer(uint16 timeout)
{
    dcmComStatus.p2eTimer = timeout / DCM_MAIN_TICK;

    return;
}

/******************************************************************************/
/**
 * @brief               <stop P2E timer>
 * 
 * <stop P2E timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void	Dcm_StopP2eTimer(void)
{
    dcmComStatus.p2eTimer = 0;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <process P2E timer>
 * 
 * <process P2E timer,and send pending> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_P2eTimerCheck(void)
{
    if (dcmComStatus.p2eTimer > 0x00u)
    {
        dcmComStatus.p2eTimer --;
        if (0x00u == dcmComStatus.p2eTimer)
        {            
            /* start P2E timer to 5000ms timeout */
            Dcm_StartP2eTimer(DCM_P2SMAX_TIME);
            
            if(DCM_BUFF_FREE == dcmComStatus.txBuff.buffStatus)
            {
                if(dcmComStatus.pendingCount < DCM_PENDING_ATTEMPT_NUM)
                {
                    Dcm_SendNcr(DCM_E_PENDING);
                }
                else
                {
                    Dcm_SendNcr(DCM_E_GENERAL_REJECT);
                }  
            }
        }	
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <transmit response>
 * 
 * <transmit response, but not include pending> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_StartTransmit( void )
{
    dcmComStatus.txBuff.buffStatus = DCM_BUFF_FOR_SERVICE;
    
    dcmComStatus.txBuff.pduId = DCM_TX_PDU_ID;
    
    CanTp_Transmit(dcmComStatus.txBuff.pduId, &(dcmComStatus.txBuff.pduInfo)); 
    
    return;
}

/******************************************************************************/
/**
 * @brief               <process security timer>
 * 
 * <process security timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_SecurityTimerCheck(void)
{
    if (dcmRunTime.securityTimer > 0x00u)
    {
        dcmRunTime.securityTimer --;
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <stop reset timer>
 * 
 * <stop reset timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_StopResetTimer(void)
{    
    /* clear reset timer value */
    dcmRunTime.resetTimer = 0x00u;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <process reset timer>
 * 
 * <process reset timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_ResetTimerCheck(void)
{
    if (dcmRunTime.resetTimer > 0x00u)
    {
        dcmRunTime.resetTimer --;
        
        /* check if reset timer is timeout */
        if (0x00u == dcmRunTime.resetTimer)
        {
        	/* ECU reset */
            Appl_EcuReset();
        }
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <process session timer P2E,P3C,reset ,security timer>
 * 
 * <process session timer P2E,P3C,reset,security timer> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Dcm_TimerProc(void)
{
    /* process security timer */
    Dcm_SecurityTimerCheck();

    /* process P3C timer */
    Dcm_P3cTimerCheck();

    /* process P2E timer */
    Dcm_P2eTimerCheck();

    /* process ECU reset timer */
    Dcm_ResetTimerCheck();
    
    return;
}



