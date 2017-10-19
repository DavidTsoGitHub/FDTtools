/*                                                               */
/* FILE        : ECU_RTC.c                                       */
/* DATE        : 2015-06-18                                      */
/* AUTHOR      : Zhu Mingqi                                      */
/* DESCRIPTION : source file of RTC module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Zhu Mingqi on 2015-06-18       */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "Platform_Types.h"
#include "RTC.h"
#include "SIUL.h"
#include "BSP.h"
#include "MCU_RTC.h"

#define YEAR_OFFSET   2015u

static uint32 u32g_RtcConvert;
static uint8  u8g_RtcYear;
static uint8  u8g_RtcMonth;
static uint8  u8g_RtcDay;
static uint8  u8g_RtcHour;
static uint8  u8g_RtcMinute;
static uint8  u8g_RtcSecond;

/*
 * Name        : ECU_Rtc_Init
 * Description : ECU Interface to initialize RTC variables
 * Arguments   : none
 * Return      : none
 */
void ECU_Rtc_Init(void)
{
	u8g_RtcYear   = 0u;
	u8g_RtcMonth  = 0u;
	u8g_RtcDay    = 0u;
	u8g_RtcHour   = 0u;
	u8g_RtcMinute = 0u;
	u8g_RtcSecond = 0u;
}

/*
 * Name        : MCU_Api_Int
 * Description : MCU API Interrupt
 */
void MCU_Api_ISR(void)
{
	uint16 u16t_RtcYear;

	CLEAR_RTC_API_INT_FLAG();
	
	u16t_RtcYear = (uint16)u8g_RtcYear + 2015u;
	if(u8g_RtcSecond < 60u)
	{
		u8g_RtcSecond++;
	}
	if(u8g_RtcSecond == 60u)
	{
		u8g_RtcSecond = 0;
		if(u8g_RtcMinute < 60u)
		{
			u8g_RtcMinute++;
		}
		if(u8g_RtcMinute == 60u)
		{
			u8g_RtcMinute = 0;
			if(u8g_RtcHour < 24u)
			{
				u8g_RtcHour++;
			}
			if(u8g_RtcHour == 24u)
			{
				u8g_RtcHour = 0u;
				if (((u16t_RtcYear % 4u) == 0u) && ((u16t_RtcYear % 400u) == 0u))  /* leap year */
				{
					if((u8g_RtcMonth == 2u) && (u8g_RtcDay < 30u))    /* February handler*/
					{
						u8g_RtcDay++;
					}
				}
				else    /* ordinary year */
				{
					if((u8g_RtcMonth == 2u) && (u8g_RtcDay < 29u))    /* February handler*/
					{
						u8g_RtcDay++;
					}
				}
				if((u8g_RtcMonth == 1u) || (u8g_RtcMonth == 3u) || (u8g_RtcMonth == 5u) || (u8g_RtcMonth == 7u)
					 || (u8g_RtcMonth == 9u) || (u8g_RtcMonth == 10u) || (u8g_RtcMonth == 12u))
				{
					if(u8g_RtcDay < 32u)
					{
						u8g_RtcDay++;
					}
				}
				if((u8g_RtcMonth == 4u) || (u8g_RtcMonth == 6u) || (u8g_RtcMonth == 9u) || (u8g_RtcMonth == 11u))
				{
					if (u8g_RtcDay < 31u)
					{
						u8g_RtcDay++;
					}
				}
				if(((u16t_RtcYear % 4u) == 0u) && ((u16t_RtcYear % 400u) == 0u))    /* leap year */
				{
					if((u8g_RtcMonth == 2u) && (u8g_RtcDay == 30u))
					{
						u8g_RtcDay = 1;
						u8g_RtcMonth = 3;
					}
				}
				else   /* ordinary year */
				{
					if((u8g_RtcMonth == 2u) && (u8g_RtcDay == 29u))
					{
						u8g_RtcDay = 1u;
						u8g_RtcMonth = 3u;
					}
				}
				if((u8g_RtcMonth == 1u) || (u8g_RtcMonth == 3u) || (u8g_RtcMonth == 5u) || (u8g_RtcMonth == 7u)
					 || (u8g_RtcMonth == 9u) || (u8g_RtcMonth == 10u) || (u8g_RtcMonth == 12u))
				{
					if (u8g_RtcDay < 32u)
					{
						u8g_RtcDay = 1u;
						if (u8g_RtcMonth < 12u)
						{
							u8g_RtcMonth++;
						}
						else
						{
							u8g_RtcMonth = 1u;
							if (u16t_RtcYear < RTC_MAXYEAR)
							{
								u16t_RtcYear++;
							}
						}
					}
				}
				if((u8g_RtcMonth == 4u) || (u8g_RtcMonth == 6u) || (u8g_RtcMonth == 9u) || (u8g_RtcMonth == 11u))
				{
					if (u8g_RtcDay < 31u)
					{
						u8g_RtcDay = 1u;
						u8g_RtcMonth++;
					}
				}
			}
		}
	}

	u8g_RtcYear = (uint8)(u16t_RtcYear - YEAR_OFFSET);
	u32g_RtcConvert = u8g_RtcSecond;
	u32g_RtcConvert = u32g_RtcConvert + ((uint32)u8g_RtcMinute << 6u);
	u32g_RtcConvert = u32g_RtcConvert + ((uint32)u8g_RtcHour << 12u);
	u32g_RtcConvert = u32g_RtcConvert + ((uint32)u8g_RtcDay << 17u);
	u32g_RtcConvert = u32g_RtcConvert + ((uint32)u8g_RtcMonth << 22u);
	u32g_RtcConvert = u32g_RtcConvert + ((uint32)u8g_RtcYear << 26u);
}

