/*                                                               */
/* FILE        : MCU_Gauge.c                                     */
/* DATE        : 2015-06-24                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of motor module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-24          */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "MCU_Gauge.h"
#include "Gauge.h"
#include "PIT.h"
#include "CAL.h"
#include "Cal_cfg.h"


uint16 speed_speed[NUM_OF_FILTER];
uint8  speed_inc;

uint16 tacho_speed[NUM_OF_FILTER];
uint8  tacho_inc;

ST_METER Meter0_Flag,Meter1_Flag,Meter2_Flag,Meter3_Flag,Meter4_Flag,Meter5_Flag;
ST_STEPMOTOR_CTRL smctrl_gauge[6];

/*For micro step*/
static const sint16 PWM1_WaveData[24] =
{
	LEVEL0,  LEVEL1,  LEVEL2,  LEVEL3,  LEVEL4,  LEVEL5, LEVEL6, LEVEL5, 
	LEVEL4, LEVEL3,LEVEL2, LEVEL1, LEVEL0, -LEVEL1, -LEVEL2, -LEVEL3,
	-LEVEL4, -LEVEL5, -LEVEL6, -LEVEL5, -LEVEL4, -LEVEL3, -LEVEL2, -LEVEL1 
};

static const sint16 PWM2_WaveData[24]=
{
	LEVEL6, LEVEL5, LEVEL4,LEVEL3,LEVEL2, LEVEL1,LEVEL0, -LEVEL1, -LEVEL2,
	-LEVEL3, -LEVEL4, -LEVEL5, -LEVEL6, -LEVEL5, -LEVEL4, -LEVEL3, -LEVEL2,
	-LEVEL1, LEVEL0,LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5
};

/*
 * Name         : void UpdateMicroStep(uint8 motor, uint8 step)
 * Description  : Update Micro Step
 * Arguments    : motor - index of motor
 *              : step  - steps of motor'moving 
 * Return       : void
 *                   
 */
void UpdateMicroStep(uint8 motor, uint8 step)
{
	if (PWM1_WaveData[step] < 0)		
	{
		/*P = "Low" , M = "PWM",SIGN[4] = 1;*/
		DirectOfSMCMove(motor * 2) = 0x10;
		SMCDutyCycleLength(motor * 2) = - PWM1_WaveData[step];
	}
	else
	{
		/*P = "PWM" , M = "Low" //SIGN[4] =0;*/
		DirectOfSMCMove(motor * 2) = 0x00;
		SMCDutyCycleLength(motor * 2) = PWM1_WaveData[step];
	}


	if (PWM2_WaveData[step]<0)		
	{
		/*P = "Low" , M = "PWM",SIGN[4] = 1;*/
		DirectOfSMCMove(2 * motor + 1) = 0x10;
		SMCDutyCycleLength(2 * motor + 1) = - PWM2_WaveData[step];
	}
	else
	{
		/*P = "PWM" , M = "Low" //SIGN[4] =0;*/
		DirectOfSMCMove(2 * motor + 1) = 0x00;
		SMCDutyCycleLength(2 * motor + 1) = PWM2_WaveData[step];
	}
}
/*
 * Name         : void pwm_ccw(unsigned char motor)
 * Description  : pwm signal driver motor moving  anti-clockwise  
 * Arguments    : motor - index of motor
 * 
 * Return       : void
 *                   
 */
void pwm_ccw(unsigned char motor)
{
	UpdateMicroStep(motor,smctrl_gauge[motor].CUR_STEP);
	
	if (smctrl_gauge[motor].CUR_STEP <= 0)
	{
		smctrl_gauge[motor].CUR_STEP = 23;
	}
	else
	{
		smctrl_gauge[motor].CUR_STEP--;
	}
	
}

/*
 * Name         : pwm_cw(unsigned char motor)
 * Description  : pwm signal driver motor moving clockwise
 * Arguments    : motor - index of motor
 *              : 
 * Return       : void
 *                   
 */
void pwm_cw(unsigned char motor)
{
	UpdateMicroStep(motor,smctrl_gauge[motor].CUR_STEP);
	
	smctrl_gauge[motor].CUR_STEP++;
	if (smctrl_gauge[motor].CUR_STEP >= 24u)
	{
		smctrl_gauge[motor].CUR_STEP = 0;
	} 
	else{
	}
		
}

