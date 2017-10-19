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
 * file                   : smc_init.c
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
 * Brief Description      : This file contains initializations for SMC
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains initializations for SMC Control registers,
 *                          PWM Period, SMC Channel configuration and Short-Circuit Detector 
 *                          Configuration.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "smc_init.h"



void smc_init_fnc(void)
{
/*--------------------------------------------------------------*/
/*          SMC Controller Configuration                                    */
/*--------------------------------------------------------------*/
    SMC.CTL0.R = 0x60;  //coefficient
        /* Motor Controller Prescaler :8*/
        /* Motor Controller Halt Mode : Disabled*/
        /* Motor Controller Dither Feature : Disabled*/

    SMC.CTL1.R = 0x80;
        /* Recirculation in (Dual) Full H-Bridge Mode: Enabled*/
        /* Motor Controller Timer Counter Overflow Interrupt: Disabled*/

    SMC.PER.R = 0x0100;  //run in the frequency 31.25k
        /* Motor Controller Period: 0x0100*/

    
/*--------------------------------------------------------------*/
/*          SMC Controller Configuration                        */
/*--------------------------------------------------------------*/

/*---------------------- SMC Channel 0 ------------------------*/

    SMC.CC[0].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[0].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 1 ------------------------*/

    SMC.CC[1].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[1].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 2 ------------------------*/

    SMC.CC[2].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[2].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 3 ------------------------*/

    SMC.CC[3].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[3].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 4 ------------------------*/

    SMC.CC[4].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[4].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 5 ------------------------*/

    SMC.CC[5].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[5].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 6 ------------------------*/

    SMC.CC[6].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[6].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 7 ------------------------*/

    SMC.CC[7].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[7].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 8 ------------------------*/

    SMC.CC[8].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[8].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 9 ------------------------*/

    SMC.CC[9].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; PWM on MnCxM; MnCxP is released*/
        /* Channel PWM Alignment Mode: Channel disabled*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[9].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 10 ------------------------*/

    SMC.CC[10].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; */
        /* Channel PWM Alignment Mode: Left aligned*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[10].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/

/*---------------------- SMC Channel 11 ------------------------*/

    SMC.CC[11].R = 0xD0;
        /* Channel Output Mode: Half H-bridge mode; */
        /* Channel PWM Alignment Mode: Left aligned*/
        /* Channel Delay in PWM Clocks: 0*/

    SMC.DC[11].R = 0x0000;
        /* For Dual Full H-Bridge mode, Sign bit: Disabled*/
        /* Duty Cycle Counts for the Channel:0*/




/*--------------------------------------------------------------*/
/*          Short-Circuit Detector Configuration                             */
/*--------------------------------------------------------------*/

/*--- Short Circuit Detector Re-start for Low-Power Modes ---*/
    SMC.SDE[0].R = 0x00;
    SMC.SDE[1].R = 0x00;
    SMC.SDE[2].R = 0x00;

    SMC.SDTO.R = 0x02;
        /* Short-circuit Timeout count: 2*/

    SMC.SDE[0].R = 0x00;        
        /* Short Circuit Detector 7(M1COP): Disabled*/
        /* Short Circuit Detector 6(M0COP): Disabled*/
        /* Short Circuit Detector 5(M5C1P): Disabled*/
        /* Short Circuit Detector 4(M4C1P): Disabled*/
        /* Short Circuit Detector 3(M3C1P): Disabled*/
        /* Short Circuit Detector 2(M2C1P): Disabled*/
        /* Short Circuit Detector 1(M1C1P): Disabled*/
        /* Short Circuit Detector 0(M0C1P): Disabled*/

    SMC.SDE[1].R = 0x00;
        /* Short Circuit Detector 15(M3C1M): Disabled*/
        /* Short Circuit Detector 14(M2C1M): Disabled*/
        /* Short Circuit Detector 13(M1C1M): Disabled*/
        /* Short Circuit Detector 12(M0C1M): Disabled*/
        /* Short Circuit Detector 11(M5C0P): Disabled*/
        /* Short Circuit Detector 10(M4C0P): Disabled*/
        /* Short Circuit Detector 9(M3C0P): Disabled*/
        /* Short Circuit Detector 8(M2C0P): Disabled*/

    SMC.SDE[2].R = 0x00;
        /* Short Circuit Detector 23(M5COM):Disabled*/
        /* Short Circuit Detector 22(M4COM):Disabled*/
        /* Short Circuit Detector 21(M3C0M):Disabled*/
        /* Short Circuit Detector 20(M2C0M):Disabled*/
        /* Short Circuit Detector 19(M1C0M):Disabled*/
        /* Short Circuit Detector 18(M0C0M):Disabled*/
        /* Short Circuit Detector 17(M5C1M):Disabled*/
        /* Short Circuit Detector 16(M4C1M):Disabled*/

    SMC.SDIEN[0].R = 0x00;
        /* Short Circuit Detector Interrupt 7(M1COP): Disabled*/
        /* Short Circuit Detector Interrupt 6(M0COP): Disabled*/
        /* Short Circuit Detector Interrupt 5(M5C1P): Disabled*/
        /* Short Circuit Detector Interrupt 4(M4C1P): Disabled*/
        /* Short Circuit Detector Interrupt 3(M3C1P): Disabled*/
        /* Short Circuit Detector Interrupt 2(M2C1P): Disabled*/
        /* Short Circuit Detector Interrupt 1(M1C1P): Disabled*/
        /* Short Circuit Detector Interrupt 0(M0C1P): Disabled*/

    SMC.SDIEN[1].R = 0x00;
        /* Short Circuit Detector Interrupt 15(M3C1M): Disabled*/
        /* Short Circuit Detector Interrupt 14(M2C1M): Disabled*/
        /* Short Circuit Detector Interrupt 13(M1C1M): Disabled*/
        /* Short Circuit Detector Interrupt 12(M0C1M): Disabled*/
        /* Short Circuit Detector Interrupt 11(M5C0P): Disabled*/
        /* Short Circuit Detector Interrupt 10(M4C0P): Disabled*/
        /* Short Circuit Detector Interrupt 9(M3C0P): Disabled*/
        /* Short Circuit Detector Interrupt 8(M2C0P): Disabled*/

    SMC.SDIEN[2].R = 0x00;
        /* Short Circuit Detector Interrupt 23(M5COM): Disabled*/
        /* Short Circuit Detector Interrupt 22(M4COM): Disabled*/
        /* Short Circuit Detector Interrupt 21(M3C0M): Disabled*/
        /* Short Circuit Detector Interrupt 20(M2C0M): Disabled*/
        /* Short Circuit Detector Interrupt 19(M1C0M): Disabled*/
        /* Short Circuit Detector Interrupt 18(M0C0M): Disabled*/
        /* Short Circuit Detector Interrupt 17(M5C1M): Disabled*/
        /* Short Circuit Detector Interrupt 16(M4C1M): Disabled*/
}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

