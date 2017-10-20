/*=======[I N C L U D E S]====================================================*/
#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "Mcu.h"
#include "MPC5645S.h"

/*=======[M A C R O S]========================================================*/

#define MCU_ENABLE  0x01u
#define MCU_DISABLE 0x00u

#define PCTL_PC1	0x01u

#define SYSCLOCK_SELECT_PLL	0x2

/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
STATIC uint8 EccError;

/**
 * Function to setup the internal flash for optimal performance
 */
static void Mcu_ConfigureFlash(void)
{
	/* These flash settings increases the CPU performance of 7 times compared
   	   to reset default settings!! */

#if defined(CFG_MPC5516)
	/* Disable pipelined reads when flash options are changed. */
	FLASH.MCR.B.PRD = 1;

	/* Enable master prefetch for e200z1 and eDMA. */
	FLASH.PFCRP0.B.M0PFE = 1;
	FLASH.PFCRP0.B.M2PFE = 1;

	/* Address pipelining control. Must be set to the same value as RWSC. */
	FLASH.PFCRP0.B.APC = 2;
	FLASH.PFCRP0.B.RWSC = 2;

	/* Write wait states. */
	FLASH.PFCRP0.B.WWSC = 1;

	/* Enable data prefetch. */
	FLASH.PFCRP0.B.DPFEN = 1;

	/* Enable instruction prefetch. */
	FLASH.PFCRP0.B.IPFEN = 1;

	/* Prefetch algorithm. */
	/* TODO: Ask Freescale about this option. */
	FLASH.PFCRP0.B.PFLIM = 2;

	/* Enable line read buffers. */
	FLASH.PFCRP0.B.BFEN = 1;

	/* Enable pipelined reads again. */
	FLASH.MCR.B.PRD = 0;
#elif defined(CFG_MPC5668)
	/* Check values from cookbook and MPC5668x Microcontroller Data Sheet */

	/* Should probably trim this values */
	/* GCC cannot deal with this for some strange reason, ie BUG */
#if 0
	const typeof(FLASH.PFCRP0.B) val = {.M0PFE = 1, .M2PFE=1, .APC=3,
								 .RWSC=3, .WWSC =1, .DPFEN =1, .IPFEN = 1, .PFLIM =2,
								 .BFEN  = 1 };
#endif
	FLASH.PFCRP0.R = 0x00056B55;

	/* Enable pipelined reads again. */
#elif defined(CFG_MPC5554) || defined(CFG_MPC5567)
	//TODO: 

#elif defined(CFG_SPC56XL70)
    /*
     * Flash: 3 wait-states @120Mhz
     * SRAM : 1 wait-states @120Mhz
     */
    const FLASH_PFCR0_32B_tag val = {
            .B.B02_APC=3,
            .B.B02_WWSC=3,
            .B.B02_RWSC=3,
            .B.B02_P1_BCFG=3,
            .B.B02_P1_DPFE=0,
            .B.B02_P1_IPFE=1,
            .B.B02_P1_PFLM=2,
            .B.B02_P1_BFE=1,
            .B.B02_P0_BCFG=3,
            .B.B02_P0_DPFE=0,
            .B.B02_P0_IPFE=1,
            .B.B02_P0_PFLM=2,
            .B.B02_P0_BFE=1
    };

    FLASH.PFCR0.R = val.R;

    ECSM.MUDCR.B.MUDC30 = 1;       /* One wait-state for RAM@120Mhz */

    ECSM.ECR.B.EPRNCR = 1;

#elif defined(CFG_MPC5645S)
    /* 
     const typeof(CFLASH0.PFCR0.R) val0 = 0x30017B17;
     const typeof(CFLASH0.PFCR1.R) val1 = 0x007C7B43;
     CFLASH0.PFCR0.R = val0;
     CFLASH0.PFCR1.R = val1;
     */
    
    CFLASH0.PFCR0.R = 0x30017B17;
    CFLASH0.PFCR1.R = 0x007C7B43;
    
#elif defined(CFG_MPC560X)
	CFLASH1.PFCR0.R =  0x10840B6Fu; /* Instruction prefetch enabled and other according to cookbook */
#elif defined(CFG_MPC5634M)
    /* enable ECC flash error reporting */
    ECSM.ECR.B.EFNCR = 1;
#elif defined(CFG_MPC5642A)
#if 0
    const typeof(FLASH_A.BIUCR.B) val = {
                .M6PFE=0;     /* FlexRay  */
                .M4PFE=0;     /* eDMA     */
                .M1PFE=1;     /* z4 Core Load/Store  */
                .M0PFE=1;     /* z4 Core Instruction */
                .APC=4;
                .WWSC=3;
                .RWSC=4;
                .DPFEN=0;
                .IPFEN=1;
                .PFLIM=0b10;
                .BFEN=1;
            };
#endif
	/* BIUCR FLASH A */
    FLASH_A.BIUCR.R=0x39C15;

	/* ensure instruction prefetch stops here */
	asm("isync");
	/* msync machine code: ensure prior store completed */
	asm("msync");

#if 0
    const typeof(FLASH_B.BIUCR2.B) val = {
    	.LBCFG=3;
    };
#endif
	FLASH_A.BIUCR2.R=0xFFFFFFFF;

	/* ensure instruction prefetch stops here */
	asm("isync");
	/* msync machine code: ensure prior store completed */
	asm("msync");

#else
#error No flash setup for this MCU
#endif
}


