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
* $FileName: eed_highlevel.c$
* $Version : 1.2.0$
* $Date    : Jun-10-2015$
*
* Comments:
*
*   This file consitutes of high level API's.
*
*END************************************************************************/

#include "ee_internal.h"

/*******************************************************************
| global variable definitions (scope: module-local)
|------------------------------------------------------------------*/

/* Flag to keep track of Erase State */
EE_EraseType eraseStatus_Flag = ERASE_NOTSTARTED;

/* Variable to store internal erase cycle */
UINT32 erasingCycleInternal = 0x00U;

/* Variable to store index of the erasing block while swapping */
UINT32 sourceBlockIndexInternal;

/* Flag to keep track of ECC Error Status */
VUINT8 eccErrorStatus_Flag = FALSE;

/* Flag to call FlashDepletionRecover() or not */
#if (FLASH_MODULE == C90FL)
BOOL depletion_Flag = FALSE;
#endif

/***************************************************************************!
 *
 * @name:  FSL_InitEeprom
 *
 * @brief: Init the EEPROM Emulation driver (software)
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD
 *         EE_ERROR_WRITE_IN_PROGRESS
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR
 *         EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK
 *         EE_ERROR_BLOCK_CONFIG
 *         EE_ERROR_NO_ENOUGH_SPACE
 *
 ****************************************************************************/
