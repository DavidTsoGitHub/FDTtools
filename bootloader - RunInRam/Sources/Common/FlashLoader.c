/*=======[I N C L U D E S]====================================================*/
#include "FlashLoader.h"
#include "Nvm.h"
#include "Fls.h"
#include "SecM.h"
#include "Appl.h"

/*=======[M A C R O S]========================================================*/
/** maxmun length of program buffer */
#define FL_PROGRAM_WORD_SIZE	0x20uL
#define FL_PROGRAM_SIZE	(FL_PROGRAM_WORD_SIZE * 0x04uL)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** flashloader job status */
typedef enum
{
    FL_JOB_IDLE,
    
    FL_JOB_ERASING,
    
    FL_JOB_PROGRAMMING,
    
    FL_JOB_CHECKING
    
} FL_ActiveJobType;

/** flashloader download step */
typedef enum
{
    FL_REQUEST_STEP,
    
    FL_TRANSFER_STEP,
    
    FL_EXIT_TRANSFER_STEP,
    
    FL_CHECKSUM_STEP
    
} FL_DownloadStepType;

/** flashloader status information */
typedef struct
{
    /* flag if fingerprint has written */
    boolean fingerPrintWritten;
    
    /* flag if fingerprint buffer */
    uint8 fingerPrint[FL_FINGER_PRINT_LENGTH];
    
    /* flag if flash driver has downloaded */
    boolean flDrvDownloaded; 
    
    /* error code for flash active job */
    FL_ResultType errorCode;
    
    /* flag if current block is erased */
    boolean blockErased;
    
    /* current process block index */
    uint8 blockIndex;
    
    /* current process start address */
    uint32 startAddr;
    
    /* current process length */
    uint32 length;
    
    /* current process buffer point,point to buffer supplied from DCM */
    const uint8 *dataBuff;
    
    /* segment list of current process block */
    FL_SegmentListType segmentList;
    
    /* block checksum received from diagnostic tool */
    uint32 blockChecksum;

    /* flashloader download step */
    FL_DownloadStepType downloadStep;
    
    /* current job status */
    FL_ActiveJobType activeJob;
    
} FL_DownloadStateType;

/*=======[E X T E R N A L   D A T A]==========================================*/
/** NVM information which include bootloader information,read from EEPROM */
FL_NvmInfoType FL_NvmInfo[FL_NUM_LOGICAL_BLOCKS];

/*=======[I N T E R N A L   D A T A]==========================================*/
/** flashloader status information */
STATIC FL_DownloadStateType downloadStatus;

/** flashloader block header information */
FL_BlockHeader swBlockInfo[FL_NUM_LOGICAL_BLOCKS];

/** flashloader program buffer */
STATIC uint32 programData[FL_PROGRAM_WORD_SIZE];

/** flashloader program length */
STATIC uint32 programLength;

/** flashloader program align size,transfered address may not aligned */
STATIC uint32 alignedProgramSize;

/** flash driver API input parameter */
STATIC tFlashParam flashParamInfo = 
{
    BLFLASH_DRIVER_VERSION_PATCH,
    BLFLASH_DRIVER_VERSION_MINOR,
    BLFLASH_DRIVER_VERSION_MAJOR,
    0x00u,
    kFlashOk,
    0x0000u,
    0x00000000uL,
    0x00000000uL,
    NULLPTR,
    &Appl_UpdateTriggerCondition,
};

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
STATIC uint32 FL_GetAlignSize(void);

STATIC FL_ResultType FL_Erasing(void);

STATIC FL_ResultType FL_CheckDownloadSegment(void);

STATIC FL_ResultType FL_ProgrammingAlignData(void);

STATIC FL_ResultType FL_ProgrammingData(void);

STATIC FL_ResultType FL_Programming(void);

#if(FL_USE_GAP_FILL == TRUE)
STATIC FL_ResultType FL_FillGap(void);
#endif

STATIC FL_ResultType FL_CheckSuming(void);

STATIC FL_ResultType FL_UpdateNvm(uint8 FL_NvmInfoIndex, uint8 blockindex);

STATIC FL_ResultType FL_EmptyNvm(void);

/*=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/******************************************************************************/
/**
 * @brief               <flashloader module initialize>
 * 
 * <initialize download status> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void FL_InitState(void)
{
	uint8 i;

    /* fingerprint is not written */
    downloadStatus.fingerPrintWritten = FALSE;
    
    /* flash driver is not downloaded */
    downloadStatus.flDrvDownloaded = FALSE;
    
    /* current block is not erased */
    downloadStatus.blockErased = FALSE;
    
    /* download step is download request 0x34 */
    downloadStatus.downloadStep = FL_REQUEST_STEP;
    
    /* current active job is idle */
    downloadStatus.activeJob = FL_JOB_IDLE;

    /* initialize download blocks */
    for (i=0;i<FL_NUM_LOGICAL_BLOCKS; i++) {
       swBlockInfo[i].blkValid = 0xffff;
    }

    return;
}

/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF15B>
 * 
 * <read data by identifier 0xF15B Finger print from all blocks>.
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>    
 */
/******************************************************************************/
uint16 FL_ReadDidF15B(uint8 *readData)
{
	uint8 i;
	uint8* nextdata=readData;

	for (i=0; i<FL_NUM_LOGICAL_BLOCKS;i++) {
		Appl_Memcpy(nextdata, (const uint8*)FL_NvmInfo[i].fingerPrint, FL_FINGER_PRINT_LENGTH);
		nextdata += FL_FINGER_PRINT_LENGTH;
	}
    
    return i*FL_FINGER_PRINT_LENGTH;
}

