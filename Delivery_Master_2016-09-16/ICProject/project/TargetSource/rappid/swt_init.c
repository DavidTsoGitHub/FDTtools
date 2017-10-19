/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : detc_ecu
 *
 * Project File           : detc_ecu.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : swt_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 04-Sep-2014 15:25:01
 *
 * Created on Date        : 04-Sep-2014 15:25:02
 *
 * Brief Description      : SWT Initialization file
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains Software Watchdog Timer (SWT) parameter
 *                          initialization function.The function configures SWT parameters
 *                          such as watchdog status, watchdog timeout, debug mode control,
 *                          sleep mode control, window mode status & window value. Also,
 *                          the filehas a function for Watchdog Service Routine.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "swt_init.h"




/********************* Initialization Function(s) *********************** */


void swt_init_fnc(void)
{

        uint32_t init_status;        
        int32_t delay=100;        
        /*check whether the Hardware is enabled or not    */

        if(1 != SWT.CR.B.HLK)    
        {    
            SWT.SR.B.WSC = 0xC520;    
            SWT.SR.B.WSC = 0xD928;    
            /*Disable Software lock of register by writing sequence register with unlock data      */


            while ( 0<(delay--) )    
            {    
                if(1 != SWT.CR.B.SLK)    
                {    
                    /*Proceed with initialization    */

                    init_status = 0;    
                    break;    
                    /*Break from the while loop    */

                }    
                else    
                {    
                    init_status = 1;    
                    /*SWT initialization cannot be proceed as software lock is still enabled     */

                }    
            }    
        }    
        else    
        {    
            init_status = 1;    
                /*SWT initialization cannot be proceed as Hardware lock is still enabled */
    
        }    
        if(0 == init_status)    
        {    
            SWT.CR.B.WEN =0;    
                /*Software Watchdog Timer Enable : Disabled */
    
            SWT.TO.R = 0x00000500;    
                /*Software Watchdog Timer timeout is 10ms    */

            SWT.WN.R = 0x00000000;    
                /*SWT's Service Window time is 0ms    */

            SWT.CR.R = 0x8000010A;    
                /*Master Access Control for Master 0 is Enabled*/
                /*SWT's Window Mode is Disabled    */
                /*SWT's Interrupt Then Reset is Disabled    */
                /*SWT's Clock Source Selected is System Clock    */
                /*SWT's Stop Mode Control is Disabled    */
                /*SWT's Debug Mode Control is Enabled    */

            SWT.CR.B.WEN = 0;    
                /*Software Watchdog Timer Enable: Disabled    */

        }    

}
/********************* SWT Service Sequence Function *********************** */
void swt_srvc_seq_fnc(void)
{

        SWT.SR.B.WSC = 0xA602;    
        SWT.SR.B.WSC = 0xB480;    
}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

