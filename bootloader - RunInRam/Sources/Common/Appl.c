/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Arc_Types.h"
#include "config.h"
#include "Appl.h"
#include "Mcu.h"
#include "Gpt.h"
#include "Wdg.h"
#include "SecM.h"
#include "ComStack_Types.h"
#include "Can.h"
#include "CanTp.h"
#include "Dcm.h"
#include "MPC5645S.h"
#include "Board.h"

/*=======[I N T E R N A L   D A T A]==========================================*/
/** timer counter for ECU shut down */
#if (FL_SLEEP_TIMER > 0)
STATIC uint32 ecuShutDownTimer;
#endif

/*=======[E X T E R N A L   D A T A]==========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

#if (FL_SLEEP_TIMER > 0)
STATIC void Appl_EcuShutdownTimer(void);
#endif

extern void Board_JumpApp( void );
extern void Board_GetBootRequest(uint32 *ProgRequest, uint32 *InverseProgRequest);
extern void Board_SetBootRequest(uint32 ProgRequest, uint32 InverseProgRequest);
extern void Board_FeedWdg(void);
extern void Board_SetStayInBoot(void);
STATIC void Appl_GotoAppSW(void);
extern uint8 Board_StayInBoot(void);

uint32 Counter ;

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/


/******************************************************************************/
/**
 * @brief               <ECU initialize>
 * 
 * <This routine shall be called by the bootcode contained in the flashloader 
 *  to initialize common hardware.> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/

FL_ResultType Appl_EcuStartup (void)
{
    uint32 ProgRequest = 0x0uL;
    uint32 InverseProgRequest = 0x0uL;

	/* realize the function  of app and bootloader software' merge*/
    (void)FL_MergeAppAndBootloader();
	  
    Appl_InitNvmInfo(FL_NvmInfo);

    /* stay in bootloader and initialize */
    Board_GetBootRequest(&ProgRequest, &InverseProgRequest);

    /* check if bootloader program is requested in app */ 
    if ((FL_EXT_PROG_REQUEST_RECEIVED == ProgRequest)&&(FL_EXT_PROG_REQUEST_RECEIVED == ~InverseProgRequest)) {

    	(void)Appl_FlStartup();	
        /* clear bootloader request flash and app update flag */
        Board_SetBootRequest(0, 0);

        /* DCM go programming mode and response programming session service */
        Dcm_ProgramInit();

    } else {

    	/* Check if Flash is damaged? */
        if ((FL_APPL_FORCE_STAY_IN_BOOT == ((ProgRequest) & 0xff))&&(FL_APPL_FORCE_STAY_IN_BOOT == (~InverseProgRequest)&0xff)) {

        	/* This is a result of a ECC failure in earlier execution.
        	 * We should not access any application flash when it could result in ECC exception */
        	(void)Appl_FlStartup();
        	Board_SetStayInBoot();

        } else {

        	/* check if application software is consistent or if we should stay in boot */
           	switch (Appl_CheckConsistency()) {
           		case FL_OK:
           			if ( Board_StayInBoot() == FALSE ) {
           				/* SIU.GPDO[87].R = 1;  This has been used for startup measurement */
               			Appl_GotoAppSW();
           			}
           			break;
           		case FL_IN_DEBUG:
           			Appl_GotoAppSW();
           			break;
           		default:
                    (void)Appl_FlStartup();
           			break;
        	}
        }

		/* We end up here when we should stay in boot */

		/* check if ECU reset was requested before the reset happened */
		if ((FL_APPL_RESPONSE_TO_RESET == ProgRequest)&&
				(FL_APPL_RESPONSE_TO_RESET == ~InverseProgRequest)) {

			/* DCM respond to the ECU reset request */
			Dcm_ResetResponse();

		}

		/* check if default session was requested before the reset happened */
		if ((FL_EXT_DEFAULT_SESSION_RECEIVED == ProgRequest) &&
				(FL_EXT_DEFAULT_SESSION_RECEIVED == ~InverseProgRequest)) {

			/* DCM respond to the default session request */
			Dcm_DefaultSessionResponse();

		}

		/* clear bootloader request flash and app update flag */
		Board_SetBootRequest(0, 0);

    }

    return FL_OK;
}

