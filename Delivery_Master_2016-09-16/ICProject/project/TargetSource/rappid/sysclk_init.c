/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : detc_ecu
 *
 * Project File           : detc_ecu.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : sysclk_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 04-Sep-2014 15:25:01
 *
 * Created on Date        : 04-Sep-2014 15:25:02
 *
 * Brief Description      : Mode Entry, System Clock, CMU initialization file.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains definations for initialization functions
 *                          of Mode Entry, System Clock and CMU module. Mode Entry function
 *                          will configure the system SoC for different operating modes.
 *                          In System Clock function the Clock Generation Module parameters
 *                          are initialized for DRUN mode. CMU function is initialized to
 *                          monitor the system clock parameters and report the failures.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "sysclk_init.h"


/* *********************************************************** */
/*       Global functions            */
/* *********************************************************** */


void sysclk_module_init_fnc(void)
{
    mode_entry_init_fnc();
        /* Mode Entry Initialization Function */
    sysclk_init_fnc();
        /* System Clock Initialization Function */
    cmu_init_fnc();
        /* CMU Initialization Function */
}


void mode_entry_init_fnc(void)
{

/* ----------------------------------------------------------- */
/*              Operating Modes Initialization - Start            */
/* ----------------------------------------------------------- */


/* ----------------------------------------------------------- */
/*              Mode Entry Setup            */
/* ----------------------------------------------------------- */

    ME.MER.R = 0x0000043D;
        /* STANDBY0 Mode : Disabled    */
        /* STOP0 Mode : Disabled    */
        /* HALT0 Mode : Disabled    */
        /* RUN3 Mode : Disabled    */
        /* RUN2 Mode : Disabled    */
        /* RUN1 Mode : Disabled    */
        /* RUN0 Mode : Enabled    */
        /* DRUN Mode : Enabled    */
        /* SAFE Mode : Enabled    */
        /* TEST Mode : Disabled    */
        /* RESET Mode : Enabled    */

/*---------------------------TEST Mode--------------------------------*/
    ME.TEST.R = 0x001F0010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------SAFE Mode--------------------------------*/
    ME.SAFE.R  = 0x009F0010;
        /* I/O Power Down Control: Enabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------RUN0 Mode--------------------------------*/
    ME.RUN[0].R = 0x001F00F4;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): ON    */
        /* System PLL (PLL0): ON    */
        /* Crystal Oscillator: ON    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: System PLL (PLL0)    */

/*---------------------------RUN1 Mode--------------------------------*/
    ME.RUN[1].R = 0x001F00F4;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): ON    */
        /* System PLL (PLL0): ON    */
        /* Crystal Oscillator: ON    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: System PLL(PLL0)     */

/*---------------------------RUN2 Mode--------------------------------*/
    ME.RUN[2].R = 0x001F0010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------RUN3 Mode--------------------------------*/
    ME.RUN[3].R = 0x001F0010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------HALT0 Mode--------------------------------*/
    ME.HALT0.R = 0x001A0010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Low Power Mode    */
        /* Code Flash Power Down Control: Low Power Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------STOP0 Mode--------------------------------*/
    ME.STOP0.R = 0x00150010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Power Down Mode    */
        /* Code Flash Power Down Control: Power Down Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */

/*---------------------------STANDBY0 Mode--------------------------------*/
    ME.STANDBY0.R = 0x0085001F;
        /* I/O Power Down Control: Enabled    */
        /* Main Voltage Regulator Control: OFF    */
        /* Data Flash Power Down Control: Power Down Mode    */
        /* Code Flash Power Down Control: Power Down Mode    */
        /* secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: Disable System Clock    */

/* ----------------------------------------------------------- */
/*              Peripheral RUN Mode Configuration            */
/* ----------------------------------------------------------- */

    ME.RUNPC[0].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Enabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[1].R = 0x00000018;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Enabled    */
        /* Peripheral Operation in DRUN  mode: Enabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[2].R = 0x00000028;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[3].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[4].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[5].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[6].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[7].R = 0x00000008;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */


/* ----------------------------------------------------------- */
/*              Peripheral Low-Power Mode Configuration            */
/* ----------------------------------------------------------- */

    ME.LPPC[0].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[1].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[2].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[3].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[4].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[5].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[6].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */

    ME.LPPC[7].R = 0x00000000;
        /* Peripheral Operation in STTANDBY0 mode: Disabled    */
        /* Peripheral Operation in STOP0 mode: Disabled    */
        /* Peripheral Operation in HALT0 mode: Disabled    */


