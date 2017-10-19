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
 * file                   : intc_init.c
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
 * Brief Description      : This file contains  the interrupt service routine for the SIU
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains initialization function for Interrupt.
 *                          It sets the vector mode ,size and global priority .It 
 *                          also defines the Interrupt enable Function.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rappid_ref.h"
#include "intc_init.h"
#include "rappid_utils.h"


extern uint32_t IntcIsrVectorTable [];
/*********************  Initialization Function(s) ************************/

void intc_init_fnc (void)
{
/*--------------------------------------------------------------------*/
/*                         INTC General Setup                            */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*  			INTC Module Configuration Register (INTC_MCR) 	      */
/*--------------------------------------------------------------------*/
    INTC.MCR.R = 0x00000000;
    /* Module Control Register - MCR                   */
    /* Bit 26 - The vector table size for e200z0 Core is: 4 bytes     */
    /* Bit 31 - The module for e200z0 Core is in Software Vector Mode */

/*--------------------------------------------------------------------*/
/*    INTC Module Configuration Register For e200z0 Core (IACKR) 	   */
/*--------------------------------------------------------------------*/
    INTC.IACKR.R = (vuint32_t)&IntcIsrVectorTable;
    /* Interrupt Acknowledge Register for e200z0 Core - IACKR      */
    /* Bits 0-20 - The vector base address for e200z0 Core is: 0     */

    INTC.PSR[39].R = 0x0A;
    /* PITimer Channel 0 Priority is: 10       */
/* -------------------------------------------------------------------*/
/*	           Priority Set Register for interrupt vector 59          */
/* -------------------------------------------------------------------*/
    INTC.PSR[59].R = 0x05;
    /* PITimer Channel 0 Priority is: 5        */
    
    INTC.PSR[60].R = 0x0c;
    
	 /* PITimer Channel1 Priority is: 12        */
	INTC.PSR[61].R = 0x0c;
	
	INTC.PSR[127].R = 0x05;

    INTC.PSR[30].R = 0x06;
       /* STM Timer Channel 0 Priority is: 5        */
    
    INTC.PSR[46].R = 0x04;
           /* Wake Up 0 Priority is: 4        */
    
    INTC.PSR[47].R = 0x05;
           /* Wake Up 1  Priority is: 5        */
    
    INTC.PSR[65].R = 0x06;
               /* FlexCAN_ESR  Priority is: 6    */
    
    INTC.PSR[66].R = 0x06;
               /* FlexCAN_ESR_BOFF  Priority is: 6   */
    
    INTC.PSR[68].R = 0x06;
               /* FlexCAN_BUF_00_03  Priority is: 6    */

    INTC.PSR[69].R = 0x06;
               /* FlexCAN_BUF_04_07  Priority is: 6    */

    INTC.PSR[70].R = 0x06;
               /* FlexCAN_BUF_08_11  Priority is: 6    */

    INTC.PSR[71].R = 0x06;
               /* FlexCAN_BUF_12_15  Priority is: 6    */

    INTC.PSR[72].R = 0x06;
               /* FlexCAN_BUF_16_31  Priority is: 6    */

    INTC.PSR[73].R = 0x06;
               /* FlexCAN_BUF_32_63  Priority is: 6    */
    
    INTC.PSR[145].R = 0x04;
       /* eMIOS16_17 Priority is: 6        */
    
    INTC.PSR[146].R = 0x08;
       /* eMIOS18_19  Priority is: 8        */
    
    INTC.PSR[183].R = 0x07;
    /* SGL Priority is: 7        */

    en_int_fnc(0);

}

/*********************  Interrupt Enable Function *************************/

void en_int_fnc (vuint32_t priority)
{
/* -------------------------------------------------------------------*/
/*	           Enable Routine - Priority equal 0-15                   */
/*	    (0 - Enable All interrupts, 15 - Disable All interrupts)      */
/* -------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/*    INTC Current Priority Register  for Core e200z0 (INTC_CPR)     */
/*--------------------------------------------------------------------*/
    INTC.CPR.R = priority;
    /* Bits 4 ~ 0 - The Interrupt Prevention Limit for Core e200z0 is: 0*/


}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

