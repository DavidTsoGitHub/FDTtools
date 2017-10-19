/*                                                               */
/* FILE        : SGL.c                                           */
/* DATE        : 2015-04-30                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : source file of SGL module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-04-30    */
/*                                                               */
/*****************************************************************/
#include "Platform_Types.h"
#include "jdp.h"
#include "SGL.h"

/*
 * Name        : SGL_Init
 * Description : initialize sound
 * Arguments   : none
 * Return      : none
 */
void SGL_Init(void)
{    
	/* assign clock */
    CGM.AC2SC.B.SELCTL = 3u;			/* For eMIOS_1 */
    CGM.AC2DC.R = 0x80000000u;

   /* initialize eMIOS_1*/
    EMIOS_1.MCR.B.GPRE  = 63u; /* Divide 64 MHz sysclk by 63+1 = 64 for 1MHz eMIOS clk*/
    EMIOS_1.MCR.B.GPREN = 1u;  /* Enable eMIOS clock */
    EMIOS_1.MCR.B.GTBE  = 1u;  /* Enable global time base */
    EMIOS_1.MCR.B.FRZ   = 1u;  /* Enable stopping channels when in debug mode */

    /* initialize channel 23 of eMIOS_1 */
    EMIOS_1.CH[23].CCR.B.MODE  = 0x50u; /* Modulus Counter Buffered (MCB), up counter  */
    EMIOS_1.CH[23].CCR.B.BSL   = 0x03u; /* Use internal counter */
    EMIOS_1.CH[23].CCR.B.UCPRE = 0x00u; /* Set channel prescaler to divide by 1 */
    EMIOS_1.CH[23].CCR.B.UCPEN = 0x01u; /* Enable prescaler counter to count */
    EMIOS_1.CH[23].CCR.B.FREN  = 0x01u; /* Freeze channel counting when in debug mode */
   
    /* EMIOS 1 CH 18: Output Pulse Width Modulation */
    EMIOS_1.CH[18].CCR.B.MODE  = 0x60u; /* Mode is OPWM Buffered */
    EMIOS_1.CH[18].CCR.B.BSL   = 0x00u; /* Use counter bus A (default) */
    EMIOS_1.CH[18].CCR.B.EDPOL = 0x01u; /* Polarity-leading edge sets pin; trailing clears*/
 
    SGL.MODE_SEL.B.MP      = 0x01u; /* monophonic sound */
    SGL.MODE_SEL.B.SDCIE   = 0x01u; /* enable sound external interrupt */
    SGL.MODE_SEL.B.pre     = 63u;   /* frequency =  64MHz / (63 + 1) = 1MHz */
    SGL.MODE_SEL.B.ch1_sel = 0x0Au; /* select EMIOS_1ch18 as input */
    SGL.MODE_SEL.B.ch2_sel = 0x0Au; /* select EMIOS_1ch18 as input */
    SGL.MODE_SEL.B.SN_CTRL = 0x00u; /* NO sound generation */
}

/*
 * Name        : MCU_ToneSet
 * Description : set sound tone
 * Arguments   : u32_tone- tone to be set
 * Return      : none
 */
void MCU_ToneSet(uint32 u32_tone)
{
	EMIOS_1.CH[23].CADR.R = u32_tone;       /* The internal counter counts up until its value matches CADR's value,
	                                           and a clock tick occurs. The counter is set to 0x1 and counts up again.
	                                           The clock tick period is 454 + 1 = 455 clocks (0.45 ms, i.e 2.2KHz) */
	EMIOS_1.CH[18].CADR.R = 10u;            /* Leading edge when channel counter bus=250 */ 
	EMIOS_1.CH[18].CBDR.R = u32_tone - 10u; /* Trailing edge when channel counter bus=500 */
}