/* ----------------------------------------------------------- */
/*              Peripheral Mode Control Setup            */
/* ----------------------------------------------------------- */

    /* DSPI0 Control */
    ME.PCTL[4].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* DSPI1 Control */
    ME.PCTL[5].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* QUADSPI Control */
    ME.PCTL[10].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* FlexCAN0 Control */
    ME.PCTL[16].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* FlexCAN1 Control */
    ME.PCTL[17].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* ADC0 Control */
    ME.PCTL[32].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* I2C_DMA0 Control */
    ME.PCTL[44].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* I2C_DMA1 Control */
    ME.PCTL[45].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* I2C_DMA2 Control */
    ME.PCTL[46].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* I2C_DMA3 Control */
    ME.PCTL[47].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* LIN_FLEX0 Control */
    ME.PCTL[48].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* LIN_FLEX1 Control */
    ME.PCTL[49].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* GaugeDriver Control */
    ME.PCTL[56].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* CANSampler Control */
    ME.PCTL[60].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* LCD Control */
    ME.PCTL[61].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* SGL Control */
    ME.PCTL[62].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* DCU Control */
    ME.PCTL[63].R = 0x00;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* SIUL Control */
    ME.PCTL[68].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 1    */

    /* eMIOS0 Control */
    ME.PCTL[72].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* eMIOS1 Control */
    ME.PCTL[73].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* RTC/API Control */
    ME.PCTL[91].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 0    */

    /* PIT_RTI Control */
    ME.PCTL[92].R = 0x01;
        /* Peripheral Operation in Debug mode: Disabled    */
        /* Low-Power Mode Peripheral Configuration: Low-Power Mode Configuration 0    */
        /* RUN Mode Peripheral Configuration: RUN Mode Configuration 1    */


/* ----------------------------------------------------------- */
/*              Interrupt Setup            */
/* ----------------------------------------------------------- */

    ME.IM.R = 0x00000000;
        /* Invalid Mode Configuration Interrupt: Disabled    */
        /* Invalid Mode Interrupt: Disabled    */
        /* SAFE Mode Interrupt: Disabled    */
        /* Mode Transition Complete Interrupt: Disabled    */

/* ----------------------------------------------------------- */
/*              Operating Modes Initialization - End                */
/* ----------------------------------------------------------- */

}

