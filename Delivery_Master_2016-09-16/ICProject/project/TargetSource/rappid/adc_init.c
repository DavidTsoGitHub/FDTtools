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
 * file                   : adc_init.c
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
 * Brief Description      : This file contains the ADC Configuration and initialization functions.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This File contains function that setup Normal & injected channels,
 *                          Sampling, interrupt and Threshold.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "adc_init.h"






/*********************  Initialization Function(s) ************************/

void adc_init_fnc(void)
{

/*-----------------------------------------------------------*/
/*            Abort all conversion in process                */
/*-----------------------------------------------------------*/

    
        ADC_0.MCR.B.ABORT = 1; 

        
/*-----------------------------------------------------------*/
/*            ADC Sampling and ThresHold Configuration                    */
/*-----------------------------------------------------------*/

        adc_Sampling_Setup_fnc();            
/*-----------------------------------------------------------*/
/*            ADC General Setup                    */
/*-----------------------------------------------------------*/

        adc_general_Setup_fnc();            


/*-----------------------------------------------------------*/
/*            ADC Interrupt Configuration                            */
/*-----------------------------------------------------------*/

        adc_Interrupt_Setup_fnc();            

/*-----------------------------------------------------------*/
/*            ADC Channel Configuration                    */
/*-----------------------------------------------------------*/

        adc_Channel_Setup_fnc();            

/*-----------------------------------------------------------*/
/*            ADC Trigger Configuration                    */
/*-----------------------------------------------------------*/

        adc_Trigger_Setup_fnc();            
/*-----------------------------------------------------------*/
/*            DMA Setup Function(s)                    */
/*-----------------------------------------------------------*/

        adc_Dma_Setup_fnc();        
/*-----------------------------------------------------------*/
/*            Exit the ABORT state                    */
/*-----------------------------------------------------------*/

        ADC_0.MCR.B.ABORT = 0; 

            
}

/*********************  Trigger Initialization Function(s) ************************/

void adc_Trigger_Setup_fnc()
{
/*-----------------------------------------------------------*/
/*                    ADC 0 Trigger Configuration            */
/*-----------------------------------------------------------*/
        ADC_0.MCR.R |= 0x00000000;            
            /* Injection Trigger : Disabled     */
            /* Injection Trigger Edge : Falling*/
            /* Trigger Normal : Disabled     */
            /* External Trigger Edge : Falling    */
}
/*********************  Interrupt Initialization Function(s) ************************/

void adc_Interrupt_Setup_fnc(void)
{
/*-----------------------------------------------------------*/
/*                    Interrupt Mask Register(ADC0_IMR)        */
/*-----------------------------------------------------------*/

        ADC_0.IMR.R = 0x00000000;    
                /* End of Chain Conversion Interrupt : Disabled */
                /* End of Injected Chain Interrupt : Disabled */
                /* End of Channel Conversion Interrupt : Disabled */
                /* End of Injected Channel Interrupt : Disabled */
/*-----------------------------------------------------------*/
/*            ADC 0 Channel Interrupt Mask Registers CIMR_1    */
/*-----------------------------------------------------------*/
        ADC_0.CIMR[1].R =0x00000000;
            /* Channel Interrupt for Channel 32 Disabled */
            /* Channel Interrupt for Channel 33 Disabled */
            /* Channel Interrupt for Channel 34 Disabled */
            /* Channel Interrupt for Channel 35 Disabled */
            /* Channel Interrupt for Channel 36 Disabled */
            /* Channel Interrupt for Channel 37 Disabled */
            /* Channel Interrupt for Channel 38 Disabled */
            /* Channel Interrupt for Channel 39 Disabled */
            /* Channel Interrupt for Channel 40 Disabled */
            /* Channel Interrupt for Channel 41 Disabled */
            /* Channel Interrupt for Channel 42 Disabled */
            /* Channel Interrupt for Channel 43 Disabled */
            /* Channel Interrupt for Channel 44 Disabled */
            /* Channel Interrupt for Channel 45 Disabled */
            /* Channel Interrupt for Channel 46 Disabled */
            /* Channel Interrupt for Channel 47 Disabled */
}

/*********************  Sampling Initialization Function(s) ************************/

