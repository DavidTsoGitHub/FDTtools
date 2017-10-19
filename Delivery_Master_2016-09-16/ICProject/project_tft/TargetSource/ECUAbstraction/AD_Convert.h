/*
 * AD_Convert.h
 *
 *  Created on: Sep 16, 2015
 *      Author: Administrator
 */

#ifndef AD_CONVERT_H_
#define AD_CONVERT_H_


/*0 show direct proportion; 1 show invers proportion*/
extern unsigned char direct_proportion;

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
extern unsigned short BSP_GetEnginValue(unsigned short meter_rf,unsigned short data_input,unsigned short *meter_value_ref, unsigned short *engin_value_ref,unsigned short vol_old_num,unsigned char signal_type,unsigned short k_voltage, unsigned short k_ad);

#endif /* AD_CONVERT_H_ */
