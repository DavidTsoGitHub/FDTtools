/**HEADER********************************************************************
*
* Copyright (c) 2015 Freescale Semiconductor
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: eed_lowlevel.h$
* $Version : 1.2.0$
* $Date    : Jun-10-2015$
*
* Comments:
*
*   This file consitutes of low level API's which will be called by high level
*   and middle level API's to perform desired tasks.
*
*END************************************************************************/

#include "ee_internal.h"

/* Flag to keep track of ECC Error Module */
static volatile EE_ModuleType eccErrorModule_Flag = NONE;

#if (EER_OPTION == IVOR_EXCEPTION)
void cRestoreEei(UINT32 value)
{
	value = value;    /* prvent compiler error */
    /*$ Write MSR */
    asm("mtmsr r3\n    ");
}

UINT32 cReadAndClearEei(void)
{
    /* Read MSR */
    asm("mfmsr    r3\n    " "wrteei 0 \n    ");
}
#endif /* End of #if (EEC_OPTION == IVOR_EXCEPTION) */
/* FlashDepletionRecover function for C90FL flash module */
#if (FLASH_MODULE == C90FL)
#if (VLE_IS_ON)
const unsigned char FSL_FlashDepletionRecover_C[] =
{
    0x18, 0x21, 0x06, 0xD0, 0x00, 0x80, 0x1A, 0xE1, 0x09, 0x0C, 0xDD, 0x01, 0x01, 0x3F, 0x01, 0x4E, 0x01, 0x5D, 0x01, 0x6C
    , 0x01, 0x7B, 0x03, 0x0A, 0x01, 0xF8, 0x52, 0xF8, 0x00, 0x00, 0x1A, 0xE6, 0xC8, 0x14, 0x2A, 0x06, 0xE6, 0x03, 0x48, 0x23
    , 0xE8, 0x50, 0x48, 0x09, 0x00, 0xED, 0x77, 0x87, 0x07, 0x3F, 0x74, 0xFC, 0x80, 0x1F, 0x77, 0x7B, 0x01, 0x3F, 0x7F, 0xA7
    , 0xE3, 0x78, 0xD4, 0x7F, 0xD5, 0xBF, 0x70, 0xC4, 0xE4, 0xE2, 0x70, 0xCB, 0xC3, 0x81, 0x54, 0xDF, 0x00, 0xAC, 0x70, 0xF4
    , 0xE5, 0x88, 0x70, 0xFD, 0xC7, 0x0B, 0x54, 0xFF, 0x00, 0xBC, 0x70, 0xDF, 0xE1, 0x7E, 0x70, 0xD0, 0xC1, 0x78, 0x54, 0xDF
    , 0x00, 0xA4, 0x70, 0xF7, 0xE4, 0xD9, 0x70, 0xF4, 0xC5, 0x5D, 0x54, 0xFF, 0x00, 0xA8, 0x62, 0x06, 0x54, 0xDF, 0x01, 0x00
    , 0x70, 0xE1, 0xE1, 0x8F, 0x70, 0xE1, 0xC0, 0xE9, 0x54, 0xFF, 0x01, 0x4C, 0xC0, 0x68, 0x65, 0xD6, 0xD0, 0x68, 0x2C, 0x07
    , 0xD0, 0x7E, 0xC0, 0x68, 0x65, 0xF6, 0xD0, 0x68, 0xC0, 0x78, 0x67, 0x57, 0xE2, 0x07, 0x18, 0x1A, 0xAC, 0xFF, 0xE6, 0xFB
    , 0x00, 0xBA, 0x00, 0x07, 0xE8, 0xF8, 0xC0, 0x78, 0x61, 0xF7, 0xD0, 0x78, 0xC0, 0x68, 0x67, 0x66, 0xE2, 0x02, 0x4C, 0x09
    , 0xC0, 0x78, 0x61, 0xD7, 0xD0, 0x78, 0x2C, 0x06, 0x54, 0xDF, 0x01, 0x4C, 0x48, 0x07, 0x54, 0xFF, 0x01, 0x00, 0x01, 0x93
    , 0x1A, 0xE1, 0x08, 0x0C, 0xCD, 0x01, 0x00, 0x90, 0x18, 0x21, 0x80, 0x30, 0x00, 0x04
    , 0x56, 0x4C, 0x45, 0x39, 0x30, 0x46, 0x4C, 0x44, 0x52, 0x30, 0x31, 0x30

}; /* Total Size = 226 bytes */
#else
const unsigned char FSL_FlashDepletionRecover_C[] =
{
    0x94, 0x21, 0xFF, 0xB0, 0x7C, 0x08, 0x02, 0xA6, 0xBE, 0xE1, 0x00, 0x2C, 0x90, 0x01, 0x00, 0x54, 0x7C, 0x7F, 0x1B, 0x78
    , 0x7C, 0x9E, 0x23, 0x78, 0x7C, 0xBD, 0x2B, 0x78, 0x7C, 0xDC, 0x33, 0x78, 0x7C, 0xFB, 0x3B, 0x78, 0x7D, 0x1A, 0x43, 0x78
    , 0x7F, 0xF8, 0xFB, 0x78, 0x82, 0xF8, 0x00, 0x00, 0x72, 0xEB, 0x00, 0x14, 0x2C, 0x0B, 0x00, 0x00, 0x41, 0x82, 0x00, 0x0C
    , 0x38, 0x60, 0x00, 0x02, 0x48, 0x00, 0x00, 0xEC, 0x3B, 0x20, 0x00, 0x00, 0x57, 0xBD, 0x04, 0x3E, 0x57, 0x8A, 0x07, 0x3E
    , 0x55, 0x5C, 0x80, 0x1E, 0x57, 0x7B, 0x01, 0x3E, 0x7F, 0xAB, 0xE3, 0x78, 0x91, 0x7F, 0x00, 0x10, 0x93, 0x7F, 0x00, 0x14
    , 0x3D, 0x20, 0x24, 0xE2, 0x61, 0x29, 0x5B, 0x81, 0x91, 0x3F, 0x00, 0xAC, 0x3D, 0x40, 0xA5, 0x88, 0x61, 0x4A, 0xEF, 0x0B
    , 0x91, 0x5F, 0x00, 0xBC, 0x3D, 0x80, 0xF9, 0x7E, 0x61, 0x8C, 0x81, 0x78, 0x91, 0x9F, 0x00, 0xA4, 0x3D, 0x60, 0xBC, 0xD9
    , 0x61, 0x6B, 0xA5, 0x5D, 0x91, 0x7F, 0x00, 0xA8, 0x3D, 0x20, 0x80, 0x00, 0x91, 0x3F, 0x01, 0x00, 0x3D, 0x40, 0x09, 0x8F
    , 0x61, 0x4A, 0x08, 0xE9, 0x91, 0x5F, 0x01, 0x4C, 0x81, 0x98, 0x00, 0x00, 0x61, 0x8C, 0x00, 0x04, 0x91, 0x98, 0x00, 0x00
    , 0x39, 0x60, 0xFF, 0xFF, 0x91, 0x7E, 0x00, 0x00, 0x81, 0x58, 0x00, 0x00, 0x61, 0x4A, 0x00, 0x01, 0x91, 0x58, 0x00, 0x00
    , 0x81, 0x98, 0x00, 0x00, 0x55, 0x8C, 0x05, 0x6A, 0x2C, 0x0C, 0x00, 0x00, 0x40, 0x82, 0x00, 0x18, 0x2C, 0x1A, 0xFF, 0xFF
    , 0x41, 0x82, 0xFF, 0xEC, 0x7F, 0x49, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x21, 0x4B, 0xFF, 0xFF, 0xE0, 0x81, 0x98, 0x00, 0x00
    , 0x55, 0x8C, 0x00, 0x3C, 0x91, 0x98, 0x00, 0x00, 0x81, 0x78, 0x00, 0x00, 0x55, 0x6B, 0x05, 0xAC, 0x2C, 0x0B, 0x00, 0x00
    , 0x40, 0x82, 0x00, 0x08, 0x3B, 0x20, 0x00, 0x40, 0x81, 0x98, 0x00, 0x00, 0x55, 0x8C, 0x07, 0xB8, 0x91, 0x98, 0x00, 0x00
    , 0x39, 0x60, 0xFF, 0xFF, 0x91, 0x7F, 0x01, 0x4C, 0x39, 0x40, 0x00, 0x00, 0x91, 0x5F, 0x01, 0x00, 0x7F, 0x23, 0xCB, 0x78
    , 0xBA, 0xE1, 0x00, 0x2C, 0x80, 0x01, 0x00, 0x54, 0x7C, 0x08, 0x03, 0xA6, 0x38, 0x21, 0x00, 0x50, 0x4E, 0x80, 0x00, 0x20
    , 0x4D, 0x50, 0x43, 0x39, 0x30, 0x46, 0x4C, 0x44, 0x52, 0x30, 0x31, 0x30

}; /* Total Size = 332 bytes */
#endif /* of VLE_ONE */
#endif /* of FLASH_MODULE == C90FL */


