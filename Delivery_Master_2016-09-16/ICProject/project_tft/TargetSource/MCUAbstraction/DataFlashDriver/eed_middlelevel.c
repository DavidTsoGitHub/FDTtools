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
* $FileName: eed_middlelevel.c$
* $Version : 1.2.0$
* $Date    : Jun-10-2015$
*
* Comments:
*
*   This file consitutes of middle level API's.
*
*END************************************************************************/

#include "ee_internal.h"

#ifndef SWAP_CACHE_SIZE
#define SWAP_CACHE_SIZE 0x04
#endif

static UINT8 swapCache[SWAP_CACHE_SIZE];

/***************************************************************************!
 *
 * @name  FSL_ReadRecordHead
 *
 * @brief This function will read record head from specified address.
 *          It stores data into the data record structure, with the data size,
 *          data id and some small data.
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void FSL_ReadRecordHead(UINT32 addr, DATA_RECORD_HEAD * pRecHead, void (*CallBack)(void))
{
    UINT32 returnCode;
    /* Read data record header from flash */
    returnCode = FSL_FlashRead(READ, addr, ID_OFFSET + WORD_SIZE, (UINT32)pRecHead, CallBack);
#if (SIZE_FIELD == 0U)
    /* For fix length record scheme */
    pRecHead->dataSize = DATA_SIZE;
#endif
}


/***************************************************************************!
 *
 * @name: FSL_SearchRecordFromTop
 *
 * @brief:This API is to search the required data record IDs from the start of
 *        the specific block region. It will check NUMBER_OF_SEARCHING_RECORD_IN_BLOCK
 *        data records at most so that it can return within the time limit.
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void FSL_SearchRecordFromTop(BLOCK_CONFIG* blockConf, UINT32 bufferAddress, UINT32 bufferSize,     \
                             UINT16 startID, UINT32* startAddrInBlock, UINT16* nextStartID,        \
                             BOOL *deleted ,void (*CallBack) (void))
{
    DATA_RECORD_HEAD record;    /* Local data record head structure */
    UINT32 ret = EE_OK;         /* Return code */
    UINT32 index;               /* Loop index */
    UINT32 endAddrInBlock;      /* End address in block */
    UINT32 recordLength;        /* The length of the record */
    UINT32 status;              /* Record status */
    UINT16 endID;               /* Define the data ID range */
    UINT16 dataSize;            /* Size of record */
    BOOL isFinish = FALSE;      /* Finish searching flag */

    *deleted = FALSE;

    /* Calculate the boundary of the data ID range */
    endID = startID + (UINT16)(bufferSize / WORD_SIZE);

    /* Calculate the end address of the specific block */
    endAddrInBlock = blockConf->blockStartAddr + blockConf->blockSize;

    if (startID > 0x7FFFU)                                               /* Finish searching block */
    {
        /* Update the start address */
        *startAddrInBlock = endAddrInBlock;
    }
    else if (*startAddrInBlock > (endAddrInBlock - MIN_RECORD_SIZE))
    {
        /* Update the start address */
        *startAddrInBlock = endAddrInBlock;
    }
    else                                                                 /* Searching */
    {
        for (index = 1U; index <= NUMBER_OF_SEARCHING_RECORD_IN_BLOCK; index++)
        {
            /* Callback service */
            if ((index % CALLBACK_SEARCHING_BYTES_NUMBER) == 0U)
            {
                if (CallBack != NULL_CALLBACK)
                {
                    CallBack();
                }
            }

            /* Read the record */
            FSL_ReadRecordHead(*startAddrInBlock, &record, CallBack);
            status = record.dataStatus;
            dataSize = record.dataSize;

            /* If there is no ECC error, check the record status with various values */
            if ((status == R_PROGRAM_COMPLETE) && (eccErrorStatus_Flag == FALSE))
            {
                /* This is valid record and check the ID */
                if ((record.dataID >= startID) && (record.dataID < endID))
                {
                    WRITE32(bufferAddress + ((record.dataID - startID) * WORD_SIZE), *startAddrInBlock);
                }

                if ((record.dataID >= endID) && (record.dataID < *nextStartID))
                {
                    /* The start ID for next call */
                    *nextStartID = record.dataID;
                }

                if (record.dataID == startID)
                {
                    *deleted = FALSE;
                }
            }
            else if ((status == R_DELETE) && (eccErrorStatus_Flag == FALSE))
            {
                /* check the ID */
                if ((record.dataID >= startID) && (record.dataID < endID))
                {
                    /* delete found result */
                    WRITE32(bufferAddress + ((record.dataID - startID) * WORD_SIZE), ERASED_WORD);
                }

                if (record.dataID == startID)
                {
                    *deleted = TRUE;
                }
            }
            else if ((status == ERASED_WORD) && (eccErrorStatus_Flag == FALSE))
            {
                /* Do blank check for the record header */
                ret = FSL_FlashRead(BLANK_CHECK, *startAddrInBlock, MIN_RECORD_SIZE, 0U, CallBack);

                /* If record head is fully blanked, it means this this blank space, so stop searching */
                if (ret == EE_OK)
                {
                    /* Update blank space for the block */
                    blockConf->blankSpace = *startAddrInBlock;

                    /* End of searching operation */
                    *startAddrInBlock = endAddrInBlock;

                    /* Finish the searching */
                    isFinish = TRUE;
                }
                /* Record head is not blanked, skip address determined by dataSize to continue searching */
                else
                {
                    /* Determine to dataSize for calculate record length */
                    if (dataSize == 0xFFFFU)
                    {
                        dataSize = DATA_HEAD_SIZE;
                    }
                }
            }
            else        /* Have ECC error or random value in read record head */
            {
                /* Check if having ECC in ID/SIZE field */
#if (SIZE_FIELD != 0U)
                ret = FSL_FlashRead(READ, *startAddrInBlock + ID_OFFSET, WORD_SIZE, (UINT32)(&status), CallBack);
                if (eccErrorStatus_Flag == TRUE)
                {
                    dataSize = DATA_HEAD_SIZE;
                }
#endif
            }

            if (isFinish == FALSE)
            {
                /* Calculate record length */
                recordLength = FSL_GetRecordLength(dataSize);

                /* Update the start address */
                *startAddrInBlock += recordLength;

                /* Check for reaching end of the block */
                if (*startAddrInBlock > (endAddrInBlock - MIN_RECORD_SIZE))
                {
                    /* Update blank space for the block */
                    blockConf->blankSpace = endAddrInBlock;

                    /* Update the start address */
                    *startAddrInBlock = endAddrInBlock;

                    isFinish = TRUE;
                }
            }

            if (isFinish == TRUE)    /* Finish searching */
            {
                break;
            }
        }
    }

    return;
}


/***************************************************************************!
 *
 * @name: FSL_SearchRecordFromBottom
 *
 * @brief: API will search the latest record ID with status is completed or deleted in block.
 *
 * @param
 *
 * @return  EE_OK
 *          EE_ERROR_DATA_NOT_FOUND: can not found the data record
 *
 ****************************************************************************/
#if (SIZE_FIELD == 0U)
UINT32 FSL_SearchRecordFromBottom(BLOCK_CONFIG* blockConf, UINT16 recordID, UINT32* recordAddr, BOOL *deleted, void (*CallBack)(void))
{
    DATA_RECORD_HEAD record;                            /* Local data record head structure */
    UINT32 returnCode = EE_ERROR_DATA_NOT_FOUND;        /* Return code */
    UINT32 recordLength;                                /* Length of record */
    UINT32 count = 0U;

    /* Calculate the end address of the specific block */
    *recordAddr = blockConf->blankSpace;

    /* Store number of searched records for CallBack() service */
    recordLength = FSL_GetRecordLength(DATA_SIZE);

    /* Reset the deleted flag before searching */
    *deleted = FALSE;

    while (*recordAddr > (blockConf->blockStartAddr + RECORD_OFFSET))
    {
        /* Point to start address of previous record */
        *recordAddr -= recordLength;

        /* CallBack service routine */
        count++;
        if ((count % CALLBACK_SEARCHING_BYTES_NUMBER) == 0U)
        {
            if (CallBack != NULL_CALLBACK)
            {
               CallBack();
            }
        }

        /* Read record head */
        FSL_ReadRecordHead(*recordAddr, &record, CallBack);

        /* If have no ECC while reading record head, check dataID and record status for validation */
        if ((recordID == record.dataID) && (eccErrorStatus_Flag == FALSE))
        {
            if (record.dataStatus == R_PROGRAM_COMPLETE)
            {
                returnCode = EE_OK;             /* Update status */
            }
            if (record.dataStatus == R_DELETE)
            {
                *recordAddr = 0xFFFFFFFFU;      /* Reset record address */
                *deleted = TRUE;
            }
            break;  /* Stop searching */
        }
    }

    return returnCode;
}
#endif


