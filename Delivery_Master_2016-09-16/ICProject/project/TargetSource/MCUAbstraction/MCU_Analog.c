/*****************************************************************/
/*                                                               */
/* FILE        : MCU_Analog.c                                    */
/* DATE        : 2015-06-15                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of Analog module in MCU             */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-17    */
/*                                                               */
/*****************************************************************/
#include "Platform_Types.h"
#include "jdp.h"
#include "IODefine.h"
#include "SIUL.h"
#include "MCU_Analog.h"



void MCU_OpenADSwitchPort(uint8 u8_port1,uint8 u8_port2,uint8 u8_port3)
{
	SIUL_Output(PORT_A, u8_port1);
	SIUL_Output(PORT_B, u8_port2);
	SIUL_Output(PORT_C, u8_port3);
}
