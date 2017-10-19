/*****************************************************************/
/*                                                               */
/* FILE        : Indicator.c                                     */
/* DATE        : 2015-05-14                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : source file of indicator module                 */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*****************************************************************/
#include "jdp.h"
#include "BSP.h"
#include "Indicator.h"
#include "SIUL.h"
#include "IODefine.h"
#include "MCU_SwitchMode.h"

/* -------------------start of calibration section-------------------------*/
static const uint8 u8g_PhyPin_Tbl[INDICATOR_NUM] =   /* mapping from indicator ID to pin ID */
{
    0x0Fu,  /* 0x00, Group=0; Cascade=0; PIN=out15: 0000 1111. Tractor turn left lamp */
    0x0Eu,  /* 0x01, Group=0; Cascade=0; PIN=out14: 0000 1110. Trailer turn left lamp */
    0x0Du,  /* 0x02, Group=0; Cascade=0; PIN=out13: 0000 1101. Tractor turn right lamp */
    0x0Cu,  /* 0x03, Group=0; Cascade=0; PIN=out12: 0000 1100. Trailer turn right lamp */
    0x0Bu,  /* 0x04, Group=0; Cascade=0; PIN=out11: 0000 1011. Water temperature overhigh warning */
    0x0Au,  /* 0x05, Group=0; Cascade=0; PIN=out10: 0000 1010. Front fog lamp */
    0x09u,  /* 0x06, Group=0; Cascade=0; PIN=out09: 0000 1001. Water in fuel indicator */
    0x08u,  /* 0x07, Group=0; Cascade=0; PIN=out08: 0000 1000. Engine maintenance warning */
    0x07u,  /* 0x08, Group=0; Cascade=0; PIN=out07: 0000 0111. MIL lamp, */
    0x06u,  /* 0x09, Group=0; Cascade=0; PIN=out06. 0000 0110. Airbag warning lamp */
    0x05u,  /* 0x0A, Group=0; Cascade=0; PIN=out05. 0000 0101. Engine repair warning lamp */
    0x04u,  /* 0x0B, Group=0; Cascade=0; PIN=out04. 0000 0100. Parking lamp */
    0x03u,  /* 0x0C, Group=0; Cascade=0; PIN=out03. 0000 0011. Low oil pressure warning lamp */
    0x02u,  /* 0x0D, Group=0; Cascade=0; PIN=out02. 0000 0010. Low water level warning lamp */
    0x01u,  /* 0x0E, Group=0; Cascade=0; PIN=out01. 0000 0001. Lamp filament dection indicator */
    0x00u,  /* 0x0F, Group=0; Cascade=0; PIN=out00. 0000 0000. High beam lamp */

    0x1Fu,  /* 0x10, Group=0; Cascade=1; PIN=out15: 0001 1111. Rear Fog Light */
    0x1Eu,  /* 0x11, Group=0; Cascade=1; PIN=out14: 0001 1110. Exhaust Brake lamp */
    0x1Du,  /* 0x12, Group=0; Cascade=1; PIN=out13: 0001 1101. ASR indication lamp */
    0x1Cu,  /* 0x13, Group=0; Cascade=1; PIN=out12: 0001 1100. Retarder_Indication lamp */
    0x1Bu,  /* 0x14, Group=0; Cascade=1; PIN=out11: 0001 1011. Low Beam lamp */
    0x1Au,  /* 0x15, Group=0; Cascade=1; PIN=out10: 0001 1010. Inter-wheel differential indicator */
    0x19u,  /* 0x16, Group=0; Cascade=1; PIN=out09: 0001 1001. Inter-axle differential indicator */
    0x18u,  /* 0x17, Group=0; Cascade=1; PIN=out08: 0001 1000. EBS warning lamp */
    0x17u,  /* 0x18, Group=0; Cascade=1; PIN=out07: 0001 0111. EBS failure lamp, */
    0x15u,  /* 0x19, Group=0; Cascade=1; PIN=out05. 0001 0101. Door open warning lamp */
    0x14u,  /* 0x1A, Group=0; Cascade=1; PIN=out04. 0001 0100. Engine brake indicator */
    0x13u,  /* 0x1B, Group=0; Cascade=1; PIN=out03. 0001 0011. Engine overspeed warning lamp */
    0x12u,  /* 0x1C, Group=0; Cascade=1; PIN=out02. 0001 0010. Engine stop warning lamp */
    0x10u,  /* 0x1D, Group=0; Cascade=1; PIN=out00. 0001 0000. Tyre pressure low warning lamp */
  
    0x2Fu,  /* 0x1E, Group=1; Cascade=0; PIN=out15. 0010 1111. VDC indicator */
    0x2Eu,  /* 0x1F, Group=1; Cascade=0; PIN=out14. 0010 1110. Seatbelt indicator */
    0x2Du,  /* 0x20, Group=1; Cascade=0; PIN=out13: 0000 1101. Urea level indicator */
    0x2Cu,  /* 0x21, Group=1; Cascade=0; PIN=out12: 0000 1100. Cruise indicator */
    0x2Au,  /* 0x22, Group=1; Cascade=0; PIN=out10: 0000 1010. Work Light Indicator */
    0x29u,  /* 0x23, Group=1; Cascade=0; PIN=out09: 0000 1001. Brake pressure low warning lamp */
    0x28u,  /* 0x24, Group=1; Cascade=0; PIN=out08: 0000 1000. Brake pressure overhigh warning lamp */
    0x27u,  /* 0x25, Group=1; Cascade=0; PIN=out07: 0000 0111. Fuel level low warning lamp */
    0x23u,  /* 0x26, Group=1; Cascade=0; PIN=out03. 0000 0110. Tractor ABS indicator */

  
    0x3Au,  /* 0x27, Group=1, Cascade=1; PIN=out10, 0011 1010. 25% urea level indicator */
    0x39u,  /* 0x28, Group=1, Cascade=1; PIN=out09, 0011 1001. 50% urea level indicator */
    0x38u,  /* 0x29, Group=1, Cascade=1; PIN=out08, 0011 1000. 75% urea level indicator */
    0x37u,  /* 0x2A, Group=1, Cascade=1; PIN=out07, 0011 0111. 100% urea level indicator */
  
    0x56u,  /* 0x2B, Group=2; Cascade=1; PIN=out06. 0101 0110.  900 rpm indicator */
    0x4Fu,  /* 0x2C, Group=2; Cascade=0; PIN=out15. 0100 1111. 1000 rpm indicator */
    0x4Eu,  /* 0x2D, Group=2; Cascade=0; PIN=out14. 0100 1111. 1100 rpm indicator */
    0x4Du,  /* 0x2E, Group=2; Cascade=0; PIN=out13: 0100 1101. 1200 rpm indicator */
    0x4Cu,  /* 0x2F, Group=2; Cascade=0; PIN=out12: 0100 1100. 1300 rpm indicator */
    0x4Bu,  /* 0x30, Group=2; Cascade=0; PIN=out11: 0100 1011. 1400 rpm indicator */
    0x4Au,  /* 0x31, Group=2; Cascade=0; PIN=out10: 0100 1010. 1500 rpm indicator */
    0x49u,  /* 0x32, Group=2; Cascade=0; PIN=out09: 0100 1001. 1600 rpm indicator */
    0x48u,  /* 0x33, Group=2; Cascade=0; PIN=out08: 0100 1000. 1700 rpm indicator */
    0x47u,  /* 0x34, Group=2; Cascade=0; PIN=out07: 0100 0111. 1800 rpm indicator */
    0x46u,  /* 0x35, Group=2; Cascade=0; PIN=out06. 0100 0110. 1900 rpm indicator */
    0x45u,  /* 0x36, Group=2; Cascade=0; PIN=out05. 0100 0101. 2000 rpm indicator */
    0x44u,  /* 0x37, Group=2; Cascade=0; PIN=out04. 0100 0100. 2100 rpm indicator */
    0x43u,  /* 0x38, Group=2; Cascade=0; PIN=out03. 0100 0011. 2200 rpm indicator */
    0x42u,  /* 0x39, Group=2; Cascade=0; PIN=out02. 0100 0010. 2300 rpm indicator */
    0x41u,  /* 0x3A, Group=2; Cascade=0; PIN=out01. 0100 0001. 2400 rpm indicator */
    0x40u,  /* 0x3B, Group=2; Cascade=0; PIN=out00. 0100 0000. 2500 rpm indicator */
    0x5Fu,  /* 0x3C, Group=2; Cascade=1; PIN=out15. 0101 1111. 2600 rpm indicator */
    0x5Eu,  /* 0x3D, Group=2; Cascade=1; PIN=out14. 0101 1110. 2700 rpm indicator */
    0x5Du,  /* 0x3E, Group=2; Cascade=1; PIN=out13. 0101 1101. 2800 rpm indicator */
    0x5Cu,  /* 0x3F, Group=2; Cascade=1; PIN=out12. 0101 1100. 2900 rpm indicator */
    0x5Bu,  /* 0x40, Group=2; Cascade=1; PIN=out11. 0101 1011. 3000 rpm indicator */
    0x5Au,  /* 0x41, Group=2; Cascade=1; PIN=out10. 0101 1010. 3100 rpm indicator */
    0x59u,  /* 0x42, Group=2; Cascade=1; PIN=out09. 0101 1001. 3200 rpm indicator */
    0x58u,  /* 0x43, Group=2; Cascade=1; PIN=out08. 0101 1000. 3300 rpm indicator */
    0x57u,  /* 0x44, Group=2; Cascade=1; PIN=out07. 0101 0111. 3400 rpm indicator */

    0x26u,  /* 0x45, Group=1; Cascade=0; PIN=out06. 0000 0110. Brake shoe wear indicator */
    0xFFu,  /* 0x46, dummy */
    0xFFu,  /* 0x47, dummy */
    0xFFu,  /* 0x48, dummy */
    0xFFu,  /* 0x49, dummy */
    0xFFu,  /* 0x4A, dummy */
    0x22u,  /* 0x4B, Group=1; Cascade=0; PIN=out02. 0010 0010. Trialer ABS indicator */
    0x21u,  /* 0x4C, Group=1; Cascade=0; PIN=out01. 0010 0001. ECAS warning lamp */
    0x20u,  /* 0x4D, Group=1; Cascade=0; PIN=out01. 0010 0000. ECAS failure indicator */
    0x3Fu,  /* 0x4E, Group=1; Cascade=1; PIN=out15. 0000 1111. Charging indicator */
    0x3Eu,  /* 0x4F, Group=1; Cascade=1; PIN=out14. 0000 1110. Air filter block warning lamp */
    0x3Du,  /* 0x50, Group=1; Cascade=1; PIN=out13. 0000 1101. Cab unfixed warning lamp */
    0x3Cu,  /* 0x51, Group=1; Cascade=1; PIN=out12. 0000 1100. Power take-off indicator */
    0x3Bu   /* 0x52, Group=1; Cascade=1; PIN=out11. 0000 1011. Gearbox failure indicato */
};

