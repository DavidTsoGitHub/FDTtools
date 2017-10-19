/*****************************************************************/
/*                                                               */
/* FILE        : Watchdog.c                                      */
/* DATE        : 2015-04-20                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : source file of watchdog module                  */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao ChenGuang on 2015-04-20    */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "Watchdog.h"

/*
 * Name        : MCU_DisableWatchdog
 * Description : disable watchdog
 * Arguments   : none
 * Return      : none
 */
void MCU_DisableWatchdog(void)
{
  SWT.SR.R = 0x0000c520; /* key */
  SWT.SR.R = 0x0000d928; /* key */
  SWT.CR.R = 0x8000010A; /* disable WEN */
}  

/*
 * Name        : MCU_EnableWatchdog
 * Description : enable watchdog
 * Arguments   : none
 * Return      : none
 */
void MCU_EnableWatchdog(void)
{
  SWT.SR.R = 0x0000c520; /* key */
  SWT.SR.R = 0x0000d928; /* key */
  SWT.TO.R = 0x0001F400; /* 1000ms timeout */
  SWT.CR.R = 0x8000010B; /* Enable WEN */
}

void MCU_ClearWatchdog(void)
{
    SWT.SR.B.WSC = 0xA602;    
    SWT.SR.B.WSC = 0xB480;
}
