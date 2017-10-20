/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2009-2011 ArcCore AB <contact@arccore.com>
 * Licensed under ArcCore Embedded Software License Agreement.
 * -------------------------------- Arctic Core ------------------------------*/

/* ----------------------------[includes]------------------------------------*/
#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "Mcu.h"
#include "ComStack_Types.h"
#include "FlashLoader.h"
#include "Can.h"
#include "MPC5645S.h"
#include "Gpt_Cfg.h"
#include "Dcm.h"
#include "SecM.h"
#include "Wdg.h"
#include "Appl.h"

/* ----------------------------[private define]------------------------------*/
typedef uint8 Appl_BootHeaderType[8];
/* ----------------------------[private macro]-------------------------------*/
#define STAY_IN_BOOT_WAIT       0
#define STAY_IN_BOOT_TIMEOUT    1
#define STAY_IN_BOOT_TRIGGED    2

#define PCR_PA(_x)			((_x)<<10)
#define PCR_OBE				(1<<(15-6))
#define PCR_IBE				(1<<(15-7))
#define PCR_ODE				(1<<(15-10))
#define PCR_SRC(_x)			((_x)<<(15-13))
#define PCR_WPE				(1<<(15-14))
#define PCR_WPS				(1<<(15-15))

#define RGM_FES_F_EXR		0x8000
#define RGM_FES_F_SOFT		0x0004
#define RGM_DES_F_POR		0x8000

/* ----------------------------[private typedef]-----------------------------*/
/* ----------------------------[private function prototypes]-----------------*/
/* ----------------------------[private variables]---------------------------*/
STATIC uint8 stayInBootEvent;
/* ----------------------------[private functions]---------------------------*/
/* ----------------------------[public variables]----------------------------*/
struct
{
	uint32 Counter;
	uint32 Feed_Dog_Flag;
}Hardware_Watchdog ={0,1};

#if (CFG_FL_MODE_STAY_TIME > 0)
STATIC uint32 ecuBootStayTimer;
#endif

#pragma push /* Save the current state */
#pragma section sconst_type ".__bootarea_header"
#pragma pack(1)
__attribute__((force_export)) const Appl_BootHeaderType boot_header = "00.01";
#pragma pop

/* ----------------------------[public functions]----------------------------*/

void Board_InitClocks( void ) 
{ 
	Mcu_InitClock();
}

void Board_InitGpio( void ) 
{
	SIU.PCR[72].R = 0x0201u;  
	SIU.GPDO[72].B.PDO = 0x00u;
	   
   /* Set The Pcr register for Can Controller */
	SIU.PCR[121].R = 0x0200u;	/** 3.3v */
	SIU.GPDO[121].R = 0;
	
	SIU.PCR[18].R = 0x0200u;  	/** 5v */
	SIU.GPDO[18].R = 0;
	
#if defined(USE_CAN_A)

   SIU.PSMI[0].R = 0x00u;

   SIU.PCR[16].R = PCR_PA(1) | PCR_OBE |  PCR_ODE | PCR_WPE | PCR_WPS | PCR_SRC(1); /* 0x0624, CAN A TX PIN */
   SIU.PCR[17].R = PCR_PA(1) | PCR_IBE; /* 0x0500, CAN A RX PIN */

   SIU.PCR[116].R = 0x0200u;  /** stb */
   SIU.GPDO[116].R = 0x00u;
#endif
#if defined(USE_CAN_B)
   SIU.PCR[42].R = 0x0600u; /**< CAN B TX PIN */
   SIU.PCR[43].R = 0x0D03u; /**< CAN B RX PIN */

   SIU.PSMI[0].R = 0x01u;

#endif

}


void Board_Reset( void ) 
{
	/*	SIU.SRCR.B.SER = 1;*/
	SystemReset();
}



/**
 * Early init does very basic things, such as:
 *
 * !! NOTE !!
 * .data is NOT copied here, .bss is not cleared!
 * !! NOTE !!
 *
 * - PLL
 * - GPIO
 * - Setup flash
 *
 */
void Board_EarlyInit( void ) 
{
}


/**
 * Initiate board specific things such as:
 * - more more...
 */

void Board_Init( void ) 
{
    Mcu_Init(NULL_PTR);
	Board_InitClocks();
    Board_InitGpio();
}