/***************************************************************************!
 *
 * @name: FSL_GetRecordLength
 *
 * @brief: API will calculate the record length based on data size input.
 *
 * @param
 *
 * @return  length of the data record
 *
 ****************************************************************************/
UINT32 FSL_GetRecordLength(UINT16 dataSize)
{
    UINT16 recordLength;

    if (dataSize <= DATA_HEAD_SIZE)
    {
        recordLength = MIN_RECORD_SIZE;
    }
    else
    {
        /* Increment by ECC_SIZE */
        if (((dataSize - DATA_HEAD_SIZE) % ECC_SIZE) != 0U)
        {
            recordLength = (UINT16)MIN_RECORD_SIZE + (ECC_SIZE * (((dataSize - DATA_HEAD_SIZE) / (UINT16)ECC_SIZE) + 1U));
        }
        else
        {
            recordLength = (UINT16)MIN_RECORD_SIZE - (UINT16)DATA_HEAD_SIZE + dataSize;
        }
    }

    return ((UINT32)recordLength & 0x0000FFFFU) ;
}


/***************************************************************************!
 *
 * @name: FSL_ReadBlockStatus
 *
 * @brief: API will read status of EEE block.
 *
 * @param
 *
 * @return  B_INVALID
 *          B_DEAD
 *          B_ACT
 *          B_COPY_DONE
 *          B_ERASED
 *          B_ALT
 *
 ****************************************************************************/