static const uint8 u8g_PinGrp_Tbl[INDICATOR_GRP_NUM] = /* mapping from group to a certain PIN */
{
    LED_SDI,    /* group = 0 */
    LED_SDI2,   /* group = 1 */
    LED_SDI3    /* group = 2 */
};

static uint8 u8g_Flash1Hz_Tbl[FLASH1HZ_IND_NUM][3] = 
{
    {0x0Fu, 0x00u, 0x00u},     /* High beam LAMP */
    {0x0Eu, 0x00u, 0x00u},     /* Engine stop warning lamp */
    {0x0Du, 0x00u, 0x00u}      /* Engine overspeed warning lamp */
};

static uint8 u8g_Flash2Hz_Tbl[FLASH2HZ_IND_NUM][3] =
{
    {0x0Cu, 0x00u, 0x00u},     /* High beam LAMP */
    {0x0Bu, 0x00u, 0x00u},     /* Tyre pressure low warning lamp */
};

static uint32 u32g_PinStatus[INDICATOR_GRP_NUM] = {0};

static uint8 u8s_RunCnt = 0u;

static void delay(uint32_t counter)    /* this function need to be removed and unified */
{
	uint32_t i,j;
	for(i=0;i<counter;i++)
	{
		for(j=0;j<counter;j++)
		{
			asm(nop);
		}
	}
}
/* -------------------End of calibration section-------------------------*/