/******************************************************************************/
/**
 * @brief               <flashloader initialize>
 * 
 * <This routine shall be called by the flashloader to initialize the software 
 *  environment needed for ECU reprogramming (after the decision to switch to 
 *  flashloader mode).> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType Appl_FlStartup (void)
{
/*	SIU.PCR[87].B.OBE = 1;  This has been used for startup measurement */
   /* stay in bootloader mode and initialize */
	WdgStart();
    Gpt_Init(NULL_PTR);
    Can_Init();
    CanTp_Init();   
    Dcm_Init();
   /*  FL_InitState();*/
    #if (FL_SLEEP_TIMER > 0)
    ecuShutDownTimer = FL_SLEEP_TIMER / GPT_PERIOD_TIME;
    #endif
    
    return FL_OK;
}

#if (FL_SLEEP_TIMER > 0)
/******************************************************************************/
/**
 * @brief               <ECU goto sleep mode>
 * 
 * <This routine shall be called by the flashloader to shut down the ECU (put 
 *  the ECU to sleep).> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_EcuShutdown (void)
{
    /* do nothing */
    /* reserved for user, application related */
    return;
}
#endif

/******************************************************************************/
/**
 * @brief               <ECU reset>
 * 
 * <This routine shall be called by the flashloader to reset the ECU upon 
 *  request from the external programming device.> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_EcuReset (void)
{
    /* clear flash driver in RAM */
    Appl_Memset((uint8 *)FL_DEV_BASE_ADDRESS, 0x00u, FL_DEV_SIZE);
    
    /* watch dog reset */
    Board_Reset();
    
    return;
}

