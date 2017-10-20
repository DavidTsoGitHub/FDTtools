/*=======[I N C L U D E S]====================================================*/

#include "config.h"
#include "Wdg.h"
#include "Wdg_Cfg.h"
#include "MPC5645S.h"

/*=======[M A C R O S]========================================================*/

#define COP_RESET 0u

/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/******************************************************************************/
/**
 * @brief               <WdgStart>
 * 
 * <Initializes the WDG driver> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <void>    
 */
/******************************************************************************/
void WdgStart(void)
{
	
    SWT.SR.R = 0x0000c520ul;/* Write keys to clear soft lock bit */
    SWT.SR.R = 0x0000d928ul;
    SWT.CR.R = 0x8000010Aul;/* disable WEN */
    
    SWT.TO.R = (uint32)(WDG_TIME*80000u);

    SWT.CR.R |= 0x80000108ul;

    /* WEN  enable */
    SWT.CR.R |= 0x00000001ul;/* enable WEN */
	WdgKick();
	return;
 }


/******************************************************************************/
/**
 * @brief               <WdgKick>
 * 
 * <This Funtion Triggers the Watchdog Hardware> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <void>    
 */
/******************************************************************************/
void WdgKick(void)
{
    SWT.SR.R = 0x0000A602ul;
    SWT.SR.R = 0x0000B480ul;
    return;
}

/******************************************************************************/
/**
 * @brief               <SystemReset>
 * 
 * <This Funtion resets the Wdg module> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <void>    
 */
/******************************************************************************/
void SystemReset(void)
{
    
    /* invaild key cause reset when RIA in MCR is set*/
	SWT.SR.R = COP_RESET;

	return;
}


