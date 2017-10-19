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
 * file                   : lcd_init.c
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
 * Brief Description      : This file contain functions for configuration of LCD 
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contain functions for configuration of 
 *                          LCD Initialization,FrontPlane configuration and
 *                          RAM initialization.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "lcd_init.h"



void lcd_init_fnc(void)
{
/*-------------------------------------------------------------------------*/
/*  LCD General Setup                                                      */
/*-------------------------------------------------------------------------*/
    LCD.CR.B.LCDEN= 0;
        /* LCD can be Configured until LCD is Disabled  */


/*-------------------------------------------------------------------------*/
/*        LCD_LCDCR Register setup                                         */
/*-------------------------------------------------------------------------*/
    LCD.CR.R = 0x0fa80000;
        /* LCD Reference Clock Selected as: System Clock*/
        /* LCD Duty Selected: 1-1/1*/
        /* BIAS Voltage Select: 1/1*/
        /* LCD voltage reference select: VDDX*/
        /* LCD Power mode: Standard Current*/
        /* LCD output current boost: Disabled*/
        /* Current boost selected: 8 Times*/
        /* LCD Boost always on Feature: Disabled*/
        /* CLCD OSC Clock selected: First OSC clock 32KHz*/
        /* LCD interrupt: Disabled*/
        /* Number of  frames are counted until an interrupt: 0 Frames */
        /* Backplane Adding: Disabled*/
        /* Backplane Shifting: 0*/
        /* LCD in Stop Condition : Disabled*/


/*-------------------------------------------------------------------------*/
/*     LCD_LCDPCR prescaler control Register setup                         */
/*-------------------------------------------------------------------------*/
    LCD.PCR.R = 0x0b000000;
        /* LCLK: 0    */
        /* For LCLK bit value : 0 , LCD Clock Prescaler Divider :  480*/
        /* LCD Frame Frequency (Hz)= System Clock/Divider ) */
        /* LCD Frame Frequency (Hz)= 33333.33333 Hz */



/*-------------------------------------------------------------------------*/
/*     LCD contrast control register Register setup                        */
/*-------------------------------------------------------------------------*/
    LCD.CCR.R = 0x800f0000;
        /* LCD Contrast Control: Disabled        */
        /* Contrast phase width :  0*/


    LCD.CR.B.LCDEN= 1;
        /* LCD Module is: Disabled*/

/*-------------------------------------------------------------------------*/
/*     LCD RAM Initialization Function                                     */
/*-------------------------------------------------------------------------*/
    lcd_ram_init_fnc();
}
void lcd_ram_init_fnc(void)
{
/*-------------------------------------------------------------------------*/
/*     LCD contrast control register Register setup                        */
/*-------------------------------------------------------------------------*/
    LCD.RAM[0].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_0 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_1 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_2 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_3 BackPlane_0: OFF */

    LCD.RAM[1].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_4 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_5 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_6 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_7 BackPlane_0: OFF */

    LCD.RAM[2].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_8 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_9 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_10 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_11 BackPlane_0: OFF */

    LCD.RAM[3].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_12 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_13 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_14 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_15 BackPlane_0: OFF */

    LCD.RAM[4].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_16 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_17 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_0: OFF */

    LCD.RAM[5].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_18 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_19 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_20 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_21 BackPlane_0: OFF */

    LCD.RAM[6].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_22 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_23 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_24 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_25 BackPlane_0: OFF */

    LCD.RAM[7].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_26 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_27 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_28 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_29 BackPlane_0: OFF */

    LCD.RAM[8].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_32 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_33 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_34 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_35 BackPlane_0: OFF */

    LCD.RAM[9].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_36 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_37 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_38 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_39 BackPlane_0: OFF */

    LCD.RAM[10].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_40 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_41 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_42 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_43 BackPlane_0: OFF */

    LCD.RAM[11].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_44 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_45 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_46 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_47 BackPlane_0: OFF */

    LCD.RAM[12].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_48 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_49 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_50 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_51 BackPlane_0: OFF */

    LCD.RAM[13].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_52 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_53 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_54 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_55 BackPlane_0: OFF */

    LCD.RAM[14].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_56 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_57 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_58 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_59 BackPlane_0: OFF */

    LCD.RAM[15].R = 0x00000000;
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_60 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_61 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_62 BackPlane_0: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_5: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_4: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_3: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_2: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_1: OFF */
        /* LCD Segment Connected Between FrontPlane_63 BackPlane_0: OFF */


}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