static void  ECU_InidcatorOutput(void);

/*
 * Name        : ECU_Indicator_Init
 * Description : initialize indicators
 * Arguments   : none
 * Return      : none
 */
void ECU_Indicator_Init(void)
{
    /*  turn off the indicators */
  //  SIUL_Output(LED_LE, 0u);    /* LE down */
  //  SIUL_Output(LED_OE,1u);     /* OE up */

}

/*
 * Name        : ECU_Telltale_Init
 * Description : initialization function for BSP runnables
 * Arguments   : none
 * Return      : none
 */
void ECU_Telltale_Init(void)
{
    /* TODO: extra initialization */
}

/*
 * Name        : BSP_Set_Telltale
 * Description : command to control the indicated telltale
 * Arguments   : TelltaleId - The telltale defined ID
 *               IndOutCmd  - command defining output to apply for the indicator
 * Return      : see macro definition: BSP_StdReturnType
 */
BSP_StdReturnType BSP_Set_Telltale(uint8 TelltaleId, BSP_IndOutType IndOutCmd)
{
    uint8 u8t_PinID;       /* Physical PIN id */
    uint8 u8t_PinGrp;      /* STP16CP05 group number */
    uint8 u8t_PinPos;      /* Pin number in STP16CP05, out0 - out15 */
    sint32 i;
    BSP_StdReturnType ret;

    /* Note: parameter validation is ignored */

    /* update PinStatus */
    u8t_PinID  = u8g_PhyPin_Tbl[TelltaleId];
    if(u8t_PinID == 0xFFu)
    {
        return BSP_INVALID_RANGE;
    }
    u8t_PinGrp = u8t_PinID >> 5u;      /* highest 3 bits are group number */
    u8t_PinPos = u8t_PinID & 0x1Fu;    /* combination of PIN number and cascade */
    
    ret = BSP_OK;
    switch(IndOutCmd)
    {
        case BSP_IND_OFF:
            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] & (~(0x01uL << u8t_PinPos));  /* set 0 on corresponding bit */
            break;
        case BSP_IND_ON:
            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] | (0x01uL << u8t_PinPos);  /* set 1 on corresponding bit */
            break;
        case BSP_IND_1HZ_FLASH:
         
            for(i = 0; i < FLASH1HZ_IND_NUM; i++)    /* search for the indicator which is able to twinkle */
            {
                if(u8g_Flash1Hz_Tbl[i][0] == u8t_PinID)
                {
                    u8g_Flash1Hz_Tbl[i][2] = 1u;    /* enable twinkling */
                    break;
                }
            }
            if(i == FLASH1HZ_IND_NUM)
            {
               ret = BSP_NOT_SUPPORTED; 
            }

            break;
        case BSP_IND_2HZ_FLASH:
            for(i = 0; i < FLASH2HZ_IND_NUM; i++)    /* search for the indicator which is able to twinkle */
            {
                if(u8g_Flash2Hz_Tbl[i][0] == u8t_PinID)
                {
                    u8g_Flash2Hz_Tbl[i][2] = 1u;    /* enable twinkling */
                    break;
                }
            }
            if(i == FLASH2HZ_IND_NUM)
            {
               ret = BSP_NOT_SUPPORTED; 
            }
            break;
        default:
            ret = BSP_NOT_SUPPORTED;
            break;
    }

    return ret;
}