/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF180>
 *
 * <read data by identifier 0xF180 Boot Software Identification Data Identifier> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>
 */
/******************************************************************************/
uint16 FL_ReadDidF180(uint8 *readData)
{
    uint8* didPtr	= (uint8*)DID_BOOT_SW_ID;

    /* Read Boot Software Identifier from dedicated memory */
    Appl_Memcpy(readData, (const uint8*)didPtr, DID_BOOT_SW_ID_LEN);

    return DID_BOOT_SW_ID_LEN;
}


/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF18B>
 *
 * <read data by identifier 0xF18B ECU Manufacturing Date Data Identifier > .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>
 */
/******************************************************************************/
uint16 FL_ReadDidF18B(uint8 *readData)
{
    uint8* didPtr	= (uint8*)DID_ECU_MANUFACTURING_DATE;

    /* Read ECU Manufacturing Data from dedicated memory */
    Appl_Memcpy(readData, (const uint8*)didPtr, DID_ECU_MANUFACTURING_DATE_LEN);

    return DID_ECU_MANUFACTURING_DATE_LEN;
}

/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF18C>
 *
 * <read data by identifier 0xF18C ECU Serial Number Data Identifier> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>
 */
/******************************************************************************/
uint16 FL_ReadDidF18C(uint8 *readData)
{
    uint8* didPtr	= (uint8*)DID_ECU_SERIAL_NO;

    /* Read System Supplier ECU Serial Number Data from dedicated memory */
    Appl_Memcpy(readData, (const uint8*)didPtr, DID_ECU_SERIAL_NO_LEN);

    return DID_ECU_SERIAL_NO_LEN;
}

/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF192>
 *
 * <read data by identifier 0xF192 System Supplier ECU Hardware Number Data Identifier > .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>
 */
/******************************************************************************/
uint16 FL_ReadDidF192(uint8 *readData)
{
    uint8* didPtr	= (uint8*)DID_SYS_SUPL_ECU_HW_NO;

    /* Read System Supplier ECU Hardware Number Data from dedicated memory */
    Appl_Memcpy(readData, (const uint8*)didPtr, DID_SYS_SUPL_ECU_HW_NO_LEN);

    return DID_SYS_SUPL_ECU_HW_NO_LEN;
}

/******************************************************************************/
/**
 * @brief               <read data by identifier 0xF1FA>
 *
 * <read data by identifier 0xF1FA Boot Identifier > .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <readData (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint16>
 */
/******************************************************************************/
uint16 FL_ReadDidF1FA(uint8 *readData)
{
    const uint8* didPtr	= (const uint8*)DID_BOOT_ID;

    /* Read System Supplier ECU Hardware Number Data from dedicated memory */
    Appl_Memcpy(readData, (const uint8*)didPtr, DID_BOOT_ID_LEN);

    return DID_BOOT_ID_LEN;
}

/******************************************************************************/
/**
 * @brief               <read memory for checksum>
 * 
 * <Needed by the security module to read only those memory areas that are in 
 *  the scope of the flashloader (e.g. not erase/write protected, not RAM, ...)> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <address (IN), length (IN)>
 * @param[out]          <data (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint32>    
 */
/******************************************************************************/
uint32 FL_ReadMemory(uint32 address, uint32 length, uint8 * data)
{
    uint32 readLength = 0x00u;
    uint8 curBlockIndex = 0;
    
    /* read data from flash block */
    for (curBlockIndex = 0; 
         curBlockIndex < FL_NUM_LOGICAL_BLOCKS; 
         curBlockIndex ++)
    {
        /* check if address is in range of logical blocks */
        if ((address >= FL_BlkInfo[curBlockIndex].address) && 
            (address < (FL_BlkInfo[curBlockIndex].address + 
                        FL_BlkInfo[curBlockIndex].length)))
        {
            while ((length > 0) && 
                   (address < (FL_BlkInfo[curBlockIndex].address + 
                               FL_BlkInfo[curBlockIndex].length)))
            {
                /* read data from global address */
                *data = *(uint8 *)address;
                data ++;
                address ++;
                length --;
                readLength ++;
            }
        }
    }
    
    /* read data from RAM of flash driver */
    if ((address >= FL_DEV_BASE_ADDRESS) && 
        (address < (FL_DEV_BASE_ADDRESS + FL_DEV_SIZE)))
    {
        while ((length > 0) && 
               (address < (FL_DEV_BASE_ADDRESS + FL_DEV_SIZE)))
        {
            /* read data from local address in RAM,direct read and write */
            *data = *(uint8 *)address;
            data ++;
            address ++;
            length --;
            readLength ++;
        }
    }
    
    return readLength;    
}

/******************************************************************************/
/**
 * @brief               <0x31 check program pre-condition>
 * 
 * <0x31 check program pre-condition .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <conditions (OUT)>
 * @param[in/out]       <NONE>
 * @return              <uint8>    
 */
/******************************************************************************/
uint8 FL_CheckProgPreCondition(uint8 * conditions)										 
{
    *conditions = 0x00u; /* Since when in boot there is always OK to start a programming */
    
    return 0x01u;
}

/******************************************************************************/
/**
 * @brief               <0x2E service write fingerprint>
 * 
 * <save fingerprint to internal buffer,fingerprint is written to EEPROM when
 *  request erase routine control.> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <data (IN), length (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_WriteFingerPrint(uint8 *data,const uint16 length)										 
{
    FL_ResultType ret = FL_OK;

    /* check if fingerprint length is correct */
    if (FL_FINGER_PRINT_LENGTH == length)
    {
        /* save the BlockFingerPrint info*/
        Appl_Memcpy(downloadStatus.fingerPrint, data, FL_FINGER_PRINT_LENGTH);
        
        /* set the download  state of the BlockFingerPrint */
        downloadStatus.fingerPrintWritten = TRUE;
        
        ret = FL_OK;
    }
    else
    {
        ret = FL_FAILED;
    }
 
    return ret;
}