/***************************************************************************!
 *
 * @name: FSL_FlashProgramStart
 *
 * @brief: API will program the specified flash areas with the provided source data.
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *
 ****************************************************************************/
UINT32 FSL_FlashProgramStart(UINT32  dest, UINT16  size, UINT32  source)
{
    UINT32 returnCode = EE_OK;
    UINT32 buffer[PGM_SIZE/WORD_SIZE];  /* Internal source data buffer */
    UINT32 counter;                     /* Loop counter */
    UINT32 temp;                        /* Temporary variable */

    if (size > 0U)                      /* Only program if size of data > 0 */
    {
       /* Check the high voltage operation
        * ********************************/
       if ((READ32(FLASH_REG_BASE + FLASH_MCR) & (FLASH_MCR_ERS | FLASH_MCR_PGM)) != 0U)
       {
           /* Reuse variable counter for checking erase suspend */
           counter = READ32(FLASH_REG_BASE + FLASH_MCR) & (FLASH_MCR_ERS | FLASH_MCR_ESUS);
           if (counter == (FLASH_MCR_ERS | FLASH_MCR_ESUS))
           {
               /* Erase has been suspended and program operation can be performed */
           }
           else
           {
               /* if any P/E operation in progress, return error */
               returnCode = EE_INFO_HVOP_INPROGRESS;
           }
       }


       /* Program sequence
        ******************/
       if (returnCode == EE_OK)
       {
            /* Initialize buffer */
            for (counter = 0U; counter < (PGM_SIZE / WORD_SIZE); counter++)
            {
                buffer[counter] = 0xFFFFFFFFU;
            }
    
            /* Prepare source buffer to program */
            for (counter = 0U; counter < size; counter++)
            {
                /* Copy data */
                temp = (UINT32)buffer + counter;    /* Use temp as the temporary address variable */
                WRITE8(temp, READ8(source));

                /* Update source data pointer */
                source += 1U;
            }
    
            /* Set MCR PGM bit */
            SET32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_PGM);

            /* Number of words need to be programmed */
            if((size % WORD_SIZE) != 0U)
            {
                temp = (size / WORD_SIZE) + 1U;
            }
            else
            {
                temp = (size / WORD_SIZE);
            }
    
            /* Write interlockwrite */
            for (counter = 0U; counter < temp; counter++)
            {
                WRITE32(dest, buffer[counter]);
                dest += WORD_SIZE;
            }
    
            /* Set MCR EHV bit */
            SET32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_EHV);
       }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_FlashEraseStart
 *
 * @brief: API will launch a command to erase the enabled blocks in the main array of flash.
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *
 ****************************************************************************/