/*
 * Name         : void ms_backzero(void)
 * Description  : driver motor back to 0 dial 
 * Arguments    : void
 *              : 
 * Return       : void
 *                   
 */
void ms_backzero(void)
{
	uint8 parameter;
	
	Turn_CCW(SPEEDOMETER);
	Turn_CCW(ENGINERPM);
	
	CAL_Read8(FUEL_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CCW(FUELMETER);
	
	CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CCW(WATERCOOLANTTEMP);
	
	CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CCW(LEFTBRAKEPRESSURE);
	
	CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CCW(RIGHTBRAKEPRESSURE);
}

/*
 * Name         : void ms_forward(void)
 * Description  : driver motor move 1 step 
 * Arguments    : void
 *              : 
 * Return       : void
 *                   
 */
void ms_forward(void)	
{
	uint8 parameter;
	
	Turn_CW(SPEEDOMETER);
	Turn_CW(ENGINERPM);
	
	CAL_Read8(FUEL_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CW(FUELMETER);
	
	CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CW(WATERCOOLANTTEMP);
	
	CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CW(LEFTBRAKEPRESSURE);
	
	CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);
	if((parameter&0x08) == 0x00)
		Turn_CW(RIGHTBRAKEPRESSURE);
}

/*
 * Name         : void Turn_CW(uint8 motor)
 * Description  : driver motor moving clockwise according conditions
 * Arguments    : motor - index of motor
 *              : 
 * Return       : void
 *                   
 */
void Turn_CW(uint8 motor)
{
	uint8 parameter,u8t_NormalMoving_Clockwise; 
	
	switch (motor)
	{
		case(SPEEDOMETER):
			CAL_Read8(VEHSPD_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
			
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter0_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter0_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
	
			break;
				
		case(ENGINERPM):
			CAL_Read8(ENGINERPM_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
			
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter1_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter1_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			break;

		case(FUELMETER):
			CAL_Read8(FUEL_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
			
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter2_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter2_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
	
			break;
		
		case(WATERCOOLANTTEMP):
			CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
					
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter3_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter3_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
	
			break;
				
		case(LEFTBRAKEPRESSURE):
		    CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
							
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter4_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter4_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
	
			break;
				
		case(RIGHTBRAKEPRESSURE):
		    CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
									
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter5_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			else
			{
				if(Meter5_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
	
			break;					
				
		default:
			break;
	}
	
	smctrl_gauge[motor].CUR_POS++;
}
/*
 * Name         : void Turn_CCW(uint8 motor)
 * Description  : driver motor moving anti-clockwise according conditions
 * Arguments    : motor - index of motor
 *              : 
 * Return       : void
 *                   
 */
void Turn_CCW(uint8 motor)
{
	uint8 parameter,u8t_NormalMoving_Clockwise; 
	
	switch (motor)
	{
		case(SPEEDOMETER):
			CAL_Read8(VEHSPD_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
			
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter0_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter0_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			break;
			
		case(ENGINERPM):
		    CAL_Read8(ENGINERPM_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
					
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter1_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter1_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
			break;
		
		case(FUELMETER):
			CAL_Read8(FUEL_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
			
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter2_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter2_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
	
			break;
		
		case(WATERCOOLANTTEMP):
			CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
						
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter3_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter3_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
	
			break;
				
		case(LEFTBRAKEPRESSURE):
			CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
									
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter4_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter4_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
	
			break;
				
		case(RIGHTBRAKEPRESSURE):
			CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);
			u8t_NormalMoving_Clockwise = (parameter&0x02) >> 1u;
											
			if(u8t_NormalMoving_Clockwise == 0x00u)
			{
				if(Meter5_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_ccw(motor);
				else
					pwm_cw(motor);
			}
			else
			{
				if(Meter5_Flag.FLAG.BIT.PlateCharacteristic == 0) 
					pwm_cw(motor);
				else
					pwm_ccw(motor);
			}
	
			break;					
								
		default:
				break;
	}
		
	smctrl_gauge[motor].CUR_POS--;
}
/*
 * Name         : void Motor_Driver(uint8 motor)
 * Description  : driver motor moving  according conditions ,calculate the gap between current position and target position
 * Arguments    : motor - index of motor
 *              : 
 * Return       : void
 *                   
 */
void Motor_Driver(uint8 motor)
{
	if (smctrl_gauge[motor].ACTIONFLAG != 0) 
	{         
		/* motors will move */
		if (smctrl_gauge[motor].CUR_POS == smctrl_gauge[motor].TAR_POS) /* if the current position equals the taget's position ,the motor will stop */
		{      
			/* even the motor stop running ,but the  magnetic force is alwas present */ 
			smctrl_gauge[motor].ACTIONFLAG = 0;	
			return;
		} 
				
		/* calculate the abslute gap between the current position and taget's position and update the flag of distance */

		smctrl_gauge[motor].Distance_Gap = ((sint16)smctrl_gauge[motor].TAR_POS - (sint16)smctrl_gauge[motor].CUR_POS);

		if (smctrl_gauge[motor].Distance_Gap >= 0) 
		{			
				Turn_CW(motor);        /* clockwise rotate */		
		}
		else 
		{			
				Turn_CCW(motor);       /* anticlockwise rotate */		
		}
		
     }
}

void Motor_Speed (uint8 motor)
{
	uint8  dir_c,i;
	uint32  total,speed_step;
	
	dir_c=0;
	/* motors will move */
	if (smctrl_gauge[motor].ACTIONFLAG != 0u) 
	{          
		/* if the current position equals the taget's position ,the motor will stop */
		if (smctrl_gauge[motor].CUR_POS == smctrl_gauge[motor].TAR_POS) 
		{      
			/* even the motor stop running ,but the  magnetic force is alwas present */ 
			smctrl_gauge[motor].ACTIONFLAG = 0;
			
			return;
		} 
		
		dir_c = 0u;
		/* calculate the abslute gap between the current position and taget's position and update the flag of distance */
		smctrl_gauge[motor].Distance_Gap = ((sint16)smctrl_gauge[motor].TAR_POS - (sint16)smctrl_gauge[motor].CUR_POS);
			
		if (smctrl_gauge[motor].Distance_Gap >= 0) 
		{
			if (smctrl_gauge[motor].DIR == (-1))  
				dir_c = 1; 
				
			smctrl_gauge[motor].DIR = 1;		/* clockwise rotate */		
		}
		else 
		{
			smctrl_gauge[motor].Distance_Gap = smctrl_gauge[motor].Distance_Gap*(-1);
				
			if (smctrl_gauge[motor].DIR == 1)  
				dir_c = 1; 
					
			smctrl_gauge[motor].DIR = -1;		/* anticlockwise rotate */		
		}
		
		total = 500000ul / ((uint32)smctrl_gauge[motor].Distance_Gap);
		 
		if (total > 65535u)
			speed_step = 65535u;
		else
			speed_step = total;
		  
		if (speed_step < 250u) 
			speed_step = 250u;					/* it will get max speed at this time */
		
		total = 0u;
		
		for (i=0; i<NUM_OF_FILTER; i++)			/* sum the coefficent */
			total = total + speed_speed[i];
		
		total = total - speed_speed[speed_inc];
		total = total + speed_step;
		speed_step = total / NUM_OF_FILTER;
		
		speed_speed[speed_inc] = (uint16)speed_step;
		
		speed_inc ++;
		if (speed_inc == NUM_OF_FILTER) 
			speed_inc = 0;
		
		PIT.CH[1].LDVAL.R  = 64 * speed_step;  
		
		if (smctrl_gauge[motor].STATE_FLAG == 0u)
		{
			PIT.CH[1].TCTRL.B.TEN = 0x1;  		/* start timering */
			smctrl_gauge[motor].STATE_FLAG = 1u;
		}  
		else
		{
			if (dir_c == 1u)
			{
				dir_c = 0u;
				
				PIT.CH[1].TCTRL.B.TEN = 0x0; 
				smctrl_gauge[motor].STATE_FLAG = 0u;
				
				for (i=0; i<NUM_OF_FILTER; i++)
					speed_speed[i] = 0xFFFF;
			}
		}		 
	}
}

/*
 * Name        : Speed_Ctrl_ISR
 * Description : deal with speed motor's routine
 * Arguments   : none
 * Return      : none
 */
void Speed_Ctrl_ISR(void)
{
	CLEAR_PIT_INT_FLAG(1);       								/* clear PIT 1 interrupt flag */

	if (smctrl_gauge[0].CUR_POS == smctrl_gauge[0].TAR_POS) 
	{
		PIT.CH[1].TCTRL.B.TEN = 0x0; 							/* stop the pit1 timer */
		smctrl_gauge[0].STATE_FLAG = 0;                         /* motor stops running */
		
		return;	
    }
	
	if (smctrl_gauge[0].DIR == 1)
	{
		Turn_CW(0);
	}	
	else
	{
		Turn_CCW(0);
	}   	
} 


void Motor_Tacho(uint8 motor)
{
	uint8  dir_c,i;
	uint32  total,speed_step;
	
	dir_c = 0u;

	/* motors will move */
	if (smctrl_gauge[motor].ACTIONFLAG != 0) 
	{         
		/* if the current position equals the taget's position ,the motor will stop */
		if (smctrl_gauge[motor].CUR_POS == smctrl_gauge[motor].TAR_POS) 
		{          
			/* even the motor stop running ,but the  magnetic force is alwas present */ 
			smctrl_gauge[motor].ACTIONFLAG = 0;
			
			return;
		} 
		
		
		dir_c = 0;
		/* calculate the abslute gap between the current position and taget's position and update the flag of distance */
		smctrl_gauge[motor].Distance_Gap = ((sint16)smctrl_gauge[motor].TAR_POS - (sint16)smctrl_gauge[motor].CUR_POS);
			
		if (smctrl_gauge[motor].Distance_Gap >= 0) 
		{				
			if (smctrl_gauge[motor].DIR == (-1))  
				dir_c = 1u; 
			
			smctrl_gauge[motor].DIR = 1;         /* clockwise rotate */			
		}
		else 
		{	
			smctrl_gauge[motor].Distance_Gap = smctrl_gauge[motor].Distance_Gap*(-1);
			
			if (smctrl_gauge[motor].DIR == 1)  
				dir_c = 1u; 
				
			smctrl_gauge[motor].DIR = -1;     /* anticlockwise rotate */		
		}
		
		 total = 500000ul / (uint32)smctrl_gauge[motor].Distance_Gap;
		 
		if (total > 65535u)
		  speed_step = 65535u;
		else
		  speed_step = total;
		  
		if (speed_step < 250u) 
		  speed_step = 250u;
		
		total = 0;
		for (i=0; i<NUM_OF_FILTER; i++)
		  total = total+ tacho_speed[i];
		  
		total = total - tacho_speed[speed_inc];
		total = total + speed_step;
		speed_step = total / NUM_OF_FILTER;
		
		tacho_speed[tacho_inc] = (uint16)speed_step;
		
		tacho_inc ++;
		if (tacho_inc == NUM_OF_FILTER) 
		  tacho_inc = 0;
		
		PIT.CH[2].LDVAL.R  = 64 * speed_step;  
		
		if (smctrl_gauge[motor].STATE_FLAG == 0u)
		{
			PIT.CH[2].TCTRL.B.TEN = 0x1;   
			smctrl_gauge[motor].STATE_FLAG = 1u;
		}  
		else
		{
			if (dir_c == 1u)
			{
				dir_c = 0;
				PIT.CH[2].TCTRL.B.TEN = 0x0u;  
				smctrl_gauge[motor].STATE_FLAG = 0u;

				for (i=0; i<NUM_OF_FILTER; i++)
				  tacho_speed[i] = 0xFFFF;
			}
		}		 
	}
}

/*
 * Name        : Speed_Ctrl_ISR
 * Description : deal with tacho motor's routine
 * Arguments   : none
 * Return      : none
 */
void Tacho_Ctrl_ISR(void)
{
	CLEAR_PIT_INT_FLAG(2);       						/* clear PIT 2 interrupt flag */
	
	if (smctrl_gauge[1].CUR_POS == smctrl_gauge[1].TAR_POS) 
	{
		PIT.CH[2].TCTRL.B.TEN = 0x0; 

		smctrl_gauge[1].STATE_FLAG = 0;                 /*motor stops running*/
		
		return;
	}

	if (smctrl_gauge[1].DIR == 1)						/*clock rotor*/
	{ 
		Turn_CW(1);
	}	
	else
	{
		Turn_CCW(1);
	}   

}  