/******************************************************************************/
/**
 * @brief               <0x31 service routine checksum>
 * 
 * <checksum routine for flash driver or logical blocks,only for current 
 *  download address> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <checkSum (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_CheckSumRoutine(const uint8 *checkSum)
{
    
    FL_ResultType ret = FL_OK;
    
    /* record checksum buffer address */
    downloadStatus.dataBuff = checkSum;
    
    /* check if download step is checksum step */
    if (FL_CHECKSUM_STEP == downloadStatus.downloadStep)
    {
        /* set the download active state to checksum */
        downloadStatus.activeJob = FL_JOB_CHECKING;
        downloadStatus.errorCode = FL_OK;
    }
    else
    {
        ret = FL_ERR_SEQUENCE;
        
        /* initialize the flash download state */
        FL_InitState();
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <0x31 service routine erase>
 * 
 * <erase routine for  logical block> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <blockIndex (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_EraseRoutine(const uint8 blockIndex)
{
    FL_ResultType ret = FL_OK;

    /* check if finger print is written */
    if (FALSE == downloadStatus.fingerPrintWritten)
    {
        ret = FL_NO_FINGERPRINT;
    }
    else
    {
        /* check if flash driver is downloaded */
        if (FALSE == downloadStatus.flDrvDownloaded)
        {
            ret = FL_NO_FLASHDRIVER;
        }
        else
        {
            /* check download step sequence */
            if (FL_REQUEST_STEP != downloadStatus.downloadStep)
            {
                ret = FL_ERR_SEQUENCE;
            }
        }
    }
    
    /* check the conditions of erase */
    if (FL_OK == ret)
    {
        /* check the erase block index and the program attempt */
        if (blockIndex < FL_NUM_LOGICAL_BLOCKS)
        {
            if (FL_NvmInfo[blockIndex].programming_attempts < FL_MAX_PROGRAMMING_ATTEMPTS)
            {
                /* set current block is valid */
                FL_NvmInfo[blockIndex].areaType = blockIndex;

                /* increment program attempt counter of current block */
                FL_NvmInfo[blockIndex].programming_attempts ++;
                
                /* store finger print of current block */
                Appl_Memcpy(FL_NvmInfo[blockIndex].fingerPrint,
                            downloadStatus.fingerPrint,FL_FINGER_PRINT_LENGTH);
                            
                /* change and initialize status of the programmed block */
                downloadStatus.segmentList.nrOfSegments = 0x00u;
                downloadStatus.blockIndex = blockIndex;
                downloadStatus.blockErased = FALSE;
                downloadStatus.errorCode = FL_OK;
                
                /* set the download active state to erase */
                downloadStatus.activeJob = FL_JOB_ERASING;
            }
            else
            {
                ret = FL_FAILED;
            }
        }
        else
        {
            ret = FL_INVALID_DATA;
        }
    }
    
    if (ret != FL_OK)
    {
        /* initialize the flash download state */
        FL_InitState();
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <0x34 service download request>
 * 
 * <download request for current logical block> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <startAdd (IN), length (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_DownloadRequestValid(const uint32 startAdd,const uint32 length)
{
    FL_ResultType ret = FL_OK;
    
    /* save the parameter used for active job use */
    downloadStatus.startAddr = startAdd;    
    downloadStatus.length = length;

    /* check if finger print is written */
    if (FALSE == downloadStatus.fingerPrintWritten)
    {
        ret = FL_NO_FINGERPRINT;
    }
    else
    {
        /* check download step sequence */
        if ((FL_REQUEST_STEP != downloadStatus.downloadStep) && 
            (FL_CHECKSUM_STEP != downloadStatus.downloadStep))
        {
            ret = FL_ERR_SEQUENCE;
        }
    }
    
    /* check the condition for download request */
    if (FL_OK == ret)
    {
        /* check if flash driver is downloaded */
        if (FALSE == downloadStatus.flDrvDownloaded)
        {
            /* check if download address is in flash driver RAM range */
            if ((FL_DEV_BASE_ADDRESS == downloadStatus.startAddr) &&
                (downloadStatus.length <= FL_DEV_SIZE))
            {
                /* set the download step*/
                downloadStatus.downloadStep = FL_TRANSFER_STEP;
                downloadStatus.segmentList.nrOfSegments = 0x01u;
                downloadStatus.segmentList.segmentInfo[0].address = downloadStatus.startAddr;
                downloadStatus.segmentList.segmentInfo[0].length = downloadStatus.length;
            }
            else
            {
                ret = FL_NO_FLASHDRIVER;
            }
        }
        else
        {
            /* check if download address is in correct range */
            ret = FL_CheckDownloadSegment();
            
            if (FL_OK == ret)
            {
                /* if address is not aligned ,save the align size */
                uint32 alignSize = FL_GetAlignSize() - 1;
                
                alignedProgramSize = downloadStatus.startAddr & alignSize;
                
                /* set the download step*/
                downloadStatus.downloadStep = FL_TRANSFER_STEP;
                
                /* initialize the program buffer */
                Appl_Memset((uint8 *)&programData[0],
                            (uint8)FL_GAP_FILL_VALUE,
                            (uint32)FL_PROGRAM_SIZE);   
            }
        }
    }
    
    if (ret != FL_OK)
    {
        /* initialize the flash download state */
        FL_InitState();
    }
    
    downloadStatus.activeJob = FL_JOB_IDLE;
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <0x36 service download data>
 * 
 * <download data for current logical block> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <destAddr (IN), sourceBuff (IN), length (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_FlashProgramRegion(const uint32 destAddr,
									const uint8 *sourceBuff,
									const uint32 length)
{
    FL_ResultType ret = FL_OK;
    
    /* change local address to global address*/
    uint32 globalAddr = destAddr;
    
    /* check the conditions of the program*/
    if (FL_TRANSFER_STEP != downloadStatus.downloadStep)
    {
        ret = FL_ERR_SEQUENCE;
    }
    else
    {
        if ((downloadStatus.startAddr != globalAddr) || 
            (downloadStatus.length < length))
        {
            ret = FL_ERR_ADDR_LENGTH;
        }
    }
    
    if (FL_OK == ret)
    {
        /* check if flash driver is downloaded */
        if (FALSE == downloadStatus.flDrvDownloaded)
        { 
        	/* This is where we end up if the transfer is the flash driver! */
            /* copy the data to the request address*/
            Appl_Memcpy((uint8 *)globalAddr, sourceBuff, (uint32)length);
            
            /* index start address and length */
            downloadStatus.startAddr += length;
            downloadStatus.length -= length;
            
            /* check if flash driver download is finished */
            if (0x00uL == downloadStatus.length)
            {
                /* set the download step*/
                downloadStatus.downloadStep = FL_EXIT_TRANSFER_STEP;
            }
            
            downloadStatus.activeJob = FL_JOB_IDLE;
        }
        else
        {
            /* save parameter for program active job */
            downloadStatus.dataBuff = sourceBuff;
            programLength = length;
            
            /* set the download active state to program*/
            downloadStatus.activeJob = FL_JOB_PROGRAMMING;
            downloadStatus.errorCode = FL_OK;
        }
    }

    if (ret != FL_OK)
    {
        /* initialize the flash download state */
        FL_InitState();
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <0x37 service download finish>
 * 
 * <download finish for current logical block> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
FL_ResultType FL_ExitTransferData(void)
{
    FL_ResultType ret = FL_OK;
    
    if (FL_EXIT_TRANSFER_STEP == downloadStatus.downloadStep)
    {
        /* set the download step */
        downloadStatus.downloadStep = FL_CHECKSUM_STEP;
        downloadStatus.activeJob = FL_JOB_IDLE;
    }
    else
    {
        /* initialize the flash download state */
        ret = FL_ERR_SEQUENCE;
        FL_InitState();
    }
    
    return ret;
}


/******************************************************************************/
/**
 * @brief               <get service status>
 * 
 * <supply active job status for service witch can not response with in 50ms,
 *  and send pending> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <NON Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <error (OUT)>
 * @param[in/out]       <NONE>
 * @return              <boolean>    
 */
/******************************************************************************/
boolean FL_ServiceFinished(FL_ResultType *error)
{
    boolean ret = FALSE;
    
    /* check if service is finished */
    if (FL_JOB_IDLE == downloadStatus.activeJob)
    {
        *error = downloadStatus.errorCode;
        ret = TRUE;
    }
    else
    {
        *error = FL_OK;
        ret = FALSE;
    }
    
    return ret;
}


/******************************************************************************/
/**
 * @brief               <flash main function for active job>
 * 
 * <flash main function for active job,process checksum,erase and program> .
 * Service ID   :       <NONE>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <NONE>    
 */
/******************************************************************************/
void FL_MainFunction(void)
{
    switch (downloadStatus.activeJob)
    {	
        case FL_JOB_ERASING:
            /* do the flash erase*/
            downloadStatus.errorCode = FL_Erasing();
            break;

        case FL_JOB_PROGRAMMING:
            /* do the flash program*/
            downloadStatus.errorCode = FL_Programming();
            break;

        case FL_JOB_CHECKING:
            /* do the flash checksum*/
            downloadStatus.errorCode = FL_CheckSuming();
            break;
			
        default:
            break;
    }
	
    if (downloadStatus.errorCode != FL_OK)
    {
        /* Save any properly downloaded block header before aborting this download */
        FL_WriteBlockHeaders();

        /* initialize the flash download state */
        FL_InitState();
    }
    
    downloadStatus.activeJob = FL_JOB_IDLE;
    
    return;
}

/******************************************************************************/
/**
 * @brief               <search for end element of NvmInfo in the EEPROM>
 *
 * <loops through the NvmInfo area to look for the first empty element> .
 * @param[in]           <uint8* pointer to the index return variable>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>
 */
/******************************************************************************/
FL_ResultType  FL_FindFirstEmptyFlashInfo(uint8 *FL_NvmInfo_Index)
{
	FL_NvmInfoType *NvmInfoPtr = (FL_NvmInfoType*)FL_FingerPrint;

	for (*FL_NvmInfo_Index = 0; *FL_NvmInfo_Index < FL_MAX_NO_FINGER_PRINTS; (*FL_NvmInfo_Index)++ ) {
		if ( NvmInfoPtr[*FL_NvmInfo_Index].areaType == 0xFF) {
			break; /* break for the first empty record */
		}
	}

	if (*FL_NvmInfo_Index == FL_MAX_NO_FINGER_PRINTS) {
		return FL_NO_FINGERPRINT;
	} else {
		return FL_OK;
	}
}

/******************************************************************************/
/**
 * @brief               <get aligned size by address>
 *
 * <code flash align size is 0x08,data flash align size is 0x02,this mean align
 *  size if the length that should program once time> .
 * @param[in]           <addr (IN)>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <uint32>
 */
/******************************************************************************/
STATIC uint32 FL_GetAlignSize(void)
{
	/* flash minimum write size is 8 byte */
    return FL_FLASH_ALIGN_SIZE;
}

/******************************************************************************/
/**
 * @brief               <Erase the NVM area and return the current values as first entry>
 *
 * <erase all information in EEPROM,e.g. block valid,checksum, fingerprint
 *  and then write back the current/last values>.
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>
 */
/******************************************************************************/
STATIC FL_ResultType FL_EmptyNvm(void)
{
    FL_ResultType ret = FL_FAILED;
	FL_NvmInfoType NvmInfo[FL_NUM_LOGICAL_BLOCKS];

    /************************************/
    /* save last entries                */
    /************************************/
    Appl_InitNvmInfo(NvmInfo);

    /*************************************/
    /* Erase finger print area            */
    /*************************************/
    /* set flash driver input parameter */
    flashParamInfo.address = (long)FL_FingerPrint;
    flashParamInfo.length = FL_MAX_NO_FINGER_PRINTS*sizeof(FL_NvmInfoType);

    /* erase the flash of the requested block*/
    BLFlash_Info.flashEraseFct(&flashParamInfo);

    /* check if erase success */
    if (kFlashOk == flashParamInfo.errorCode)
    {
		/*************************************/
		/* Write back the current NVM values */
		/*************************************/

		/* set input parameter of flash driver interface */
		flashParamInfo.data = (const uint8 *)&NvmInfo[0];
		flashParamInfo.address = (tFlashAddress)&FL_FingerPrint[0];
		flashParamInfo.length = sizeof(FL_NvmInfoType);

		/* program blocks information */
		BLFlash_Info.flashWriteFct(&flashParamInfo);

		if (kFlashOk == flashParamInfo.errorCode)
		{
			/* set input parameter of flash driver interface */
			flashParamInfo.data = (const uint8 *)&NvmInfo[1];
			flashParamInfo.address = (tFlashAddress)&FL_FingerPrint[1];
			flashParamInfo.length = sizeof(FL_NvmInfoType);

			/* program blocks information */
			BLFlash_Info.flashWriteFct(&flashParamInfo);

			if (kFlashOk == flashParamInfo.errorCode)
			{
				ret = FL_OK;
			}
		}
    }
    return ret;
}


/******************************************************************************/
/**
 * @brief               <program bootloader information to EEPROM>
 * 
 * <program bootloader information to EEPROM,e.g. block valid,checksum,
 *  fingerprint..> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_UpdateNvm(uint8 FL_NvmInfoIndex, uint8 blockindex)
{
    FL_ResultType ret = FL_FAILED;
    uint32 alignSize = FL_GetAlignSize();
    /* CRC32 parameter */
    SecM_CRCParamType crcParam;
    
    /* Check that the size of the FL_NvmInfoType is in range */
    if (FL_NvmInfoIndex > FL_MAX_NO_FINGER_PRINTS)
    {
        return FL_FAILED;
    }
    
    /* Initialize CRC32 parameter */    
    crcParam.crcState = CRC_INIT;
    crcParam.crcSourceBuffer = (const uint8 *)&FL_NvmInfo[blockindex];
    crcParam.crcByteCount = sizeof(FL_NvmInfoType) - 4;
    
    /* compute CRC of the block information */
    (void)SecM_ComputeCRC(&crcParam);
    crcParam.crcState = CRC_COMPUTE;
    (void)SecM_ComputeCRC(&crcParam);
    crcParam.crcState = CRC_FINALIZE;
    (void)SecM_ComputeCRC(&crcParam);
    
    /* program computed CRC value to flash */
    FL_NvmInfo[blockindex].checksum = crcParam.currentCRC;

	/* set input parameter of flash driver interface */
    flashParamInfo.data = (const uint8 *)&FL_NvmInfo[blockindex];
    flashParamInfo.address = (tFlashAddress)&FL_FingerPrint[FL_NvmInfoIndex];
    flashParamInfo.length = sizeof(FL_NvmInfoType);
    
  	/* program blocks information */
    BLFlash_Info.flashWriteFct(&flashParamInfo);
    
    if (kFlashOk == flashParamInfo.errorCode)
    {
        ret = FL_OK;
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <active job erase>
 * 
 * <erase the current logical block witch requested by 0x31 service> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_Erasing(void)
{
    FL_ResultType ret = FL_OK;
    uint8 FL_NvmInfo_Index;
    
    /* check if fingerprint area is full */
    if ((ret = FL_FindFirstEmptyFlashInfo(&FL_NvmInfo_Index)) != FL_OK) {
        /* If fingerprint area is full erase and write back last entries as first entries */
        ret = FL_EmptyNvm();
        FL_NvmInfo_Index = FL_NUM_LOGICAL_BLOCKS;
    }
    /* update the bootloader information to EEPROM */
    ret = FL_UpdateNvm(FL_NvmInfo_Index, downloadStatus.blockIndex); /* update record after last found */
    
    if (FL_OK == ret)
    {
        /* set flash driver input parameter */
        flashParamInfo.address = FL_BlkInfo[downloadStatus.blockIndex].address;
        flashParamInfo.length = FL_BlkInfo[downloadStatus.blockIndex].length;
        				
        /* erase the flash of the requested block*/
        //BLFlash_Info.flashEraseFct(&flashParamInfo);
        BlFlash_Erase(&flashParamInfo);
        /* check if erase success */
        if (kFlashOk == flashParamInfo.errorCode)
        {
            /*set the block erased */
            downloadStatus.blockErased = TRUE;
        }
        else
        {
            ret = FL_FAILED;
        }
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <check segment address>
 * 
 * <check if the transfered address is in current block,and if the address is 
 *  increased by segment> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_CheckDownloadSegment(void)
{
    FL_ResultType ret = FL_OK;
    
    /* 
    ** check if block is erased, if current num of segment is less than maximum,
    ** if address is in current block.
    */
    if (FALSE == downloadStatus.blockErased)
    {
        ret = FL_ERR_SEQUENCE;
    }
    else
    {
        if ((downloadStatus.segmentList.nrOfSegments < FL_MAX_SEGMENTS) &&
            (downloadStatus.startAddr >= 
             FL_BlkInfo[downloadStatus.blockIndex].address) &&
            ((downloadStatus.startAddr + downloadStatus.length) <= 
             (FL_BlkInfo[downloadStatus.blockIndex].address + 
              FL_BlkInfo[downloadStatus.blockIndex].length)))
        {
            FL_SegmentInfoType * curSegment;
            
            /* get current segment num */
            uint8 segmentIndex = downloadStatus.segmentList.nrOfSegments;
            
            /* check if segment address is increase */
            if (segmentIndex > 0x00u)
            {
                uint32 alignSize = FL_GetAlignSize() - 1;
                 
                curSegment = &downloadStatus.segmentList.segmentInfo[segmentIndex - 1];
                
                /* check if download address is in front segment range */
                if ((downloadStatus.startAddr & (~alignSize)) < 
                    (curSegment->address + curSegment->length))
                {
                    ret = FL_ERR_ADDR_LENGTH;
                }
            }
            
            if (FL_OK == ret)
            {
                /* set the flash download info */	
                curSegment = &downloadStatus.segmentList.segmentInfo[segmentIndex];
                downloadStatus.segmentList.nrOfSegments ++;
                curSegment->address = downloadStatus.startAddr;
                curSegment->length = downloadStatus.length;
            }		
        }
        else
        {
            ret = FL_ERR_ADDR_LENGTH;
        }
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <program aligned data>
 * 
 * <when the last 0x36 service request is received, the not aligned data should
 *  be program,and the pad address if filled for default vaule > .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_ProgrammingAlignData(void)
{
    FL_ResultType ret = FL_OK;
    
    /* set the download step*/
    downloadStatus.downloadStep = FL_EXIT_TRANSFER_STEP;
    
    /* check if there is some no aligned data that needed programmed */
    if (alignedProgramSize > 0)
    {
        /* the last 0x36 */
        uint32 alignSize;
        uint8 *programDataPtr = (uint8 *)&programData[0];
			
        flashParamInfo.address = downloadStatus.startAddr - alignedProgramSize;
        
        alignSize = FL_GetAlignSize();
        
        /* fill pad for the left data */
        Appl_Memset(&programDataPtr[alignedProgramSize],
				            (const uint8)FL_GAP_FILL_VALUE,
				            FL_PROGRAM_SIZE - alignedProgramSize);
				            
        /* set the flash download info */		
        flashParamInfo.data = (uint8 *)&programData[0];
        
        /* set the alignedProgramSize to multi of flash phrase */
        if ((alignedProgramSize & (alignSize - 1)) > 0)
        {
            alignedProgramSize &= ~(alignSize - 1);
            alignedProgramSize += alignSize;
        }
        
        flashParamInfo.length = alignedProgramSize;
        alignedProgramSize = 0;
        
        /* write the last 0x36 aligned data */
        BLFlash_Info.flashWriteFct(&flashParamInfo);
        
        if (flashParamInfo.errorCode != kFlashOk)
        {
            ret = FL_FAILED;
        }
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <program data>
 * 
 * <program the aligned data transfered by 0x36 service request > .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_ProgrammingData(void)
{
    FL_ResultType ret = FL_OK;
    uint8 *programDataPtr;
	
    /* check the program length and program status */
    while ((programLength > 0) && (FL_OK == ret))
    {
        /* check if the program size is more than maximum size of program buffer */
        if ((programLength + alignedProgramSize) >= FL_PROGRAM_SIZE)
        {
            /* get the download data */
	     	programDataPtr = (uint8 *)&programData[0];
            Appl_Memcpy(&programDataPtr[alignedProgramSize],
					              downloadStatus.dataBuff,
					              FL_PROGRAM_SIZE - alignedProgramSize);
					              
            /* index the databuff point in transfered buffer */
            downloadStatus.dataBuff += FL_PROGRAM_SIZE - alignedProgramSize;
            
            /* index the total program length */
            programLength -= FL_PROGRAM_SIZE - alignedProgramSize;
            
            /* set the flash driver input parameter */		
            flashParamInfo.address = downloadStatus.startAddr - alignedProgramSize;
            flashParamInfo.length = (uint32)FL_PROGRAM_SIZE;
            flashParamInfo.data = (uint8 *)&programData[0];
            
            /* program the data */
            BLFlash_Info.flashWriteFct(&flashParamInfo);
            
            /* index the start address and length that record in 0x34 service */
            downloadStatus.startAddr += FL_PROGRAM_SIZE - alignedProgramSize;
            downloadStatus.length -= FL_PROGRAM_SIZE - alignedProgramSize; 
            
            alignedProgramSize = 0;
            
            /* check if program success */
            if (flashParamInfo.errorCode != kFlashOk)
            {
                ret = FL_FAILED;
            }
        }
        else
        {
            /* set the last data for write of current service 0x36 */
	     	programDataPtr = (uint8 *)&programData[0];
            Appl_Memcpy(&programDataPtr[alignedProgramSize],
        					      downloadStatus.dataBuff,
        					      (uint32)programLength);
        					      
        		/* index the start address and length that record in 0x34 service */			      
            downloadStatus.startAddr += programLength;
            downloadStatus.length -= programLength; 
            
            /* save the data length that was not programmed */
            alignedProgramSize += programLength;
            
            /* end of current service 0x36 */
            programLength = 0; 
        }
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <active job program>
 * 
 * <program the data transfered by 0x36 service request > .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_Programming(void)
{
    FL_ResultType ret = FL_OK;
    
    /* program buffer aligned data */
    ret = FL_ProgrammingData();
    
    /* check if the last not aligned data should be programmed */
    if ((downloadStatus.length == 0x00uL) && 
        (FL_OK == ret))
    {
        /* program the not aligned data */
        ret = FL_ProgrammingAlignData();
    }
    
    return ret;
}

/******************************************************************************/
/**
 * @brief               <fill pad>
 * 
 * <fill the pad between segments of current block> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
#if(FL_USE_GAP_FILL == TRUE)
STATIC FL_ResultType FL_FillGap(void)
{
    FL_ResultType ret = FL_OK;
    uint8 segmentIndex = 0;
    uint32 startAddress = FL_BlkInfo[downloadStatus.blockIndex].address;
    uint32 gapLength;
    uint32 alignSize = FL_GetAlignSize();
	
    /* set the download datas with FL_GAP_FILL_VALUE */
    Appl_Memset((uint8 *)&programData[0], (uint8)FL_GAP_FILL_VALUE, (uint32)FL_PROGRAM_SIZE); 
    
    flashParamInfo.data = (uint8 *)&programData[0];
    
    while ((segmentIndex <= downloadStatus.segmentList.nrOfSegments) &&
           (FL_OK == ret))
    {
        /* find length ofthe gap in the segment*/
        if (segmentIndex < downloadStatus.segmentList.nrOfSegments)
        {
            gapLength = downloadStatus.segmentList.segmentInfo[segmentIndex].address
					              - startAddress;
        }
        else
        {
            gapLength = (FL_BlkInfo[downloadStatus.blockIndex].address + 
							           FL_BlkInfo[downloadStatus.blockIndex].length) - 
							          startAddress;
        }
        
        gapLength &= ~(alignSize - 1);
        
        /* set the flash download address of gap */
        flashParamInfo.address = startAddress;
        
        while ((gapLength > 0) && (FL_OK == ret))
        {
            if (gapLength >= FL_PROGRAM_SIZE)
            {
                /* set the download length */
                flashParamInfo.length = FL_PROGRAM_SIZE;
                
                /* update the gap length*/
                gapLength -= FL_PROGRAM_SIZE;
            }
            else
            {
                /* the last gap*/
                flashParamInfo.length = gapLength;
                gapLength = 0;
            }
            
            /* write the flash of the FlashParam for gap*/
            BLFlash_Info.flashWriteFct(&flashParamInfo);	
            		
            flashParamInfo.address += flashParamInfo.length;
            
            /* check if program pad success */
            if (flashParamInfo.errorCode != kFlashOk)
            {
                ret = FL_FAILED;
            }
        }
        
        if (segmentIndex < downloadStatus.segmentList.nrOfSegments)
        {
            /* set the next start address */
            startAddress = downloadStatus.segmentList.segmentInfo[segmentIndex].address + 
							             downloadStatus.segmentList.segmentInfo[segmentIndex].length;
							          
            if ((startAddress & (alignSize - 1)) > 0)
            {
                startAddress &= ~(alignSize - 1);
                startAddress += alignSize;
            }
        }
        
        segmentIndex ++;
    }
    
    return ret;
}
#endif

/******************************************************************************/
/**
 * @brief               <active job checksum>
 * 
 * <active checksum that request by 0x31 service> .
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>    
 */
/******************************************************************************/
STATIC FL_ResultType FL_CheckSuming(void)
{
    FL_ResultType ret = FL_OK;
    SecM_StatusType secMStatus;
    SecM_VerifyParamType verifyParam;
    uint8 FL_NvmInfo_Index = 0;
    
    /* set verification API input parameter */
    verifyParam.segmentList = &downloadStatus.segmentList;
    verifyParam.verificationData = downloadStatus.dataBuff;
    
    /* CRC compute and verification */
    secMStatus = SecM_Verification(&verifyParam);
    
    /* set block not erased */
    downloadStatus.blockErased = FALSE;
    
    /* check if CRC is correct */
    if (SECM_OK == secMStatus)
    {
        /* check if flash driver is downloaded */
        if (FALSE == downloadStatus.flDrvDownloaded)
        {
        	/* copy flashcodes to ram in order to overlap the T enginnering's flash algorithm */
			/* copy the data to the request address*/
			uint32 *pRodata = (uint32 *)F_FLASHHEADER;
			uint32 *pRoCode = (uint32 *)F_FLASHTEXT;
			
			Appl_Memcpy((uint8 *)pRodata, (const uint8 *)F_FLASHHEADER_ROM, (uint32)F_FLASHHEADER_VOLUME);
			Appl_Memcpy((uint8 *)pRoCode, (const uint8 *)F_FLASHTEXT_ROM, (uint32)F_FLASHTEXT_VOLUME);
			
            /* flash driver initialize */
            BLFlash_Info.flashInitFct(&flashParamInfo);
            
            /* check if flash driver is initialized success */
            if (flashParamInfo.errorCode != kFlashOk)
            {
                ret = FL_FAILED;
            }
            else
            {
                downloadStatus.flDrvDownloaded = TRUE;
            }
        }
        else
        {

            /* set current block is valid */
        	swBlockInfo[downloadStatus.blockIndex].blkValid = TRUE;
            
            /* set current program attempt counter to match with Nvm info for current block */
        	swBlockInfo[downloadStatus.blockIndex].blkProgAttempt = FL_NvmInfo[downloadStatus.blockIndex].programming_attempts;

        	/* save computed CRC to NVM if CRC success */
        	swBlockInfo[downloadStatus.blockIndex].blkChecksum = verifyParam.crcTotle;

        	/* fill the gap if configured */
            #if(FL_USE_GAP_FILL == TRUE)
            ret = FL_FillGap();
            #endif
            
        }
    }
    else
    {
        ret = FL_FAILED;
    }
    
    /* reset download step */
    downloadStatus.downloadStep = FL_REQUEST_STEP;
    
    return ret;
}


/******************************************************************************/
/**
 * @brief               <write block headers>
 *
 * <writes the block header information to flash>.
 * @param[in]           <NONE>
 * @param[out]          <NONE>
 * @param[in/out]       <NONE>
 * @return              <FL_ResultType>
 */
/******************************************************************************/
FL_ResultType FL_WriteBlockHeaders(void)
{
   uint8 i;

   if (downloadStatus.flDrvDownloaded == TRUE) {
      for (i=0; i<FL_NUM_LOGICAL_BLOCKS; i++) {
	      if (swBlockInfo[i].blkValid == TRUE) {
            /* only program updated block */
            flashParamInfo.address = FL_BlkInfo[i].header;
            flashParamInfo.length = sizeof(FL_BlockHeader);
            flashParamInfo.data = (uint8*)&swBlockInfo[i];

            /* update the block header in flash */
            BLFlash_Info.flashWriteFct(&flashParamInfo);
            
         }
      }
   }
   return FL_OK;
}

FL_ResultType FL_MergeAppAndBootloader(void)
{
	uint8 blockIndex,i;
	FL_ResultType ret = FL_OK;
	uint8 readData[24];
	uint32 readData_BootCode,readData_ApplicationMarkCode,readData_calibarationMarkCode;
	
	uint8* memoryPtr	= (uint8*)FL_NVM_INFO_ADDRESS;
	/* Read Boot Software Identifier from dedicated memory */
	Appl_Memcpy(readData, (const uint8*)memoryPtr, 24);
	
	memoryPtr	= (uint8*)FL_APP_RCHW_ADDR;
	Appl_Memcpy((uint8*)&readData_BootCode, (const uint8*)memoryPtr, 4);
	
	/* read app and calibration mark code*/
	memoryPtr	= (uint8*)(uint32*)0x001FFFF8;
	Appl_Memcpy((uint8*)&readData_ApplicationMarkCode, (const uint8*)memoryPtr, 4);
	
	memoryPtr	= (uint8*)(uint32*)0x001FFFF0;
	Appl_Memcpy((uint8*)&readData_calibarationMarkCode, (const uint8*)memoryPtr, 4);
		
	if ((readData[0] >= 0) && (readData[0] < FL_NUM_LOGICAL_BLOCKS))
	{
		return FL_OK;
	}
	else
	{
		if (readData[0] == 0xff)
		{
			if(readData_BootCode != 0x015A0000u)
			{
				return FL_OK;
			}
			else
			{
				if( (readData_ApplicationMarkCode == 0xFFFFFFFFu)&&(readData_calibarationMarkCode == 0xFFFFFFFFu) )
				{
					;
				}
				else
				{
					return FL_OK;
				}
			}
		}
		else
		{
			return FL_OK;
		}
	}
		
	uint32 *pRodata = (uint32 *)F_FLASHHEADER;
	uint32 *pRoCode = (uint32 *)F_FLASHTEXT;
	
	Appl_Memcpy((uint8 *)pRodata, (const uint8 *)F_FLASHHEADER_ROM, (uint32)F_FLASHHEADER_VOLUME);
	Appl_Memcpy((uint8 *)pRoCode, (const uint8 *)F_FLASHTEXT_ROM, (uint32)F_FLASHTEXT_VOLUME);
	
    /* flash driver initialize */
    BLFlash_Info.flashInitFct(&flashParamInfo);
    
    
	for(blockIndex=0u; (blockIndex<FL_NUM_LOGICAL_BLOCKS)&&(ret==FL_OK); blockIndex++)
	{
		 /* set current block is valid */
		FL_NvmInfo[blockIndex].areaType = blockIndex;
		for(i=0; i<FL_FINGER_PRINT_LENGTH-1; i++)
		{
			FL_NvmInfo[blockIndex].fingerPrint[i] = 0x31;//1111111111
		}
		
		FL_NvmInfo[blockIndex].fingerPrint[FL_FINGER_PRINT_LENGTH-1] = 0x00u;
		
		/* increment program attempt counter of current block */
		FL_NvmInfo[blockIndex].programming_attempts = 1u;
		
		ret = FL_UpdateNvm(blockIndex, blockIndex); 
	}
	
	
	for (i=0; (i<FL_NUM_LOGICAL_BLOCKS)&&(ret==FL_OK); i++) 
	{
        /* set current block is valid */
    	swBlockInfo[i].blkValid = TRUE;
        
        /* set current program attempt counter to match with Nvm info for current block */
    	swBlockInfo[i].blkProgAttempt = FL_NvmInfo[i].programming_attempts;

    	/* save computed CRC to NVM if CRC success */
    	swBlockInfo[i].blkChecksum = 0xFFFFFFFFu;
    	
		/* only program updated block */
		flashParamInfo.address = FL_BlkInfo[i].header;
		flashParamInfo.length = sizeof(FL_BlockHeader);
		flashParamInfo.data = (uint8*)&swBlockInfo[i];

		/* update the block header in flash */
		BLFlash_Info.flashWriteFct(&flashParamInfo);
			
	}
	 
	return FL_OK;
}
