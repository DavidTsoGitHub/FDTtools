/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : RappID_New_Prj_RTC_0619
 *
 * Project File           : RappID_New_Prj_RTC_0619.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : rtc_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 23-Jun-2015 08:33:33
 *
 * Created on Date        : 23-Jun-2015 08:33:35
 *
 * Brief Description      : RTC/API Initialization file
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains Real Time Clock (RTC) & Automatic Periodic
 *                          Interrupt (API ) initialization function. The function configures
 *                          parameters such RTC/API Module status, RTC/API Interrupt
 *                          status, RTC/API compare values and RTC Clock source.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rtc_init.h"




/********************* Initialization Function(s) *********************** */


void rtc_init_fnc(void)
{
    RTC.RTCC.B.CNTEN = 0;    
        /* Disable Real Timer Counter Module for initialization        */

    RTC.RTCC.R = 0x0001C7FF;    
        /* RTC Interrupt: Disabled        */
        /* Counter Rollover Interrupt : Disabled        */
        /* API Module : Enabled        */
        /* API Interrupt Request to the System : Enabled        */
        /* RTC's Clock is selected to 32 kHz OSC Divided        */
        /* RTC's Clock divide by 512: Disabled        */
        /* RTC's Clock divide by 32: Enabled        */
        /* RTC Compare Value is 1        */
        /* API Compare Value is 1023        */

    RTC.RTCC.B.CNTEN = 0;    
        /* Real Timer Counter Module : Enabled        */

}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

