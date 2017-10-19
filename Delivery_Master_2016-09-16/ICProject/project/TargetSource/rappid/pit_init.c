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
 * file                   : pit_init.c
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
 * Brief Description      : PIT Initialization file
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains Periodic Interrupt Timer (PIT) parameter initialization
 *                          function.Along with PIT module, the function configures parameters
 *                          such as individual channel status, interrupt status & timer load value.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "pit_init.h"




/********************* Initialization Function(s) *********************** */


void pit_init_fnc(void)
{
	
	   PIT.PITMCR.B.MDIS   = 1;    
        /*Disable PIT for initialization         */

   /* ----------------------------------------------------------- */
   /*                     Configure Load Value Registers                */
   /* ----------------------------------------------------------- */

       PIT.CH[0].LDVAL.R  = 64000;//0x0061A800;    
           /*Value loaded in the Timer0: 6400000    0.1s*/
           
       PIT.CH[1].LDVAL.R  = 64000;//0x0061A8000;    
           /*Value loaded in the Timer0: 6400000    */
       
       PIT.CH[2].LDVAL.R  = 64000;//0x0061A8000;    
              /*Value loaded in the Timer0: 6400000    */
       
       PIT.CH[3].LDVAL.R  = 64000;//0x0061A8000;    
              /*Value loaded in the Timer0: 6400000    */
   /* ----------------------------------------------------------- */
   /*                     Enable Interrupts                  */
   /* ----------------------------------------------------------- */

       PIT.CH[0].TCTRL.B.TIE  = 0x1;    
           /*Timer 0 Interrupt : Enabled    */
       PIT.CH[1].TCTRL.B.TIE  = 0x1;  
       
       PIT.CH[2].TCTRL.B.TIE  = 0x1;  
       
       PIT.CH[3].TCTRL.B.TIE  = 0x1;  

/* ----------------------------------------------------------- */
/*                   Start Timers                 */
/* ----------------------------------------------------------- */

       PIT.CH[0].TCTRL.B.TEN = 0x1;    
           /*Start Timer 0 is : Enabled    */
           
       PIT.CH[1].TCTRL.B.TEN = 0x1;    
           /*Start Timer 1 is : Enabled    */
       
       PIT.CH[2].TCTRL.B.TEN = 0x1;    
       
       PIT.CH[3].TCTRL.B.TEN = 0x1;    
       
        
       PIT.PITMCR.B.MDIS = 0;    
        /*PIT Module : Enabled        */

}


/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