void sysclk_init_fnc(void)
{

/* ----------------------------------------------------------- */
/*             Enable All Peripheral Clocks           */
/* ----------------------------------------------------------- */
    CGM.SCDC.R = 0x80808000;
            /* Peripheral Set1 Clock Divider Status : Enabled*/
            /* Peripheral Set2 Clock Divider Status : Enabled*/
            /* Peripheral Set3 Clock Divider Status : Enabled*/

    CGM.AC1DC.R = 0x80000000;
            /* eMIOS_0 (Aux Clock 1 Divider): Enabled */

    CGM.AC2DC.R = 0x80000000;
            /* eMIOS_1 (Aux Clock 2 Divider): Enabled */


/* ----------------------------------------------------------- */
/*              Clock Source Configuration            */
/* ----------------------------------------------------------- */

    CGM.RC_CTL.R = 0x00000000;
        /* IRC Oscillator Trimming: 0x0    */
        /* The 16MHz IRC Clock Divider: 1    */

    CGM.OSC_CTL.R = 0x00800000;
        /* Oscillator Output is Enabled as rootclock    */
        /* The End of Count Value for XOSC = 0x80    */
        /* The XOSC Clock Interrupt: Disabled    */
        /* The XOSC Clock Divider: 1    */

    CGM.FMPLL[0].CR.R = 0x05400000;
        /* Input Division Factor: 2    */
        /* Output Division Factor: 4    */
        /* Loop Division Factor: 64    */
        /* Progressive Clock Switching Disabled in Standby Mode    */

    CGM.FMPLL[0].MR.R = 0x00000000;
        /* The selected Spread Type: Center Spread    */
        /* The Modulation Period for FMPLL: 0    */
        /* Frequency modulation for PLL: Disabled    */
        /* The Increment Step for FMPLL: 0    */

    CGM.FMPLL[1].CR.R = 0x05400000;
        /* Input Division Factor: 2    */
        /* Output Division Factor: 4    */
        /* Loop Division Factor: 64    */
        /* Progressive Clock Switching Disabled in Standby Mode    */

    CGM.FMPLL[1].MR.R = 0x00000000;
        /* The selected Spread Type: Center Spread    */
        /* The Modulation Period for FMPLL: 0    */
        /* Frequency modulation for PLL: Disabled    */
        /* The Increment Step for FMPLL: 0    */



/* ----------------------------------------------------------- */
/*                    System Clock Setup (16MHz IRC)                 */
/* ----------------------------------------------------------- */

    ME.DRUN.R = 0x001F00F0;
        /* I/O Power Down Control :Disabled*/
        /* Main Voltage Regulator Control:  OFF */
        /* Data Flash Power Down Control: Normal */
        /* Code Flash Power Down Control: Normal */
        /* Secondary PLL (PLL1) : ON */
        /* System PLL (PLL0) : ON */
        /* Crystal Oscillator: ON */
        /* 16MHz IRC: ON */
        /* System Clock Source:16MHz internal RC oscillator     */

        /* Re-enter in DRUN mode to update the configuration */
    ME.MCTL.R = 0x30005AF0;
        /* Mode & Key */
    ME.MCTL.R = 0x3000A50F;
        /* Mode & Key  Inverted  */
    while(0x1 == ME.GS.B.S_MTRANS){}; 
        /* Wait for mode entry to complete */
    while(0x3 != ME.GS.B.S_CURRENTMODE){}; 
        /* Check DRUN mode has been entered */

        /* Poll Global Status Register to get current System Clock */
    while(0x0!= ME.GS.B.S_SYSCLK){}; 
        /* Fail is stuck here */

/* ----------------------------------------------------------- */
/*          Activate the clock Configuration              */
/* -----------------------------------------------------------  */

    ME.MCTL.R = ((ME.GS.B.S_CURRENTMODE << 28) | 0x00005AF0);    
    ME.MCTL.R = ((ME.GS.B.S_CURRENTMODE << 28) | 0x0000A50F);    

/* ----------------------------------------------------------- */
/*                    System Output Clock Setup                 */
/* ----------------------------------------------------------- */

    CGM.OCDSSC.R = 0x00000000;    
        /* Clock Divider: 1 */
        /* Output Clock Source: 16MHz internal RC oscillator  */
    CGM.OCEN.R = 0x00000000;    
        /* Output Clock: Disabled */


/* ----------------------------------------------------------- */
/*                    Low Power Clock Setup                 */
/* ----------------------------------------------------------- */
/* ------------------------LPOSC 32KHz Configuration------------------- */
    CGM.LPOSC_CTL.R = 0x00800001;    
        /* LP Oscillator Output is Enabled as rootclock*/
        /* The End of Count Value for LPOSC = 0x80 */
        /* The LPOSC Clock Interrupt: Disabled */
        /* The LPOSC Clock Divider: 1 */
        /* Crystal Oscillator Powerdown Control: Disabled*/


/* ------------------------128KHz Slow IRC Configuration------------------- */
    CGM.LPRC_CTL.R = 0x00000300;    
        /* LPRC Oscillator Trimming: 0x0 */
        /* The 128KHz RC Clock Divider = 4 */
        /* LPRC Oscillator is switched ON in Standby Mode */

/* ----------------------------------------------------------- */
/*          Peripheral & Auxiliary Clock Setup                 */
/* -----------------------------------------------------------  */

/* -------------------- Peripheral Clock -----------------  */

    CGM.SCDC.R = 0x80808000;
        /*Peripheral Set 1 Clock Divider Value: 1*/
        /*Peripheral Set 2 Clock Divider Value: 1*/
        /*Peripheral Set 3 Clock Divider Value: 1*/


/* -------------------Auxiliary Clock 0------------------------  */

    CGM.AC0SC.R = 0x00000000;
        /*DCU Clock Source (Auxiliary Clock 0): Crystal Oscillator*/

/* -------------------Auxiliary Clock 1------------------------  */

    CGM.AC1SC.R = 0x03000000; 
        /* eMIOS_0 Clock Source: System PLL (PLL0)    */

    CGM.AC1DC.R = 0x80000000;
        /* eMIOS_0 Clock Divider Value: 1    */

/* -------------------Auxiliary Clock 2------------------------ */

    CGM.AC2SC.R = 0x00000000;
        /* eMIOS_1 Clock Source: Crystal Oscillator Divided    */

    CGM.AC2DC.R = 0x80000000;
        /* eMIOS_1 Clock Divider Value: 1    */

/* -------------------Auxiliary Clock 3------------------------ */

    CGM.AC3SC.R = 0x02000000;
        /* Quad SPI Clock Source: Secondary PLL (PLL1)    */

}