/*******************************************************************************/
/***
 * @brief			:Mcu_Init,config the PLL for system. crystal:16MHz .Fsys:80MHZ
 *
 * Service ID		: <Mcu_Init>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>
 */
/*******************************************************************************/
void Mcu_Init(const Mcu_ConfigType* ConfigPtr)
{
	(void)ConfigPtr;
	
	Mcu_ConfigureFlash();
    return;
}

static void Mcu_Hw_InitClock(void)
{

	CGM.SC_DC[0].R = 0x80;
	CGM.SC_DC[1].R = 0x80;
	CGM.SC_DC[2].R = 0x80;
	CGM.SC_DC[3].R = 0X80;
	
    CGM.FMPLL[0].CR.R = FMPLL_IDF(MCU_FMPLL1) | FMPLL_ODF(MCU_FMPLL3) |
        		FMPLL_NDIV(MCU_FMPLL2) | FMPLL_EN_PLL_SW;

	/* At this stage we are in DRUN mode, running from FIRC (16Mhz) */

	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL0 */
	ME.RUN[0].R = 0x001F00F4;
	/* Peri. Cfg. 1 settings: only run in RUN0 mode */
	ME.RUNPC[1].R = 0x00000010;

	/* MPC56xxB/S: select ME.RUNPC[1] */
	/* SIUL control */
	ME.PCTL[68].R = 0x01;
	/* PIT_RTI control */
	ME.PCTL[92].R = 0x01;
	/* FlexCAN0 control */
	ME.PCTL[16].R = 0x01;
	/* FlexCAN1 control */
	ME.PCTL[17].R = 0x01;
	/* QuadSPI control */
	ME.PCTL[8].R = 0x01;

	/* Mode Transition to enter RUN0 mode: */
	/* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x40005AF0;
	/* Enter RUN0 Mode & Inverted Key */
	ME.MCTL.R = 0x4000A50F;

	/* Wait for mode transition to complete */
	while (ME.GS.B.S_MTRANS > 0)
	{
	}
	/* Verify RUN0 is the current mode */
	while(ME.GS.B.S_CURRENTMODE != 4)
	{
	}
}

/**
 *
 * @return
 */