void adc_Sampling_Setup_fnc(void)
{

/*-----------------------------------------------------------*/
/*                ADC_0 Sampling Control            */
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/*                Conversion Timing Register 1(ADC_0_CTR1)    */
/*-----------------------------------------------------------*/
            ADC_0.CTR[1].R = 0x00008607;
                /*Phase duration Latch : Enabled */
                /*Input Sampling Duration : 3 clock Cycles     */
                /*Input Comparison Duration : 7 clock Cycles     */

            ADC_0.MCR.B.PWDN = 0x0;
                /* Exit from power down state */
}
/*********************  General Initialization Function(s) ************************/

void adc_general_Setup_fnc(void)
{

/*-----------------------------------------------------------*/
/*                ADC 0 Device Setup             */
/*-----------------------------------------------------------*/

        ADC_0.MCR.R |= 0x80000000;
            /* Auto Clock Off feature : Disabled */
            /* Conversion data Overwrite: Disabled */
            /* Conversion Data Aligned: Right Aligned */
            /* Conversion Mode Selected :  One Shot */
            /* ADC Conversion clock selected as :  System Clock/2 */


        ADC_0.DSDR.R = 0x00000010;
            /* The Decode Signal Delay : 1 clock Cycle      */

        ADC_0.PDEDR.R = 0x00000000;
            /* The Power Down Delay : 0 clock Cycle      */

}
/*********************  Channel Initialization Function(s) ************************/