void cmu_init_fnc(void)
{

/*-----------------------CMU 0 Setup------------------------*/

    CGM.CMU_0_MDR.R = 0x00000000;
        /* IRC Measurement Duration in IRC Clock Cycles: 0*/

    CGM.CMU_0_HFREFR_A.R = 0x00000FFF;
        /* High Frequency Reference Value for FMPLL0: 1023.75 MHz*/

    CGM.CMU_0_LFREFR_A.R = 0x00000000;
        /* Low Frequency Reference Value for FMPLL0: 0.0 MHz*/

    CGM.CMU_0_CSR.R = 0x00000006;
        /* RC Oscillator to be calibrated by Frequency Meter: 16MHz IRC*/
        /* RC Clock Division factor: 8 */
        /* Clock Monitoring Unit 0: Disable */
}

void mode_entry_post_config_fnc(void)
{
/* ----------------------------------------------------------- */
/*              Peripheral RUN Mode Configuration            */
/* ----------------------------------------------------------- */

    ME.RUNPC[0].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[1].R = 0x00000018;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Enabled    */
        /* Peripheral Operation in DRUN  mode: Enabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[2].R = 0x00000028;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Enabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Enabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[3].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */


    ME.RUNPC[4].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[5].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[6].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */

    ME.RUNPC[7].R = 0x00000000;
        /* Peripheral Operation in RUN3  mode: Disabled    */
        /* Peripheral Operation in RUN2  mode: Disabled    */
        /* Peripheral Operation in RUN1  mode: Disabled    */
        /* Peripheral Operation in RUN0  mode: Disabled    */
        /* Peripheral Operation in DRUN  mode: Disabled    */
        /* Peripheral Operation in SAFE  mode: Disabled    */
        /* Peripheral Operation in TEST  mode: Disabled    */
        /* Peripheral Operation in RESET  mode: Disabled    */


/* ----------------------------------------------------------- */
/*                     DRUN Re-Entry                    */
/* ----------------------------------------------------------- */

        /* Re-enter in DRUN mode to update the configuration */
    ME.MCTL.R = 0x30005AF0;    
        /* Mode & Key */
    ME.MCTL.R = 0x3000A50F;    
        /* Mode & Key  Inverted */
    while(0x1 == ME.GS.B.S_MTRANS){};    
        /* Wait for mode entry to complete */
    while(0x3 != ME.GS.B.S_CURRENTMODE){};    
        /* Check DRUN mode has been entered  */

/* ----------------------------------------------------------- */
/*      DRUN  Clock & Auxiliary Clock Divider Settings    */
/* ----------------------------------------------------------- */
/*  Section is put after the DRUN Re-Entry as a workaround 
        for XPC560S Errata sheet issue number e426PS   */

    ME.DRUN.R = 0x001F0010;
        /* I/O Power Down Control: Disabled    */
        /* Main Voltage Regulator Control: ON    */
        /* Data Flash Power Down Control: Normal Mode    */
        /* Code Flash Power Down Control: Normal Mode    */
        /* Secondary PLL (PLL1): OFF    */
        /* System PLL (PLL0): OFF    */
        /* Crystal Oscillator: OFF    */
        /* 16MHz IRC:  ON    */
        /* System Clock Source: 16MHz internal RC oscillator     */


    CGM.SCDC.R = 0x80808000;
        /* Peripheral Set 1 Clock Divider Status: Enabled */
        /* Peripheral Set 2 Clock Divider Status: Enabled */
        /* Peripheral Set 3 Clock Divider Status: Enabled */

    CGM.AC1DC.R = 0x80000000;
        /* eMIOS_0 (Aux Clock 1 Divider):  Enabled */

    CGM.AC2DC.R = 0x80000000;
        /* eMIOS_1 (Aux Clock 2 Divider): Enabled */

        /* Re-enter in DRUN mode to update the configuration */
    ME.MCTL.R = 0x30005AF0;    
        /* Mode & Key */
    ME.MCTL.R = 0x3000A50F;    
        /* Mode & Key  Inverted */
    while(0x1 == ME.GS.B.S_MTRANS){};    
        /* Wait for mode entry to complete */
    while(0x3 != ME.GS.B.S_CURRENTMODE){};    
        /* Check DRUN mode has been entered  */

}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

