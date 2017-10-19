/*
 * AD_Convert.c
 *
 *  Created on: Sep 16, 2015
 *      Author: Administrator
 */

#include "AD_Convert.h"
static unsigned short ad_calculate_res(unsigned short meter_rf, unsigned short data_input, unsigned short *r_pointer, unsigned short vol_old_num, unsigned short k_voltage, unsigned short k_ad);
static unsigned short convert_to_engin_value(unsigned short data_input, unsigned short *meter_value_ref, unsigned short *engin_value_ref );
unsigned char direct_proportion = 0;/*0 show direct proportion; 1 show invers proportion*/

/*
 * Name        : ad_calculate_res()
 * Description : get the actual resistor value from AD data
 *               the formula to get resistor value is :
 *               R=data_input*meter_rf*k_voltage/(vol_old_num*k_ad-data_input*k_voltage)
 * Arguments   : meter_rf    ---- reference resistor value 
 *               data_input  ---- input AD value 
 *               *r_pointer  ---- a pointer to parameters array
 *               vol_old_num ---- reference voltage value
 *               k_voltage   ---- k1 value
 *               k_ad        ---- k2 value
 * Return      : the actual resistor value              
 */
unsigned short ad_calculate_res(unsigned short meter_rf, unsigned short data_input, unsigned short *r_pointer, unsigned short vol_old_num, unsigned short k_voltage, unsigned short k_ad)  
{    
	unsigned long temp1,temp2;
    unsigned short resistor_value;     
    temp1 = (unsigned long)(vol_old_num * k_ad);     /* reference voltage value multiply by k_ad */
    temp2 = (unsigned long)(data_input * k_voltage); /* input AD value multiply by k_voltage */
    
    if (temp2 < temp1)                               /* if temp2 < temp1, which means the input AD value is valid */
    {
    	temp1 = temp1 - temp2;    
        temp2 = (unsigned long)(data_input * meter_rf * k_voltage);
        temp2 = temp2/temp1;                          /* get the value of resistance */
        if (temp2 >= (*(r_pointer+15)*2))             /* if the current resistor value is twice greater than the max resistor value */
        {
            resistor_value = *(r_pointer + 15)*2;     /* resistor value is the value twice of the max resistor value */
        }
        else
        {
            resistor_value=(unsigned short)temp2;     /* else resistor value is temp2 */
        }
    }
    else                                              /* else the input AD value is invalid */
    { 
        resistor_value = 0xffff;                      /* set resistor value as 0xffff */
    }
    return resistor_value;                            /* return the value */
    
}


/*
 * Name        : convert_to_engin_value()
 * Description : convert voltage or resistor value to engineering value
 * Arguments   : data_input       ---- input value 
 *               meter_value_ref  ---- voltage or resistor value table
 *               engin_value_ref  ---- engineering value table
 * Return      : engineering value
 *               if the value is invalid, then 0xffff will be returned
 */
unsigned short convert_to_engin_value(unsigned short data_input, unsigned short *meter_value_ref, unsigned short *engin_value_ref )
{
	unsigned char i;
	unsigned short engin_value;
	unsigned long temp;
	
	if(data_input <= meter_value_ref[0])
	{
		if(data_input < meter_value_ref[0]/2)
		{
			engin_value = 0xffff;
			return engin_value;
		}
		else
		{
			engin_value = engin_value_ref[0];
			return engin_value;
		}
	}
	
	if( data_input >= meter_value_ref[0]  &&  data_input<=meter_value_ref[15])
	{
		if(direct_proportion == 0u)
		{	
			for(i = 1; i < 16; i++ )
			{
				if(data_input == meter_value_ref[i])
				{
					engin_value = engin_value_ref[i];
					return engin_value;
				}
				
				if(data_input < meter_value_ref[i])
				{
					temp = ((unsigned long)(data_input - meter_value_ref[i - 1]) * (unsigned long)(engin_value_ref[i] - engin_value_ref[i - 1]));
					engin_value = (unsigned short) (engin_value_ref[i-1] + temp/(meter_value_ref[i] - meter_value_ref[i - 1]));
					return engin_value;
				}
			}
		}
		else
		{
			for(i = 1; i < 16; i++ )
			{
				if(data_input == meter_value_ref[i])
				{
					engin_value = engin_value_ref[i];
					return engin_value;
				}
				
				if(data_input < meter_value_ref[i])
				{
					temp = ((unsigned long)(meter_value_ref[i] - data_input) * (unsigned long)(engin_value_ref[i - 1] - engin_value_ref[i]));
					engin_value = (unsigned short) (engin_value_ref[i] + temp/(meter_value_ref[i] - meter_value_ref[i - 1]));
					return engin_value;
				}
			}
		}
	}
	else if(data_input > meter_value_ref[15])
	{
		if(data_input >= meter_value_ref[15]*2)
		{
			engin_value = 0xffff;
			return engin_value;
		}
		else
		{
			engin_value = engin_value_ref[15];
			return engin_value;
		}
	}
}	
/*
 * Name        : BSP_GetEnginValue()
 * description : convert voltage or resistor value to engineering value
 * Arguments   : meter_rf   ---- reference resistor value
 *               data_input ---- input AD value
 *               meter_value_ref ---- resistor or voltage value table
 *               engin_value_ref ---- engineering value table
 *               vol_old_num     ---- reference voltage value
 *               signal_type     ---- input signal type
 *                                    1 -- voltage signal
 *                                    0 -- resistor signal
 *               k_voltage       ---- k1 value
 *               k_ad            ---- k2 value
 * Return      : Engineering value
 *               if actual engineering value contains decimal fraction, then the decimal fraction will be abandoned, 
 *               and the smaller the actual engineering value, the bigger the inaccuracy  will be caused
 *               if signal_type or data_iput is invalid, then 0xffff will be returned  
 */
unsigned short BSP_GetEnginValue(unsigned short meter_rf,unsigned short data_input,unsigned short *meter_value_ref, unsigned short *engin_value_ref,unsigned short vol_old_num,unsigned char signal_type,unsigned short k_voltage, unsigned short k_ad)
{
    unsigned short resistor_value, engin_value;
    if(signal_type == 0)
    {   
        resistor_value = ad_calculate_res(meter_rf, data_input, meter_value_ref, vol_old_num, k_voltage, k_ad);
        engin_value = convert_to_engin_value(resistor_value,meter_value_ref,engin_value_ref);
    }
    else if(signal_type == 1)
    {
    	engin_value = convert_to_engin_value(data_input/meter_rf, meter_value_ref,engin_value_ref);
    }
    else
    {
    	engin_value = 0xffff;
    }
    return engin_value;
}
