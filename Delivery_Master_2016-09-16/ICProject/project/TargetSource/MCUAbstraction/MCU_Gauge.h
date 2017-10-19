/*****************************************************************/
/*                                                               */
/* FILE        : MCU_Gauge.h                                     */
/* DATE        : 2015-06-17                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : header file of motor module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-17          */
/*                                                               */
/*****************************************************************/
#ifndef MCU_GAUGE_H__
#define MCU_GAUGE_H__

#include "jdp.h"
#include "Platform_Types.h"

/* the microstep stage of motor moving */	
#define LEVEL6 218
#define LEVEL5 211
#define LEVEL4 189
#define LEVEL3 139
#define LEVEL2 80
#define LEVEL1 41
#define LEVEL0 0

typedef struct _ST_STEPMOTOR_CTRL
{
	sint8  DIR; 					/* the motor's moving direct */
	uint16 TAR_POS;					/* the Taget's postion of motor */
	uint16 CUR_POS;					/* the current postion of motor */			
	uint8  CUR_STEP;				/* the microstep stage of motor moving */		
	uint8  ACTIONFLAG;				/* if motor will move */	
	uint8  STATE_FLAG;				/* if the signal is connected */
	sint16  Distance_Gap;
} ST_STEPMOTOR_CTRL;

extern ST_STEPMOTOR_CTRL smctrl_gauge[];

typedef struct {
	union{
		uint8 BYTE;
		struct {		
			uint8 :2;						/* reserved bits */
			uint8 Revise:1;					/* corrected value */
			uint8 BackCharacteristic:1;		/* Back to zero's characteristics */
			uint8 SignalType:1;				/* the type of signal */
			uint8 Relation:1;				/* the relation of signal and angle */
			uint8 PlateCharacteristic:1;	/* palte's characteristics */
			uint8 SamplingWay:1;			/* the way of sampling */	
			}BIT;
		}FLAG;
}ST_METER;

extern ST_METER Meter0_Flag,Meter1_Flag,Meter2_Flag,Meter3_Flag,Meter4_Flag,Meter5_Flag;

#define STEPMOTORMAXSPEED 360*MICROSTEP /* micro steps every second, 4320microsteps  = 360 degrees */

/*!***************************************
*define every motor's max steps and min steps 
******************************************/
#define STEPMOTOR0MIN 0
#define STEPMOTOR0MAX 3780 /* 3780\12=315бу */

#define STEPMOTOR1MIN 0
#define STEPMOTOR1MAX 3780             

#define STEPMOTOR2MIN 0
#define STEPMOTOR2MAX 3780

#define STEPMOTOR3MIN 0
#define STEPMOTOR3MAX 3780

#define STEPMOTOR4MIN 0
#define STEPMOTOR4MAX 3780

#define STEPMOTOR5MIN 0
#define STEPMOTOR5MAX 3780

#define DirectOfSMCMove(num)  	SMC.DC[num].B.S
#define SMCDutyCycleLength(num) SMC.DC[num].B.D

#define REST 						0xffffu
#define NUM_OF_FILTER				3u

#define DO_NOTING					0x00u
#define NORMAL_OPRATION				0x01u
#define BIG_RETURN_ZERO 	 		0x02u
#define IGNITION_OFF_RETURN_ZERO	0x03u

#define BIG_RETURN_ZERO_METHOD_1	0x04u	/* return to stop position */
#define BIG_RETURN_ZERO_METHOD_2	0x05u	/* return to stop position,then to 0scale'position */

void ms_backzero(void);
void ms_forward(void); 
void StepMotorUpdate(uint8 GaugeId, uint16 GaugePos);

void UpdateMicroStep(uint8 motor, uint8 step);
void Speed_Ctrl_ISR(void);
void Motor_Speed(uint8 motor);
void Tacho_Ctrl_ISR(void);
void Motor_Tacho(uint8 motor);

void Motor_Driver(uint8 motor);
void pwm_ccw(uint8 motor);
void pwm_cw(uint8 motor);
void Turn_CW(uint8 motor);
void Turn_CCW(uint8 motor);

extern uint16 calculate_ang(uint8 motorId ,uint16 meter_rf,uint16 data_input,uint16 *corres_value,uint16 *ang_pointer);
extern uint8  u8g_returnZeroMode,u8g_BigReturnZeroSpeedStage;
extern uint16 ArraGaugePos[6];

extern uint16 speed_speed[NUM_OF_FILTER];
extern uint8  speed_inc;
extern uint16 tacho_speed[NUM_OF_FILTER];
extern uint8  tacho_inc;

extern uint16 u16g_CallMotor0MoveOneStepTime;
extern uint16 u16g_CallMotor1MoveOneStepTime;

extern uint16 u16g_CallMotor3MoveOneStepTime;
extern uint16 u16g_CallMotor4MoveOneStepTime;
extern uint16 u16g_CallMotor5MoveOneStepTime;

#endif