UINT32 FSL_InitEeprom(EEPROM_CONFIG* eepromConfig, void (*CallBack) (void))
{
    BLOCK_CONFIG* pBlockConf;
    UINT32 returnCode = EE_OK;
    UINT32 temp = 0U;
    UINT32 i = 0U;
    UINT32 dest;
    UINT32 size;
    UINT32 source;
    UINT32 eraseCycle;
    UINT32 oldestActIdx = 0U;
    BOOL tempBool = TRUE;

    if (eepromConfig->numberOfBlock <= NUMBER_OF_ACTIVE_BLOCKS)
    {
        /* There isn't enough number of block for emulation */
        returnCode = EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN;
    }
    else if (eepromConfig->blockWriteFlag == TRUE)       /* Check for Write operation */
    {
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Set the write flag */
        eepromConfig->blockWriteFlag = TRUE;

        /* Reset erase status flag */
        eraseStatus_Flag = ERASE_NOTSTARTED;

        /* Reset depletion flag*/
#if (FLASH_MODULE == C90FL)
        depletion_Flag = FALSE;
#endif

        /* Clear Cache table */
        if (eepromConfig->cacheEnable == TRUE)
        {
            temp = eepromConfig->cTable->size / 4U;  /* Get number of Cache items */
            for (i = 0U; i < temp; i++)
            {
                 tempBool = FSL_UpdateCacheTable(eepromConfig->cTable, (UINT16)i, 0xFFFFFFFFU);
            }
        }

        /* Validate Invalid and Dead blocks */
        returnCode = FSL_ValidateInvalidAndDeadBlocks(eepromConfig, CallBack);

        /* Get the last job before brownout. The lastest ACTIVE block index is also updated */
        if (returnCode == EE_OK)
        {
            returnCode = FSL_GetLastJobStatus(eepromConfig, CallBack);

            /* Recorver the brownout operations
             **********************************/

            /* ========================== First time initialization ========================= */
            if (returnCode == EE_LAST_JOB_FIRST_TIME)
            {
                returnCode = EE_OK;             /* Reset returnCode */

                temp = eepromConfig->numberOfBlock;     /* Get number of blocks */

                /* Erase all blocks in opposite order with the blocks initialization progress */
                for (i = temp; i > 0U; i--)
                {
                    if (returnCode == EE_OK)
                    {
                        /* Blank check the block */
                        pBlockConf = eepromConfig->flashBlocks[i - 1U];
                        returnCode = FSL_FlashRead(BLANK_CHECK, pBlockConf->blockStartAddr, pBlockConf->blockSize, 0U, CallBack);

                        if (returnCode != EE_OK)    /* Erase the block if it isn't blank */
                        {
                            returnCode = FSL_EraseEEBlock(eepromConfig, (i - 1U), TRUE, CallBack);
                        }
                    }

                    /* Make to DEAD if erasing failed */
                    if ((returnCode == EE_ERROR_MISMATCH) || (returnCode == EE_ERROR_PE_OPT))
                    {
                        returnCode = FSL_MakeBlock2Dead(eepromConfig, pBlockConf, CallBack);
                        if (returnCode == EE_MAKE_DEAD_OK)
                        {
                            returnCode = EE_OK;  /* Reset returnCode */
                        }
                    }
                }

                /* Init all blocks */
                temp = eepromConfig->numberOfBlock;     /* Re-get number of blocks because may be have DEAD block */
                eraseCycle = 1U;                        /* The first erase cycle value */
                for (i = 0U; i < temp; i++)
                {
                    /* Only make the block to ALTER by writing erase cycle if the previous step passed*/
                    if (returnCode == EE_OK)
                    {
                        pBlockConf = eepromConfig->flashBlocks[i];
                        returnCode = FSL_SyncProgram(pBlockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                                     WORD_SIZE, (UINT32)(&eraseCycle), CallBack);
                    }

                    /* Make the first block to ACTIVE */
                    if ((returnCode == EE_OK) && (i == 0U))
                    {
                        source = ACT_INDICATOR_ACT;
                        returnCode = FSL_SyncProgram(pBlockConf->blockStartAddr + ACT_IND_OFFSET,
                                                     WORD_SIZE, (UINT32)(&source), CallBack);
                    }
                }

                /* Set lastest ACTIVE block index */
                eepromConfig->activeBlockIndex = 0U;
            }

            /* ============== Brownout while copying data, finishing copy data =============== */
            else if ((returnCode == EE_LAST_JOB_COPY_DONE) || (returnCode == EE_LAST_JOB_UPDATE))
            {
                if (returnCode == EE_LAST_JOB_UPDATE)
                {
                    /* Call swapping function synchronously to finish the brownout progress */
                    returnCode = FSL_BlockSwapping(eepromConfig, TRUE, CallBack);
                }
                else
                {
                    returnCode = EE_OK;     /* Reset returnCode */
                }

                /* Erase the oldest ACTIVE block */
                if (returnCode == EE_OK)
                {
                    /* Get index of the oldest ACTIVE block */
                    oldestActIdx = (eepromConfig->activeBlockIndex + eepromConfig->numberOfBlock - NUMBER_OF_ACTIVE_BLOCKS) %
                                    eepromConfig->numberOfBlock;

                    pBlockConf = eepromConfig->flashBlocks[oldestActIdx];

                    /* Erase the oldest ACTIVE block */
                    returnCode = FSL_EraseEEBlock(eepromConfig, oldestActIdx, TRUE, CallBack);
                }

                /* Make the erased block to ALTER */
                if (returnCode == EE_OK)
                {
                    /* Get index of the block right after the erased blocks */
                    i = (oldestActIdx + 1U) % eepromConfig->numberOfBlock;

                    /* Get erased cycle of that block */
                    temp = READ32(eepromConfig->flashBlocks[i]->blockStartAddr + ERASE_CYCLE_OFFSET);

                    /* Canculate erase cycle for the erased block base on the round-robin sequence */
                    if (i != 0U)
                    {
                        temp++;
                    }

                    /* Write erase cycle to the erased block */
                    returnCode = FSL_SyncProgram(pBlockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                                 WORD_SIZE, (UINT32)(&temp), CallBack);
                }
                else    /* Make the unsuccess erased block to DEAD */
                {
                    returnCode = FSL_MakeBlock2Dead(eepromConfig, pBlockConf, CallBack);
                }

                /* Make the COPY_DONE block to ACTIVE */
                if (returnCode == EE_OK)
                {
                    /* Get the COPY_DONE block */
                    pBlockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];
                    source = ACT_INDICATOR_ACT;

                    /* Mark as ACTIVE */
                    returnCode = FSL_ProgramBlockIndicator(pBlockConf->blockStartAddr + ACT_IND_OFFSET,
                                                           (UINT32)(&source), CallBack);
                }
            }
            else if (returnCode == EE_LAST_JOB_NORMAL)
            {
                returnCode = EE_OK;     /* Reset returnCode */
            }
            else
            {
                /* Can't identify the situation */
                returnCode = EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK;
            }
        }

        /* Unlock the write lock */
        eepromConfig->blockWriteFlag = FALSE;
    }


    /* Update blankspace and fill cache table
     ****************************************/
    if (returnCode == EE_OK)
    {
        /* Get index of the oldest ACTIVE block */
        oldestActIdx = (eepromConfig->activeBlockIndex + eepromConfig->numberOfBlock - NUMBER_OF_ACTIVE_BLOCKS) %
                        eepromConfig->numberOfBlock;

        /* Get address and size of the cache table */
        if (eepromConfig->cacheEnable == TRUE)
        {
            dest = eepromConfig->cTable->startAddress;  /* Use dest to store address of cache table */
            size = eepromConfig->cTable->size;          /* Use size to store size of cache table */
        }
        else
        {
            dest = (UINT32)(&eraseCycle);               /* Use eraseCycle as temporary buffer */
            size = WORD_SIZE;
        }

        /* Scan from the oldest ACTIVE block to ensure that
           newest records will be update in to Cache table */
        for (i = oldestActIdx; i < (eepromConfig->numberOfBlock + oldestActIdx); i++)
        {
            /* Get block config */
            temp = i % eepromConfig->numberOfBlock;
            pBlockConf = eepromConfig->flashBlocks[temp];

            /* Read status, use the 'temp' as a temporary variable to store block status */
            temp = FSL_ReadBlockStatus(pBlockConf, CallBack);
            if (temp == B_ACT)
            {
                /* Use source as a temporary variable to store startID,
                 * reset startID to '0' fill cache table from the first location */
                source = 0U;

                /* The function fill cache table and update blankspace at the same time */
                FSL_RecordBuffering(pBlockConf, dest, size, (UINT16*)(&source), CallBack);
            }
            if (temp == B_ALT)
            {
                /* With ALT block the blank space is start of Record region */
                pBlockConf->blankSpace = pBlockConf->blockStartAddr + RECORD_OFFSET;
            }
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name:  FSL_ReadEeprom
 *
 * @brief: This API is to read the specific data record. The data record size
 *        is determined by the data size written into the EEPROM emulation.
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_WRITE_IN_PROGRESS
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_DATA_NOT_FOUND
 *
 * comments:
 * User must ensure that the data size to be read does not exceed the RAM available.
 * This API can be called when an erase is going on such as a swapping is being done.
 * If the erased block is in different partition with the read target block, it will
 * read the expected data record without suspending that high voltage operation. Otherwise,
 * if the erased block is in the same partition, it will read the expected data record
 * after suspending this high voltage. However, FSL_MainFunction still need to be called
 * after that to update block status for all blocks.
 ****************************************************************************/
UINT32 FSL_ReadEeprom(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT16 dataSize, UINT32 buffAddr, void (*CallBack) (void))
{
    BLOCK_CONFIG *blockConf;     /* Local block configuration pointer */
    UINT32 returnCode = EE_OK;   /* Return code */
    UINT32 recordAddr;           /* Data record address */
    UINT32 suspendState = EE_OK;
    UINT32 i;
    UINT8 iReq = EE_IMMEDIATE_NONE;


    /* Check the write lock */
    if (eepromConfig->blockWriteFlag == TRUE)
    {
        /* A write operation is already in progress, no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Mask the immediate bit from data ID */
        if ((dataID & IMMEDIATE_MASK) != 0U)
        {
            dataID = dataID & (~IMMEDIATE_MASK);
            iReq = EE_IMMEDIATE_READ;    /* Immediate flag */
        }


        /* Search in the cache table
         ***************************/
        if (eepromConfig->cacheEnable == TRUE)
        {
            returnCode = FSL_SearchInTable(eepromConfig->cTable, dataID, (UINT32*)(&recordAddr));
            if (returnCode == EE_OK)
            {
                for (i = 0U; i < eepromConfig->numberOfBlock; i++)
                {
                    blockConf = eepromConfig->flashBlocks[i];

                    /* If the record address belong to the block range */
                    if ((blockConf->blockStartAddr <= recordAddr) &&
                        (recordAddr < (blockConf->blockStartAddr + blockConf->blockSize)))
                    {
                        /* Suspend the high voltage operaton if the request is immediate */
                        suspendState = FSL_ProcessImmediateRequest(eepromConfig, blockConf, iReq);

                        /* Do nothing with normal request while erasing a block */
                        if (suspendState == EE_INFO_HVOP_INPROGRESS)
                        {
                            returnCode = EE_INFO_HVOP_INPROGRESS;
                        }

                        break;
                    }
                }
            }
        }


        /* Search in flash block if cache is disable or the record is not in cache
         *************************************************************************/
        if ((eepromConfig->cacheEnable == FALSE) || (returnCode == EE_ERROR_NOT_IN_CACHE))
        {
            /* The FSL_SearchInAllBlocks function will suspend High voltage operation if need */
            returnCode = FSL_SearchInAllBlocks(eepromConfig, dataID, iReq,
                                               (UINT32*)(&recordAddr), (UINT32*)(&suspendState), CallBack);
        }


        /* Read record if found */
        if (returnCode == EE_OK)
        {
            /* Read the record if found */
            FSL_ReadRecordAtAddr(recordAddr, dataSize, buffAddr, CallBack);
        }


        /* Resume high voltage if it has been suspended */
        if (suspendState == EE_INFO_ERASE_SUSPEND)
        {
#if((FLASH_MODULE != C90LC) && (FLASH_MODULE != C90LC_DFO))
            FSL_FlashResume(suspendState);
#else
           /* Re-erase the suspended operation */
            i = FSL_EraseEEBlock(eepromConfig, sourceBlockIndexInternal, FALSE, CallBack);
#endif
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name:  FSL_ReportEepromStatus
 *
 * @brief: This API is to report block erasing cycles and check the block status.
 *
 * @param
 *
 * @return EE_ERROR_WRITE_IN_PROGRESS
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_BLOCK_STATUS
 *         EE_OK
 *
 ****************************************************************************/
UINT32 FSL_ReportEepromStatus(EEPROM_CONFIG* eepromConfig, UINT32* erasingCycles)
{
    BLOCK_CONFIG *blockConf;    /* local block configuration pointer */
    UINT32 returnCode;
    UINT32 blockStatus;         /* Block status */
    UINT32 i;                   /* Loop counter */

    /* Check the write lock */
    if (eepromConfig->blockWriteFlag != 0U)
    {
        /* A write operation is already in progress, */
        /* no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Get flash status */
        returnCode = FSL_FlashCheckStatus();

        if (returnCode != EE_INFO_HVOP_INPROGRESS)
        {
            /* Read erase cycle of the last ACTIVE block */
            blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

            /* Use blockStatus as temporary return value */
            blockStatus = FSL_FlashRead(READ, blockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                        WORD_SIZE, (UINT32)erasingCycles, NULL_CALLBACK);

            /* Check the block status */
            for (i = 0U; i < eepromConfig->numberOfBlock; i++)
            {
                /* Get block config */
                blockConf = eepromConfig->flashBlocks[i];
                blockStatus = FSL_ReadBlockStatus(blockConf, NULL_CALLBACK);

                /* Check the block status */
                if ((blockStatus != B_ACT) && (blockStatus != B_COPY_DONE) &&
                    (blockStatus != B_ALT) && (blockStatus != B_ERASED))
                {
                    /* Error status -> need call FSL_InitEeprom to refresh the system */
                    returnCode = EE_ERROR_BLOCK_STATUS;
                    break;
                }
            }
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name:  FSL_DeleteRecord
 *
 * @brief: This API is to delete a data record in the EEPROM emulated Flash.
 *
 * @param
 *
 * @return EE_ERROR_WRITE_IN_PROGRESS
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_DATA_NOT_FOUND
 *         EE_ERROR_PE_OPT
 *         EE_ERROR_MISMATCH
 *         EE_OK
 *
****************************************************************************/
UINT32 FSL_DeleteRecord(EEPROM_CONFIG* eepromConfig, UINT16 dataID, void (*CallBack) (void))
{
    BLOCK_CONFIG *blockConf;     /* Local block configuration pointer */
    UINT32 returnCode = EE_OK;   /* Return code */
    UINT32 recordAddr;           /* Data record address */
    UINT32 suspendState = EE_OK;
    UINT32 i;
    UINT8 iReq = EE_IMMEDIATE_NONE;
    BOOL updateCache;


    /* Check the write lock */
    if (eepromConfig->blockWriteFlag == TRUE)
    {
        /* A write operation is already in progress, no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Set the write lock flag */
        eepromConfig->blockWriteFlag = TRUE;

        /* Mask the immediate bit from data ID */
        if ((dataID & IMMEDIATE_MASK) != 0U)
        {
            dataID = dataID & (~IMMEDIATE_MASK);
            iReq = EE_IMMEDIATE_WRITE;    /* Immediate flag */
        }


        /* Search in the cache table
         ***************************/
        if (eepromConfig->cacheEnable == TRUE)
        {
            returnCode = FSL_SearchInTable(eepromConfig->cTable, dataID, (UINT32*)(&recordAddr));
            if (returnCode == EE_OK)
            {
                for (i = 0U; i < eepromConfig->numberOfBlock; i++)
                {
                    blockConf = eepromConfig->flashBlocks[i];

                    /* If the record address belong to the block range */
                    if ((blockConf->blockStartAddr <= recordAddr) &&
                        (recordAddr < (blockConf->blockStartAddr + blockConf->blockSize)))
                    {
                        /* Suspend the high voltage operaton if the request is immediate */
                        suspendState = FSL_ProcessImmediateRequest(eepromConfig, blockConf, iReq);

                        /* Do nothing with normal request while erasing a block */
                        if (suspendState == EE_INFO_HVOP_INPROGRESS)
                        {
                            returnCode = EE_INFO_HVOP_INPROGRESS;
                        }

                        break;
                    }
                }
            }
        }


        /* Search in flash block if cache is disable or the record is not in cache
         *************************************************************************/
        if ((eepromConfig->cacheEnable == FALSE) || (returnCode == EE_ERROR_NOT_IN_CACHE))
        {
            /* The FSL_SearchInAllBlocks function will suspend High voltage operation if need */
            returnCode = FSL_SearchInAllBlocks(eepromConfig, dataID, iReq,
                                               (UINT32*)(&recordAddr), (UINT32*)(&suspendState), CallBack);
        }


        /* Mark the record as deleted */
        if (returnCode == EE_OK)
        {
            i = R_DELETE;   /* Use i as a data source for writing */
            returnCode = FSL_SyncProgram(recordAddr, MIN_PGM_SIZE, (UINT32)(&i), CallBack);

            if (returnCode == EE_OK)
            {
                /* Check if the cache table is enabled */
                if (eepromConfig->cacheEnable == TRUE)
                {
                    updateCache = FSL_UpdateCacheTable(eepromConfig->cTable, dataID, 0xFFFFFFFFU);
                }
            }
        }


        /* Resume high voltage if it has been suspended */
        if (suspendState == EE_INFO_ERASE_SUSPEND)
        {
#if((FLASH_MODULE != C90LC) && (FLASH_MODULE != C90LC_DFO))
            FSL_FlashResume(suspendState);
#else
           /* Re-erase the suspended operation */
            i = FSL_EraseEEBlock(eepromConfig, sourceBlockIndexInternal, FALSE, CallBack);
#endif
        }

        /* Unlock the write lock */
        eepromConfig->blockWriteFlag = FALSE;
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name:  FSL_RemoveEeprom
 *
 * @brief: This function is to clear all blocks used for EEPROM emulation
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_BLOCK_CONFIG: block configuration is not correct
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_WRITE_IN_PROGRESS:
 *         EE_ERROR_PE_OPT:
 *
 ****************************************************************************/
UINT32 FSL_RemoveEeprom(EEPROM_CONFIG* eepromConfig, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;      /* Return code */
    UINT32 i;

    /* Check the write lock of the FlashEE */
    if (eepromConfig->blockWriteFlag != 0U)
    {
        /* A write operation is already in progress, */
        /* no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Set the write lock flag */
        eepromConfig->blockWriteFlag = TRUE;

        /* Erase all blocks */
        for (i = 0U; i < eepromConfig->numberOfBlock; i++)
        {
            returnCode |= FSL_EraseEEBlock(eepromConfig, i, TRUE, CallBack);
        }

        if ((returnCode & EE_ERROR_MISMATCH) != 0U)
        {
            returnCode = EE_ERROR_PE_OPT;
        }

        /* Unlock the write lock */
        eepromConfig->blockWriteFlag = FALSE;
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_WriteEeprom
 *
 * @brief; This API is to write data records to the EEPROM emulated Flash
 *          and re-write data record if this program operation fails.
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_WRITE_IN_PROGRESS
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_NO_ENOUGH_SPACE
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR
 *
 * comment:
 * If an immediate data request while an erase operation is going on, the operation
 * will be suspended to serve this writing in advance.Note that if this API is called
 * to write a normal data while an erase is going on such as a swapping is being going
 * on, it will return EE_INFO_HVOP_INPROGRESS
 ****************************************************************************/
UINT32 FSL_WriteEeprom(EEPROM_CONFIG* eepromConfig, \
                        UINT16 dataID, UINT16 dataSize, UINT32 source, void (*CallBack) (void))
{
    BLOCK_CONFIG *blockConf;    /* BlockConf configuration */
    UINT32 returnCode = EE_OK;
    UINT32 recordLength;        /* Length of record */
    UINT32 temp;
    UINT32 suspendState = EE_OK;
    UINT32 recordAddr;
    UINT8 iReq = EE_IMMEDIATE_NONE;
    BOOL updateCache;
    

#if(SIZE_FIELD == 0U)
    dataSize = DATA_SIZE;
#endif

    /* Check the write lock */
    if (eepromConfig->blockWriteFlag == TRUE)
    {
        /* A write operation is already in progress, no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Set the write lock flag */
        eepromConfig->blockWriteFlag = TRUE;

        /* Mask the immediate bit from data ID */
        if ((dataID & IMMEDIATE_MASK) != 0U)
        {
            dataID = dataID & (~IMMEDIATE_MASK);
            iReq = EE_IMMEDIATE_WRITE;    /* Immediate flag */
        }

        /* Suspend the high voltage operaton if the request is immediate */
        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];
        suspendState = FSL_ProcessImmediateRequest(eepromConfig, blockConf, iReq);


        /* Start writing the record
        **************************/
        if (suspendState != EE_INFO_HVOP_INPROGRESS)
        {
            /* Get record length base on size of raw data */
            recordLength = FSL_GetRecordLength(dataSize);

            do
            {
                /* Get writing option base on recordLength */
                returnCode = FSL_GetWriteRecordOption(eepromConfig, recordLength, CallBack);

                /* Write record base on situations
                 *********************************/
                if ((returnCode == EE_WRITE_NORMAL) ||
                    (returnCode == EE_WRITE_ON_NEW_ACTIVE) ||
                    (returnCode == EE_WRITE_ON_COPYDONE))
                {
                    if (returnCode == EE_WRITE_ON_NEW_ACTIVE)
                    {
                        /* Update index of the next ACTIVE block */
                        eepromConfig->activeBlockIndex = (eepromConfig->activeBlockIndex + 1U) % eepromConfig->numberOfBlock;

                        /* Make the next ALT block to ACTIVE */
                        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];
                        temp = ACT_INDICATOR_ACT;
                        returnCode = FSL_ProgramBlockIndicator(blockConf->blockStartAddr + ACT_IND_OFFSET,
                                                               (UINT32)(&temp), CallBack);

                        /* Update blank space */
                        if (returnCode == EE_OK)
                        {
                            blockConf->blankSpace = blockConf->blockStartAddr + RECORD_OFFSET;
                        }
                    }
                    else
                    {
                        returnCode = EE_OK;     /* Reset returnCode, to goto the next step */
                    }

                    if (returnCode == EE_OK)
                    {
                        /* Set blockConf to current ACTIVE block */
                        blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

                        /* Backup record address */
                        recordAddr = blockConf->blankSpace;

                        /* Copy one data record to free space in the active block */
                        returnCode = FSL_WriteDataRecord(blockConf, FALSE, dataID, dataSize, source, CallBack);
                        if (returnCode == EE_OK)
                        {
                            /* Check if the cache table is enabled */
                            if (eepromConfig->cacheEnable == TRUE)
                            {
                                updateCache = FSL_UpdateCacheTable(eepromConfig->cTable, dataID, recordAddr);
                            }
                        }
                    }
                }
                else if (returnCode == EE_WRITE_SWAP)
                {
                    /* Get the next ALT block */
                    temp = (eepromConfig->activeBlockIndex + 1U) % eepromConfig->numberOfBlock;
                    blockConf = eepromConfig->flashBlocks[temp];

                    /* Write data to the next ALT block. We use FSL_CopyDataRecord instead of FSL_WriteDataRecord because
                     * nest swapping isn't allowed. If it isn't enough space, FSL_CopyDataRecord will return
                     * EE_ERROR_NO_ENOUGH_SPACE*/
                    returnCode = FSL_CopyDataRecord(eepromConfig, blockConf, FALSE, dataID, dataSize, source, CallBack);

                    if (returnCode == EE_OK)
                    {
                        /* Async Swapping */
                        returnCode = FSL_BlockSwapping(eepromConfig, FALSE,CallBack);
                    }
                }
                else /* EE_ERROR_NO_ENOUGH_SPACE */
                {
                    /* Return an error */
                    returnCode = EE_ERROR_NO_ENOUGH_SPACE;
                }
            }
            while (returnCode == EE_ERROR_PE_OPT);   /* Rewrite if there is an EE_ERROR_PE_OPT error */
        }
        else
        {
            returnCode = EE_INFO_HVOP_INPROGRESS;  /* Return an HVOP error */
        }


        /* Check erase suspend
         *********************/
        if (suspendState == EE_INFO_ERASE_SUSPEND)
        {
#if((FLASH_MODULE != C90LC) && (FLASH_MODULE != C90LC_DFO))
            FSL_FlashResume(suspendState);
#else
           /* Re-erase the suspended operation */
            temp = FSL_EraseEEBlock(eepromConfig, sourceBlockIndexInternal, FALSE, CallBack);
#endif
        }

        /* Unlock the write lock */
        eepromConfig->blockWriteFlag = FALSE;
    }

    return returnCode;
}


/**************************************************************************//*!
 *
 * @name: FSL_MainFunction
 *
 * @brief: This API will help in erasing the oldest ACTIVE block asynchronously,
 *          update erase cycles and block status.
 *
 * @param
 *
 * @return EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD: failed to make the block to DEAD
 *         EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN: not enough block for round robin
 *         EE_MAKE_DEAD_OK: make to DEAD successfully
 *         EE_OK
 *         EE_ERROR_PROGRAM_ERASE_CYCLE
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR
 *
 ****************************************************************************/
UINT32 FSL_MainFunction(EEPROM_CONFIG *eepromConfig, void (*CallBack) (void))
{
    BLOCK_CONFIG* blockConf;
    UINT32 returnCode = EE_OK;
    UINT32 blockStatus;

    /* CallBack service routine */
    if (CallBack != NULL_CALLBACK)
    {
        CallBack();
    }

    /* Check status and end operation
     ********************************/
    if (eraseStatus_Flag == ERASE_INPROGRESS)
    {
        blockConf = eepromConfig->flashBlocks[sourceBlockIndexInternal];

        /* Get the erase operation status */
        returnCode = FSL_GetEraseStatus(eepromConfig, CallBack);

        if (returnCode == EE_INFO_HVOP_INPROGRESS)
        {
            /* Update erase status */
            eraseStatus_Flag = ERASE_INPROGRESS;
        }
        else if ((returnCode == EE_ERROR_PE_OPT) || (returnCode == EE_ERROR_MISMATCH))   /* Failed to erase ********/
        {
            /* Update erase status */
            eraseStatus_Flag = ERASE_FAIL;

            /* Make the block to DEAD */
            returnCode = FSL_MakeBlock2Dead(eepromConfig, blockConf, CallBack);
            if (returnCode != EE_MAKE_DEAD_OK)
            {
                eraseStatus_Flag = ERASE_SWAPERROR;
            }
        }
        else    /* Erase successful ******************************************/
        {
            /* Update erase status */
            eraseStatus_Flag = ERASE_DONE;

            /* Write erase cycle to the erased block */
            returnCode = FSL_SyncProgram(blockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                         WORD_SIZE, (UINT32)(&erasingCycleInternal),  CallBack);
            if (returnCode != EE_OK)
            {
                eraseStatus_Flag = ERASE_SWAPERROR;
                returnCode = EE_ERROR_PROGRAM_ERASE_CYCLE;
            }
            else
            {
                /* Update blank space pointer */
                blockConf->blankSpace = blockConf->blockStartAddr + RECORD_OFFSET;
            }
        }


        /* Make the COPY_DONE block to ACTIVE
         ************************************/
        if ((returnCode == EE_OK) || (returnCode == EE_MAKE_DEAD_OK))
        {
            /* Make the COPY_DONE block to ACTIVE */
            blockStatus = ACT_INDICATOR_ACT;
            blockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];
            returnCode = FSL_ProgramBlockIndicator(blockConf->blockStartAddr, (UINT32)(&blockStatus), CallBack);
            if (returnCode == EE_OK)
            {
                eraseStatus_Flag = ERASE_NOTSTARTED;    /* Reset erase flag */
            }
            else
            {
                eraseStatus_Flag = ERASE_SWAPERROR;
                returnCode = EE_ERROR_PROGRAM_BLOCK_INDICATOR;
            }
        }
    }

    if (returnCode == EE_INFO_HVOP_INPROGRESS)
    {
        /* If there is a on-going HVOP, we continue waiting.
         * So reset the returnCode */
        returnCode = EE_OK;
    }
    return returnCode;
}


/**************************************************************************//*!
* @Function: FSL_RecoverEmulationSystem
*
* @Brief:
*    This function is used to recover the case that the FSL_InitEeprom returns
*    EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK. At this time the blocks' status are:
*    COPY_DONE - COPY_DONE or COPYDONE - ERASED or ACT - COPY_DONE.
*
*    Note: The COPY_DONE block is considered as valid block only if all data records
*    with COMPLETE status within that block are valid.
*
* @param: eepromConfig: pointer to eeprom configuration
* @param: invalidBlockIndex: index of the invalid block
* @param: Callback: call back function pointer
*
* @return: EE_ERROR_WRITE_IN_PROGRESS
*          EE_ERROR_BLOCK_CONFIG: block configuration is not correct
*          EE_INFO_HVOP_INPROGRESS
*          EE_ERROR_PE_OPT
*          EE_ERROR_MISMATCH
*          EE_OK
*
****************************************************************************/
UINT32 FSL_RecoverEmulationSystem(EEPROM_CONFIG* eepromConfig, UINT8 invalidBlockIndex, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;      /* Return code */
    UINT32 eraseCycle, i;

    /* Check the write lock of the FlashEE */
    if (eepromConfig->blockWriteFlag != 0U)
    {
        /* A write operation is already in progress, */
        /* no write operation is permitted */
        returnCode = EE_ERROR_WRITE_IN_PROGRESS;
    }
    else
    {
        /* Set the write lock flag */
        eepromConfig->blockWriteFlag = TRUE;

        /* Erase the invalid block */
        returnCode = FSL_EraseEEBlock(eepromConfig, invalidBlockIndex, TRUE, CallBack);

        if (returnCode == EE_OK)
        {
            /* Get index of the block right after the erased blocks */
            i = ((UINT32)invalidBlockIndex + 1U) % eepromConfig->numberOfBlock;

            /* Get erased cycle of that block */
            eraseCycle = READ32(eepromConfig->flashBlocks[i]->blockStartAddr + ERASE_CYCLE_OFFSET);

            /* Canculate erase cycle for the erased block base on the round-robin sequence */
            if (i != 0U)
            {
                eraseCycle++;
            }

            /* Write erase cycle to the erased block */
            returnCode = FSL_SyncProgram(eepromConfig->flashBlocks[invalidBlockIndex]->blockStartAddr + ERASE_CYCLE_OFFSET,
                                         WORD_SIZE, (UINT32)(&eraseCycle), CallBack);
        }

        /* Unlock the write lock */
        eepromConfig->blockWriteFlag = FALSE;

        /* Call FSL_InitEeprom to recover */
        if (returnCode == EE_OK)
        {
            returnCode = FSL_InitEeprom(eepromConfig, CallBack);
        }
    }

    return returnCode;
}
/* EOF */
