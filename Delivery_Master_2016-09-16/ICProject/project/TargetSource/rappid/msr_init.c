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
 * file                   : msr_init.c
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
 * Brief Description      : This file contains configuration for machine state register and Hardware Implementation dependent register. 
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains function which configures machine state
 *                          register which is used to define state of the processor.It also
 *                          configures hardware implementation dependent register which is
 *                          used for various configuration and control functions.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "msr_init.h"

/*********************  Initialization Function(s) ************************/

asm void msr_init_fnc (void)
{


/* ----------------------------------------------------------- */

/*   Machine State Setup                     */

/* ----------------------------------------------------------- */


/* ----------------------------------------------------------- */
/*	                  MSR Setup                                */
/* ----------------------------------------------------------- */
   e_lis r12, 0x0000
         /* Load upper 16 bits of register value               */
         /* Execution of SPE APU vector instructions : Disabled   */
         /* Critical Interrupt : Disabled                      */
         /* Wait State : Disabled                              */
   e_or2i r12,0x0000
         /* Load lower 16 bits of register value               */
         /* External Interrupt : Disabled                      */
         /* Problem State : Disabled                           */
         /* FP available : Disabled                            */
         /* Machine Check : Disabled                           */
         /* Debug Interrupt : Disabled                         */
         /* Instruction Address Space : Disabled               */
         /* Data Address Space : Disabled                      */
         /* Recoverable Interrupt : Disabled                   */
   mtmsr r12
         /* Move data to MSR register                          */

/* ----------------------------------------------------------- */
/*	                  HID0 Setup                               */
/* ----------------------------------------------------------- */
   e_lis r12, 0x0000
         /* Load upper 16 bits of register value               */
   e_or2i r12,0x0000
         /* Load lower 16 bits of register value               */
   mtspr 1008,r12
         /* Move data to HID0 register                         */
         /* Debug Interrupt Clears (DCLREE) : Disabled         */
         /* Debug Interrupt Clears (DCLRCE) : Disabled         */
         /* Critical Interrupt Clears : Disabled               */
         /* Machine Check Interrupt Clears : Disabled          */
         /* Debug APU : Disabled                               */
         /* Interrupt Inputs Clear Reservation : Disabled      */
         /* Machine Check Exception : Disabled          */



}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

