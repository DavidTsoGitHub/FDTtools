/*****************************************************************/
/*                                                               */
/* FILE        : Gauge.h                                         */
/* DATE        : 2015-06-17                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : header file of motor module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-17          */
/*                                                               */
/*****************************************************************/
#ifndef GAUGE_H__
#define GAUGE_H__

#include "Platform_Types.h"
#include "BSP.h"

#define SPEEDOMETER 		 			0u
#define ENGINERPM 			 			1u
#define FUELMETER      		 		    2u
#define WATERCOOLANTTEMP	 		    3u
#define LEFTBRAKEPRESSURE    		    4u
#define RIGHTBRAKEPRESSURE	  			5u

#define IGNITION_OFF_RETURNZERO_PERIOD 	10u

extern uint8 CurrentStateflag;
extern uint16 ArraGaugePos[6];

extern void PIT_Isr3(void);
extern void Accelerate_Backzero(uint8 u8t_TotalNumber,uint8 Return_Zero_Mode);
extern void ECU_StepMotor_Init(void);

#endif
