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
 * file                   : siu_init.c
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
 * Brief Description      : This file contains Pads, external Interrupts and Wakeups configuration
 *
 ******************************************************************************** 
 *
 * Detail Description     : This File contains function for peripheral assignment for each
 *                          pads, External Interrupt configuration, Wakeup configuration
 *                          and Trigger configuration. It also handles Port parallel masking.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rappid_ref.h"
#include "siu_init.h"




/* ----------------------------------------------------------- */
/*	                SIU Initialization                         */
/* ----------------------------------------------------------- */
void siu_init_fnc(void)
{
    siu_portA_init_fnc();
    siu_portB_init_fnc();
    siu_portC_init_fnc();
    siu_portD_init_fnc();
    siu_portE_init_fnc();
    siu_portF_init_fnc();
    siu_portG_init_fnc();
    siu_portH_init_fnc();
    siu_portJ_init_fnc();
    siu_portK_init_fnc();
    siu_general_init_fnc();
    
	lcd_sfr_init_fnc();
    siu_psmi_init_fnc();
}


/* -----------------------------------------------------------------------------*/
/*                 SIU PORT A PAD Configuration                                 */
/* -----------------------------------------------------------------------------*/
void siu_portA_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[0]  DCU_R0_eMIOS0_22_SOUND_PA0_FP23 (A1) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[0].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[1]  DCU_R1_eMIOS0_21_PA1_FP22 (B1)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[1].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[2]  DCU_R2_eMIOS0_20_PA2_FP21 (C1)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[2].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[3]  DCU_R3_eMIOS0_19_PA3_FP20 (C2)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[3].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[4]  DCU_R4_eMIOS0_18_PA4_FP19 (D1)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[4].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[5]  DCU_R5_eMIOS0_17_PA5_FP18 (D2)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[5].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[6]  DCU_R6_eMIOS0_15_PA6_FP17 (E1)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[6].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[7]  DCU_R7_eMIOS0_16_PA7_FP16 (E2)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[7].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[8]  DCU_G0_eMIOS1_23_SCL_2_PA8_FP15 (F1) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[8].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[9]  DCU_G1_eMIOS1_18_SDA_2_PA9_FP14 (F2) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[9].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[10]  DCU_G2_eMIOS1_20_PA10_FP13 (G1)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[10].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[11]  DCU_G3_eMIOS0_13_PA11_FP12 (G2)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[11].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */
             
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[12]  DCU_G4_eMIOS0_12_PA12_FP11 (H1)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[12].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[13]  DCU_G5_eMIOS0_11_PA13_FP10 (H2)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[13].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[14]  DCU_G6_eMIOS0_10_PA14_FP9 (J2)      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[14].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA14          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[15]  DCU_G7_eMIOS0_9_PA15_FP8 (H3)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[15].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PA15          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* -----------------------------------------------------------------------------*/
/*                 SIU PORT B PAD Configuration                                 */
/* -----------------------------------------------------------------------------*/
void siu_portB_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[16]  CNTX_0_PDI1_PB0 (T15)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[16].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[17]  CNRX_0_PDI0_PB1 (T14)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[17].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[18]  TXD_0_PB2 (R14)                     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[18].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[19]  RXD_0_PB3 (R13)                     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[19].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[20]  SCK_1_MA0_PB4 (P8)                  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[20].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[21]  SOUT_1_MA1_FABM_PB5 (N8)            */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[21].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB5 I         */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Enabled           */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Enabled       */
             /*	Weak Pull Up/Down Select :Down    */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[22]  SIN_1_MA2_ABS[0]_PB6 (R7)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[22].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB6 I         */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Enabled           */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Enabled       */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[23]  SIN_0_eMIOS1_22_PB7 (P7)            */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[23].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[24]  SOUT_0_eMIOS1_21_PB8 (N7)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[24].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[25]  SCK_0_eMIOS1_20_PB9 (T6)            */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[25].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[26]  CNRX_1_PDI2_eMIOS0_23_PB10 (P13)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[26].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[27]  CNTX_1_PDI3_eMIOS0_16_PB11 (N12)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[27].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[28]  RXD_1_eMIOS1_19_PCS_0_2_PB12 (R6)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[28].R = 0x4200;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[29]  TXD_1_eMIOS1_18_PCS_0_1_PB13 (P6)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[29].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PB13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* -----------------------------------------------------------------------------*/