void adc_Channel_Setup_fnc(void)
{

/*-----------------------------------------------------------*/
/*                ADC 0 Channel Setup            */
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
/*    Normal Channel Conversion Mask Register 1(ADC_0_NCMR1)*/
/*-----------------------------------------------------------*/
        ADC_0.NCMR[1].R = 0x0000ffff;
            /* ADC Channel 32 in Normal Mode : Disabled*/
            /* ADC Channel 33 in Normal Mode : Disabled*/
            /* ADC Channel 34 in Normal Mode : Disabled*/
            /* ADC Channel 35 in Normal Mode : Disabled*/
            /* ADC Channel 36 in Normal Mode : Disabled*/
            /* ADC Channel 37 in Normal Mode : Disabled*/
            /* ADC Channel 38 in Normal Mode : Disabled*/
            /* ADC Channel 39 in Normal Mode : Disabled*/
            /* ADC Channel 40 in Normal Mode : Disabled*/
            /* ADC Channel 41 in Normal Mode : Disabled*/
            /* ADC Channel 42 in Normal Mode : Disabled*/
            /* ADC Channel 43 in Normal Mode : Disabled*/
            /* ADC Channel 44 in Normal Mode : Disabled*/
            /* ADC Channel 45 in Normal Mode : Disabled*/
            /* ADC Channel 46 in Normal Mode : Disabled*/
            /* ADC Channel 47 in Normal Mode : Disabled*/


/*-----------------------------------------------------------*/
/*        Injected Channel Conversion Mask Register 1(ADC_0_JCMR1)*/
/*-----------------------------------------------------------*/
        ADC_0.JCMR[1].R = 0x00000000;
            /* ADC Channel 32 in Injected Mode : Disabled */
            /* ADC Channel 33 in Injected Mode : Disabled */
            /* ADC Channel 34 in Injected Mode : Disabled */
            /* ADC Channel 35 in Injected Mode : Disabled */
            /* ADC Channel 36 in Injected Mode : Disabled */
            /* ADC Channel 37 in Injected Mode : Disabled */
            /* ADC Channel 38 in Injected Mode : Disabled */
            /* ADC Channel 39 in Injected Mode : Disabled */
            /* ADC Channel 40 in Injected Mode : Disabled */
            /* ADC Channel 41 in Injected Mode : Disabled */
            /* ADC Channel 42 in Injected Mode : Disabled */
            /* ADC Channel 43 in Injected Mode : Disabled */
            /* ADC Channel 44 in Injected Mode : Disabled */
            /* ADC Channel 45 in Injected Mode : Disabled */
            /* ADC Channel 46 in Injected Mode : Disabled */
            /* ADC Channel 47 in Injected Mode : Disabled */


/*-----------------------------------------------------------*/
/*         Threshold Control Register 0 (ADC_0_TRC0)            */
/*-----------------------------------------------------------*/
        ADC_0.TRC[0].R = 0x00000020;
            /* Threshold detection for WatchDog 0 : Disabled */
            /* Threshold Inversion for WatchDog 0:Disabled */
            /* Threshold Channel Select for WatchDog 0 : 32    */

/*-----------------------------------------------------------*/
/*         Threshold Control Register 1 (ADC_0_TRC1)            */
/*-----------------------------------------------------------*/
        ADC_0.TRC[1].R = 0x00000020;
            /* Threshold detection for WatchDog 1 : Disabled */
            /* Threshold Inversion for WatchDog 1:Disabled */
            /* Threshold Channel Select for WatchDog 1 : 32    */

/*-----------------------------------------------------------*/
/*         Threshold Control Register 2 (ADC_0_TRC2)            */
/*-----------------------------------------------------------*/
        ADC_0.TRC[2].R = 0x00000020;
            /* Threshold detection for WatchDog 2 : Disabled */
            /* Threshold Inversion for WatchDog 2:Disabled */
            /* Threshold Channel Select for WatchDog 2 : 32    */

/*-----------------------------------------------------------*/
/*         Threshold Control Register 3 (ADC_0_TRC3)            */
/*-----------------------------------------------------------*/
        ADC_0.TRC[3].R = 0x00000020;
            /* Threshold detection for WatchDog 3 : Disabled */
            /* Threshold Inversion for WatchDog 3:Disabled */
            /* Threshold Channel Select for WatchDog 3 : 32    */

/*-----------------------------------------------------------*/
/*            Threshold Register 0 (ADC_0_THRHLR0)          */
/*-----------------------------------------------------------*/
        ADC_0.THRHLR[0].R = 0x03FF0000;
            /* Lower Threshold value : 0    */
            /* Higher Threshold value : 1023    */

/*-----------------------------------------------------------*/
/*            Threshold Register 1 (ADC_0_THRHLR1)          */
/*-----------------------------------------------------------*/
        ADC_0.THRHLR[1].R = 0x03FF0000;
            /* Lower Threshold value : 0    */
            /* Higher Threshold value : 1023    */

/*-----------------------------------------------------------*/
/*            Threshold Register 2 (ADC_0_THRHLR2)          */
/*-----------------------------------------------------------*/
        ADC_0.THRHLR[2].R = 0x03FF0000;
            /* Lower Threshold value : 0    */
            /* Higher Threshold value : 1023    */

/*-----------------------------------------------------------*/
/*            Threshold Register 3 (ADC_0_THRHLR3)          */
/*-----------------------------------------------------------*/
        ADC_0.THRHLR[3].R = 0x03FF0000;
            /* Lower Threshold value : 0    */
            /* Higher Threshold value : 1023    */

/*-----------------------------------------------------------*/
/*Watchdog Threshold Interrupt Mask Register (ADC_0_WTIMR) */
/*-----------------------------------------------------------*/
        ADC_0.WTIMR.R = 0x00000000;
            /* Low Threshold Interrupt for Watchdog 0 : Disabled */
            /* Low Threshold Interrupt for Watchdog 1 : Disabled */
            /* Low Threshold Interrupt for Watchdog 2 : Disabled */
            /* Low Threshold Interrupt for Watchdog 3 : Disabled */
            /* High Threshold Interrupt for Watchdog 0 : Disabled */
            /* High Threshold Interrupt for Watchdog 1 : Disabled */
            /* High Threshold Interrupt for Watchdog 2 : Disabled */
            /* High Threshold Interrupt for Watchdog 3 : Disabled */

}

/*********************  DMA Setup Function(s)************************/

void adc_Dma_Setup_fnc(void)
{
/*-----------------------------------------------------------*/
/*            ADC 0  DMA Enable Register (DMAE)            */
/*-----------------------------------------------------------*/
        ADC_0.DMAE.R |= 0x00000000;    
            /*DMA Global : Disabled */
            /*DMA clear sequence : ACK from DMA */
/*-----------------------------------------------------------*/
/*            ADC 0  DMA Channel Select Registers DMAR_1        */
/*-----------------------------------------------------------*/
        ADC_0.DMAR[1].R =0x00000000;
            /* DMA 32 Disabled */
            /* DMA 33 Disabled */
            /* DMA 34 Disabled */
            /* DMA 35 Disabled */
            /* DMA 36 Disabled */
            /* DMA 37 Disabled */
            /* DMA 38 Disabled */
            /* DMA 39 Disabled */
            /* DMA 40 Disabled */
            /* DMA 41 Disabled */
            /* DMA 42 Disabled */
            /* DMA 43 Disabled */
            /* DMA 44 Disabled */
            /* DMA 45 Disabled */
            /* DMA 46 Disabled */
            /* DMA 47 Disabled */
}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

