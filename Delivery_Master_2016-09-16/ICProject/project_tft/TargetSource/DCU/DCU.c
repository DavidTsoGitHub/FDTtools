/*                                                               */
/* FILE        : DCU.c                                           */
/* DATE        : 2015-12-24                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : source file of DCU module                       */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-24          */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "SIUL.h"
#include "DCU.h"

DCU_CallbackType DCU_CallbackLSBFVS;

#define FREQ  64u

static void delay(uint32_t delayCnt)
{
    uint32_t i, j;
    for(i = 0; i < delayCnt; i++)
    {
        for(j = 0; j < delayCnt; j++)
        {
            __asm(" nop");
        }
    }
}

/*
 * Name        : DCU_Init
 * Description : initial DCU registers
 * Arguments   : none
 * Return      : none
 */
void DCU_Init(void)
{
	uint32 i;

    DCU.DCUMODE.R = 0x00000000;    /* enable DCU */
    DCU.BGND.R = 0x00000000;    //Background colour = black

	/* Configure port A as output (red & green) */
	for(i = PA0; i < PB0; i++)
	{
	    SIU.PCR[i].R = 0x0600;    /* Enable output buffer */
	}
	
	/* Configure port PG0..7 as output (blue) */
	for(i=PG0;i<PG8;i++)
	{
		SIU.PCR[i].R = 0x0600;	//Enable output buffer
	}

	/* Configure port PG8..11 as control */
    SIU.PCR[PG8].R = 0x0600;   /* Vsynch */
	SIU.PCR[PG9].R = 0x0600;   /* Hsynch */
    SIU.PCR[PG10].R = 0x0600; /* DE just pull-up - connected to display ON */
	SIU.PCR[PG11].R = 0x0600; /* Pclk */

	/* Fast slew rate */
	SIU.PCR[PA14].B.SRC = 1;
	SIU.PCR[PG0].B.SRC = 1;
	SIU.PCR[PG2].B.SRC = 1;
	SIU.PCR[PG4].B.SRC = 1;
	SIU.PCR[PG6].B.SRC = 1;
		
    /* DCU */
    DCU.DSPSIZE.B.DELTAY = 480;	 /* Y = 240 */
    DCU.DSPSIZE.B.DELTAX = 40;	 /* X = 50 * 16 = 800 */

    DCU.HSYNPARA.B.BPH = 48;    /* Horizontal back porch */
    DCU.HSYNPARA.B.PWH = 96;    /* Horizontal sync pulse */
    DCU.HSYNPARA.B.FPH = 16;    /* Horizontal front porch */

    DCU.VSYNPARA.B.BPV = 33;    /* Vertical back porch */
    DCU.VSYNPARA.B.PWV = 2;	    /* Vertical sync pulse */
    DCU.VSYNPARA.B.FPV = 10;    /* Vertical front porch */

    DCU.DIVRATIO.R = (uint32)(FREQ / 21) - 1u;    /* Clock divider */

    DCU.SYNPOL.R = 1;           /* Synchronize */  

	SIU.PCR[PF0].R = 0x4200;    /* VCC3.3V  Enable */
	SIU.GPDO[PF0].B.PDO = 0;    

	SIU.PCR[PK1].R = 0x4200;    /* TFT  Enable */
	SIU.GPDO[PK1].B.PDO = 0;

	SIU.PCR[PB2].R = 0x4200;    /* TFT  BackLight Enable */
	SIU.GPDO[PB2].B.PDO = 1;

	SIU.PCR[PF9].R = 0x4200;    /* RESET */
	SIU.GPDO[PF9].B.PDO = 0; 	

	//DCU.INTMASK.R = 0xFFFFF;

    DCU.DCUMODE.B.BLEND_ITER = 3;       // Three layer blend
    DCU.DCUMODE.B.RASTER_EN = 1;        // Raster scan on
    DCU.DCUMODE.B.DCUMODE = 1;          // Enable DCU in Standard Mode
    delay(800);                         /// this delay is necessary
    SIU.GPDO[PK1].B.PDO = 1;
}

void DCU_Modeswitch(uint8 mode)
{
	DCU.DCUMODE.B.DCUMODE = mode;
    delay(800);
	/*while (DCU.DCUMODE.B.DCUMODE != mode)
    {
        // Wait for it...
    }*/
}