/*                 SIU PORT C PAD Configuration                                 */
/* -----------------------------------------------------------------------------*/
void siu_portC_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[30]  ANS0_PC0 (T13)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[30].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[31]  ANS1_PC1 (T12)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[31].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[32]  ANS2_PC2 (R12)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[32].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[33]  ANS3_PC3 (P12)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[33].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[34]  ANS4_PC4 (R11)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[34].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[35]  ANS5_PC5 (P11)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[35].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[36]  ANS6_PC6 (N11)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[36].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[37]  ANS7_PC7 (R10)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[37].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[38]  ANS8_PC8 (P10)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[38].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[39]  ANS9_PC9 (N10)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[39].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[40]  ANS10(mux)_SOUND_PC10 (T9)          */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[40].R = 0x0A00;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[41]  ANS11_MA0_PCS_1_2_PC11 (R9)         */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[41].R = 0x2001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[42]  ANS12_MA1_PCS_1_1_PC12 (P9)         */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[42].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[43]  ANS13_MA2_PCS_1_0_PC13 (N9)         */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[43].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[44]  ANS14_EXTAL32_PC14 (T8)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[44].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC14          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[45]  ANS15_XTAL32_PC15 (R8)              */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[45].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PC15          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* -----------------------------------------------------------------------------*/
/*                 SIU PORT D PAD Configuration                                 */
/* -----------------------------------------------------------------------------*/
void siu_portD_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[46]  M0C0M_SSD0_0_eMIOS1_23_PD0 (R16)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[46].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[47]  M0C0P_SSD0_1_eMIOS1_22_PD1 (P16)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[47].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[48]  M0C1M_SSD0_2_eMIOS1_21_PD2 (P15)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[48].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[49]  M0C1P_SSD0_3_eMIOS1_20_PD3 (N16)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[49].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[50]  M1C0M_SSD1_0_eMIOS1_19_PD4 (N15)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[50].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[51]  M1C0P_SSD1_1_eMIOS1_18_PD5 (M15)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[51].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[52]  M1C1M_SSD1_2_eMIOS1_17_PD6 (M16)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[52].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[53]  M1C1P_SSD1_3_eMIOS1_16_PD7 (K16)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[53].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[54]  M2C0M_SSD2_0_PD8 (J16)              */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[54].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[55]  M2C0P_SSD2_1_PD9 (K15)              */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[55].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[56]  M2C1M_SSD2_2_PD10 (N14)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[56].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[57]  M2C1P_SSD2_3_PD11 (M14)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[57].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[58]  M3C0M_SSD3_0_PD12 (L14)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[58].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[59]  M3C0P_SSD3_1_PD13 (K14)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[59].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[60]  M3C1M_SSD3_2_PD14 (M13)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[60].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD14          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[61]  M3C1P_SSD3_3_PD15 (L13)             */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[61].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PD15          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* -----------------------------------------------------------------------------*/
/*                 SIU PORT E PAD Configuration                                 */
/* -----------------------------------------------------------------------------*/
void siu_portE_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[62]  M4C0M_SSD4_0_eMIOS0_15_PE0 (J15)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[62].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[63]  M4C0P_SSD4_1_eMIOS0_14_PE1 (G15)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[63].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[64]  M4C1M_SSD4_2_eMIOS0_13_PE2 (J14)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[64].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[65]  M4C1P_SSD4_3_eMIOS0_12_PE3 (K13)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[65].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[66]  M5C0M_SSD5_0_eMIOS0_11_PE4 (J13)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[66].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[67]  M5C0P_SSD5_1_eMIOS0_10_PE5 (H13)    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[67].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[68]  M5C1M_SSD5_2_eMIOS0_9_PE6 (H14)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[68].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[69]  M5C1P_SSD5_3_eMIOS0_8_PE7 (G14)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[69].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PE7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ----------------------------------------------------------------------*/
/*                 SIU PORT F PAD Configuration                          */
/* ----------------------------------------------------------------------*/
void siu_portF_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[70]  eMIOS0_13_PDI4_eMIOS0_22_PF0_FP39 (A8) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[70].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[71]  eMIOS0_12_PDI5_eMIOS0_21_PF1_FP38 (B8) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[71].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[72]  NMI_PF2 (L3)                        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[72].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[73]  eMIOS0_11_PDI6_PF3_FP37 (C8)        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[73].R = 0x0103;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[74]  eMIOS0_10_PDI7_PF4_FP36 (D8)        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[74].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[75]  eMIOS0_9_DCU_TAG_PF5_FP35 (A9)      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[75].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[76]  SDA_0_PF6_FP34 (B9)                 */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[76].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[77]  SCL_0_PCS_1_2_PF7_FP33 (C9)         */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[77].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[78]  SDA_1_PCS_1_1_RXD_1_PF8_FP32 (T4)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[78].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[79]  SCL_1_PCS_1_0_TXD_1_PF9_FP31 (R4)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[79].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[80]  eMIOS0_16_PCS_2_0_PF10_FP29 (A14)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[80].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[81]  eMIOS1_23_PCS_2_1_PF11_FP28 (A15)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[81].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[82]  eMIOS1_16_PCS_2_2_PF12_FP27 (A16)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[82].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[83]  SIN_2_CNRX_1_PF13_FP26 (B16)        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[83].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[84]  SOUT_2_CNTX_1_PF14_FP25 (C16)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[84].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF14          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[85]  SCK_2_PF15_FP24 (D16)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[85].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PF15          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ----------------------------------------------------------------------*/
/*                 SIU PORT G PAD Configuration                          */
/* ----------------------------------------------------------------------*/
void siu_portG_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[86]  DCU_B0_SCL_3_SOUND_PG0_FP7 (D3)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[86].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[87]  DCU_B1_SDA_3_PG1_FP6 (E3)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[87].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[88]  DCU_B2_eMIOS1_19_PG2_FP5 (E4)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[88].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[89]  DCU_B3_eMIOS1_21_PG3_FP4 (F3)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[89].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[90]  DCU_B4_eMIOS1_17_PG4_FP3 (F4)       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[90].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[91]  DCU_B5_eMIOS0_8_PG5_FP2 (G3)        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[91].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[92]  DCU_B6_PG6_FP1 (G4)                 */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[92].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[93]  DCU_B7_PG7_FP0 (H4)                 */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[93].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[94]  DCU_VSYNC_PG8 (J3)                  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[94].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[95]  DCU_HSYNC_PG9 (K3)                  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[95].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[96]  DCU_DE_PG10 (J4)                    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[96].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[97]  DCU_PCLK_PG11 (K4)                  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[97].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[98]  eMIOS0_23_SOUND_eMIOS0_8_PG12_FP30 (D10) */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[98].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PG12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ----------------------------------------------------------------------*/
/*                 SIU PORT H PAD Configuration                          */
/* ----------------------------------------------------------------------*/
void siu_portH_init_fnc(void)
{
  /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[99]  TCK_PH0 (R1)                        */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[99].R = 0x0503;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : TCK           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Enabled           */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Enabled       */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[100]  TDI_PH1 (P2)                       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[100].R = 0x0503;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : TDI           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Enabled           */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Enabled       */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[101]  TDO_PH2 (N3)                       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[101].R = 0x0601;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : TDO           */
             /*	Output Buffers : Enabled          */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[102]  TMS_PH3 (M3)                       */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[102].R = 0x0503;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : TMS           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Enabled           */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Enabled       */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[103]  PCS_0_0_eMIOS1_16_CLKOUT_PH4 (R5)  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[103].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PH4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[104]  VLCD_PH5 (N2)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[104].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PH5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ----------------------------------------------------------------------*/
/*                 SIU PORT J PAD Configuration                          */
/* ----------------------------------------------------------------------*/
void siu_portJ_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[105]  PDI_DE_PJ0 (A2)                    */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[105].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[106]  PDI_HSYNC_PJ1 (A3)                 */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[106].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[107]  PDI_VSYNC_PJ2 (B3)                 */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[107].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[108]  PDI_PCLK_PJ3 (A4)                  */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[108].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[109]  PDI0_CNRX_0_PJ4 (B4)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[109].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[110]  PDI1_CNTX_0_PJ5 (A5)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[110].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[111]  PDI2_CNRX_1_eMIOS0_22_PJ6 (B5)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[111].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[112]  PDI3_CNTX_1_eMIOS0_21_PJ7 (A6)     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[112].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[113]  PDI4_PJ8 (B6)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[113].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[114]  PDI5_PJ9 (C4)                      */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[114].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[115]  PDI6_PJ10 (C5)                     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[115].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ10          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[116]  PDI7_PJ11 (D5)                     */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[116].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ11          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[117]  PDI8_eMIOS1_17_PJ12 (C6)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[117].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ12          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[118]  PDI9_eMIOS1_20_PJ13 (D6)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[118].R = 0x0101;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ13          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[119]  PDI10_eMIOS0_20_PJ14 (A7)          */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[119].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ14          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[120]  PDI11_eMIOS0_19_PJ15 (B7)          */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[120].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PJ15          */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ----------------------------------------------------------------------*/
/*                 SIU PORT K PAD Configuration                          */
/* ----------------------------------------------------------------------*/
void siu_portK_init_fnc(void)
{
    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[121]  PDI12_eMIOS0_18_DCU_TAG_PK0 (C7)   */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[121].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK0           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[122]  PDI13_eMIOS0_17_PK1 (D7)           */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[122].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK1           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[123]  MCKO_PDI10_PK2 (B12)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[123].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK2           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[124]  MSEO_PDI11_PK3 (C12)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[124].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK3           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[125]  EVTO_PDI12_PK4 (D12)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[125].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK4           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[126]  EVTI_PDI13_PK5 (A11)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[126].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK5           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[127]  MDO0_PDI14_PK6 (B11)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[127].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK6           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[128]  MDO1_PDI15_PK7 (C11)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[128].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK7           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[129]  MDO2_PDI16_PK8 (D11)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[129].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK8           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[130]  MDO3_PDI17_PK9 (A10)               */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[130].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK9           */
             /*	Output Buffers : Disabled         */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[131]  SDA_1_eMIOS0_15_PK10 (N6)          */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[131].R = 0x0001;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK10 O        */
             /*	Output Buffers : Enabled          */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

    /* -----------------------------------------------------------------------------*/
    /*      Pad Configuration Register PCR[132]  SCL_1_eMIOS0_14_PK11 (N5)          */
    /* -----------------------------------------------------------------------------*/
       SIU.PCR[132].R = 0x0201;
             /* Safe Mode Control: Disabled       */
             /* Analog Pad Control : Disabled     */
             /*	Selected Function : PK11 O        */
             /*	Output Buffers : Enabled          */
             /*	Input Buffers : Disabled          */
             /*	Output Drain : Disabled           */
             /*	Slew Rate Control : Minimum       */
             /*	Weak Pull Up/Down : Disabled      */
             /*	Weak Pull Up/Down Select :Up      */

}


/* ---------------------------------------------------------------------------------*/
/*               SIU General PAD Configuration                                      */
/* ---------------------------------------------------------------------------------*/
void siu_general_init_fnc(void)
{

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Clock Prescaler Register- IFCPR                           */
    /* -----------------------------------------------------------------------------*/
        SIU.IFCPR.R = 0x00000000;
 	    	     /* Filter Clock Prescalar:- 1     */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Request Enable Register- IRER                                    */
    /* -----------------------------------------------------------------------------*/
        SIU.IRER.R = 0x00000000;
             /* Edge Trigger External Interrupt 0 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 1 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 2 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 3 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 4 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 5 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 6 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 7 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 8 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 9 Enable  :- Disabled */
             /* Edge Trigger External Interrupt 10 Enable :- Disabled */
             /* Edge Trigger External Interrupt 11 Enable :- Disabled */
             /* Edge Trigger External Interrupt 12 Enable :- Disabled */
             /* Edge Trigger External Interrupt 13 Enable :- Disabled */

    /* -----------------------------------------------------------------------------*/
    /*   External IRQ Rising-Edge Event Enable Register - IREER                     */
    /* -----------------------------------------------------------------------------*/
        SIU.IREER.R = 0x00000000;

             /* IRQ0 Rising-Edge Event  : Disabled */
             /* IRQ1 Rising-Edge Event  : Disabled */
             /* IRQ2 Rising-Edge Event  : Disabled */
             /* IRQ3 Rising-Edge Event  : Disabled */
             /* IRQ4 Rising-Edge Event  : Disabled */
             /* IRQ5 Rising-Edge Event  : Disabled */
             /* IRQ6 Rising-Edge Event  : Disabled */
             /* IRQ7 Rising-Edge Event  : Disabled */
             /* IRQ8 Rising-Edge Event  : Disabled */
             /* IRQ9 Rising-Edge Event  : Disabled */
             /* IRQ10 Rising-Edge Event : Disabled */
             /* IRQ11 Rising-Edge Event : Disabled */
             /* IRQ12 Rising-Edge Event : Disabled */
             /* IRQ13 Rising-Edge Event : Disabled */

    /* -----------------------------------------------------------------------------*/
    /*   External IRQ Falling-Edge Event Enable Register - IFEER                    */
    /* -----------------------------------------------------------------------------*/
        SIU.IFEER.R = 0x00000000;
             /* IRQ0 Falling-Edge Event  : Disabled */
             /* IRQ1 Falling-Edge Event  : Disabled */
             /* IRQ2 Falling-Edge Event  : Disabled */
             /* IRQ3 Falling-Edge Event  : Disabled */
             /* IRQ4 Falling-Edge Event  : Disabled */
             /* IRQ5 Falling-Edge Event  : Disabled */
             /* IRQ6 Falling-Edge Event  : Disabled */
             /* IRQ7 Falling-Edge Event  : Disabled */
             /* IRQ8 Falling-Edge Event  : Disabled */
             /* IRQ9 Falling-Edge Event  : Disabled */
             /* IRQ10 Falling-Edge Event : Disabled */
             /* IRQ11 Falling-Edge Event : Disabled */
             /* IRQ12 Falling-Edge Event : Disabled */
             /* IRQ13 Falling-Edge Event : Disabled */

    /* -----------------------------------------------------------------------------*/
    /*    Interrupt Filter Enable Register - IFER                                   */
    /* -----------------------------------------------------------------------------*/
        SIU.IFER.R = 0x00000000;
             /* Analog glitch filter on External Input0 Pad:-  Disabled */
             /* Analog glitch filter on External Input1 Pad:-  Disabled */
             /* Analog glitch filter on External Input2 Pad:-  Disabled */
             /* Analog glitch filter on External Input3 Pad:-  Disabled */
             /* Analog glitch filter on External Input4 Pad:-  Disabled */
             /* Analog glitch filter on External Input5 Pad:-  Disabled */
             /* Analog glitch filter on External Input6 Pad:-  Disabled */
             /* Analog glitch filter on External Input7 Pad:-  Disabled */
             /* Analog glitch filter on External Input8 Pad:-  Disabled */
             /* Analog glitch filter on External Input9 Pad:-  Disabled */
             /* Analog glitch filter on External Input10 Pad:- Disabled */
             /* Analog glitch filter on External Input11 Pad:- Disabled */
             /* Analog glitch filter on External Input12 Pad:- Disabled */
             /* Analog glitch filter on External Input13 Pad:- Disabled */

    /* -----------------------------------------------------------------------------*/
    /*          NMI Configuration Register - NCR                                    */
    /* -----------------------------------------------------------------------------*/
        WKUP.NCR.R = 0x00000000;
             /* NMI Destination Source Select:- Non-maskable interrupt */
             /* NMI Rising Edge Trigger Event:- Disabled               */
             /* NMI Falling Edge Trigger Event:- Disabled              */
             /* Analog glitch filter on NMI Input pad:- Disabled       */
    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register0- IFMC0                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[0].R = 0x00000000;
             /* External Interrupt0 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register1- IFMC1                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[1].R = 0x00000000;
             /* External Interrupt1 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register2- IFMC2                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[2].R = 0x00000000;
             /* External Interrupt2 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register3- IFMC3                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[3].R = 0x00000000;
             /* External Interrupt3 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register4- IFMC4                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[4].R = 0x00000000;
             /* External Interrupt4 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register5- IFMC5                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[5].R = 0x00000000;
             /* External Interrupt5 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register6- IFMC6                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[6].R = 0x00000000;
             /* External Interrupt6 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register7- IFMC7                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[7].R = 0x00000000;
             /* External Interrupt7 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register8- IFMC8                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[8].R = 0x00000000;
             /* External Interrupt8 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register9- IFMC9                          */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[9].R = 0x00000000;
             /* External Interrupt9 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register10- IFMC10                        */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[10].R = 0x00000000;
             /* External Interrupt10 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register11- IFMC11                        */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[11].R = 0x00000000;
             /* External Interrupt11 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register12- IFMC12                        */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[12].R = 0x00000000;
             /* External Interrupt12 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Interrupt Filter Maximum Counter Register13- IFMC13                        */
    /* -----------------------------------------------------------------------------*/
        SIU.IFMC[13].R = 0x00000000;
             /* External Interrupt13 maximum Filter Counter:- 0 */

    /* -----------------------------------------------------------------------------*/
    /*   Wakeup Request Enable Register - WRER                                      */
    /* -----------------------------------------------------------------------------*/
        WKUP.WRER.R = 0x00000000;
             /* Edge Trigger External Wakeup 0 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 1 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 2 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 3 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 4 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 5 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 6 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 7 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 8 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 9 Enable  :-Disabled */
             /* Edge Trigger External Wakeup 10 Enable :-Disabled */
             /* Edge Trigger External Wakeup 11 Enable :-Disabled */
             /* Edge Trigger External Wakeup 12 Enable :-Disabled */
             /* Edge Trigger External Wakeup 13 Enable :-Disabled */
             /* Edge Trigger External Wakeup 14 Enable :-Disabled */
             /* Edge Trigger External Wakeup 15 Enable :-Disabled */
             /* Edge Trigger External Wakeup 16 Enable :-Disabled */
             /* Edge Trigger External Wakeup 17 Enable :-Disabled */
             /* Edge Trigger External Wakeup 18 Enable :-Disabled */
    /* -----------------------------------------------------------------------------*/
    /*   Wakeup Interrupt Request Enable Register - WIRER                           */
    /* -----------------------------------------------------------------------------*/
        WKUP.IRER.R = 0x00000000;
             /* Edge Trigger External Wakeup Interrupt 0 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 1 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 2 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 3 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 4 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 5 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 6 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 7 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 8 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 9 Enable  :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 10 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 11 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 12 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 13 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 14 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 15 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 16 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 17 Enable :-Disabled */
             /* Edge Trigger External Wakeup Interrupt 18 Enable :-Disabled */
    /* -----------------------------------------------------------------------------*/
    /*   Wakeup Rising-Edge Event Enable Register - WIREER                          */
    /* -----------------------------------------------------------------------------*/
        WKUP.WIREER.R = 0x00000000;
             /* Wakeup0 Rising-Edge Event  :Disabled */
             /* Wakeup1 Rising-Edge Event  :Disabled */
             /* Wakeup2 Rising-Edge Event  :Disabled */
             /* Wakeup3 Rising-Edge Event  :Disabled */
             /* Wakeup4 Rising-Edge Event  :Disabled */
             /* Wakeup5 Rising-Edge Event  :Disabled */
             /* Wakeup6 Rising-Edge Event  :Disabled */
             /* Wakeup7 Rising-Edge Event  :Disabled */
             /* Wakeup8 Rising-Edge Event  :Disabled */
             /* Wakeup9 Rising-Edge Event  :Disabled */
             /* Wakeup10 Rising-Edge Event :Disabled */
             /* Wakeup11 Rising-Edge Event :Disabled */
             /* Wakeup12 Rising-Edge Event :Disabled */
             /* Wakeup13 Rising-Edge Event :Disabled */
             /* Wakeup14 Rising-Edge Event :Disabled */
             /* Wakeup15 Rising-Edge Event :Disabled */
             /* Wakeup16 Rising-Edge Event :Disabled */
             /* Wakeup17 Rising-Edge Event :Disabled */
             /* Wakeup18 Rising-Edge Event :Disabled */
    /* -----------------------------------------------------------------------------*/
    /*   Wakeup Faling-Edge Event Enable Register - WIFEER                          */
    /* -----------------------------------------------------------------------------*/
        WKUP.WIFEER.R = 0x00000000;
             /* Wakeup0 Faling-Edge Event  :Disabled */
             /* Wakeup1 Faling-Edge Event  :Disabled */
             /* Wakeup2 Faling-Edge Event  :Disabled */
             /* Wakeup3 Faling-Edge Event  :Disabled */
             /* Wakeup4 Faling-Edge Event  :Disabled */
             /* Wakeup5 Faling-Edge Event  :Disabled */
             /* Wakeup6 Faling-Edge Event  :Disabled */
             /* Wakeup7 Faling-Edge Event  :Disabled */
             /* Wakeup8 Faling-Edge Event  :Disabled */
             /* Wakeup9 Faling-Edge Event  :Disabled */
             /* Wakeup10 Faling-Edge Event :Disabled */
             /* Wakeup11 Faling-Edge Event :Disabled */
             /* Wakeup12 Faling-Edge Event :Disabled */
             /* Wakeup13 Faling-Edge Event :Disabled */
             /* Wakeup14 Faling-Edge Event :Disabled */
             /* Wakeup15 Faling-Edge Event :Disabled */
             /* Wakeup16 Faling-Edge Event :Disabled */
             /* Wakeup17 Faling-Edge Event :Disabled */
             /* Wakeup18 Faling-Edge Event :Disabled */

    /* -----------------------------------------------------------------------------*/
    /*    Wakeup/Interrupt Filter Enable Register - WIFER                           */
    /* -----------------------------------------------------------------------------*/
        WKUP.WIFER.R = 0x00000000;
             /* Analog glitch filter on External Input0 Pad :- Disabled */
             /* Analog glitch filter on External Input1 Pad :- Disabled */
             /* Analog glitch filter on External Input2 Pad :- Disabled */
             /* Analog glitch filter on External Input3 Pad :- Disabled */
             /* Analog glitch filter on External Input4 Pad :- Disabled */
             /* Analog glitch filter on External Input5 Pad :- Disabled */
             /* Analog glitch filter on External Input6 Pad :- Disabled */
             /* Analog glitch filter on External Input7 Pad :- Disabled */
             /* Analog glitch filter on External Input8 Pad :- Disabled */
             /* Analog glitch filter on External Input9 Pad :- Disabled */
             /* Analog glitch filter on External Input10 Pad:- Disabled */
             /* Analog glitch filter on External Input11 Pad:- Disabled */
             /* Analog glitch filter on External Input12 Pad:- Disabled */
             /* Analog glitch filter on External Input13 Pad:- Disabled */
             /* Analog glitch filter on External Input14 Pad:- Disabled */
             /* Analog glitch filter on External Input15 Pad:- Disabled */
             /* Analog glitch filter on External Input16 Pad:- Disabled */
             /* Analog glitch filter on External Input17 Pad:- Disabled */
             /* Analog glitch filter on External Input18 Pad:- Disabled */



    /* -----------------------------------------------------------------------------*/
    /*    Wakeup/Interrupt Pullup Enable Register - WIPUER                          */
    /* -----------------------------------------------------------------------------*/
        WKUP.WIPUER.R = 0x00000000;
           /* Pullup an unconnected wakeup/interrupt0
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt1
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt2
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt3
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt4
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt5
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt6
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt7
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt8
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt9
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt10
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt11
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt12
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt13
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt14
                               input to a value of ��:-Disabled */
           /* Pullup an unconnected wakeup/interrupt15
                               input to a value of ��:-Disabled */
          /* Pullup an unconnected wakeup16
                               input to a value of ��:-Disabled */
          /* Pullup an unconnected wakeup17
                               input to a value of ��:-Disabled */
          /* Pullup an unconnected wakeup18
                               input to a value of ��:-Disabled */
}


void lcd_sfr_init_fnc(void)
{
    vuint32_t status = 0;

    if(LCD.CR.B.LCDEN == 1)
        {
            status = LCD.CR.B.LCDEN;
            LCD.CR.B.LCDEN = 0;
        }
/*-----------------------------------------------------------*/
/*        LCD Frontplane Enable Register 0                   */
/*-----------------------------------------------------------*/

    LCD.FPENR0.R = 0x00000FFF;
        /* Frontplane 0 at PG7 PCR_93 is : Enabled*/
        /* Frontplane 1 at PG6 PCR_92 is : Enabled*/
        /* Frontplane 2 at PG5 PCR_91 is : Enabled*/
        /* Frontplane 3 at PG4 PCR_90 is : Enabled*/
        /* Frontplane 4 at PG3 PCR_89 is : Enabled*/
        /* Frontplane 5 at PG2 PCR_88 is : Enabled*/
        /* Frontplane 6 at PG1 PCR_87 is : Enabled*/
        /* Frontplane 7 at PG0 PCR_86 is : Enabled*/
        /* Frontplane 8 at PG15 PCR_15 is : Enabled*/
        /* Frontplane 9 at PG14 PCR_14 is : Enabled*/
        /* Frontplane 10 at PG13 PCR_13 is : Enabled*/
        /* Frontplane 11 at PG12 PCR_12 is : Enabled*/
        /* Frontplane 12 at PG11 PCR_11 is : Enabled*/
        /* Frontplane 13 at PG10 PCR_10 is : Enabled*/
        /* Frontplane 14 at PG9 PCR_9 is : Enabled*/
        /* Frontplane 15 at PG8 PCR_8 is : Enabled*/
        /* Frontplane 16 at PG7 PCR_7 is : Enabled*/
        /* Frontplane 17 at PG6 PCR_6 is : Enabled*/
        /* Frontplane 18 at PG5 PCR_5 is : Enabled*/
        /* Frontplane 19 at PG4 PCR_4 is : Enabled*/
        /* Frontplane 20 at PG3 PCR_3 is : Enabled*/
        /* Frontplane 21 at PG2 PCR_2 is : Enabled*/
        /* Frontplane 22 at PG1 PCR_1 is : Enabled*/
        /* Frontplane 23 at PG0 PCR_0 is : Enabled*/
        /* Frontplane 24 at PG15 PCR_85 is : Disabled*/
        /* Frontplane 25 at PG14 PCR_84 is : Disabled*/
        /* Frontplane 26 at PG13 PCR_83 is : Disabled*/
        /* Frontplane 27 at PG12 PCR_82 is : Disabled*/
        /* Frontplane 28 at PG11 PCR_81 is : Disabled*/
        /* Frontplane 29 at PG10 PCR_80 is : Disabled*/
        /* Frontplane 30 at PG12 PCR_98 is : Disabled*/
        /* Frontplane 31 at PG9 PCR_79 is : Disabled*/

/*-----------------------------------------------------------*/
/*        LCD Frontplane Enable Register 1                   */
/*-----------------------------------------------------------*/
    LCD.FPENR1.R = 0x00000000;
        /* Frontplane 32 at PG8 PCR_78 is : Disabled*/
        /* Frontplane 33 at PG7 PCR_77 is : Disabled*/
        /* Frontplane 34 at PG6 PCR_76 is : Disabled*/
        /* Frontplane 35 at PG5 PCR_75 is : Disabled*/
        /* Frontplane 36 at PG4 PCR_74 is : Disabled*/
        /* Frontplane 37 at PG3 PCR_73 is : Disabled*/
        /* Frontplane 38 at PG2 PCR_72 is : Disabled*/
        /* Frontplane 39 at PG1 PCR_71 is : Disabled*/

        if(status != 0)
        {
            LCD.CR.B.LCDEN = 1;
        }
}

/* -----------------------------------------------------------------------------*/
/*               SIU PSMI PAD Configuration                                     */
/* -----------------------------------------------------------------------------*/
void siu_psmi_init_fnc(void)
{
/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[0]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[0].R = 0x00;
            /*Function Pad Selected CAN0_RX :- PCR17*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[1]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[1].R = 0x00;
            /*Function Pad Selected CAN1_RX :- PCR26*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[2]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[2].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[0] :- PCR17*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[3]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[3].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[1] :- PCR16*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[4]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[4].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[2] :- PCR26*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[5]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[5].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[3] :- PCR27*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[6]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[6].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[4] :- PCR70*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[7]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[7].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[5] :- PCR71*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[8]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[8].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[6] :- PCR73*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[9]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[9].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[7] :- PCR74*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[10]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[10].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[10] :- PCR119*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[11]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[11].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[11] :- PCR120*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[12]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[12].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[12] :- PCR121*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[13]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[13].R = 0x00;
            /*Function Pad Selected PDI_DATAIN[13] :- PCR122*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[14]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[14].R = 0x00;
            /*Function Pad Selected DSPI_1_CS0 :- PCR43*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[15]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[15].R = 0x00;
            /*Function Pad Selected EMIOS0_CH8 :- PCR69*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[16]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[16].R = 0x00;
            /*Function Pad Selected EMIOS0_CH9 :- PCR15*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[17]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[17].R = 0x00;
            /*Function Pad Selected EMIOS0_CH10 :- PCR14*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[18]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[18].R = 0x00;
            /*Function Pad Selected EMIOS0_CH11 :- PCR13*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[19]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[19].R = 0x00;
            /*Function Pad Selected EMIOS0_CH12 :- PCR12*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[20]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[20].R = 0x00;
            /*Function Pad Selected EMIOS0_CH13 :- PCR11*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[21]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[21].R = 0x00;
            /*Function Pad Selected EMIOS0_CH14 :- PCR63*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[22]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[22].R = 0x00;
            /*Function Pad Selected EMIOS0_CH15 :- PCR6*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[23]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[23].R = 0x00;
            /*Function Pad Selected EMIOS0_CH16 :- PCR7*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[24]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[24].R = 0x00;
            /*Function Pad Selected EMIOS0_CH17 :- PCR5*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[25]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[25].R = 0x00;
            /*Function Pad Selected EMIOS0_CH18 :- PCR4*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[26]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[26].R = 0x00;
            /*Function Pad Selected EMIOS0_CH19 :- PCR3*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[27]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[27].R = 0x00;
            /*Function Pad Selected EMIOS0_CH20 :- PCR2*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[28]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[28].R = 0x00;
            /*Function Pad Selected EMIOS0_CH21 :- PCR1*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[29]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[29].R = 0x00;
            /*Function Pad Selected EMIOS0_CH22 :- PCR0*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[30]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[30].R = 0x00;
            /*Function Pad Selected EMIOS0_CH23 :- PCR26*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[31]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[31].R = 0x00;
            /*Function Pad Selected EMIOS1_CH16 :- PCR53*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[32]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[32].R = 0x00;
            /*Function Pad Selected EMIOS1_CH17 :- PCR52*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[33]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[33].R = 0x00;
            /*Function Pad Selected EMIOS1_CH18 :- PCR9*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[34]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[34].R = 0x00;
            /*Function Pad Selected EMIOS1_CH19 :- PCR28*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[35]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[35].R = 0x00;
            /*Function Pad Selected EMIOS1_CH20 :- PCR10*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[36]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[36].R = 0x00;
            /*Function Pad Selected EMIOS1_CH21 :- PCR24*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[37]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[37].R = 0x00;
            /*Function Pad Selected EMIOS1_CH22 :- PCR23*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[38]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[38].R = 0x00;
            /*Function Pad Selected EMIOS1_CH23 :- PCR8*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[39]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[39].R = 0x00;
            /*Function Pad Selected I2C1_SCL :- PCR79*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[40]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[40].R = 0x00;
            /*Function Pad Selected I2C1_SDA :- PCR78*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[41]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[41].R = 0x00;
            /*Function Pad Selected LIN1_RXD :- PCR28*/

/*------------------------------------------------------------------------------*/
/* Pad Selection for Multiplex Input Register PSMI[42]                           */
/*------------------------------------------------------------------------------*/
        SIU.PSMI[42].R = 0x00;
            /*Function Pad Selected EVTI :- PCR126*/

}



 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

