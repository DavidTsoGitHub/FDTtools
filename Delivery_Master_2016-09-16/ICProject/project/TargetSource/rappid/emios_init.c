/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : RappID_PWM_Prj
 *
 * Project File           : RappID_PWM_Prj.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : emios_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 16-Jun-2015 06:14:28
 *
 * Created on Date        : 16-Jun-2015 06:14:29
 *
 * Brief Description      : This file initializes eMIOS registers
 *
 ******************************************************************************** 
 *
 * Detail Description     : This module provides upto 24 Timed I/O Channels to generate or measure
 *                          time events. The various modes supported are Input Capture, Output Compare,
 *                          Modulus Counter, PWM Outputs etc. All the Channels are identical and allows
 *                          the flag generation to generate system Interrupts or DMA request.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "emios_init.h"



/**********************  Initialization Function(s) *************************/


void emios_init_fnc (void) 
{
    emios_0_init_fnc();
    emios_1_init_fnc();
}
void emios_0_init_fnc (void) 
{
/* ----------------------------------------------------------- */
/*	EMIOS Module Configuration Register (EMIOS_MCR)            */
/* ----------------------------------------------------------- */
   EMIOS_0.MCR.R = 0x14003F00;
            /* Module disable: Clock is running          */
            /* Global Freeze: Disabled                  */
            /* Global Time Base Enable: Yes             */
            /* Global Prescaler Enable: Yes             */
            /* Global Prescaler Value:  64              */

/* ----------------------------------------------------------- */
/* EMIOS Output Update Disable Register (EMIOS_OUDR)           */
/* ----------------------------------------------------------- */
  EMIOS_0.OUDR.R = 0x00000000;
            /* Channel n output (1= Enabled 0= Disable)  */
            /*   n = 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,                   */

/* ----------------------------------------------------------- */
/* EMIOS Channel Disable Register  (EMIOS_UCDIS)           */
/* ----------------------------------------------------------- */
  EMIOS_0.UCDIS.R = 0x00000000;
            /* Channel n (0= Enabled 1= Disable  */
            /*   n = 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,                   */

/* ----------------------------------------------------------- */
/*	Unified Channel 08 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[8].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR8 Data Register - eMIOS_0_CADR8 : Value = 0*/

  EMIOS_0.CH[8].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR8 Data Register - eMIOS_0_CBDR8 : Value = 0*/

  EMIOS_0.CH[8].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 09 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[9].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR9 Data Register - eMIOS_0_CADR9 : Value = 0*/

  EMIOS_0.CH[9].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR9 Data Register - eMIOS_0_CBDR9 : Value = 0*/

  EMIOS_0.CH[9].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 10 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[10].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR10 Data Register - eMIOS_0_CADR10 : Value = 0*/

  EMIOS_0.CH[10].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR10 Data Register - eMIOS_0_CBDR10 : Value = 0*/

  EMIOS_0.CH[10].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 11 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[11].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR11 Data Register - eMIOS_0_CADR11 : Value = 0*/

  EMIOS_0.CH[11].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR11 Data Register - eMIOS_0_CBDR11 : Value = 0*/

  EMIOS_0.CH[11].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 12 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[12].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR12 Data Register - eMIOS_0_CADR12 : Value = 0*/

  EMIOS_0.CH[12].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR12 Data Register - eMIOS_0_CBDR12 : Value = 0*/

  EMIOS_0.CH[12].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 13 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[13].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR13 Data Register - eMIOS_0_CADR13 : Value = 0*/

  EMIOS_0.CH[13].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR13 Data Register - eMIOS_0_CBDR13 : Value = 0*/

  EMIOS_0.CH[13].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 14 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[14].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR14 Data Register - eMIOS_0_CADR14 : Value = 0*/

  EMIOS_0.CH[14].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR14 Data Register - eMIOS_0_CBDR14 : Value = 0*/

  EMIOS_0.CH[14].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 15 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[15].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR15 Data Register - eMIOS_0_CADR15 : Value = 0*/

  EMIOS_0.CH[15].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR15 Data Register - eMIOS_0_CBDR15 : Value = 0*/

  EMIOS_0.CH[15].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 16 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[16].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR16 Data Register - eMIOS_0_CADR16 : Value = 0*/

  EMIOS_0.CH[16].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR16 Data Register - eMIOS_0_CBDR16 : Value = 0*/

  EMIOS_0.CH[16].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 17 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[17].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR17 Data Register - eMIOS_0_CADR17 : Value = 0*/

  EMIOS_0.CH[17].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR17 Data Register - eMIOS_0_CBDR17 : Value = 0*/

  EMIOS_0.CH[17].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 18 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[18].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR18 Data Register - eMIOS_0_CADR18 : Value = 0*/

  EMIOS_0.CH[18].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR18 Data Register - eMIOS_0_CBDR18 : Value = 0*/

  EMIOS_0.CH[18].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 19 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[19].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR19 Data Register - eMIOS_0_CADR19 : Value = 0*/

  EMIOS_0.CH[19].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR19 Data Register - eMIOS_0_CBDR19 : Value = 0*/

  EMIOS_0.CH[19].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 20 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[20].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR20 Data Register - eMIOS_0_CADR20 : Value = 0*/

  EMIOS_0.CH[20].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR20 Data Register - eMIOS_0_CBDR20 : Value = 0*/

  EMIOS_0.CH[20].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 21 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[21].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR21 Data Register - eMIOS_0_CADR21 : Value = 0*/

  EMIOS_0.CH[21].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR21 Data Register - eMIOS_0_CBDR21 : Value = 0*/

  EMIOS_0.CH[21].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 22 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[22].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CADR22 Data Register - eMIOS_0_CADR22 : Value = 0*/

  EMIOS_0.CH[22].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR22 Data Register - eMIOS_0_CBDR22 : Value = 0*/

  EMIOS_0.CH[22].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 23 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_0.CH[23].CADR.R = 0x0000047E;
            /*  [8-31]  Channel eMIOS_0_CADR23 Data Register - eMIOS_0_CADR23 : Value = 1150*/

  EMIOS_0.CH[23].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_0_CBDR23 Data Register - eMIOS_0_CBDR23 : Value = 0*/

  EMIOS_0.CH[23].CCR.R = 0x02000650;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Enable                */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Not Applicable                */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Internal counter                */
            /* Edge Selection Bit :Not Applicable                 */
            /* Edge Polarity :Not Applicable                   */
            /* Mode : Modulus Counter Buffered (Up counter)  */

/* ----------------------------------------------------------- */
/* EMIOS Channel Disable Register  (EMIOS_UCDIS)           */
/* ----------------------------------------------------------- */
  EMIOS_0.UCDIS.R = 0x007FFF00;
            /* Channel n (0= Enabled 1= Disable  */
            /*   n = 8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,                   */

}


