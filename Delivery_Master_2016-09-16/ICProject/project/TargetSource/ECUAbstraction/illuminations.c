/*****************************************************************/
/*                                                               */
/* FILE        : illunimation.c                                  */
/* DATE        : 2015-08-25                                      */
/* AUTHOR      : Zhu Mingqi                                      */
/* DESCRIPTION : source file of illumination module              */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao Zhu Mingqi on 2015-08-25   */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "Platform_Types.h"
#include "BSP.h"

#include "illuminations.h"
#include "SIUL.h"
#include "IODefine.h"
#include "emios.h"

#define ILLUMINATIONLEVELNUM 6
#define ILLUMINATIONPERCENT (100 / (ILLUMINATIONLEVELNUM - 1))

/* Display Illumination grade table */
static const uint16 u16g_DispTbl[ILLUMINATIONLEVELNUM] = {0u, 12u, 80u, 161u, 230u, 345u};

 /*Gauge Illumination grade table */
static const uint16 u16g_GaugeTbl[ILLUMINATIONLEVELNUM] = {0u, 80u, 161u, 230u, 460u, 598u};

/*
 * Name        : BSP_Set_Illumination
 * Description : set back light illumination, for display and gauge 
 * Arguments   : none       
 * Return      : to be added 
 */
void BSP_Set_Illumination(uint8 ChannelId, uint8 IllumLevel)  
{
	uint16 u16t_HDuration;
	uint8 u8t_emios_channel, u8t_LevelIndex;

	if(ChannelId == 1u)                           /* display illumination */
	{
        u8t_emios_channel = EMIOSCHANNEL0;          /* set corresponding emios0 channel for display illumination */
        u8t_LevelIndex = IllumLevel / ILLUMINATIONPERCENT;
        u16t_HDuration = (u16g_DispTbl[u8t_LevelIndex + 1] - u16g_DispTbl[u8t_LevelIndex]) * (IllumLevel - ILLUMINATIONPERCENT * u8t_LevelIndex) / ILLUMINATIONPERCENT
                				+ u16g_DispTbl[u8t_LevelIndex];
        SIUL_SetMode2AsEmiosOutput(LCD_BL);         /* set corresponding pin mode for display illumination*/
	}
	else if(ChannelId == 0u)                        /* gauge illumination */
	{
        u8t_emios_channel = EMIOSCHANNEL1;          /* set the corresponding emios0 channel for gauge illumination */
        u8t_LevelIndex = IllumLevel / ILLUMINATIONPERCENT;
        u16t_HDuration = (u16g_DispTbl[u8t_LevelIndex + 1] - u16g_DispTbl[u8t_LevelIndex]) * (IllumLevel - ILLUMINATIONPERCENT * u8t_LevelIndex) / ILLUMINATIONPERCENT
        				+ u16g_DispTbl[u8t_LevelIndex];
        SIUL_SetMode3AsEmiosOutput(GAUGE_BL);       /* set the corresponding pin mode for gauge illumination*/
	}
	else             /* if channelId is unsupported, do nothing */
	{
        return ;                                   
	}
	MCU_SetEmios0Channel(u8t_emios_channel, u16t_HDuration);
}


/*
 * Name        : BSP_ReportError_Illumination
 * Description : while an error exists, return back the error message 
 * Arguments   : none       
 * Return      : to be added
 */
void BSP_ReportError_Illumination(uint8 ChannelId, uint8 *FTB) 
{
	ChannelId = ChannelId;
	*FTB = 0;
}
