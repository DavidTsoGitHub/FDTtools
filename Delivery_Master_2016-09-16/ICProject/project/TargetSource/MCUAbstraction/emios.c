/*****************************************************************/
/*                                                               */
/* FILE        : emios.c                                         */
/* DATE        : 2015-06-16                                      */
/* AUTHOR      : Yuan Jia                                        */
/* DESCRIPTION : source file of emios module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-17    */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "Platform_Types.h"
#include "emios.h"

/*
 * Name        : MCU_Set_Emios0_Channel
 * Description : Set Register value of Emios0 Channel , to output PWM waveform
 * Arguments   : u8_emiosChannel - emios channel
 *               u16_HDuration   - high duration
 * Return      : to be added
 */
void MCU_SetEmios0Channel(uint8 u8_emiosChannel, uint16 u16_HDuration)
 {
	uint32 u32t_channelValue;

	u32t_channelValue = (uint32)(1u << u8_emiosChannel);
	u32t_channelValue = ~u32t_channelValue;
	EMIOS_0.UCDIS.R &= u32t_channelValue;
	
	/*set Data Register - CADR: value = HALF_PWM_DURATION - HDuration/2; */
	EMIOS_0.CH[u8_emiosChannel].CADR.R = (HALF_PWM_DURATION - (u16_HDuration / 2u));
	
	/*set Data Register - CBDR: value = HALF_PWM_DURATION + HDuration/2; */
	EMIOS_0.CH[u8_emiosChannel].CBDR.R = (HALF_PWM_DURATION + (u16_HDuration / 2u));

	EMIOS_0.CH[u8_emiosChannel].CCR.R = 0x020000E0u;      /* Channel Control Register                 */
                                                            /* Freeze Enable : Normal operation         */
                                                            /* Output Disable :  No                     */
                                                            /* Output Disable Select : ODI 0                */
                                                            /* Prescaler Divide Ratio : 1             */
                                                            /* Prescaler Enable : Enable                */
                                                            /* Flag generation Request: Assigned to Interrupt */
                                                            /* Input Filter: Not Applicable                */
                                                            /* Filter clock select : Prescaler clock    */
                                                            /* Flag Enable : IRQ/DMA not generated      */
                                                            /* Force Match A : Disabled                 */
                                                            /* Force Match B : Disabled                 */
                                                            /* Bus Select : Counter bus[A]                */
                                                            /* Edge Selection Bit :Not Applicable                 */
                                                            /* Edge Polarity :Set on Match A                   */
                                                            /* Mode : Output Pulse Width Modulation Buff*/
                                                            /* ered(flag on B1 matches)                 */
 }
