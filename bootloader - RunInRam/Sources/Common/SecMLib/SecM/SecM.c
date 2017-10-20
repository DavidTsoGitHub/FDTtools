
/*=======[I N C L U D E S]====================================================*/

#include "SecM.h"
#include "SecM_Cfg.h"
#include "Cal.h"
#include "Appl.h"
#include "stdlib.h"
#include "Board.h"

/*=======[E X T E R N A L   D A T A]==========================================*/
/** CRC buffer */
uint8 SecM_CrcDataBuffer[SECM_CRC_BUFFER_LEN];

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
STATIC SecM_StatusType SecM_ProcessCrc(SecM_CRCParamType *crcParam,
                                       uint32 Address,
                                       uint32 Length);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/**
 * @brief               <compare key>
 * 
 * <Needed by the UDS module,compare seed> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <key (IN), seed (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <SecM_StatusType>    
 */
/******************************************************************************/
SecM_StatusType SecM_CompareKey(SecM_KeyType key, 
                                SecM_SeedType seed)
{
    SecM_StatusType ret = SECM_OK;
    SecM_KeyType getkey;
    
    /* execute key compute */
    ret = Board_ComputeKey(seed, SEC_ECU_KEY, &getkey);
    
    /* compare key */
    if ((ret != SECM_OK) || (getkey != key))
    {
        ret = SECM_NOT_OK;
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <compute CRC>
 * 
 * <process CRC compute,include init,compute and finish> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <crcParam (IN/OUT)>
 * @return              <SecM_StatusType>    
 */
/******************************************************************************/
SecM_StatusType SecM_ComputeCRC(SecM_CRCParamType *crcParam)
{
    SecM_StatusType ret = SECM_OK;
    
    switch (crcParam->crcState)
    {
        case CRC_INIT:
            /* CRC value init */
            Cal_Crc32Init(&crcParam->currentCRC);
            break;
            
        case CRC_COMPUTE:
            /* CRC value compute */
            Cal_ComputeCrc32(&crcParam->currentCRC,
                             crcParam->crcSourceBuffer,
                             crcParam->crcByteCount);
            break;

        case CRC_FINALIZE:
            /* CRC value finish */
            Cal_CrcFinalize(&crcParam->currentCRC);
            break;

        default:
            ret = SECM_NOT_OK;
            break;
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <verificate CRC value>
 * 
 * <verificate if transfered CRC is equal to computed CRC> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <verifyParam (IN/OUT)>
 * @return              <SecM_StatusType>    
 */
/******************************************************************************/
SecM_StatusType SecM_Verification(SecM_VerifyParamType *verifyParam)
{
    SecM_StatusType ret = SECM_OK;
    SecM_CRCParamType crcParam;
    uint8 segmentIndex = 0;
    
    if (NULLPTR == verifyParam->segmentList)
    {
        /* if segment list is NULL ,verificate is failed */
        ret = SECM_NOT_OK;
    }
    else
    {
        /* init CRC */
        crcParam.currentCRC = 0;
        crcParam.crcState = (uint8)CRC_INIT;
        ret = SecM_ComputeCRC(&crcParam);   
    }
    
    for (segmentIndex = 0; 
         (segmentIndex < verifyParam->segmentList->nrOfSegments) && (ret == SECM_OK);
         segmentIndex ++)
    {
        /* compute each segment CRC */
        ret = SecM_ProcessCrc(&crcParam,
                              verifyParam->segmentList->segmentInfo[segmentIndex].address,
                              verifyParam->segmentList->segmentInfo[segmentIndex].length);                                              
    }
    
    if (ret == SECM_OK)
    {
        /* finish compute CRC */
        crcParam.crcState = CRC_FINALIZE;
        ret = SecM_ComputeCRC(&crcParam);
    }
    
    if (ret == SECM_OK)
    {
        /* get CRC transfered from  client */
        SecM_CRCType transferedCrc = 0x00uL;
        
        transferedCrc = ((SecM_CRCType)verifyParam->verificationData[0]) << 24;
        transferedCrc += ((SecM_CRCType)verifyParam->verificationData[1]) << 16;
        transferedCrc += ((SecM_CRCType)verifyParam->verificationData[2]) << 8;
        transferedCrc += (SecM_CRCType)verifyParam->verificationData[3];
        
        /* compare CRC */
        if (transferedCrc != crcParam.currentCRC)
        {
            ret = SECM_NOT_OK;
        }
        
        /* save CRC */
        verifyParam->crcTotle = crcParam.currentCRC;
    }
    
    return ret;
}


/******************************************************************************/
/**
 * @brief               <process CRC compute>
 * 
 * <CRC compute> .
 * @param[in]           <Address (IN), Length (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <crcParam (IN/OUT)>
 * @return              <SecM_StatusType>    
 */
/******************************************************************************/
STATIC SecM_StatusType SecM_ProcessCrc(SecM_CRCParamType *crcParam,
                                       uint32 Address,
                                       uint32 Length)
{
    SecM_StatusType ret = SECM_OK;
    uint32 readLength = 0;
    
    /* set CRC compute step */
    crcParam->crcState = CRC_COMPUTE;
    
    while ((Length > 0) && (ret == SECM_OK))
    {
        /* read maxmun length is  SECM_CRC_BUFFER_LEN*/
        if (Length > SECM_CRC_BUFFER_LEN)
        {
            readLength = SECM_CRC_BUFFER_LEN;
        }
        else
        {
            readLength = Length;
        }
        
        /* get source data from memory */
        crcParam->crcByteCount = (uint16)FL_ReadMemory (Address, 
                                                        readLength,
                                                        SecM_CrcDataBuffer);
        Length -= readLength;
        Address += readLength;
        crcParam->crcSourceBuffer = SecM_CrcDataBuffer;
        
        /* update watch dog */
        Appl_UpdateTriggerCondition();
        
        /* compute CRC */
        ret = SecM_ComputeCRC(crcParam);    
    }
    
    return ret;
}