BSP_StdReturnType BSP_Service_TelltaleTest(BSP_TestCommand TestCommand)
{
    /* TODO: to be implemented */
}
/*
 * Name        : BSP_ReportError_Telltale
 * Description : Checks for and reports error
 * Arguments   : FTB - Failure Type Byte
 * Return      : none
 */
BSP_StdReturnType BSP_ReportError_Telltale(uint8 TelltaleId, uint8* FTB)
{
     TelltaleId = TelltaleId;
     *FTB = 0u;
     return BSP_NOT_SUPPORTED; 
}

/*
 * Name        : ECU_LED1HzFlash_Runnable
 * Description : LED twinkles at 1 Hz.
 * Arguments   : none
 * Return      : none
 * Notes       : This runnable shall be called every 10ms.
 */
void ECU_LEDFlash_Runnable(void)
{

    uint8 u8t_PinGrp;      /* STP16CP05 group number */
    uint8 u8t_PinPos;      /* Pin number in STP16CP05, out0 - out15 */
    sint32 index = 0;

    if(MCU_CUR_MODE != MCU_NORMAL_MODE)
    {
        return;
    }

    if((u8s_RunCnt % 25u) == 0)        /* scan the 2Hz flashing table */
    {
    	for(index = 0; index < FLASH2HZ_IND_NUM; index++)
    	{
    	    if(u8g_Flash2Hz_Tbl[index][2] == 1u)    /* flashing enable */
    	    {
    	        u8t_PinGrp = u8g_Flash2Hz_Tbl[index][0] >> 5u;      /* highest 3 bits are group number */
    	        u8t_PinPos = u8g_Flash2Hz_Tbl[index][0] & 0x1Fu;    /* combination of PIN number and cascade */

    	        if(u8g_Flash2Hz_Tbl[index][1] == 0u)    /* LED OFF */
    	        {
    	            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] & (~(0x01uL << u8t_PinPos));
    	        }
    	        else
    	        {
    	            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] | (0x01uL << u8t_PinPos);  /* set 1 on corresponding bit */
    	        }
  	            u8g_Flash2Hz_Tbl[index][1] = (uint8)(~u8g_Flash2Hz_Tbl[index][1]);    /* reverse the LED status */
   	        }
   	    }	
    }

    if((u8s_RunCnt ==50u) ||(u8s_RunCnt== 0))
    {
    	for(index = 0; index < FLASH1HZ_IND_NUM; index++)
    	{
            if(u8g_Flash1Hz_Tbl[index][2] == 1u)    /* flashing enable */
            {
   	            u8t_PinGrp = u8g_Flash1Hz_Tbl[index][0] >> 5u;      /* highest 3 bits are group number */
   	            u8t_PinPos = u8g_Flash1Hz_Tbl[index][0] & 0x1Fu;    /* combination of PIN number and cascade */        
    	        if(u8g_Flash1Hz_Tbl[index][1] == 0u)    /* LED OFF */
                {
    	            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] & (~(0x01uL << u8t_PinPos));
    	        }
    	        else
    	        {
    	            u32g_PinStatus[u8t_PinGrp] = u32g_PinStatus[u8t_PinGrp] | (0x01uL << u8t_PinPos);  /* set 1 on corresponding bit */
    	        }
    	        u8g_Flash1Hz_Tbl[index][1] = (uint8)(~u8g_Flash1Hz_Tbl[index][1]);    /* reverse the LED status */
    	    }
    	}
    }
    else
    {
    	;    /* do nothing */
    }

    if(u8s_RunCnt == 99u)
    {
    	u8s_RunCnt = 0;
    }
    else
    {
    	u8s_RunCnt++;
    }  
    
    ECU_InidcatorOutput();
    
}

