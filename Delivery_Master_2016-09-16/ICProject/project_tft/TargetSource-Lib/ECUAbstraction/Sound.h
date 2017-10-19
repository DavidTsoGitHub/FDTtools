/*****************************************************************/
/*                                                               */
/* FILE        : Sound.h                                         */
/* DATE        : 2015-06-04                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of Sound module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-04    */
/*                                                               */
/*****************************************************************/
#ifndef SOUND_H
#define SOUND_H

#define TICKTACK_HIGH           160000u
#define TICKTACK_LOW            500000u
#define TICKTACK_DURAION        660000u

/*
 * Name        : BSP_ActiveBuzzerRun
 * Description : Active buzzer output
 * Arguments   : none
 * Return      : none
 */
extern void BSP_ActiveBuzzerRun(void);

/*
 * Name        : ECU_Sound_Init
 * Description : initialize sound devices
 * Arguments   : none
 * Return      : none
 */
extern void ECU_Sound_Init(void);

/*
 * Name        : SglISR
 * Description : SGL ISR
 * Arguments   : none
 * Return      : none
 */
extern void SglISR(void);

#endif
