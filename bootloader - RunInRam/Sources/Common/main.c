/*=======[I N C L U D E S]====================================================*/
#include "Appl.h"
#include "FlashLoader.h"
#include "Board.h"
#include "Fls.h"
#include "QuadSPI.h"
#include "Nvm.h"

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/**
 * @brief               <main function>
 * 
 * <task schedule and process all API. 10ms task for watch dog trigger> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Non Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void main(void) 
{
	/* Initialize the board specifics */
	Board_Init();

	/* ECU Initialize */
    (void)Appl_EcuStartup();

    for ( ; ; )
    {
        /* 10ms task and watch dog tigger */
        Appl_UpdateTriggerCondition();
        
        /* task for flash driver and checksum process */
        FL_MainFunction();
    }
}
