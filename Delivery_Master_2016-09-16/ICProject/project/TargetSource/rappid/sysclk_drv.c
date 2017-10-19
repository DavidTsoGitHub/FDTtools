/* Customer Driver Example */

/********************  Dependent Include files here **********************/

#include "sysclk_init.h"



void mode_entry_run0(void)
{


    while(ME.GS.B.S_MTRANS) {}      /* Ensure any STANDBY to DRUN mode transition completed */
    
        /* enter in RUN0 mode to update the configuration*/
    ME.MCTL.R = 0x40005AF0;    
        /* Mode & Key */
    ME.MCTL.R = 0x4000A50F;    
        /* Mode & Key  Inverted */
    while(0x1 == ME.GS.B.S_MTRANS){};    
        /* Wait for mode entry to complete */
    while(0x4 != ME.GS.B.S_CURRENTMODE){};    
        /* Check RUN0 mode has been entered  */

}


void mode_entry_run1(void)
{
    while(ME.GS.B.S_MTRANS) {}               /* Ensure any STANDBY to DRUN mode transition completed */
	ME.MCTL.R = 0x50005AF0;                  /* Enter RUN1 mode and key */
    ME.MCTL.R = 0x5000A50F;                  /* Enter RUN1 mode and inverted key */
    while (ME.GS.B.S_MTRANS) {}              /* Check RUN1 mode has been entered  */ 
}

void mode_entry_drun(void)
{
	ME.MCTL.R = 0x30005AF0;                  /* Enter DRUN mode and key */
    ME.MCTL.R = 0x3000A50F;                  /* Enter DRUN mode and inverted key */
    while (ME.GS.B.S_MTRANS) {}              /* Check DRUN mode has been entered  */ 
}

void mode_entry_stop(void) 
{
    ME.MCTL.R = 0xA0005AF0;                  /* Enter STOP mode and key */
    ME.MCTL.R = 0xA000A50F;                  /* Enter STOP mode and inverted key */
    while (ME.GS.B.S_MTRANS) {}              /* Check STOP mode has been entered */
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