UINT32 FSL_FlashEraseStart( UINT32 interlockWriteAddress,
                            UINT32 lowEnabledBlock,
                            UINT32 midEnabledBlock,
                            UINT32 highEnabledBlock)
{
    UINT32 returnCode = EE_OK;

    /* Check the high voltage operation */
    if ((READ32(FLASH_REG_BASE + FLASH_MCR) & (FLASH_MCR_ERS | FLASH_MCR_PGM)) != 0U)
    {
        /* If any P/E operation in progress return error */
        returnCode = EE_INFO_HVOP_INPROGRESS;
    }
    else
    {
        /* Set MCR ERS bit */
        SET32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_ERS);

        /* Prepare low and middle enabled blocks */
        lowEnabledBlock &= 0xFFFFU;
        midEnabledBlock &= 0xFFFFU;

#if (FLASH_MODULE == C55)
        lowEnabledBlock <<= 16U;
#else
        midEnabledBlock <<= 16U;
#endif

        /* Write the block selection registers */
        WRITE32((FLASH_REG_BASE + FLASH_SEL0), (lowEnabledBlock | midEnabledBlock));
        WRITE32((FLASH_REG_BASE + FLASH_SEL1), highEnabledBlock);

        /* Interlock write */
        WRITE32(interlockWriteAddress, 0xFFFFFFFFU);

        /* Set MCR EHV bit */
        SET32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_EHV);
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_FlashCheckStatus
 *
 * @brief This function checks for the MCR register for DONE bit. Then it check
 *          whether the high voltage operation and returns appropriate error code,
 *          EE_ERROR_PE_OPT, if the operation fails else it ends the high voltage
 *          operation and then finishes the erase operation.
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *         EE_ERROR_PE_OPT: high voltage operation failed
 *
 ****************************************************************************/