void Mcu_InitClock( void )
{

	// TODO: find out if the 5554 really works like the 5516 here
    // All three (16, 54, 67) used to run the same code here though, so i'm sticking it with 5516
#if defined(CFG_MPC5516) || defined(CFG_MPC5554)  || defined(CFG_MPC5668)
    /* 5516clock info:
     * Fsys - System frequency ( CPU + all periperals? )
     *
     *  Fsys = EXTAL_FREQ *(  (emfd+16) / ( (eprediv+1) * ( erfd+1 )) ) )
     */
    // Check ranges...
//    ASSERT((MCU_FMPLL2>=32) && (MCU_FMPLL2<=132));
//    ASSERT( (MCU_FMPLL1 != 6) &&
//            (MCU_FMPLL1 != 8) &&
//            (MCU_FMPLL1 < 10) );
//    ASSERT( MCU_FMPLL3 & 1); // Must be odd
#elif defined(CFG_MPC5567)
    /* 5567 clock info:
     *  Fsys = EXTAL_FREQ *(  (emfd+4) / ( (eprediv+1) * ( 2^erfd )) ) )
     */
    // Check ranges...
//    ASSERT(MCU_FMPLL2 < 16);
//    ASSERT(MCU_FMPLL1 <= 4);
//    ASSERT(MCU_FMPLL3 < 8);
#endif

#if defined(USE_LDEBUG_PRINTF)
    {
    	uint32  extal = Mcu_Global.config->McuClockSettingConfig[Mcu_Global.clockSetting].McuClockReferencePointFrequency;
    	uint32  f_sys;

    	f_sys = CALC_SYSTEM_CLOCK( extal,
    		MCU_FMPLL2,
    		MCU_FMPLL1,
    		MCU_FMPLL3 );

        //DEBUG(DEBUG_HIGH,"/drivers/mcu: F_sys will be:%08d Hz\n",f_sys);
    }
#endif

#if defined(CFG_MPC5516)  || defined(CFG_MPC5668)
     // set post divider to next valid value to ensure that an overshoot during lock phase
     // won't result in a too high freq
     FMPLL.ESYNCR2.B.ERFD = (MCU_FMPLL3 + 1) | 1;

     // External crystal PLL mode.
     FMPLL.ESYNCR1.B.CLKCFG = 7; //TODO: Hur ställa detta för 5567?

     // Write pll parameters.
     FMPLL.ESYNCR1.B.EPREDIV = MCU_FMPLL1;
     FMPLL.ESYNCR1.B.EMFD    = MCU_FMPLL2;

     while(FMPLL.SYNSR.B.LOCK != 1) {};

     FMPLL.ESYNCR2.B.ERFD    = MCU_FMPLL3;
     // Connect SYSCLK to FMPLL
     SIU.SYSCLK.B.SYSCLKSEL = SYSCLOCK_SELECT_PLL;

#elif defined(CFG_MPC5604B) || defined(CFG_MPC5606B)

     // Write pll parameters.
//      CGM.FMPLL_CR.B.IDF = clockSettingsPtr->Pll1;
//      CGM.FMPLL_CR.B.NDIV = clockSettingsPtr->Pll2;
//      CGM.FMPLL_CR.B.ODF = clockSettingsPtr->Pll3;
      CGM.FMPLL_CR.R = FMPLL_IDF(MCU_FMPLL1) | FMPLL_ODF(MCU_FMPLL3) |
      		FMPLL_NDIV(MCU_FMPLL2) | MCU_FMPLL_EN_PLL_SW;

      /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL0 */
      ME.RUN[0].R = 0x001F0074;
      /* Peri. Cfg. 1 settings: only run in RUN0 mode */
      ME.RUNPC[1].R = 0x00000010;
      /* MPC56xxB/S: select ME.RUNPC[1] */
      ME.PCTL[68].R = 0x01; //SIUL control
      ME.PCTL[91].R = 0x01; //RTC/API control
      ME.PCTL[92].R = 0x01; //PIT_RTI control
      ME.PCTL[72].R = 0x01; //eMIOS0 control
      ME.PCTL[73].R = 0x01; //eMIOS1 control
      ME.PCTL[16].R = 0x01; //FlexCAN0 control
      ME.PCTL[17].R = 0x01; //FlexCAN1 control
      ME.PCTL[18].R = 0x01; //FlexCAN2 control
      ME.PCTL[4].R = 0x01;  /* MPC56xxB/P/S DSPI0  */
      ME.PCTL[5].R = 0x01;  /* MPC56xxB/P/S DSPI1:  */
      ME.PCTL[32].R = 0x01; //ADC0 control
  #if defined(CFG_MPC5606B)
      ME.PCTL[33].R = 0x01; //ADC1 control
  #endif
      ME.PCTL[23].R = 0x01; //DMAMUX control
      ME.PCTL[48].R = 0x01; /* MPC56xxB/P/S LINFlex  */
      ME.PCTL[49].R = 0x01; /* MPC56xxB/P/S LINFlex  */
      /* Mode Transition to enter RUN0 mode: */
      /* Enter RUN0 Mode & Key */
      ME.MCTL.R = 0x40005AF0;
      /* Enter RUN0 Mode & Inverted Key */
      ME.MCTL.R = 0x4000A50F;

      /* Wait for mode transition to complete */
      while (ME.GS.B.S_MTRANS) {}
      /* Verify RUN0 is the current mode */
      while(ME.GS.B.S_CURRENTMODE != 4) {}

      CGM.SC_DC[0].R = 0x80; /* MPC56xxB/S: Enable peri set 1 sysclk divided by 1 */
      CGM.SC_DC[1].R = 0x80; /* MPC56xxB/S: Enable peri set 2 sysclk divided by 1 */
      CGM.SC_DC[2].R = 0x80; /* MPC56xxB/S: Enable peri set 3 sysclk divided by 1 */

      SIU.PSMI[6].R = 0x01; /* CS0/DSPI_0 on PCR15 */


#elif defined(CFG_MPC5606S)

    /* This stuff is actually board specific but can be here for now
     *
     * HW: XTAL and EXTAL is connected to a 8Mhz osc. This "clock" is called FXOSC.
     * Other clock source that are used are:
     * - FIRC - Fast Internal RC (always 16Mhz)
     * - FMPLL0 (clocked by FXOSC)
     *
     * Don't care clocks for now are (FMPLL1 (emios, quadSPI, DCU))) and
     * SxOSC, Low speed RC.
     *
     * Note that it is the RUN configation (in ME) that actually selects the sys_clk.
     * (This should likely be PLL0 (FMPLL0).
     * */


    /* Write pll parameters,  See 8.9.5.1 for CR register
     * Note! You cannot check for lock here, must be done after setting runmode
     * with the pll enabled.
     */
    {
    CGM.FMPLL[0].CR.R = FMPLL_IDF(MCU_FMPLL1) | FMPLL_ODF(MCU_FMPLL3) |
    		FMPLL_NDIV(MCU_FMPLL2) | FMPLL_EN_PLL_SW;

    /* At this stage we are in DRUN mode, running from FIRC (16Mhz) */

    /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL0 */
    ME.RUN[0].R = 0x001F0074;
    /* Peri. Cfg. 1 settings: only run in RUN0 mode */
    ME.RUNPC[1].R = 0x00000010;

    /* MPC56xxB/S: select ME.RUNPC[1] */
    /* SIUL control */
    ME.PCTL[68].R = 0x01;
    /* PIT_RTI control */
    ME.PCTL[92].R = 0x01;
    /* FlexCAN0 control */
    ME.PCTL[16].R = 0x01;
    /* FlexCAN1 control */
    ME.PCTL[17].R = 0x01;
    /* QuadSPI control */
    ME.PCTL[10].R = 0x01;

	/* Mode Transition to enter RUN0 mode: */
	/* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x40005AF0;
	/* Enter RUN0 Mode & Inverted Key */
	ME.MCTL.R = 0x4000A50F;

	/* Wait for mode transition to complete */
	while (ME.GS.B.S_MTRANS > 0)
	{
	}
	/* Verify RUN0 is the current mode */
	while(ME.GS.B.S_CURRENTMODE != 4)
	{
	}

    }

#elif defined(CFG_MPC5554) || defined(CFG_MPC5567)
    // Partially following the steps in MPC5567 RM..
    FMPLL.SYNCR.B.DEPTH	= 0;	// 2.a
    FMPLL.SYNCR.B.LOLRE	= 0;	// 2.b
    FMPLL.SYNCR.B.LOLIRQ = 0;   // 2.c

    FMPLL.SYNCR.B.PREDIV 	= MCU_FMPLL1;      // 2.c
    FMPLL.SYNCR.B.MFD		= MCU_FMPLL2;      // 2.c
    FMPLL.SYNCR.B.RFD    	= MCU_FMPLL3 + 1 ; // 2.c

    while (Mcu_GetPllStatus() != MCU_PLL_LOCKED) ; 	// 3. Wait for PLL to sync.

    FMPLL.SYNCR.B.RFD    	= MCU_FMPLL3; // 4.

    FMPLL.SYNCR.B.LOLIRQ	= 1;		  // 5.

#elif defined(CFG_SPC56XL70)

    /* For some reason we are in SAFE mode from start
     * -> Need to go through DRUN to get to RUN 0
     * */
    uint32 runPC = ME_RUN_PC1;
    uint32 mode = MODE_DRUN;

    PLLD0.CR.B.NDIV = MCU_FMPLL2;
    PLLD0.CR.B.IDF  = MCU_FMPLL1;
    PLLD0.CR.B.ODF  = MCU_FMPLL3;

    /* Turn of things for RUN0 mode */
    ME.RUN0_MC.B.PDO=1;
    ME.RUN0_MC.B.MVRON=1;
    ME.RUN0_MC.B.FLAON=1;
    ME.RUN0_MC.B.XOSCON=1;
    ME.RUN0_MC.B.PLL0ON=1;
    ME.RUN0_MC.B.IRCOSCON=1;
    ME.RUN0_MC.B.SYSCLK=4;      /* System FMPLL as clock */

    /* Enable RUN0 mode in Peri config 1 */
    ME.RUN_PC1.R = 0x00000010;

    // setup clock source to xosc for PLL0
    CGM.AC3_SC.B.SELCTL = 1;

    /* Setup sources for aux clocks */
    CGM.AC0_SC.B.SELCTL = 4; // use system FMPLL as source for Aux clock 0 (motor control clock, SWG clock)
    // CGM.AC1_SC.B.SELCTL = 4; // use system FMPLL as source for Aux clock 1 (Flexray clock)
    CGM.AC2_SC.B.SELCTL = 4; // use system FMPLL as source for Aux clock 2 (FlexCAN clock)

    runPC = ME_RUN_PC1;
    mode = MODE_RUN0;

    ME.PCTL[92].R = runPC; //PIT_RTI control
    ME.PCTL[16].R = 0x01; //FlexCAN0 control
    ME.PCTL[17].R = 0x01; //FlexCAN1 control
    ME.PCTL[32].R = 0x01; //ADC0 control
    ME.PCTL[33].R = 0x01; //ADC1 control
    ME.PCTL[41].R = 0x01; //FlexPWM0 control
    ME.PCTL[48].R = 0x01; //LINFlex0 control
    ME.PCTL[49].R = 0x01; //LINFlex0 control
    switchMode(mode);

#elif defined(CFG_MPC5645S)
    Mcu_Hw_InitClock();
#elif defined(CFG_MPC5634M)

    FMPLL.ESYNCR1.B.CLKCFG = 0x7;    /* MPC563x: Change clk to PLL normal from crystal*/
    FMPLL.SYNCR.R = 0x08100000;           /* 16 MHz xtal: 0x08100000;  */
    while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */

#endif
}


/*******************************************************************************/
/***
 * @brief			:Mcu_Deinit,config the Fmpll registers to power on default value.  
 * 
 * Service ID		: <Mcu_Deinit>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Mcu_Deinit(void)
{


}

void MCU_ModeEnterDRUN(void)
{
	ME.MCTL.R = 0x30005AF0;                  /* Enter DRUN mode and key */
    ME.MCTL.R = 0x3000A50F;                  /* Enter DRUN mode and inverted key */
    while (ME.GS.B.S_MTRANS) {}              /* Check DRUN mode has been entered  */ 
    while(0x3 != ME.GS.B.S_CURRENTMODE){};   /* Check DRUN mode has been entered */
}

/**
 * Reads the ECC erros and clears it
 * @param err
 */
void Mcu_Arc_GetECCError( uint8 *err )
{

    *err = EccError;
	/* Clear stored  */
    EccError = 0u;
}