void emios_1_init_fnc (void) 
{
/* ----------------------------------------------------------- */
/*	EMIOS Module Configuration Register (EMIOS_MCR)            */
/* ----------------------------------------------------------- */
   EMIOS_1.MCR.R = 0x00000000;
            /* Module disable: Clock is running          */
            /* Global Freeze: Disabled                  */
            /* Global Time Base Enable: No              */
            /* Global Prescaler Enable: No              */
            /* Global Prescaler Value:   1              */

/* ----------------------------------------------------------- */
/* EMIOS Output Update Disable Register (EMIOS_OUDR)           */
/* ----------------------------------------------------------- */
  EMIOS_1.OUDR.R = 0x00000000;
            /* Channel n output (1= Enabled 0= Disable)  */
            /*   n = 16,17,18,19,20,21,22,23,                   */

/* ----------------------------------------------------------- */
/* EMIOS Channel Disable Register  (EMIOS_UCDIS)           */
/* ----------------------------------------------------------- */
  EMIOS_1.UCDIS.R = 0x00000000;
            /* Channel n (0= Enabled 1= Disable  */
            /*   n = 16,17,18,19,20,21,22,23,                   */

/* ----------------------------------------------------------- */
/*	Unified Channel 16 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[16].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR16 Data Register - eMIOS_1_CADR16 : Value = 0*/

  EMIOS_1.CH[16].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR16 Data Register - eMIOS_1_CBDR16 : Value = 0*/

  EMIOS_1.CH[16].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 17 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[17].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR17 Data Register - eMIOS_1_CADR17 : Value = 0*/

  EMIOS_1.CH[17].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR17 Data Register - eMIOS_1_CBDR17 : Value = 0*/

  EMIOS_1.CH[17].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 18 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[18].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR18 Data Register - eMIOS_1_CADR18 : Value = 0*/

  EMIOS_1.CH[18].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR18 Data Register - eMIOS_1_CBDR18 : Value = 0*/

  EMIOS_1.CH[18].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 19 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[19].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR19 Data Register - eMIOS_1_CADR19 : Value = 0*/

  EMIOS_1.CH[19].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR19 Data Register - eMIOS_1_CBDR19 : Value = 0*/

  EMIOS_1.CH[19].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 20 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[20].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR20 Data Register - eMIOS_1_CADR20 : Value = 0*/

  EMIOS_1.CH[20].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR20 Data Register - eMIOS_1_CBDR20 : Value = 0*/

  EMIOS_1.CH[20].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 21 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[21].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR21 Data Register - eMIOS_1_CADR21 : Value = 0*/

  EMIOS_1.CH[21].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR21 Data Register - eMIOS_1_CBDR21 : Value = 0*/

  EMIOS_1.CH[21].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 22 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[22].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR22 Data Register - eMIOS_1_CADR22 : Value = 0*/

  EMIOS_1.CH[22].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR22 Data Register - eMIOS_1_CBDR22 : Value = 0*/

  EMIOS_1.CH[22].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/*	Unified Channel 23 Registers                               */
/* ----------------------------------------------------------- */
  EMIOS_1.CH[23].CADR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CADR23 Data Register - eMIOS_1_CADR23 : Value = 0*/

  EMIOS_1.CH[23].CBDR.R = 0x00000000;
            /*  [8-31]  Channel eMIOS_1_CBDR23 Data Register - eMIOS_1_CBDR23 : Value = 0*/

  EMIOS_1.CH[23].CCR.R = 0x00000000;
            /* Channel Control Register                 */
            /* Freeze Enable : Normal operation         */
            /* Output Disable :  No        */
            /* Output Disable Select : ODI 0                */
            /* Prescaler Divide Ratio : 1             */
            /* Prescaler Enable : Disable               */
            /* Flag generation Request: Assigned to Interrupt */
            /* Input Filter: Bypassed                   */
            /* Filter clock select : Prescaler clock    */
            /* Flag Enable : IRQ/DMA not generated      */
            /* Force Match A : Disabled                 */
            /* Force Match B : Disabled                 */
            /* Bus Select : Counter bus[A]                */
            /* Edge Selection Bit :FLAG                 */
            /* Edge Polarity :Falling                   */
            /* Mode : Default Mode                      */

/* ----------------------------------------------------------- */
/* EMIOS Channel Disable Register  (EMIOS_UCDIS)           */
/* ----------------------------------------------------------- */
  EMIOS_1.UCDIS.R = 0x00000000;
            /* Channel n (0= Enabled 1= Disable  */
            /*   n = 16,17,18,19,20,21,22,23,                   */

}



 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