UINT32 FSL_FlashCheckStatus(void)
{
    UINT32 returnCode = EE_INFO_HVOP_INPROGRESS;       /* Assume that there is still a HVOP */

    /* Check if MCR DONE is set */
    if ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_DONE) != 0U)
    {
        /* Assume that operation is OK */
        returnCode = EE_OK;

        /* Check the operation status */
        if ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_PEG) == 0U)
        {
            /* High voltage operation failed */
            returnCode = EE_ERROR_PE_OPT;
        }

        /* End the high voltage operation */
        CLEAR32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_EHV);

        /* Check for program operation */
        if ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_PGM) != 0U)
        {
            /* Finish the program operation */
            CLEAR32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_PGM);
        }
        else
        {
            /* Finish the erase operation */
            CLEAR32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_ERS);
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_FlashSuspend
 *
 * @brief: API will suspend an on-going High Voltage Operation
 *
 * @param
 *
 * @return EE_OK
 *         EE_INFO_PROGRAM_SUSPEND
 *         EE_INFO_ERASE_SUSPEND
 *
 ****************************************************************************/
UINT32 FSL_FlashSuspend(void)
{
    UINT32 returnCode = EE_OK;  /* Return code */
    UINT32 MCRValue;            /* MCR register value */
    UINT16 delay = 0U;          /* Variable to introduce delay */

    /* Get the current MCR value*/
    MCRValue = READ32(FLASH_REG_BASE + FLASH_MCR);

    /* no P/E sequence: PGM=0 and ERS=0*/

    /* possible stages for program sequence:*/
    /*    a. interlock write;               (PGM=1;EHV=0;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    b. high voltage active;           (PGM=1;EHV=1;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    c. entering suspend state;        (PGM=1;EHV=1;  PSUS=1;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    d. in suspend state;              (PGM=1;EHV=1/0;PSUS=1;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    e. resuming from suspend state;   (PGM=1;EHV=1;  PSUS=0;DONE=1; ignore PEG)  OR (ERS=1;ESUS=1)*/
    /*    f. high voltage stopped;          (PGM=1;EHV=1;  PSUS=0;DONE=1; valid  PEG)  OR (ERS=1;ESUS=1)*/
    /*    g. abort period.                  (PGM=1;EHV=0;  PSUS=0;DONE=0; ignore PEG)  OR (ERS=1;ESUS=1)*/

    /* possible stages for erase sequence:*/
    /*    a. interlock write;               (ERS=1;EHV=0;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    b. high voltage active;           (ERS=1;EHV=1;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    c. entering suspend state;        (ERS=1;EHV=1;  ESUS=1;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    d. in suspend state;              (ERS=1;EHV=1/0;ESUS=1;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    e. resuming from suspend state;   (ERS=1;EHV=1;  ESUS=0;DONE=1; ignore PEG)  AND (PGM=0;PSUS=0)*/
    /*    f. high voltage stopped;          (ERS=1;EHV=1;  ESUS=0;DONE=1; valid  PEG)  AND (PGM=0;PSUS=0)*/
    /*    g. abort period.                  (ERS=1;EHV=0;  ESUS=0;DONE=0; ignore PEG)  AND (PGM=0;PSUS=0)*/

    /* b. high voltage active*/
    /* e. resuming from suspend state*/
    /* f. high voltage stopped*/

    if ((MCRValue & FLASH_MCR_EHV) != 0U)
    {
        if (((MCRValue & FLASH_MCR_PGM) != 0U) && ((MCRValue & FLASH_MCR_PSUS) == 0U))
        {
            /* Suspend the program operation if there isn't an suspended erase operation */
            if (((MCRValue & FLASH_MCR_ERS) == 0U) && ((MCRValue & FLASH_MCR_ESUS) == 0U))
            {
                /* Program suspend*/
                FLASH_MCR_BIT_SET(((UINT32)FLASH_REG_BASE + (UINT32)FLASH_MCR), (UINT32)FLASH_MCR_PSUS);   /* set MCR-PSUS bit*/
                returnCode = EE_INFO_PROGRAM_SUSPEND;

                /* Add delay to counter hardware bug*/
                /* A delay of approximately 20us for the below clock settings*/
                /* SYSTEM CLOCK FREQUENCY = 128MHZ*/
                /* A delay of about 20us*/
                for (delay = 0U; delay < 0xA0U; delay++)
                {
                    /* Do nothing */
                }
            }
        }

        if (((MCRValue & FLASH_MCR_ERS) != 0U) && ((MCRValue & FLASH_MCR_ESUS) == 0U))
        {
            /* Suspend the erase operation if there isn't an suspended program operation */
            if (((MCRValue & FLASH_MCR_PGM) == 0U) && ((MCRValue & FLASH_MCR_PSUS) == 0U))
            {
                /* Erase suspend*/
                FLASH_MCR_BIT_SET(((UINT32)FLASH_REG_BASE + (UINT32)FLASH_MCR), (UINT32)FLASH_MCR_ESUS);   /* set MCR-ESUS bit*/
                returnCode = EE_INFO_ERASE_SUSPEND;

                /* Add delay to counter hardware bug*/
                /* A delay of approximately 40us for the below clock settings*/
                /* SYSTEM CLOCK FREQUENCY = 128MHZ*/
                /* A delay of about 40us*/
                for (delay = 0U; delay < 0x140U; delay++)
                {
                    /* Do nothing */
                }
            }
        }
    }

    /* Wait until MCR-DONE = 1 */
    while ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_DONE) == 0U)
    {

    }

    /* Set MCR-EHV to 0 */
    FLASH_MCR_BIT_CLEAR (((UINT32)FLASH_REG_BASE + (UINT32)FLASH_MCR), (UINT32)FLASH_MCR_EHV);

    return (returnCode);
}


