/*                                                               */
/* FILE        : MCU_Dispaly.c                                     */
/* DATE        : 2015-06-08                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : header file of LCD module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-08          */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "LCD.h"


void MCU_SegmentLcd_Clear(void)
{
	LCD.RAM[0].R = 0x00000000;
	LCD.RAM[1].R = 0x00000000;
	LCD.RAM[2].R = 0x00000000;
}

void MCU_SegmentLcd_Fill(void)
{
	LCD.RAM[0].R = 0xffffffff;
	LCD.RAM[1].R = 0xffffffff;
	LCD.RAM[2].R = 0xffffffff;
}