/*
 * Name        : BSP_Set_RTC
 * Description : BSP Interface to Set RTC time
 * Arguments   : dateTime_t time
 * Return      : BSP_StdReturnType
 */
BSP_StdReturnType BSP_Set_RTC(dateTime_t time)
{
	if(time.year < YEAR_OFFSET)
	{
		return BSP_INVALID_RANGE;
	}
	u8g_RtcYear = (uint8)(time.year - YEAR_OFFSET);
	u8g_RtcMonth = time.month;
	u8g_RtcDay = time.day;
	u8g_RtcHour = time.hour;
	u8g_RtcMinute = time.minute;
	u8g_RtcSecond = time.second;
	
	return BSP_OK;
}

/*
 * Name        : BSP_Get_RTC
 * Description : BSP Interface to Get RTC time
 * Arguments   : time_t *time
 * Return      : BSP_StdReturnType
 */
BSP_StdReturnType BSP_Get_RTC(dateTime_t *time)
{
	uint32 temp;
	temp = u32g_RtcConvert;

	time->year   = (uint16)((temp >> 26u) + YEAR_OFFSET);
	time->month  = (uint8)((temp >> 22u) % 0x10u);
	time->day    =  (uint8)((temp >> 17u) % 0x20u);
	time->hour   = (uint8)((temp >> 12u) % 0x20u);
	time->minute = (uint8)((temp >> 6u) % 0x40u);
	time->second = (uint8)(temp % 0x40u);

	return BSP_OK;
}

/*
 * Name        : BSP_Get_Milliseconds
 * Description : BSP Interface to Get Milliseconds
 * Arguments   : uint32 milliseconds
 * Return      : BSP_StdReturnType
 */
BSP_StdReturnType BSP_Get_Milliseconds(uint32 * milliseconds)
{
	*milliseconds = RTC_COUNTER;
	return BSP_OK;
}


/*
 * Name        : BSP_ReportError_RTC
 * Description : BSP Interface to Get Milliseconds
 * Arguments   : uint8 *FTB
 * Return      : BSP_StdReturnType
 */
BSP_StdReturnType BSP_ReportError_RTC(uint8 *FTB)
{
	*FTB = 0;
}
