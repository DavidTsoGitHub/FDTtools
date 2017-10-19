/*                                                               */
/* FILE        : MCU_Display.h                                   */
/* DATE        : 2015-08-25                                      */
/* AUTHOR      : Zhu Mingqi                                      */
/* DESCRIPTION : header file of LCD module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Zhu Mingqi on 2015-08-25       */
/*                                                               */
/*****************************************************************/
#ifndef LCD_H
#define LCD_H

#define DISPLAY_ON()                                LCD.CR.B.LCDEN=1	
#define DISPLAY_OFF()                               LCD.CR.B.LCDEN=0	

#define SetSegmentLCD(num, value)  					LCD.RAM[num].R = value
#define SetSegmentLCD3(num, value)                  LCD.RAM[num].RByte.RByte_3 = value
#define SetSegmentLCD2(num, value)                  LCD.RAM[num].RByte.RByte_2 = value
#define SetSegmentLCD1(num, value)                  LCD.RAM[num].RByte.RByte_1 = value
#define SetSegmentLCD0(num, value)                  LCD.RAM[num].RByte.RByte_0 = value

extern void MCU_SegmentLcd_Clear(void);
extern void MCU_SegmentLcd_Fill(void);
#endif
