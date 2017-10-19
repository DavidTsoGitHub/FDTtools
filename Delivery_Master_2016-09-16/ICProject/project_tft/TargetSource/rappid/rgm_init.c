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
 * file                   : rgm_init.c
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
 * Brief Description      : This file contains MC_RGM module initialization.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This File contains function definitions to initialize
 *                          RGM module. The module provides the register interface to
 *                          monitor and control the device reset sequence.
 *                          The configuration function will setup the RGM to enable/disable
 *                          different reset sources, generate interrupt/SAFE mode request 
 *                          and also control the reset state machine.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rgm_init.h"
#include "rappid_ref.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Constants and macros
******************************************************************************/


/******************************************************************************
* Local types
******************************************************************************/


/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/


/******************************************************************************
* Local functions
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/




/*********************  Initialization Function(s) ************************/

void rgm_init_fnc (void)
{

    RGM.FES.R = 0x81FF;
    RGM.DES.R = 0x800F;
        /* Clear all reset flags */

/*----------------------------------------------------------------*/
/*  MC_RGM Functional Event Alternate Request Register (RGM_FEAR) */
/*----------------------------------------------------------------*/
    RGM.FEAR.R = 0x0000;
        /* 4.5V Low_Voltage Detected: SAFE Mode request to ME */
        /* CMU0 Clock Frequency Higher/Lower Than Reference: SAFE Mode request to ME */
        /* Oscillator Frequency Lower Than Reference: SAFE Mode request to ME */
        /* PLL0 Fail: SAFE Mode request to ME */
        /* Core Reset: SAFE Mode request to ME */
        /* JTAG Initiated Reset: SAFE Mode request to ME */

/*----------------------------------------------------------------*/
/*  MC_RGM Functional Event Short Sequence Register (RGM_FESS)   */
/*----------------------------------------------------------------*/
    RGM.FESS.R = 0x0000;
        /* 4.5V Low_Voltage Detected : Start reset sequence from Phase1*/
        /* CMU0 Clock Frequency Higher/Lower Than Reference : Start reset sequence from Phase1*/
        /* Oscillator Frequency Lower Than Reference : Start reset sequence from Phase1*/
        /* PLL0 Fail : Start reset sequence from Phase1*/
        /* Core Reset : Start reset sequence from Phase1*/
        /* JTAG Initiated Reset : Start reset sequence from Phase1*/

/*----------------------------------------------------------------*/
/*  MC_RGM STANDBY0 Reset Sequence Register (RGM_STDBY)            */
/*----------------------------------------------------------------*/
    RGM.STDBY.R = 0x0000;
        /* System Boot Control :Boot from flash on STANDBY0 exit*/

/*----------------------------------------------------------------*/
/*  MC_RGM Functional Bidirectional Reset Enable Register (RGM_FBRE)   */
/*----------------------------------------------------------------*/
    RGM.FBRE.R = 0x0005;
        /* 4.5V Low_Voltage Detected: External Reset asserted*/
        /* CMU0 Clock Frequency Higher/Lower Than Reference: External Reset asserted*/
        /* Oscillator Frequency Lower Than Reference: External Reset asserted*/
        /* PLL0 Fail: External Reset asserted*/
        /* Core Reset: External Reset asserted*/
        /* JTAG Initiated Reset: External Reset Not asserted*/

/*----------------------------------------------------------------*/
/*  MC_RGM Functional Event Reset Disable Register (RGM_FERD)   */
/*  MC_RGM Destructive Event Reset Disable Register (RGM_DERD)  */

/*  Note:As per Errata, both RGM_FERD/RGM_DERD  */
/*  registers are being written at the same time using word access  */
/*----------------------------------------------------------------*/
    *(vuint32_t*)&RGM.FERD.R = 0x00000000;
        /* 4.5V Low_Voltage Detected: Enable reset sequence */
        /* CMU0 Clock Frequency Higher/Lower Than Reference: Enable reset sequence */
        /* Oscillator Frequency Lower Than Reference: Enable reset sequence */
        /* PLL0 Fail: Enable reset sequence */
        /* Core Reset: Enable reset sequence */
        /* JTAG Initiated Reset: Enable reset sequence */


}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