/******************************************************************************/
/**
 * @brief               <Jumps to the address specified in the argument>
 *
 * <Jumps to the address specified is register 3 (argument to the call)>.
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
asm void JumpApp( register uint32 startAddr )
{
    #pragma unused (startAddr)
    nofralloc
    mtlr r3
    blr
}

/******************************************************************************/
/**
 * @brief               <jump to application software>
 *
 * <jump to application software> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_JumpApp( void )
{
	 JumpApp(*(uint32 *)(FL_APP_RCHW_ADDR + 0x04uL));
}

/******************************************************************************/
/**
 * @brief               <get the request to enter boot>
 *
 * <get the request to enter boot> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_GetBootRequest(uint32 *ProgRequest, uint32 *InverseProgRequest)
{
    uint32 _ProgRequest = 0x0uL;
    uint32 _InverseProgRequest = 0x0uL;

	asm("mfsprg0 %0":"=r"(_ProgRequest));
	asm("mfsprg1 %0":"=r"(_InverseProgRequest));
    *ProgRequest        = _ProgRequest;
    *InverseProgRequest = _InverseProgRequest;
}

/******************************************************************************/
/**
 * @brief               <set the request to enter boot>
 *
 * <set the request to enter boot> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_SetBootRequest(uint32 ProgRequest, uint32 InverseProgRequest)
{
	asm("mtsprg0 %0"::"r"(ProgRequest));
	asm("mtsprg1 %0"::"r"(InverseProgRequest));
}
 
/******************************************************************************/
/**
 * @brief               <feed the hardware watchdog>
 *
 * <feed the hardware watchdog> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_FeedWdg(void)
{

}

/******************************************************************************/
/**
 * @brief               <go to test software>
 *
 * <go to test software> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_Tsw(void)
{
	asm ("nop");
	asm ("nop");
	asm ("nop");
}

#if (CFG_FL_MODE_STAY_TIME > 0)
/******************************************************************************/
/**
 * @brief               <time count for ECU stay in boot>
 *
 * <time count for ECU stay in boot> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Appl_BootStayTimer(void)
{
	ecuBootStayTimer --;
	if (0x00u == ecuBootStayTimer)
	{
		/* Timeout of stay in boot waiting for request from diagnostic tool */
		stayInBootEvent=STAY_IN_BOOT_TIMEOUT;
	}

    return;
}
#endif

/******************************************************************************/
/**
 * @brief               <set variable stay in boot to value recognized as to stay in boot>
 *
 * <This function will set the local variable stayInBootEvent to the value indicating stay in boot
 *  This function is called from a receive of a start routine request diagnostic message (0xDEAD) > .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_ActivateStayInBoot(const Dcm_BuffType * rxBuff, Dcm_BuffType * txBuff)
{
	stayInBootEvent = STAY_IN_BOOT_TRIGGED;

    txBuff->pduInfo.SduLength = 0x04u;
	Dcm_SendRsp();
}

/******************************************************************************/
/**
 * @brief               <verify condition to stay in boot>
 *
 * <This function will verify the status requested to stay in the boot application.
 *  This will be used for situations where the application has been programmed correctly
 *  but there is something wrong with the downloaded application. By checking specified
 *  condition it will be possible to reprogram a malfunctioning application> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <bool> stay in boot
 */
/******************************************************************************/
uint8 Board_StayInBoot(void)
{
	/* Enter verification strategy here! return TRUE if to stay in boot */
	(void)Appl_FlStartup();
	ecuBootStayTimer = CFG_FL_MODE_STAY_TIME / GPT_PERIOD_TIME;
	stayInBootEvent=STAY_IN_BOOT_WAIT;
	while (1) {
        /* 10ms task and watch dog trigger */
        Appl_UpdateTriggerCondition();

        /* Check the stay in boot condition */
        switch (stayInBootEvent) {
        	case STAY_IN_BOOT_WAIT:
        		/* Stay in this loop */
        		break;
        	case STAY_IN_BOOT_TIMEOUT:
        		/* Timeout! continue into application */
        		return FALSE;
        	case STAY_IN_BOOT_TRIGGED:
        		/* We have received a request to stay in the boot from a diagnostic tool */
        		return TRUE;
        	default:
        		return FALSE;
		}
	}
	return FALSE;/* Should never end up here! */
}

/******************************************************************************/
/**
 * @brief               <test if we are in boot due to stay in boot>
 *
 * <This function will test if we are in boot due to stay in boot request> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
uint8 Board_IsStayInBoot(void)
{
	return (stayInBootEvent == STAY_IN_BOOT_TRIGGED);
}

/******************************************************************************/
/**
 * @brief               <set stay in boot>
 *
 * <This function will set stay in boot request> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>
 */
/******************************************************************************/
void Board_SetStayInBoot(void)
{
	stayInBootEvent=STAY_IN_BOOT_TRIGGED;
}

/******************************************************************************/
/**
 * @brief               <generate seed for UDS>
 *
 * <Needed by the UDS module,generate seed> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <seed (OUT)>
 * @param[in/out]       <NONE>
 * @return              <SecM_StatusType>
 */
/******************************************************************************/
#define SEED_LENGTH 4  // seed size
#define KEY_LENGTH  4  // key  size
SecM_StatusType Board_GenerateSeed(SecM_SeedType *seed)
{
    /* seed generate algorithm should added by user */
    /* for example */
    seed->seedX = (uint32)0;
    seed->seedY = (uint32)0;

    return SECM_OK;
}

/******************************************************************************/
/**
 * @brief               <compute key>
 *
 * <compute key before compute key> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <seed (IN), k (IN)>
 * @param[out]          <key (OUT)>
 * @param[in/out]       <NONE>
 * @return              <SecM_StatusType>
 */
/******************************************************************************/
SecM_StatusType Board_ComputeKey(SecM_SeedType seed,
                                SecM_WordType k,
                                SecM_KeyType *key)
{
    /* key compute algorithm should added by user */
    /* This is our simple inversion example */
    *key = seed.seedX + seed.seedY + k;

    return SECM_OK;
}

