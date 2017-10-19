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
 * file                   : intc_sw_vecttable.c
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
 * Brief Description      : This file contains definition for the Interrupt Vector table 
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file is generated when software vector mode is selected.
 *                          This file contains the definition for all Interrupt vector
 *                          tables.(For eg. DSPI,Linflex,ADC)
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rappid_ref.h"
#include "jdp.h"
#include "PIT.h"
#include "Sound.h"
#include "stm_init.h"
#include "RTC.h"
#include "MCU_FreqCntInput.h"
#include "BSP.h"
#include "CanInterrupts.h"

/**********************  Function Prototype here *************************/

void interrupt_handler (void);
void ShutDownISR(void);

void PIT_Isr3(void);
void Speed_Ctrl_ISR(void);
void Tacho_Ctrl_ISR(void);
/*************************** Pragma_1 Section **************************/

/* pragma for defining the section of the Interrupt Vector table */
#pragma section data_type ".vects" ".uvects"

/* Definition for the Interrupt Vector table */
uint32_t IntcIsrVectorTable[] = {
    (uint32_t)&interrupt_handler,    /* Vector - 0    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 1    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 2    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 3    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 4    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 5    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 6    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 7    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 9    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 10    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 11    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 12    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 13    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 14    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 15    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 16    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 17    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 18    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 19    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 20    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 21    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 22    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 23    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 24    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 25    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 26    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 28    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&FreqInputCapTimerISR,    /* Vector - 30    FreqInputCapTimerISR*/
    (uint32_t)&interrupt_handler,    /* Vector - 31    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 32    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 33    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 35    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 36    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 38    interrupt_handler*/
    (uint32_t)&MCU_Api_ISR,          /* Vector - 39    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 41    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 42    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 46    WakeUp group 0 ISR */
    (uint32_t)&interrupt_handler,    /* Vector - 47    WakeUp group 1 ISR */
    (uint32_t)&interrupt_handler,    /* Vector - 48    WakeUp group 2 ISR */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 51    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 52    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 53    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 54    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 56    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 57    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&PIT_ISR0,             /* Vector - 59    PIT_Isr0 */
    (uint32_t)&Speed_Ctrl_ISR,    /* Vector - 60    interrupt_handler*/
    (uint32_t)&Tacho_Ctrl_ISR,    /* Vector - 61    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 62    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 64    interrupt_handler*/
    (uint32_t)&canA_ERR_ISR,   		 /* Vector - 65   FLEXCAN0 ERROR*/
    (uint32_t)&canA_ERR_ISR,   		 /* Vector - 65   FLEXCAN0 ERROR*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&canA_MB00_03int,    /* Vector - 68    FlexCAN0 MB 00-03 */
    (uint32_t)&canA_MB04_07int,    /* Vector - 69    FlexCAN0 MB 04-07 */
    (uint32_t)&canA_MB08_11int,    /* Vector - 70    FlexCAN0 MB 08-11 */
    (uint32_t)&canA_MB12_15int,    /* Vector - 71    FlexCAN0 MB 12-15 */
    (uint32_t)&canA_MB16_31int,    /* Vector - 72    FlexCAN0 MB 16-31 */
    (uint32_t)&canA_MB32_63int,    /* Vector - 73    FlexCAN0 MB 32-63 */
    (uint32_t)&interrupt_handler,    /* Vector - 74    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 75    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 76    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 77    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 78    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 79    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 80    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 81    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 85    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 86    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 88    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 89    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 90    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 91    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 92    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 93    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 94    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 95    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 96    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 97    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 98    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 99    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 100    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 101    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 125    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 126    interrupt_handler*/
    (uint32_t)&PIT_Isr3,    /* Vector - 127    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 141    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 142    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 143    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 144    interrupt_handler*/
    (uint32_t)&FreqInputCntOverflowISR,  /* Vector - 145    FreqInputCntOverflowISR*/
    (uint32_t)&FreqInputISR,             /* Vector - 146    FreqInputISR*/
    (uint32_t)&interrupt_handler,    /* Vector - 147    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 148    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 157    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 158    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 159    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 160    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 173    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 174    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&SglISR,               /* Vector - 183    SglISR */
    (uint32_t)&interrupt_handler,    /* Vector - 184    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 185    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 186    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 187    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 192    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 193    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 194    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 195    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 196    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 197    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 198    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 199    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Vector - 201    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 202    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 203    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 204    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 205    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Vector - 206    interrupt_handler*/
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
    (uint32_t)&interrupt_handler,    /* Reserved */
};

/********************** DUMMY INTERRUPT HANDLER **********************/
void interrupt_handler (void)
{

}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