/******************************************************************************/
/**
 * @brief               <check application software consistency>
 * 
 * <This routine shall be called by the flashloader to check whether the 
 *  individual parts (logical blocks) that make up the application software of 
 *  an ECU are consistent with each other.> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType Appl_CheckConsistency (void)
{
    FL_ResultType ret = FL_OK;
    uint8 blockIndex = 0;
	FL_BlockHeader* SwBlockInfo = (FL_BlockHeader*)FL_BlkInfo[0].header;

	/* If the magic number is found in the valid flag then application has been programmed by the
	 * debugger and we should accept it as consistent */
	if (SwBlockInfo->blkValid == FL_APPL_PROG_BY_DEBUGGER_MAGIC) {
		return FL_IN_DEBUG;
	}

    for (blockIndex = 0; 
         (blockIndex < FL_NUM_LOGICAL_BLOCKS) && (ret == FL_OK); 
         blockIndex ++)
    {
    	SwBlockInfo = (FL_BlockHeader*)FL_BlkInfo[blockIndex].header;
        /* check if all blocks are valid */
        if ( (SwBlockInfo->blkValid != TRUE) || (SwBlockInfo->blkProgAttempt != FL_NvmInfo[blockIndex].programming_attempts))
        {
            ret = FL_FAILED;
        }
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <trigger watch dog>
 * 
 * <This routine shall be called by functions of the flashloader runtime 
 *  environment and the security module at least every 500 milliseconds.> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_UpdateTriggerCondition (void)
{
    /* check if tick time is overflow */
    if (TRUE == Gpt_GetFlag())
    {
    	SIU.GPDO[72].B.PDO =~SIU.GPDO[72].B.PDO;
        /* feed watch dog */
        CanTp_MainFunction();
        Dcm_MainFunction();
        WdgKick();

        #if (CFG_FL_MODE_STAY_TIME > 0)
        Appl_BootStayTimer();
		#endif
        
        #if (FL_SLEEP_TIMER > 0)
        Appl_EcuShutdownTimer();
        #endif
    }
    
    Board_FeedWdg();
    
    if(0xffff  == Counter)
    {
    	Counter = 0;
    }
    else
    {
        Counter++;	
    }
    
    Can_MainFunc();    
    return;    
}

/******************************************************************************/
/**
 * @brief               <memory copy>
 * 
 * <MISRA C 2004 forbid to use memcpy() lib,only used  to copy data buffer of 
 *  indirect address.> .
 * @param[in]           <source (IN), length (IN)>
 * @param[out]          <dest (OUT)>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_Memcpy(uint8 * dest, const uint8 *source, uint32 length)
{
    while (length > 0)
    {
        if ((dest != NULLPTR) && (source != NULLPTR))
        {
            *dest = *source;
            dest ++;
            source ++;
        }
        else
        {
            break;
        }
        
        length --;
    }
    
    return;
}

/******************************************************************************/
/**
 * @brief               <memory set>
 * 
 * <MISRA C 2004 forbid to use memset() lib,only used  to set data buffer of 
 *  indirect address.> .
 * @param[in]           <source (IN), length (IN)>
 * @param[out]          <dest (OUT)>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_Memset(uint8 * dest, const uint8 source, uint32 length)
{
    while (length > 0)
    {
        if (dest != NULLPTR)
		    {
            *dest = source;
            dest ++;
        }
        else
        {
            break;
        }
        
        length --;
    }
    
    return;
}


/******************************************************************************/
/**
 * @brief               <get nvm information>
 * 
 * <get information from EEPROM about bootloader information(e.g.fingerprint)> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void Appl_InitNvmInfo(FL_NvmInfoType* NvmInfo)
{
	/* CRC32 parameter */
	SecM_CRCParamType crcParam;
	const FL_NvmInfoType* NvmInfo_Source;
	uint8 FL_NvmInfo_Index[FL_NUM_LOGICAL_BLOCKS];
	uint8 i;

	/* Initialize FL_NvmInfo_Index */
	for (i=0; i<FL_NUM_LOGICAL_BLOCKS;i++) FL_NvmInfo_Index[i]=0xff;

    /* first find all indexes to the Nvm block information */
	FL_NvmInfoType *NvmInfoPtr = (FL_NvmInfoType*)FL_FingerPrint;
	for (i=0; i<FL_MAX_NO_FINGER_PRINTS; i++) {
		if ((NvmInfoPtr[i].areaType >= 0) && (NvmInfoPtr[i].areaType < FL_NUM_LOGICAL_BLOCKS)) {
			FL_NvmInfo_Index[NvmInfoPtr[i].areaType-FL_APPLICATION_BLOCK_TYPE] = i;
		}
	}

	/* initialize all the Nvm block informations */
	for (i=0; i<FL_NUM_LOGICAL_BLOCKS; i++) {
		/* if information was not found for the block then initialize as zeroes */
		if (FL_NvmInfo_Index[i] == 0xff) {
			Appl_Memset((uint8*)&FL_NvmInfo[i], 0x00u, sizeof(FL_NvmInfoType));
		} else {
			NvmInfo_Source = (FL_NvmInfoType*)&FL_FingerPrint[FL_NvmInfo_Index[i]];

			/* copy information from flash */
			Appl_Memcpy((uint8 *)&NvmInfo[i],
						(const uint8 *)NvmInfo_Source,
						sizeof(FL_NvmInfoType));

			/* compute CRC for NVM information */
			crcParam.crcState = CRC_INIT;
			crcParam.crcSourceBuffer = (const uint8 *)&NvmInfo[i];
			crcParam.crcByteCount = sizeof(FL_NvmInfoType) - 4;
			(void)SecM_ComputeCRC(&crcParam);
			crcParam.crcState = CRC_COMPUTE;
			(void)SecM_ComputeCRC(&crcParam);
			crcParam.crcState = CRC_FINALIZE;
			(void)SecM_ComputeCRC(&crcParam);

			/* compare CRC for NVM information */
			if (crcParam.currentCRC != NvmInfo[i].checksum) {
				/* if checksum is not correct, initialize the information with zeroes */
				Appl_Memset((uint8*)&NvmInfo[i], 0x00u, sizeof(FL_NvmInfoType));
			}
		}
	}

    return;

}

#if (FL_SLEEP_TIMER > 0)
/******************************************************************************/
/**
 * @brief               <time count for ECU shut down>
 * 
 * <time count for ECU shut down> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Appl_EcuShutdownTimer(void)
{
    /* 
    ** if diagnostic leave default session mode or security unlocked, stop 
    ** sleep timer count.
    */
    if ((ecuShutDownTimer > 0) &&
        (DCM_SECURITY_LOCKED == Dcm_GetSecurityLevel()) && 
        (DCM_SESSION_DEFAULT == Dcm_GetSessionMode()))
    {
        ecuShutDownTimer --;
        if (0x00u == ecuShutDownTimer)
        {
        	/* set ECU to sleep state */
            Appl_EcuShutdown();
        }
    }
    else
    {
        ecuShutDownTimer = 0x00u;
    }
    
    return;
}
#endif


/******************************************************************************/
/**
 * @brief               <go to application software>
 * 
 * <go to application software> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
STATIC void Appl_GotoAppSW(void)
{
    #if (CFG_FL_MODE_STAY_TIME > 0)
    /* de-initialize hardware module */
    Can_Deinit();
    Gpt_Deinit();
    Mcu_Deinit();
    #endif
    
    MCU_ModeEnterDRUN();
    /* jump to APP */
    Board_JumpApp();
    
    return;
}