/***************************************************************************!
 *
 * @name: FSL_FlashResume
 *
 * @brief: API will resume an on-going High Voltage Operation which has been suspended before.
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void FSL_FlashResume(UINT32 suspendStatus)
{
    UINT32 mask = 0U;       /* Bit mask value */
    UINT32 count;           /* Loop counter */

    /* Initialize the loop counter */
    count = FLASH_RESUME_WAIT;

    /* Check the suspend state*/
    if (suspendStatus == EE_INFO_PROGRAM_SUSPEND)
    {
        /* Need resume programming*/
        mask = FLASH_MCR_PSUS;
    }

    if (suspendStatus == EE_INFO_ERASE_SUSPEND)
    {
        /* Need resume erasing*/
        mask = FLASH_MCR_ESUS;
    }

    /* Resume */
    if ((suspendStatus == EE_INFO_PROGRAM_SUSPEND) || (suspendStatus == EE_INFO_ERASE_SUSPEND))
    {
        /* Set MCR-EHV bit*/
        FLASH_MCR_BIT_SET (((UINT32)FLASH_REG_BASE + (UINT32)FLASH_MCR), (UINT32)FLASH_MCR_EHV);

        /* Clear the MCR-ESUS bit or MCR-PSUS bit*/
        FLASH_MCR_BIT_CLEAR ((FLASH_REG_BASE + FLASH_MCR), mask);

        /* Wait the MCR-DONE bit goes low*/
        while ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_DONE) && (count > 0U))
        {
            count--;
        }
    }
}