static void  ECU_InidcatorOutput(void)
{
    sint32 i, j, k;
    uint8 u8t_IndBit = 0u;

    /* update output */
    for(i = 31; i >= 0; i--)
    {
        SIUL_Output(LED_CK, 0u); /* LED clock down */
        for (k = 0;k < 50; k++)
        {
            __asm(" nop");
        }
        for(j = 0; j < INDICATOR_GRP_NUM; j++)
        {
            u8t_IndBit = (uint8)((u32g_PinStatus[j] >> (uint32)i) & 0x01uL);
            SIUL_Output(u8g_PinGrp_Tbl[j], u8t_IndBit);
        }
        for (k = 0;k < 90; k++)
        {
            __asm(" nop");
        }
        SIUL_Output(LED_CK, 1u);  /* LED clock up */
        for (k = 0;k < 100; k++)
        {
            __asm(" nop");
        }
    }

	/* final timing sequence */
    SIUL_Output(LED_LE, 1u);    /* LE Up */
    for (k = 0;k < 100; k++)
        {
            __asm(" nop");
        }
    SIUL_Output(LED_LE, 0u);    /* LE Down */
    for (k = 0;k < 50; k++)
        {
            __asm(" nop");
        }
    SIUL_Output(LED_OE, 0u);    /* OE Down */
}

