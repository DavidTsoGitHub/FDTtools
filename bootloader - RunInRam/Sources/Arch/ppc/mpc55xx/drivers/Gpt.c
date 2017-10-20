#define GPT_C_

/*=======[I N C L U D E S]====================================================*/

#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "Gpt.h"
#include "Gpt_Cfg.h"
#include "Mcu.h"
#include "MPC5645S.h"

/*=======[M A C R O S]========================================================*/
#define GPT_PIT_ENABLE      0xFFFFFFFDuL
#define GPT_RTI_DISABLE     0x00000004uL
#define GPT_PIT_DISABLE     0x00000002uL


/*=======[T Y P E   D E F I N I T I O N S]====================================*/


typedef struct 
{   
    volatile uint32 timeLoadvalue;
    volatile uint32 currentTimevalue; 
    volatile uint32 Timercontrol; 
    volatile uint32 TimerFlag; 
}Gpt_TimerChannel_Type;

extern struct
{
	uint32 Counter;
	uint32 Feed_Dog_Flag; 
}Hardware_Watchdog;


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/*******************************************************************************/
/***
 * @brief			:Gpt_Init,config the PIT registers for time.  
 * 
 * Service ID		: <Gpt_Init>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Gpt_Init(const Gpt_ConfigType *ConfigPtr)
{
(void)ConfigPtr;

#if defined(CFG_MPC5634M) || defined(CFG_MPC5642A)

	PIT.MCR.R |= GPT_RTI_DISABLE;
    
    /* PIT module enable */
    PIT.MCR.R &= GPT_PIT_ENABLE;
    
    /* PIT set count value */
    PIT.TIMER[0].LDVAL.R = ((CLOCK_PLL_FREQ /1000) * GPT_PERIOD_TIME) - 1;

    /* PIT enable count */
    PIT.TIMER[0].TCTRL.R |= 0x00000001uL;

    /* PIT set count value, setting  interruption is 3ms ,2015-4-29 add by Wei*/
    PIT.TIMER[1].LDVAL.R = ((CLOCK_PLL_FREQ /1000)*(HARDWARE_WATCHDOG_PERIOD_TIME)) - 1;

    /* PIT enable count,,2015-4-29 add by Wei */
    PIT.TIMER[1].TCTRL.R |= 0x00000001uL;

#elif defined(CFG_MPC5668)

    /* PIT module enable */
    PIT.MCR.R &= GPT_PIT_ENABLE;

    /* PIT set count value */
    PIT.LDVAL1.R = ((CLOCK_PLL_FREQ /1000) * GPT_PERIOD_TIME) - 1;

    /* PIT enable count */
    PIT.TCTRL1.R |= 0x00000001uL;
    
    /* PIT set count value, setting  interruption is 3ms ,2015-4-29 add by Wei*/
    PIT.LDVAL2.R = ((CLOCK_PLL_FREQ /1000)*(HARDWARE_WATCHDOG_PERIOD_TIME)) - 1;

    /* PIT enable count,,2015-4-29 add by Wei */
    PIT.TCTRL2.R |= 0x00000001uL;

#elif defined(CFG_MPC5606S) || defined(CFG_MPC5606B) || defined(CFG_MPC5645S)

    /* PIT module enable */
    PIT.MCR.R &= GPT_PIT_ENABLE;

    /* PIT set count value */
    PIT.CH[0].LDVAL.R = ((CLOCK_PLL_FREQ /1000) * GPT_PERIOD_TIME) - 1;

    /* PIT enable count */
    PIT.CH[0].TCTRL.R |= 0x00000001uL;

#endif

}


/*******************************************************************************/
/***
 * @brief			:Gpt_Deinit,config the PIT registers to power on default value.  
 * 
 * Service ID		: <Gpt_Deinit>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Gpt_Deinit(void)
{
    
    PIT.MCR.R |=  GPT_PIT_DISABLE;

#if defined(CFG_MPC5634M) || defined(CFG_MPC5642A)

    /* PIT disable count */
    PIT.TIMER[0].TCTRL.R &= ~0x00000001uL;

    /* PIT disable count */
    PIT.TIMER[0].TCTRL.R &= ~0x00000001uL;

    if ((PIT.TIMER[0].TFLG.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
    	PIT.TIMER[0].TFLG.R = 1u;
    }

    /* Check Interrupt Flag,2015-4-29 add by Wei */
    if ((PIT.TIMER[1].TFLG.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
    	PIT.TIMER[1].TFLG.R = 1u;
    }

#elif defined(CFG_MPC5668)

    /* PIT disable count */
    PIT.TCTRL1.R &= ~0x00000001uL;

    /* PIT disable count */
    PIT.TCTRL2.R &= ~0x00000001uL;

    if ((PIT.TFLG1.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.TFLG1.R = 1u;
    }

    /* Check Interrupt Flag,2015-4-29 add by Wei */
    if ((PIT.TFLG2.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.TFLG2.R = 1u;
    }
#elif defined(CFG_MPC5606S) || defined(CFG_MPC5606B) || defined(CFG_MPC5645S)
    /* PIT disable count */
    PIT.CH[0].TCTRL.R &= ~0x00000001uL;

    if ((PIT.CH[0].TFLG.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.CH[0].TFLG.R = 1u;
    }

#endif

}

boolean Gpt_GetFlag(void)
{
    boolean ret = FALSE;
    
#if defined(CFG_MPC5634M) || defined(CFG_MPC5642A)
    /* Check Interrupt Flag */
    if ((PIT.TIMER[0].TFLG.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.TIMER[0].TFLG.R = 1u;
        ret = TRUE;
    }

    /* Check Interrupt Flag,2015-4-29 add by Wei */
    if ((PIT.TIMER[1].TFLG.R & 0x00000001uL) > 0u)
    {
        /*hardware watchdog counter accumulate */
        Hardware_Watchdog.Counter+=3;

        /* Clear Interrupt Flag */
        PIT.TIMER[1].TFLG.R = 1u;
        //ret = TRUE;
    }
#elif defined(CFG_MPC5668)
    /* Check Interrupt Flag */
    if ((PIT.TFLG1.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.TFLG1.R = 1u;
        ret = TRUE;
    }
    
    /* Check Interrupt Flag,2015-4-29 add by Wei */
    if ((PIT.TFLG2.R & 0x00000001uL) > 0u)
    {
        /*hardware watchdog counter accumulate */
        Hardware_Watchdog.Counter+=3;
        
        /* Clear Interrupt Flag */
        PIT.TFLG2.R = 1u;
        //ret = TRUE;
    }
#elif defined(CFG_MPC5606S) || defined(CFG_MPC5606B) || defined(CFG_MPC5645S)
    /* Check Interrupt Flag */
    if ((PIT.CH[0].TFLG.R & 0x00000001uL) > 0u)
    {
        /* Clear Interrupt Flag */
        PIT.CH[0].TFLG.R = 1u;
        ret = TRUE;
    }

#endif

    return ret;
}