/***************************************************************************!
 *
 * @name: FSL_FlashRead
 *
 * @brief: This API is used to read the content of the Flash memory.
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
UINT32 FSL_FlashAbortErase(void)
{
    UINT32 returnCode = EE_OK;

    /* Check if MCR ERS is set */
    if ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_ERS) != 0U)
    {
        /* End the high voltage operation */
        CLEAR32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_EHV);

        /* Wait for the DONE bit is set */
        while ((READ32(FLASH_REG_BASE + FLASH_MCR) & FLASH_MCR_DONE) == 0U)
        {

        }

        /* Finish the erase operation */
        CLEAR32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_ERS);

        returnCode = EE_INFO_ERASE_SUSPEND;
    }

    return returnCode;
}


/***************************************************************************!
 * @name: FSL_FlashRead
 *
 * @brief: This API is used to read the content of the Flash memory.
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_MISMATCH: blank check or program verify failed
 *
 ****************************************************************************/
UINT32 FSL_FlashRead(EE_ReadCode func, UINT32 dest, UINT32 size, UINT32 buffer, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;
    UINT32 counter;             /* loop counter */
    UINT8 value, temp = 0xFFU;

    /* Setup ECC detection method */
#if (EER_OPTION == IVOR_EXCEPTION)
    UINT32 temp_MSR;
    /* Save old MSR */
    temp_MSR = cReadAndClearEei();
    cRestoreEei((UINT32)ENABLE_MSR);
    /* Set ECC Error Module flag */
    eccErrorModule_Flag = EE_MODULE;

#else
    if ((FLASH_MCR_EER & READ32(FLASH_REG_BASE + FLASH_MCR)) != 0U)
    {
        SET32((FLASH_REG_BASE + FLASH_MCR), FLASH_MCR_EER);
    }
#endif

    /* Clear the EEC flag before reading */
    eccErrorStatus_Flag = FALSE;

    /* Read data
     ***********/
    for (counter = 1U; counter <= size; counter++)
    {
        /* Callback service */
        if ((counter % CALLBACK_READ_BYTES_NUMBER) == 0U)
        {
            if (CallBack != NULL_CALLBACK)
            {
                CallBack();
            }
        }

        /* Get data */
        value = READ8(dest);

        /* Check for ECC */
#if (EER_OPTION == EER_MCR)
        if ((FLASH_MCR_EER & READ32(FLASH_REG_BASE + FLASH_MCR)) && 
            ((dest & (~0x3U)) == FSL_GetFailedAddr()) &&
            (value == 0xFFU))
        {
            eccErrorStatus_Flag = TRUE;
        }
#endif
        if (func == READ)                               /* Reading operation */
        {
            WRITE8(buffer, value);
        }
        else
        {
            if(eccErrorStatus_Flag != FALSE)
            {
                returnCode = EE_ERROR_MISMATCH;
            }
            else
            {
                if (func == VERIFY)                     /* Verify operation */
                {
                    temp = READ8(buffer);
                }
                if (temp != value)
                {
                    returnCode = EE_ERROR_MISMATCH;
                }
            }
        }

        /* Stop the verification if there is a mismatch */
        if (returnCode != EE_OK)
        {
            break;
        }

        /* Update destination and buffer */
        dest += 1U;
        buffer += 1U;
    }

#if (EER_OPTION == IVOR_EXCEPTION)
    /* Restore ECC Module flag */
    eccErrorModule_Flag = NONE;
    /* Restore MSR */
    cRestoreEei(temp_MSR);
#endif

    return (returnCode);
}