UINT32 FSL_ReadBlockStatus(BLOCK_CONFIG* blockConf, void (*CallBack)(void))
{
    UINT32 returnCode;
    UINT32 ret;
    UINT32 temp;
    UINT8  bakActEccFlag;
    UINT8  bakCopEccFlag;

    /* Read DEAD indicator */
    ret = FSL_FlashRead(READ, blockConf->blockStartAddr + DEAD_IND_OFFSET, WORD_SIZE, (UINT32)(&temp), CallBack);

    /* != 0xFFFFFFFF or have ECC */
    if (((temp == ERASED_WORD) && (FALSE == eccErrorStatus_Flag)) == 0U)
    {
        returnCode = B_DEAD;
    }
    else
    {
        /* Read erase cycle, the FSL_FlashRead function will reset 'eccErrorStatus_Flag' */
        ret = FSL_FlashRead(READ, blockConf->blockStartAddr + ERASE_CYCLE_OFFSET, WORD_SIZE, (UINT32)(&temp), CallBack);
        if (eccErrorStatus_Flag == TRUE)
        {
            returnCode = B_INVALID;
        }
        else if (temp == ERASED_WORD)
        {
            /* Check blank for the block */
            ret = FSL_FlashRead(BLANK_CHECK, blockConf->blockStartAddr, blockConf->blockSize, 0U, CallBack);
            if (ret == EE_OK)
            {
                returnCode = B_ERASED;
            }
            else
            {
                returnCode = B_INVALID;
            }
        }
        else
        {
            /* Read ACTIVE indicator, use the returnCode as buffer temporary */
            ret = FSL_FlashRead(READ, blockConf->blockStartAddr, WORD_SIZE, (UINT32)(&returnCode), CallBack);

            /* Backup ECC flag */
            bakActEccFlag = eccErrorStatus_Flag;

            /* Read COPY_DONE indicator */
            ret = FSL_FlashRead(READ, blockConf->blockStartAddr + COPY_DONE_OFFSET, WORD_SIZE, (UINT32)(&temp), CallBack);

            /* Backup ECC flag */
            bakCopEccFlag = eccErrorStatus_Flag;

            /* Blank check the record region */
            ret = FSL_FlashRead(BLANK_CHECK, blockConf->blockStartAddr + RECORD_OFFSET,
                                blockConf->blockSize -RECORD_OFFSET, 0U, CallBack);


            /* Determine the block status
             ****************************/
            if ((bakActEccFlag == TRUE) || (returnCode != 0xFFFFFFFFU))
            {
                returnCode = B_ACT;
            }
            else if ((ret != EE_OK) &&
                     ((bakActEccFlag == FALSE) && (returnCode == 0xFFFFFFFFU)) &&
                     ((bakCopEccFlag == TRUE) || (temp != 0xFFFFFFFFU))
                    )
            {
                returnCode = B_COPY_DONE;
            }
            else if ((ret != EE_OK) &&
                     ((bakActEccFlag == FALSE) && (returnCode == 0xFFFFFFFFU)) &&
                     ((bakCopEccFlag == FALSE) && (temp == 0xFFFFFFFFU))
                    )
            {
                returnCode = B_UPDATE;
            }
            else if ((ret == EE_OK) &&
                     ((bakActEccFlag == FALSE) && (returnCode == 0xFFFFFFFFU)) &&
                     ((bakCopEccFlag == FALSE) && (temp == 0xFFFFFFFFU))
                    )
            {
                returnCode = B_ALT;
            }
            else
            {
                returnCode = B_INVALID;
            }
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_BlockSwapping
 *
 * @brief: This API is to copy the latest data records from the active block,
 *          which is nearly full, to the alternative block.
 *
 * @param
 *
 * @return EE_OK
 *         EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_ERROR_NO_ENOUGH_SPACE
 *         EE_ERROR_BLOCK_CONFIG: block configuration is not correct
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR
 *
 ****************************************************************************/
UINT32 FSL_BlockSwapping(EEPROM_CONFIG* eepromConfig,  BOOL syncErase, void (*CallBack)(void))
{
    BLOCK_CONFIG *sourceBlock;  /* The source (oldest ACTIVE) block configuration pointer */
    BLOCK_CONFIG *destBlock;    /* The destination (UPDATE) block configuration pointer */
    BLOCK_CONFIG *tempActiveBlock[NUMBER_OF_ACTIVE_BLOCKS]; /* Array of ACTIVE and UPDATE blocks */
    DATA_RECORD_HEAD recordHead;
    UINT32 returnCode = EE_OK;
    UINT32 blockIndex;
    UINT32 temp;
    UINT32 i, j;
    UINT32 recordAddr;
    UINT16 dataID;
    UINT16 bak_dataID;
    BOOL recordDeleted;


    /* Determine the dest block (the block right after the newest ACTIVE block) */
    blockIndex = (eepromConfig->activeBlockIndex + 1U) % eepromConfig->numberOfBlock;
    destBlock = eepromConfig->flashBlocks[blockIndex];

    /* Determine source block (the oldest ACTIVE) */
    if (blockIndex >= NUMBER_OF_ACTIVE_BLOCKS)
    {
        blockIndex = blockIndex - NUMBER_OF_ACTIVE_BLOCKS;
    }
    else
    {
        blockIndex = blockIndex + eepromConfig->numberOfBlock - NUMBER_OF_ACTIVE_BLOCKS;
    }
    sourceBlock = eepromConfig->flashBlocks[blockIndex];

    /* Use temp to store index of the oldest ACTIVE block */
    temp = blockIndex;

    /* Fill the temp ACTIVE and UPDATE block array */
    for (i = 0U; i < NUMBER_OF_ACTIVE_BLOCKS; i++)
    {
        /* The temp currently store index of the oldest ACTIVE block */
        temp++;     /* Point to the next ACTIVE block */
        if (temp == eepromConfig->numberOfBlock)
        {
            temp = 0U;
        }
        tempActiveBlock[i] = eepromConfig->flashBlocks[temp];
    }


    /* Seach for item in the cache table for existing in the source block
     ********************************************************************/
    dataID = 0U; /* Set the start ID for searching */

    /* We only search in the cache table if it's enabled and the function is called at runtime,
     * because the FSL_InitEeprom call the function when the cache table hasn't fetched */
    if ((eepromConfig->cacheEnable == TRUE) && (syncErase == FALSE))
    {
        /* Get number of items in the cache table */
        dataID = (UINT16)(eepromConfig->cTable->size / CTABLE_ITEM_SIZE);

        /* Check for each item in the cache table */
        for (i = 0U; i < dataID; i++)
        {
            /* Get record address */
            recordAddr = READ32(eepromConfig->cTable->startAddress + (i * CTABLE_ITEM_SIZE));

            /* If the record is in the source block */
            if ((recordAddr > sourceBlock->blockStartAddr) &&
                (recordAddr <= (sourceBlock->blockStartAddr + sourceBlock->blockSize)))
            {
                /* Read the record */
                FSL_ReadRecordHead(recordAddr, &recordHead, CallBack);

                returnCode = FSL_CopyDataRecord(eepromConfig, destBlock, TRUE, recordHead.dataID,
                                                recordHead.dataSize, recordAddr, CallBack);
                if (returnCode != EE_OK)
                {
                    break;  /* Exit if failsed to copy data */
                }
            }
        }
    }


    /* Continue copy records aren't in the cache table
     *************************************************/
    while ((dataID < 0x8000U) && (returnCode == EE_OK))   /* dataID is containing the next start ID */
    {
        /* Now, use swap cache to buffering all valid data in the oldest active block */
        /* Clear the swap cache */
        for (i = 0U; i < (SWAP_CACHE_SIZE / CTABLE_ITEM_SIZE); i++)
        {
            WRITE32(swapCache + (i * CTABLE_ITEM_SIZE), ERASED_WORD);
        }

        /* Backup dataID because it will be changed after calling FSL_RecordBuffering */
        bak_dataID = dataID;

        /* Search in the source block: all id which were not in normal cache table will be stored in swap cache */
        FSL_RecordBuffering(sourceBlock, (UINT32)swapCache, SWAP_CACHE_SIZE, &dataID, CallBack);


        /* Check all IDs in Swap Cache
         *****************************/
        for (i = 0U; i < (SWAP_CACHE_SIZE/CTABLE_ITEM_SIZE); i++)
        {
            /* Get record address */
            recordAddr = READ32(swapCache + (i * CTABLE_ITEM_SIZE));

            /* If record ID is in the oldest ACTIVE block, check if it is in tempActiveBlock array */
            if (recordAddr != ERASED_WORD)
            {
                /* Search the ID in the tempActiveBlock array */
                for (j = 0U; j < NUMBER_OF_ACTIVE_BLOCKS; j++)
                {
                    returnCode = FSL_SearchEntireBlockFromTop(tempActiveBlock[j],  bak_dataID, (UINT32*)(&temp), &recordDeleted,CallBack);
                    if (returnCode == EE_OK)
                    {
                        break;   /* If found the dataID in the block, switch to next ID */
                    }
                }

                /* If the dataID isn't in the block array, copy it */
                if (returnCode == EE_ERROR_DATA_NOT_FOUND)
                {
                    /* Read the record */
                    FSL_ReadRecordHead(recordAddr, &recordHead, CallBack);

                    /* Copy the record */
                    returnCode = FSL_CopyDataRecord(eepromConfig, destBlock, TRUE, recordHead.dataID,
                                                    recordHead.dataSize, recordAddr, CallBack);
                    if (returnCode != EE_OK)
                    {
                        /* Exit the swapping progress if data copying failed */
                        break;
                    }
                }
            }

            /* Update ID for the next searching round */
            bak_dataID += 1U;
        }
    }


    if (returnCode == EE_OK)
    {
        /* Make the UPDATE block to COPY DONE */
        temp = COPY_DONE;
        returnCode = FSL_ProgramBlockIndicator(destBlock->blockStartAddr + COPY_DONE_OFFSET, (UINT32)(&temp), CallBack);

        if (returnCode == EE_OK)
        {
            /* Set activeBlockIndex to the COPY_DONE block */
            eepromConfig->activeBlockIndex = (eepromConfig->activeBlockIndex + 1U) % eepromConfig->numberOfBlock;

            if (syncErase == FALSE)     /* Only erase if at runtime, not at initialization time */
            {
                /* Backup erase cycle and the block is erased to used in the main function */
                erasingCycleInternal = READ32(sourceBlock->blockStartAddr + ERASE_CYCLE_OFFSET) + 1U;
                sourceBlockIndexInternal = blockIndex;

                /* Erase the source block, blockIndex is containing index of the oldest ACTIVE block */
                returnCode = FSL_EraseEEBlock(eepromConfig, blockIndex, syncErase, CallBack);
            }
        }
    }

    return returnCode;
}


/***************************************************************************! *
 * @name: FSL_SearchEntireBlockFromTop
 *
 * @brief: This API is to search the required data ID in the specific block.
 *          It will go through the entire block for the latest data record.
 *
 * @param
 *
 * @return  EE_OK
 *          EE_ERROR_DATA_NOT_FOUND
 *
 ****************************************************************************/
UINT32 FSL_SearchEntireBlockFromTop(BLOCK_CONFIG* blockConf, UINT16 dataID, UINT32* expDataInBlock, BOOL *deleted, void (*CallBack)(void))
{
    UINT32 returnCode = EE_OK;
    UINT32 temp;                /* Temporary variable */
    UINT32 startAddrInBlock;    /* The data record address */
    UINT16 nextStartID;         /* Dummy variable */

    /* Initialize the expected data address */
    *expDataInBlock = 0xFFFFFFFFU;

    /* Search the data from the block start to end */
    /* address will point to the record status field */
    startAddrInBlock = blockConf->blockStartAddr + RECORD_OFFSET;

    /* Calculate the end address of the specific block */
    temp = blockConf->blockStartAddr + blockConf->blockSize;

    while (startAddrInBlock < temp)
    {
        /* Need go through entire block */
        FSL_SearchRecordFromTop(blockConf, (UINT32)expDataInBlock, WORD_SIZE, dataID, \
                                &startAddrInBlock, &nextStartID, deleted, CallBack);
    }

    /* Check if the expected data is ever found */
    if (*expDataInBlock == 0xFFFFFFFFU)
    {
        returnCode = EE_ERROR_DATA_NOT_FOUND;
    }

    return returnCode;
}


/***************************************************************************! *
 * @name: FSL_SearchEntireBlock
 *
 * @brief: This API is to search the required data ID in the specific block.
 *         It will go through the entire block for the latest data record.
 *
 * @param
 *
 * @return  EE_OK
 *          EE_ERROR_DATA_NOT_FOUND
 *
 ****************************************************************************/
UINT32 FSL_SearchEntireBlock(BLOCK_CONFIG* blockConf, UINT16 dataID, UINT32* recordAddr, BOOL *deleted, void (*CallBack)(void))
{
    UINT32 ret;
#if (SIZE_FIELD == 0U)
    ret = FSL_SearchRecordFromBottom(blockConf, dataID, recordAddr, deleted, CallBack);
#else
    ret = FSL_SearchEntireBlockFromTop(blockConf, dataID, recordAddr, deleted, CallBack);
#endif
    return ret;
}


/***************************************************************************! *
 * @name: FSL_SearchInAllBlocks
 *
 * @brief: This API is to search the required data ID in the specific block.
 *         It will go through the entire block for the latest data record.
 *
 * @param
 *
 * @return  EE_OK
 *          EE_ERROR_DATA_NOT_FOUND
 *          EE_INFO_HVOP_INPROGRESS
 *
 ****************************************************************************/
UINT32 FSL_SearchInAllBlocks(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT8 iReq, \
                             UINT32* recordAddr, UINT32 *suspendState,  \
                             void (*CallBack) (void))
{
    BLOCK_CONFIG* pBlockConf;
    UINT32 returnCode = EE_OK;
    UINT32 startIndex, endIndex;
    UINT32 searchResult = EE_ERROR_DATA_NOT_FOUND;
    BOOL deleted = FALSE;

    /* Get index of the start block for searching */
    startIndex = eepromConfig->activeBlockIndex;

    /* Calculate index of the end block */
    endIndex = (eepromConfig->activeBlockIndex + eepromConfig->numberOfBlock - NUMBER_OF_ACTIVE_BLOCKS) %
                eepromConfig->numberOfBlock;


    /* Search from the newest to oldest ACTIVE block
     ***********************************************/
    while ((searchResult == EE_ERROR_DATA_NOT_FOUND) && (startIndex != endIndex))
    {
        /* Suspend the high voltage operation if the request is immediate,
         * but doesn't call the function if there is already a suspended operation */
        pBlockConf = eepromConfig->flashBlocks[startIndex];
        if (*suspendState == EE_OK)
        {
            *suspendState = FSL_ProcessImmediateRequest(eepromConfig, pBlockConf, iReq);
        }

        if (*suspendState != EE_INFO_HVOP_INPROGRESS)
        {
            /* Search the ID in the block */
            returnCode = FSL_SearchEntireBlock(pBlockConf, dataID, recordAddr, &deleted, CallBack);
            if (returnCode == EE_OK)
            {
                searchResult = EE_OK;  /* Stop searching if the ID found */
            }
            else
            {
                if (deleted == TRUE)
                {
                    break;  /* Stop searching because the record deleted */
                }

                /* Check for the next block */
                startIndex = (startIndex + eepromConfig->numberOfBlock - 1U) % eepromConfig->numberOfBlock;
            }
        }
        else
        {
            searchResult = EE_INFO_HVOP_INPROGRESS;     /* Stop searching */
        }
    }

    return searchResult;
}


/***************************************************************************!
 *
 * @name: FSL_RecordBuffering
 *
 * @brief: This function is to search the required data ID in the specific
 *          block with given start data ID. It will go through the entire block
 *          for the eligible data IDs
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
void FSL_RecordBuffering(BLOCK_CONFIG* blockConf, UINT32 bufferAddress, UINT32 bufferSize, \
                         UINT16* startID, void (*CallBack) (void))
{
    UINT32 temp;                /* Temporary variable */
    UINT32 startAddrInBlock;    /* The local data record pointer */
    UINT16 nextStartID;         /* The start ID for next time */
    BOOL deleted;

    /* Address will point to the record status field */
    startAddrInBlock = blockConf->blockStartAddr + RECORD_OFFSET;

    /* Calculate the end address of the specific block */
    temp = blockConf->blockStartAddr + blockConf->blockSize;

    /* Initialize the next ID */
    nextStartID = 0xFFFFU;

    /* Scan from start address to end address of the block for bufferring */
    while (startAddrInBlock < temp)
    {
        /* Need go through entire block */
        FSL_SearchRecordFromTop(blockConf,
                                bufferAddress,
                                bufferSize,
                                *startID,
                                &startAddrInBlock,
                                &nextStartID,
                                &deleted,
                                CallBack);
    }

    /* Assign start ID for next time */
    *startID = nextStartID;
}


/***************************************************************************! *
 * @name: FSL_SyncProgram
 *
 * @brief: This API is used to program size bytes data into flash memory
 *          For C55 flash family, program until the destination is aligned to the programmable size.
 *          eg. For size and dest to be programmed as below:
 *         |-------------------|-> Page n addr (programable size = page size)
 *         |(PGM_SIZE - x)bytes|
 *    ---->|-------------------|-> destination to be programmed
 *      |  |                   |
 *    size |      x bytes      |
 *         |-------------------|-> Page (n+1) addr
 *      |  |      y bytes      |
 *    ---->|-------------------|
 *         |(PGM_SIZE - y)bytes|
 *         |-------------------|-> Page (n+2) addr
 *         This function must program x bytes, then program y bytes later.
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *         EE_ERROR_PE_OPT: Program is failed
 *         EE_ERROR_MISMATCH
 *
 ****************************************************************************/
UINT32 FSL_SyncProgram(UINT32 dest, UINT16 size, UINT32 source, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;
    UINT32 destBk, sourceBk;
    UINT16 sizeBk, sizePGM, temp = 0U;

    /* Set the programmed data size */
    destBk = dest;
    sizeBk = size;
    sourceBk = source;

    while (size > 0U)
    {
        /* Check size for program */
        if (size > PGM_SIZE)
        {
            sizePGM = PGM_SIZE;
        }
        else
        {
            sizePGM = size;
        }

        /* Program until the destination is aligned to the programmable size. */
        if ((dest % PGM_SIZE) != 0U)
        {
            temp = (UINT16)PGM_SIZE - (UINT16)(dest % PGM_SIZE);
            if (sizePGM > temp)
            {
                sizePGM = temp;
            }
        }

        size -= sizePGM;
        returnCode = FSL_FlashProgramStart(dest, sizePGM, source);
        if (returnCode == EE_OK)
        {
            returnCode = EE_INFO_HVOP_INPROGRESS;

            /* Check high voltage status*/
            while (returnCode == EE_INFO_HVOP_INPROGRESS)
            {
                /* CallBack service */
                if (CallBack != NULL_CALLBACK)
                {
                    CallBack();
                }

                /* Check status */
                returnCode = FSL_FlashCheckStatus();
            }
        }

        if (returnCode == EE_OK)
        {
            /* Update dest and source */
            source += sizePGM;
            dest += sizePGM;
        }
        else
        {
            break;  /* Stop programming */
        }
    }

    if (returnCode == EE_OK)
    {
        returnCode = FSL_FlashRead(VERIFY, destBk, sizeBk, sourceBk, CallBack);
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_CopyDataRecord
 *
 * @brief: This API is used to program and verify one data record into the Flash memory
 *
 * @param dataID = 0xFFFF -> backing up data from block swapping
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *         EE_ERROR_PE_OPT: high voltage operation failed
 *
 ****************************************************************************/
UINT32 FSL_WriteDataRecord(BLOCK_CONFIG *blockConf, BOOL backup_flag, UINT16 dataID,
                           UINT16 dataSize, UINT32 source, void (*CallBack) (void))
{
    UINT32 ret = EE_OK;                         /* Return code */
    UINT32 dest;                                /* Destination of writing operation */
    UINT32 temp;                                /* Temporary variable */
    UINT32 i;                                   /* Loop variable */
    UINT16 size;
    UINT8 pgmBuff[ECC_SIZE];                    /* Data buffer */

    /* Get destination */
    dest = blockConf->blankSpace;

    /* Initialize pgmBuff */
    for(i = 0U; i < ECC_SIZE; i++)
    {
        pgmBuff[i] = 0xFFU;
    }


    /* 1. Program ID_SIZE field
     **************************/
    if (backup_flag == FALSE)   /* Prepare source for normal record writing */
    {
        WRITE16(pgmBuff, dataID);                   /* Prepare Record ID field */
#if (SIZE_FIELD != 0U)
        WRITE16(pgmBuff + SIZE_FIELD, dataSize);    /* Prepare Record Size field for var length config */
#else
        dataSize = DATA_SIZE;                       /* Prepare Record Size field for fixed length config */
#endif
        if (dataSize > SMALL_DATA_AFTER_STATUS_SIZE)
        {
            if (dataSize >= DATA_HEAD_SIZE)
            {
                temp = (UINT32)SMALL_DATA_AFTER_ID_SIZE;
            }
            else
            {
                temp = (UINT32)SMALL_DATA_AFTER_STATUS_SIZE;
                temp = (UINT32)dataSize - temp;
            }
            for (i = 0U; i < temp; i++)
            {
                WRITE8(pgmBuff + i + SIZE_FIELD + ID_FIELD_SIZE, READ8(source + i + SMALL_DATA_AFTER_STATUS_SIZE));
            }
        }
        temp = (UINT32)pgmBuff;
    }
    else                        /* Prepare source for copying record */
    {
        temp = (source + ID_OFFSET);
    }

    /* Determine size for writing */
    size = ECC_SIZE;

#if ((SIZE_FIELD == 0U) && ((ECC_SIZE - MIN_PGM_SIZE) > 0U))
    size = (UINT16)ECC_SIZE - (UINT16)MIN_PGM_SIZE;
#endif
    /* Write data to the EEC_SIZE region that contain ID and SIZE fields */
    ret = FSL_SyncProgram((dest + ID_OFFSET), size, temp, CallBack);

    if (ret == EE_ERROR_PE_OPT)
    {
        /* if this program operation fails, need to do blank check again. If this record head
           is still fully blanked, do not update blank space. Otherwise, need to update blank space.
           Use temporary dest, size, source to avoid impacting to the current dest, size, source values */

        temp = FSL_FlashRead(BLANK_CHECK, dest + ID_OFFSET, (UINT32)size, 0U, CallBack);

        if ((temp == EE_OK) && (eccErrorStatus_Flag == FALSE))
        {
            /* Do not update blank space because this location is still fully blanked */
        }
#if (SIZE_FIELD != 0U)
        else if (eccErrorStatus_Flag == TRUE)
        {
            blockConf->blankSpace += (ECC_SIZE * 2U);       /* Skip ID/Size and status pages */
        }
        else
        {
            /* Read the size value */
            size = READ16(dest + ECC_SIZE + 2U);

            if (size == 0xFFFFU)
            {
                 blockConf->blankSpace += (ECC_SIZE * 2U);  /* Skip ID/Size and status pages */
            }
            else
            {
                /* Skip actual programmed size */
                blockConf->blankSpace += FSL_GetRecordLength((UINT16)size);

                /* set blank space to end of block if it is larger */
                temp = blockConf->blockStartAddr + blockConf->blockSize;
                if (blockConf->blankSpace > temp)
                {
                    blockConf->blankSpace = temp;
                }
            }
        }
#else
        else    /* Fix length */
        {
            blockConf->blankSpace += FSL_GetRecordLength(dataSize);
        }
#endif
    }


    /* 2. Program the rest of data
     *****************************/
    if (ret == EE_OK)
    {
        if (dataSize > DATA_HEAD_SIZE)
        {
            /* Prepare source */
            if (backup_flag == FALSE)
            {
                temp = source + DATA_HEAD_SIZE;
            }
            else
            {
                temp = source + MIN_RECORD_SIZE;
            }

            /* Write data */
            ret = FSL_SyncProgram(dest + MIN_RECORD_SIZE, dataSize - DATA_HEAD_SIZE, temp, CallBack);

            if (ret == EE_ERROR_PE_OPT)
            {
#if (SIZE_FIELD == 0U)
                temp = FSL_FlashRead(BLANK_CHECK, dest + MIN_RECORD_SIZE, dataSize - DATA_HEAD_SIZE, 0U, CallBack);
                if (temp != EE_OK)
#endif
                {
                    /* Update blank space */
                    blockConf->blankSpace += FSL_GetRecordLength(dataSize);
                }
            }
        }
    }


    /* 3. Program small data after record status
     *******************************************/
#if (SIZE_FIELD != 0U)
    if (ret == EE_OK)
    {
        /* Prepare source */
        if (backup_flag == FALSE)
        {
            for (i = 0U; i < ECC_SIZE; i++)
            {
                pgmBuff[i] = 0xFFU;
            }

            for (i = 0U; ((i < SMALL_DATA_AFTER_STATUS_SIZE) && (i <dataSize)); i++)
            {
                WRITE8(pgmBuff + i, READ8(source + i));
            }
            temp = (UINT32)pgmBuff;
        }
        else
        {
            temp = source + MIN_PGM_SIZE;
        }

        /* Write data */
        ret = FSL_SyncProgram(dest + MIN_PGM_SIZE ,ECC_SIZE - MIN_PGM_SIZE, temp, CallBack);

        if (ret != EE_OK)
        {
            blockConf->blankSpace += FSL_GetRecordLength(dataSize);
        }
    }
#endif


    /* 4. Update record status to COMPLETE
     *************************************/
    if (ret == EE_OK)
    {
        /* Prepare source */
        WRITE32(pgmBuff,R_PROGRAM_COMPLETE);
        WRITE32(((UINT32)pgmBuff + WORD_SIZE),ERASED_WORD);

        /* Write data */
        ret = FSL_SyncProgram(dest , MIN_PGM_SIZE, (UINT32)pgmBuff, CallBack);

        /* Update blank space */
        blockConf->blankSpace += FSL_GetRecordLength(dataSize);
    }

    return ret;
}


/***************************************************************************!
 *
 * @name: FSL_UpdateCacheTable
 *
 * @brief: This API is to update the contents of the cache table while the
 *          data address is changed by writing a new value or block swapping.
 *
 * @param
 *
 * @return FALSE: data record is out of range of the cache table
 *         TRUE: cache is updated
 *
 ****************************************************************************/
BOOL FSL_UpdateCacheTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32 newValue)
{
    UINT32 temp;
    BOOL returnCode = TRUE;

    /* Get the maximum data ID number in cache table */
    temp = (cTable->size) / CTABLE_ITEM_SIZE;

    /* Check if it is in the cache table */
    if (dataID < (UINT16)temp)
    {
        /* Insert the data ID into the cache table */
        temp = cTable->startAddress;

        /* Update the cache table item with new value */
        WRITE32(temp + (CTABLE_ITEM_SIZE * dataID), newValue);
    }
    else
    {
        /* Data record is out of range of the cache table */
        returnCode = FALSE;
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_SearchInTable
 *
 * @brief: This API is to search the required data record ID in the cache table.
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_NOT_IN_CACHE
 *         EE_ERROR_DATA_NOT_FOUND
 *
 ****************************************************************************/
UINT32 FSL_SearchInTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32* expDataAddress)
{
    UINT32 returnCode = EE_OK;
    UINT32 counter;         /* Loop counter */

    /* check if the data ID is in the cache table ID range */
    counter = cTable->size / CTABLE_ITEM_SIZE;
    if (dataID >= counter)
    {
        /* Data ID is out of range */
        returnCode = EE_ERROR_NOT_IN_CACHE;
    }
    else
    {
        /* Fetch the required data record address */
        *expDataAddress = READ32(cTable->startAddress + (dataID * CTABLE_ITEM_SIZE));
        if (*expDataAddress == 0xFFFFFFFFU)
        {
            /* No data found */
            returnCode = EE_ERROR_DATA_NOT_FOUND;
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_EraseEEBlock
 *
 * @brief: This API is used to launch command to erase the specific blocks and
 *          it will re-erase for the number of max erase if failed in case of
 *          not invoking on swapping.
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_BLOCK_CONFIG: block configuration is not correct
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_PE_OPT
 *         EE_ERROR_MISMATCH
 *
 ****************************************************************************/
UINT32 FSL_EraseEEBlock(EEPROM_CONFIG* eepromConfig,
                        UINT32 blockIndex,
                        BOOL syncErase,
                        void (*CallBack) (void))
{
    BLOCK_CONFIG* blockConf;            /* Block configuration */
    UINT32 returnCode = EE_OK;          /* The return code */
    UINT32 ebHigh = 0U;                 /* Enabled blocks for high space Flash block */
    UINT32 ebMiddle = 0U;               /* Enabled blocks for middle space Flash block */
    UINT32 ebLow = 0U;                  /* Enabled blocks for low space Flash block */
    UINT32 dest;                        /* Elank checking destination address */
    UINT32 size;                        /* Elank checking size */
    UINT32 ersNum;                      /* Number of re-erase */

    /* Get block configuration */
    blockConf = eepromConfig->flashBlocks[blockIndex];

    /* Check the address space */
    switch (blockConf->blockSpace)
    {
        case LOW_ADDRESS_SPACE:
            ebLow |= blockConf->enabledBlock;
            break;
        case MID_ADDRESS_SPACE:
            ebMiddle |= blockConf->enabledBlock;
            break;
        case HIGH_ADDRESS_SPACE:
            ebHigh |= blockConf->enabledBlock;
            break;
        default:
            returnCode = EE_ERROR_BLOCK_CONFIG;
            break;
    }

    if (returnCode == EE_OK)    /* If block config is correct, we erase the block */
    {
        /* Number of erasing */
        ersNum = (UINT32)(MAX_REERASE + 1U);

        do
        {
#if (FLASH_MODULE == C90FL)
            /* Invoke FlashDepletionRecover() after brownout only */
            if ((ersNum < (MAX_REERASE + 1U)) || (depletion_Flag == TRUE))
            {
                returnCode = pFlashDepletionRecover(FLASH_REG_BASE, blockConf->blockStartAddr, ebLow, ebMiddle, ebHigh, CallBack);
                depletion_Flag = FALSE;     /* Reset depletion flag */
                if (returnCode != EE_OK)
                {
                    break;
                }
            }
#endif
            returnCode = FSL_FlashEraseStart(blockConf->blockStartAddr, ebLow, ebMiddle, ebHigh);
            if (returnCode == EE_OK)
            {
                if (syncErase == TRUE)
                {
                    returnCode = EE_INFO_HVOP_INPROGRESS;

                    /* Check status and end operation */
                    while (returnCode == EE_INFO_HVOP_INPROGRESS)
                    {
                        if (CallBack != NULL_CALLBACK)
                        {
                            CallBack();
                        }

                        returnCode = FSL_FlashCheckStatus();
                    }

                    /* Do blank check for erased block */
                    dest = blockConf->blockStartAddr;
                    size = blockConf->blockSize;
                    returnCode = FSL_FlashRead(BLANK_CHECK, dest, size, 0U, CallBack);
                    if (returnCode != EE_OK)
                    {
                        ersNum--;
                    }
                }
                else
                {
                    /* Update erase status */
                    eraseStatus_Flag = ERASE_INPROGRESS;
                }
            }
        }
        while ((returnCode == EE_ERROR_PE_OPT) && (ersNum > 0U));    /* Re-erase if it failed and re-erase number still > 0 */
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_CopyDataRecord
 *
 * @brief:
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: high voltage operation is in progress
 *         EE_OK
 *         EE_ERROR_NO_ENOUGH_SPACE: no space in the block for this data record
 *
 ****************************************************************************/
UINT32 FSL_CopyDataRecord(EEPROM_CONFIG* eepromConfig, BLOCK_CONFIG* blockConf, BOOL backup_flag, \
                           UINT16 dataID, UINT16 dataSize, UINT32 source, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;
    UINT32 recordLength, blankSpaceBk;
    BOOL temp;

#if (SIZE_FIELD == 0U)
    dataSize = DATA_SIZE;
#endif

    /* Calculate record length */
    recordLength = FSL_GetRecordLength(dataSize);

    do
    {
        /* Check the free space */
        if (recordLength > (blockConf->blockStartAddr + blockConf->blockSize - blockConf->blankSpace))
        {
            returnCode = EE_ERROR_NO_ENOUGH_SPACE;
        }
        else
        {
            /* Backup record address */
            blankSpaceBk = blockConf->blankSpace;

            /* Write internal data to dest block */
            returnCode = FSL_WriteDataRecord(blockConf, backup_flag, dataID, dataSize, source, CallBack);

            if (returnCode == EE_OK)
            {
                /* Check if the cache table is enabled */
                if (eepromConfig->cacheEnable == TRUE)
                {
                    temp = FSL_UpdateCacheTable(eepromConfig->cTable, dataID, blankSpaceBk);
                }
            }
        }
    }
    /* if ret = EE_ERROR_PE_OPT, re-write the record into a different position,
     * is calculated by the FSL_WriteDataRecord function */
    while (returnCode == EE_ERROR_PE_OPT);

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_MakeBlock2Dead
 *
 * @brief:
 *
 * @param
 *
 * @return EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD: failed to make the block to DEAD
 *         EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN: not enough block for round robin
 *         EE_MAKE_DEAD_OK: make to DEAD successfully
 *
 ****************************************************************************/
UINT32 FSL_MakeBlock2Dead(EEPROM_CONFIG* pEepromConfig, BLOCK_CONFIG * pBlockConf, void (*CallBack)(void))
{
    BLOCK_CONFIG* pBlock;
    UINT32 returnCode = EE_OK;
    UINT32 value = DEAD_INDICATOR_DEAD;
    UINT32 dest = pBlockConf->blockStartAddr + DEAD_IND_OFFSET;
    UINT32 addr[NUMBER_OF_BLOCK];
    UINT32 i;

    /* Program dead block indicator until it is not 0xFF */
    returnCode = FSL_ProgramBlockIndicator(dest, (UINT32)(&value), CallBack);

    if (returnCode == EE_ERROR_PROGRAM_BLOCK_INDICATOR)
    {
        returnCode = EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD;
    }
    else    /* Re-arrange the block array */
    {
        /* Update DEAD block number */
        pEepromConfig->numberOfDeadBlock++;

        /* Get index of dead block */
        for (i = 0U; i < pEepromConfig->numberOfBlock; i++)
        {
            pBlock = pEepromConfig->flashBlocks[i];
            if (pBlock == pBlockConf)
            {
                value = i;
                break;
            }
        }

        /* Backup address of eeprom blocks */
        for (i = 0U; i < pEepromConfig->numberOfBlock; i++)
        {
            addr[i] = (UINT32)pEepromConfig->flashBlocks[i];
        }

        /* Shift blocks that at higher index that the DEAD block */
        for (i = value; i < (pEepromConfig->numberOfBlock - 1U); i++)
        {
            pEepromConfig->flashBlocks[i] = (BLOCK_CONFIG*)addr[i + 1U];
        }

        /* Move the DEAD block to the end of the block array */
        pEepromConfig->flashBlocks[i] = (BLOCK_CONFIG*)addr[value];

        /* Decrease non DEAD block number */
        pEepromConfig->numberOfBlock--;

        /* Update index of ACTIVE block */
        if (pEepromConfig->activeBlockIndex > value)
        {
            pEepromConfig->activeBlockIndex--;
        }

        /* Update returnCode  */
        returnCode = EE_MAKE_DEAD_OK;
    }

    /* Check for number of blocks is enough of not for round-robin sequence */
    if (pEepromConfig->numberOfBlock < (NUMBER_OF_ACTIVE_BLOCKS + 1U))
    {
        returnCode = EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN;
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_ProcessImmediateRequest
 *
 * @brief:
 *
 * @param
 *
 * @return EE_INFO_HVOP_INPROGRESS: This is not an immediate request and there is an on-going erase operation
 *         EE_INFO_ERASE_SUSPEND
 *         EE_OK
 *
 ****************************************************************************/
UINT32 FSL_ProcessImmediateRequest(EEPROM_CONFIG *eepromConfig, BLOCK_CONFIG *blockConf, UINT8 iReq)
{
    BLOCK_CONFIG *internalBlockConf;
    UINT32 suspendState = EE_OK;

    /* If there is an on-going high voltage operation */
    if (FSL_FlashCheckStatus() == EE_INFO_HVOP_INPROGRESS)
    {
        /* Get internal block config */
        internalBlockConf = eepromConfig->flashBlocks[sourceBlockIndexInternal];

        /* Determine partition number of current block */
        if (blockConf->partSelect == internalBlockConf->partSelect)  /* On the same partition */
        {
            /* Suspend HVOP if the target to immediate read request is in same partition with the one is being erased */
            if (iReq != EE_IMMEDIATE_NONE)
            {
                /* Suspend for both reading and writing operation */
#if ((FLASH_MODULE != C90LC) && (FLASH_MODULE != C90LC_DFO))
                suspendState = FSL_FlashSuspend();
#else
                suspendState = FSL_FlashAbortErase();
#endif
            }
            else
            {
                /* Keep the operation continue */
                suspendState = EE_INFO_HVOP_INPROGRESS;
            }
        }
        else                                                                /* On different partitions */
        {
            if (iReq == EE_IMMEDIATE_WRITE)
            {
                /* Only suspend for writing operation */
#if ((FLASH_MODULE != C90LC) && (FLASH_MODULE != C90LC_DFO))
                suspendState = FSL_FlashSuspend();
#else
                suspendState = FSL_FlashAbortErase();
#endif
            }
        }
    }

    return suspendState;
}


/***************************************************************************!
 *
 * @name: FSL_ProgramBlockIndicator
 *
 * @brief:
 *
 * @param
 *
 * @return EE_OK
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR
 *
 ****************************************************************************/
UINT32 FSL_ProgramBlockIndicator(UINT32 dest, UINT32 source, void (*CallBack) (void))
{
    UINT32 returnCode = EE_OK;
    UINT32 i = MAX_REPGM_BLK_IND + 1U;  /* Programming times number, +1 for the default */
    UINT32 blkInd;

    while (i > 0U)
    {
        returnCode = FSL_SyncProgram(dest, WORD_SIZE, source, CallBack);
        returnCode = FSL_FlashRead(READ, dest, WORD_SIZE, (UINT32)(&blkInd), CallBack);

        if ((eccErrorStatus_Flag == TRUE) || (blkInd != ERASED_WORD))   /* Program indicator successful */
        {
            returnCode = EE_OK;
            break;
        }
        else
        {
            returnCode = EE_ERROR_PROGRAM_BLOCK_INDICATOR;
        }

        i--;    /* Decrease re-erase number */
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_ReadRecordAtAddr
 *
 * @brief:
 *
 * @param
 *
 * @return void
 *
 ****************************************************************************/
void FSL_ReadRecordAtAddr(UINT32 recordAddr, UINT16 dataSize, UINT32 bufferAddr, void (*CallBack)(void))
{
    DATA_RECORD_HEAD record;     /* Local data record head structure */
    UINT16 size;                 /* Reading size */
    UINT32 i;

    /* Read record head to get dataSize */
    FSL_ReadRecordHead(recordAddr, &record, CallBack);
    size = record.dataSize;

    if (size > dataSize)
    {
        size = dataSize;
    }

    /* Read the data portion allocated after record status field */
    for (i = 0U; (i < SMALL_DATA_AFTER_STATUS_SIZE ) && (size > 0U); i++)
    {
        WRITE8(bufferAddr, READ8(recordAddr + i + MIN_PGM_SIZE));
        bufferAddr++;
        size--;
    }

    /* Read the data portion allocated after size/id field */
    for (i = 0U; (i < SMALL_DATA_AFTER_ID_SIZE ) && (size > 0U); i ++)
    {
        WRITE8(bufferAddr, READ8(recordAddr + i + ID_OFFSET + ID_FIELD_SIZE + SIZE_FIELD)); /* Size of ID = 2 */
        bufferAddr++;
        size--;
    }

    /* Read the rest of data */
    for (i = 0U; i < size; i++)
    {
        WRITE8(bufferAddr, READ8(recordAddr + MIN_RECORD_SIZE + i));
        bufferAddr++;
    }
}


/***************************************************************************!
 *
 * @name: FSL_GetWriteRecordOption
 *
 * @brief:
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
UINT32 FSL_GetWriteRecordOption(EEPROM_CONFIG* eepromConfig, UINT32 recordLength, void (*CallBack)(void))
{
    BLOCK_CONFIG* pBlockConf;
    UINT32 returnCode;
    UINT32 temp;
    UINT32 i;

    /* Get the current ACTIVE block */
    pBlockConf = eepromConfig->flashBlocks[eepromConfig->activeBlockIndex];

    /* Check status of the current ACTIVE block */
    returnCode = FSL_ReadBlockStatus(pBlockConf, CallBack);
    if (returnCode == B_COPY_DONE)
    {
        if (recordLength <= ((pBlockConf->blockStartAddr + pBlockConf->blockSize) - pBlockConf->blankSpace))
        {
            returnCode = EE_WRITE_ON_COPYDONE;
        }
        else
        {
            returnCode = EE_ERROR_NO_ENOUGH_SPACE;
        }
    }
    else
    {
        if (recordLength <= ((pBlockConf->blockStartAddr + pBlockConf->blockSize) - pBlockConf->blankSpace))
        {
            returnCode = EE_WRITE_NORMAL;
        }
        else
        {
            /* Use temp to store number of ACTIVE blocks */
            temp = 0U;

            /* Get number of ACTIVE block */
            for (i = 0U; i < eepromConfig->numberOfBlock; i++)
            {
                pBlockConf = eepromConfig->flashBlocks[i];
                returnCode = FSL_ReadBlockStatus(pBlockConf, CallBack);
                if (returnCode == B_ACT)
                {
                    temp++;
                }
            }

            /* Get write option */
            if (temp == NUMBER_OF_ACTIVE_BLOCKS)
            {
                returnCode = EE_WRITE_SWAP;
            }
            else    /* temp < NUMBER_OF_ACTIVE_BLOCKS */
            {
                returnCode = EE_WRITE_ON_NEW_ACTIVE;
            }
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_GetEraseStatus
 *
 * @brief:
 *
 * @param
 *
 * @return EE_OK
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_PE_OPT
 *
 ****************************************************************************/
UINT32 FSL_GetEraseStatus(EEPROM_CONFIG* eepromConfig, void (*CallBack) (void))
{
    BLOCK_CONFIG* blockConf;
    UINT32 returnCode = EE_OK;
    static UINT32 re_erase_num = MAX_REERASE;
    BOOL re_erase_flag = FALSE;


    /* Check flash status
     ********************/
    returnCode = FSL_FlashCheckStatus();

    if (returnCode == EE_OK)
    {
        /* Blank check */
        blockConf = eepromConfig->flashBlocks[sourceBlockIndexInternal];
        returnCode = FSL_FlashRead(BLANK_CHECK, blockConf->blockStartAddr,
                                   blockConf->blockSize, 0U, CallBack);
        if (returnCode == EE_OK)
        {
            re_erase_flag = FALSE;   /* Doesn't re-erase */
        }
        else
        {
            re_erase_flag = TRUE;    /* Re-erase */
        }
    }
    else if (returnCode == EE_ERROR_PE_OPT)
    {
        re_erase_flag = TRUE;       /* Re-erase */
    }
    else    /* EE_INFO_HVOP_INPROGRESS */
    {
        re_erase_flag = FALSE;      /* Doesn't re-erase */
    }


    /* Check for re-erasing
     **********************/
    if ((re_erase_flag == TRUE) && (re_erase_num > 0U))
    {
        /* Decrease re_erase number */
        re_erase_num--;

#if (FLASH_MODULE == C90FL)
        /* Set depletion_Flag to avoid increasing erase cycle */
        depletion_Flag = TRUE;
#endif
        /* Async re-erase the block */
        returnCode = FSL_EraseEEBlock(eepromConfig, sourceBlockIndexInternal, FALSE, CallBack);
        if (returnCode == EE_OK)
        {
            returnCode = EE_INFO_HVOP_INPROGRESS;
        }
    }


    /* Reset re-erase number if the operation finish */
    if (returnCode != EE_INFO_HVOP_INPROGRESS)
    {
        re_erase_num = MAX_REERASE;
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_ValidateInvalidAndDeadBlocks
 *
 * @brief:
 *
 * @param
 *
 * @return EE_OK
 *         EE_INFO_HVOP_INPROGRESS
 *         EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD
 *         EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN
 *
 ****************************************************************************/
UINT32 FSL_ValidateInvalidAndDeadBlocks(EEPROM_CONFIG* eepromConfig, void (*CallBack) (void))
{
    BLOCK_CONFIG* pBlockConf;
    UINT32 returnCode = EE_OK;
    UINT32 temp;
    UINT32 i;

    /* Get total number of blocks */
    temp = eepromConfig->numberOfBlock;

    /* Validate all blocks */
    for (i = 0U; i < temp; i++)
    {
        pBlockConf = eepromConfig->flashBlocks[i];

        /* Only check the next block if the previous loop successful */
        if (returnCode == EE_OK)
        {
            returnCode = FSL_ReadBlockStatus(pBlockConf, CallBack);
        }

        /* Erase Invalid and Dead blocks */
        if ((returnCode == B_INVALID) || (returnCode == B_DEAD))
        {
            /* Synchronize erase the block */
            returnCode = FSL_EraseEEBlock(eepromConfig, i, TRUE, CallBack);

            if (returnCode == EE_OK)
            {
                /* Program a temporary erase cycle to avoid the unexpected COPY_DONE - BLANK case
                   in the 2 block - 1 ACTIVE config, use value of returnCode as erase cycle value */
                returnCode = FSL_SyncProgram(pBlockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                             WORD_SIZE, (UINT32)(&returnCode), CallBack);
            }
            /* Make the block to DEAD if there is an error */
            else if ((returnCode == EE_ERROR_PE_OPT) || (returnCode == EE_ERROR_MISMATCH))
            {
                /* Make the block to DEAD */
                returnCode = FSL_MakeBlock2Dead(eepromConfig, pBlockConf, CallBack);

                if ((returnCode == EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD) ||
                    (returnCode == EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN))
                {
                    /* Exit if failed to make dead or
                     * there isn't enough number of blocks for round robin sequence */
                    break;
                }
            }
            else    /* For returnCode = EE_INFO_HVOP_INPROGRESS or EE_OK */
            {
                i = temp; /* Break the loop */
            }
        }
        else
        {
            returnCode = EE_OK;     /* Reset returnCode to goto the next loop */
        }
    }

    return returnCode;
}


/***************************************************************************!
 *
 * @name: FSL_GetLastJobStatus
 *
 * @brief:
 *
 * @param
 *
 * @return
 *
 ****************************************************************************/
UINT32 FSL_GetLastJobStatus(EEPROM_CONFIG *eepromConfig, void (*CallBack)(void))
{
    BLOCK_CONFIG* pBlockConf;
    UINT32 returnCode = EE_OK;
    UINT32 temp;
    UINT32 i;
    UINT32 copyDoneIdx = 0xFFFFFFFFU;
    UINT32 lastestActiveIndex = 0xFFFFFFFFU;
    UINT32 firstBlockStatus;
    UINT32 secondBlockStatus;
    UINT32 previousBlockStatus;     /* Used in determining index of the COPY_DONE block */
    UINT32 alterBlockEC = 1U;       /* Used to determine that all Alter blocks have erase cycle value == 1 ? */
    UINT32 lastJob = EE_LAST_JOB_NONE;
    UINT32 activeNum = 0U;
    UINT32 copyDoneNum = 0U;
    UINT32 erasedNum = 0U;
    UINT32 updatedNum = 0U;
    UINT32 alterNum = 0U;


    /* Read block status
     *******************/
    temp = eepromConfig->numberOfBlock; /* Use temp as temporary block number */

    /* First read status of the last block */
    pBlockConf = eepromConfig->flashBlocks[temp - 1U];
    previousBlockStatus = FSL_ReadBlockStatus(pBlockConf, CallBack);

    /* Read status of all blocks and get related information */
    for (i = 0U; i < temp; i++)
    {
        /* Get block status */
        pBlockConf = eepromConfig->flashBlocks[i];
        returnCode = FSL_ReadBlockStatus(pBlockConf, CallBack);

        /* Backup status of the first and second block, used to checking for first time initialization */
        if (i == 0U)
        {
            firstBlockStatus = returnCode;
        }

        if (i == 1U)
        {
            secondBlockStatus = returnCode;
        }

        if (returnCode == B_ACT)                /* ACTIVE block */
        {
            /* Increase ACTIVE block number */
            activeNum++;

            /* If the lastest ACTIVE block at the end of the block array */
            if ((i == (temp - 1U)) && (lastestActiveIndex == 0xFFFFFFFFU))
            {
                lastestActiveIndex = i;
            }
        }
        else
        {
            /* The lastest ACTIVE block is follow by a non-ACTIVE block */
            if ((activeNum > 0U) && (lastestActiveIndex == 0xFFFFFFFFU))
            {
                lastestActiveIndex = i - 1U;
            }

            if (returnCode == B_COPY_DONE)      /* COPY_DONE block */
            {
                copyDoneNum++;

                if (copyDoneIdx == 0xFFFFFFFFU)
                {
                    copyDoneIdx = i;
                }

                if (previousBlockStatus == B_ACT)   /* NUMBER_OF_ACTIVE_BLOCKS > 1 */
                {
                    copyDoneIdx = i;
                }

                if ((previousBlockStatus == B_COPY_DONE) && (NUMBER_OF_ACTIVE_BLOCKS == 1U))
                {
                    copyDoneIdx = i;
                }
            }
            else if (returnCode == B_UPDATE)    /* UPDATE block */
            {
                updatedNum++;
            }
            else if (returnCode == B_ERASED)    /* ERASED block */
            {
                erasedNum++;
            }
            else                                /* ALTERNATIVE block */
            {
                alterNum++;
                
                if (alterBlockEC == 1U) /* Only read Erase cycle value if all Alter blocks have erase cycle value = 1 */
                {
                    /* Read erase cycle, use 'lastJob' as temporary return value */
                    lastJob = FSL_FlashRead(READ, pBlockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                                            WORD_SIZE, (UINT32)(&alterBlockEC), CallBack);
                    if (alterBlockEC != 1U)
                    {
                        alterBlockEC = 0xFFFFFFFFU;     /* Stop read Erase cycle of Alter blocks */
                    }
                }
            }
        }

        /* Backup status */
        previousBlockStatus = returnCode;
    }

    /* Read erase cycle of the first block, use the temp variable to store the value */
    pBlockConf = eepromConfig->flashBlocks[0U];
    returnCode = FSL_FlashRead(READ, pBlockConf->blockStartAddr + ERASE_CYCLE_OFFSET,
                               WORD_SIZE, (UINT32)(&temp), CallBack);

    /* Blank check data region of the first block */
    returnCode = FSL_FlashRead(BLANK_CHECK, pBlockConf->blockStartAddr + RECORD_OFFSET,
                               pBlockConf->blockSize - RECORD_OFFSET, 0U, CallBack);

    /* Reset lastJob value */
    lastJob = EE_LAST_JOB_NONE;

    
    /* Check for the unrecoverable state if the EE system has 2 blocks
     *****************************************************************/
    if (eepromConfig->numberOfBlock == 2U)
    {
        if ((copyDoneNum == 2U) ||
            ((firstBlockStatus == B_COPY_DONE) && (secondBlockStatus == B_ERASED)) ||
            ((firstBlockStatus == B_ACT) && (secondBlockStatus == B_COPY_DONE) && (temp == 1U) && (returnCode == EE_OK)))
        {
            lastJob = EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK;
        }
    }


    /* Check for the first time initialization
     *****************************************/
    if (lastJob == EE_LAST_JOB_NONE)
    {
        if ((activeNum == 0U) && (copyDoneNum == 0U))
        {
            lastJob = EE_LAST_JOB_FIRST_TIME;
        }
        else if (((firstBlockStatus == B_COPY_DONE) || (firstBlockStatus == B_ACT)) &&
                 (erasedNum == (eepromConfig->numberOfBlock - 1U)))
        {
            lastJob = EE_LAST_JOB_FIRST_TIME;
        }
        else
        {
            /* Check the first block */
            if (firstBlockStatus == B_ACT)
            {
                if ((temp == 1U) && (returnCode == EE_OK))  /* The first block at the first time initialization state */
                {
                    if ((alterNum == (eepromConfig->numberOfBlock - 1U)) && (alterBlockEC == 1U))
                    {
                        lastJob = EE_LAST_JOB_NORMAL;
                    }
                    else if ((eepromConfig->numberOfBlock == 2U) || (erasedNum > 0U) || (secondBlockStatus == B_ALT))
                    {
                        lastJob = EE_LAST_JOB_FIRST_TIME;
                    }
                    else
                    {
                        /* Continue determine last job */
                    }
                }
            }
        }
    }


    /* Check for other cases - isn't the first time initialization
     *************************************************************/
    if (lastJob == EE_LAST_JOB_NONE)
    {
        if ((activeNum == NUMBER_OF_ACTIVE_BLOCKS) && (updatedNum != 0U))
        {
            lastJob = EE_LAST_JOB_UPDATE;
        }
        else if (copyDoneNum >= 1U)
        {
            lastJob = EE_LAST_JOB_COPY_DONE;

            /* Set lastest ACTIVE block index */
            lastestActiveIndex = copyDoneIdx;
        }
        else
        {
            lastJob = EE_LAST_JOB_NORMAL;
        }
    }
    
    /* Set lastest ACTIVE block index */
    eepromConfig->activeBlockIndex = lastestActiveIndex;

    return lastJob;
}

/* EOF */
