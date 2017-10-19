/*
 * memory.c
 *
 *  Created on: Sep 29, 2015
 *      Author: Administrator
 */

#include "ee_emulation.h"
#include "ee_internal.h"
#include "BSP.h"

extern EEPROM_CONFIG eepromConf;
extern UINT32 swap_num = 0;
extern UINT32 buffer[];
extern void CallBack(void);                             /* CallBack function */

/*
 * Name        : BSP_Write_Memory
 * Description : Write data to data flash memory
 * Arguments   : *value ---- source buffer address for program and verify
 *               length ---- source buffer size to program and verify
 *               startAddress ---- start address to program and verify
 * Return      :
 */
BSP_StdReturnType BSP_Write_Memory(uint8* value,uint16 length,uint16 startAddress)
{
	UINT32 i;                                                /* index vairable */
    UINT32 ret;                                              /* store return code */
    BOOL stop_flag = FALSE;                                  /* controll when to stop writing records*/
	BOOL re_init_flag = FALSE;                               /* controll if it needs to re-init eeprom system*/    
    uint16 size;
    uint16 id;
    size = length;
    id = startAddress;
#if  (SIZE_FIELD == 0)                                        /* Write eeprom data records */
	size = DATA_SIZE;
#endif
	while (stop_flag == FALSE)
	{
        #if (SIZE_FIELD >0)
		//size = id;
		if(size > BUFFER_SIZE)
		{
		    size = BUFFER_SIZE;
		}
		#endif
		        
		for (i = 0; i < size; i++)
		{
		    WRITE8((UINT32)buffer + i, value[i]);
		}
		ret = FSL_WriteEeprom(&eepromConf,id,size,(UINT32)buffer,&CallBack);
		if(ret == EE_OK)
	    {
		    stop_flag = TRUE;                                /* if have succeed written, stop writing records*/
		    while(eraseStatus_Flag == ERASE_INPROGRESS)
			{
				ret = FSL_MainFunction(&eepromConf,&CallBack);
			}
		    if (TRUE == stop_flag)
		    {
		        swap_num ++;
		    }
		        
		    if(eraseStatus_Flag == ERASE_SWAPERROR)
		    {
		        re_init_flag = TRUE;
		    }
		}
		else
		{
		    re_init_flag = TRUE;
		}

		if (re_init_flag == TRUE)
		{
		    ret = FSL_InitEeprom(&eepromConf,&CallBack);      /* Call to FSL_InitEeeprom to try recovering eeprom system */
		    if(ret != EE_OK)
		    {
		        return BSP_INVALID_RANGE;
		    }
		}
		//id = (++id)%BUFFER_SIZE;
    }	
	return BSP_OK;
}

/*
 * Name        : BSP_Read_Memory
 * Description : Read data from data flash memory
 * Arguments   : *value ---- source buffer address to read data
 *               length ---- source buffer size to read data
 *               startAddress ---- start address to read data
 * Return      :
 */
BSP_StdReturnType BSP_Read_Memory(uint8* value,uint16 length,uint16 startAddress)
{
	uint32 ret; 
    ret = FSL_ReadEeprom(&eepromConf, startAddress,length,(UINT32)value,&CallBack);
    if(ret != EE_OK)
    {
    	return BSP_INVALID_RANGE;
    }
	return BSP_OK;
}

/*
 * Name        : BSP_ReportError_Memory
 * Description : while an error occurs, return back error message
 * Arguments   : 
 * Return      :
 */
BSP_StdReturnType BSP_ReportError_Memory(uint8* FTB)
{
	*FTB = 0;
	return BSP_OK;
}