/***************************************************************************!
 *
 * @name: EER_exception_handler
 *
 * @brief This function is service routine for ECC read interrupt
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
#if (EER_OPTION == IVOR_EXCEPTION)
UINT32 EER_exception_handler(UINT32 return_address, UINT16 instruction)
{
    if (eccErrorModule_Flag == EE_MODULE)
    {
        eccErrorStatus_Flag = TRUE;

#if (VLE_IS_ON == 1)
        if ((instruction & 0x9000U) == 0x1000U)
        {
            /* First 4 Bits have a value of 1,3,5,7 */
            return_address += 4U;    /* Instruction was 32 bit */
        }
        else
        {
            /* First 4 Bits have a value of 0,2,4,6,8,9,A,B,C,D,E (and F, but F is reserved) */
            return_address += 2U;    /* Instruction was 16 bit */
        }

#else   /* BookE */
        return_address += 4U;
#endif	/* VLE_IS_ON */
    }

    return return_address;
}
#endif	/* EER_OPTION */


/***************************************************************************!
 *
 * @name  FSL_GetFailedAddr
 *
 * @brief This function will return the failed address based on the information.
 *        provided on ADR register.
 * @param
 *
 * @return      failed address
 *
 ****************************************************************************/

#if (EER_OPTION == EER_MCR)
static UINT32 FSL_GetFailedAddr(void)
{
    UINT32 addr = 0xFFFFFFFFU;
    UINT32 addrValue;
    UINT32 addrOffset;

    addrValue = READ32(FLASH_REG_BASE + FLASH_ADR);
    addrOffset = (addrValue & FLASH_ADR_ADDR);

    if ((addrValue & FLASH_ADR_aH) != 0U)
    {
        if ((addrValue & FLASH_ADR_a16K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aH_a16K;
        }
        else if ((addrValue & FLASH_ADR_a32K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aH_a32K;
        }
        else if ((addrValue & FLASH_ADR_a64K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aH_a64K;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if ((addrValue & FLASH_ADR_aM) != 0U)
    {
        if ((addrValue & FLASH_ADR_a16K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aM_a16K;
        }
        else if ((addrValue & FLASH_ADR_a32K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aM_a32K;
        }
        else if ((addrValue & FLASH_ADR_a64K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aM_a64K;
        }
        else
        {
            /* Do nothing */
        }
    }
    else if ((addrValue & FLASH_ADR_aL) != 0U)
    {
        if ((addrValue & FLASH_ADR_a16K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aL_a16K;
        }
        else if ((addrValue & FLASH_ADR_a32K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aL_a32K;
        }
        else if ((addrValue & FLASH_ADR_a64K) != 0U)
        {
            addr = addrOffset + BASE_ADDR_aL_a64K;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return addr;
}
#endif /* of EER_OPTION */

/* EOF */
