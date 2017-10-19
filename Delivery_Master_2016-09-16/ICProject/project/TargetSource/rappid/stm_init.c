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
 * file                   : stm_init.c
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
 * Brief Description      : STM Initialization file
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains definition for System Timer Module (STM)
 *                          initialization function.The function configures STM parameters
 *                          such as STM module status, Counter pre-scalar, Individual
 *                          Channel status & compare value.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "stm_init.h"




/********************* Initialization Function(s) *********************** */


void stm_init_fnc(void)
{

    STM.CR0.B.TEN = 0x0;    
        /* Disable System Timer Module for initialization          */

/* ----------------------------------------------------------- */
/*                     Configure Counter Prescaler                */
/* ----------------------------------------------------------- */

    STM.CR0.B.CPS = 7;    
        /* Counter Prescaler is 1        */
	
    STM.CMP0.R =80000;    //1ms
        /* System Timer Channel 0's Compare value is :     */
    
   STM.CCR0.B.CEN =0x1;    
        /* System Timer Channel 0: Disabled    */
    
    STM.CMP1.R =800000;   // 10ms
    
        /* System Timer Channel 1's Compare value is : 0    */
    
   STM.CCR1.B.CEN =0x1;    
        /* System Timer Channel 1: Disabled    */
    
 //   STM.CMP2.R =0x00000000;    
        /* System Timer Channel 2's Compare value is : 0    */
    
 //   STM.CCR2.B.CEN =0x0;    
        /* System Timer Channel 2: Disabled    */
    
 ///   STM.CMP3.R =0x00000000;    
        /* System Timer Channel 3's Compare value is : 0    */
    
 //   STM.CCR3.B.CEN =0x0;    
        /* System Timer Channel 3: Disabled    */
    


    STM.CR0.B.TEN = 1;    
        /* System Timer Module is:  Disabled        */

}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

