#ifndef FLASHLOADER_H_
#define FLASHLOADER_H_

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"
#include "FlashLoader_Cfg.h"

/*=======[M A C R O S]========================================================*/
/* FL module execute result */
#define FL_OK               0x00u
#define FL_FAILED           0x01u
#define FL_ERR_SEQUENCE     0x02u
#define FL_NO_FINGERPRINT   0x03u
#define FL_NO_FLASHDRIVER   0x04u
#define FL_ERR_ADDR_LENGTH  0x05u
#define FL_INVALID_DATA     0x06u
#define FL_IN_DEBUG         0x07u

/** Value indicate an external programming request. */
#define FL_EXT_PROG_REQUEST_RECEIVED    0x2Au

/** Value indicate an update of the application software. */
#define FL_APPL_UPDATED                 0xD5u

/** Value indicate that the application software should respond to the ECU reset
 * that was performed by the boot after successful download. */
#define FL_APPL_RESPONSE_TO_RESET 		    0x55005500u
#define FL_APPL_RESPONSE_TO_RESET_INV 	    0xAAFFAAFFu

/** Value indicate that the application software should respond to the default session
 * request that was performed by the boot during download. */
#define FL_EXT_DEFAULT_SESSION_RECEIVED       0x55001001u
#define FL_EXT_DEFAULT_SESSION_RECEIVED_INV   0xAAFFEFFEu

/** A magic value that is used in the valid variable of the application block header
 * The magic word is used when boot detect an ECC problem in flash during boot.
 * The result is that the boot saves this word and performs a reset, When boot return after
 * reset it will read this value and not attempt to read from the application area */
#define FL_APPL_FORCE_STAY_IN_BOOT            0xEAu

/** A magic value that is used in the valid variable of the application block header
 * The magic word will make the boot jump immediately to the application.
 * This is used when the application is debugged using debugger i.e. the application has been
 * downloaded by the debugger. */
#define FL_APPL_PROG_BY_DEBUGGER_MAGIC       0xBABEu


/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** Standard return type for callback routines. */
typedef uint8 FL_ResultType;

/* 
** needed in the interface between flashloader runtime environment and security
** module. 
*/
typedef volatile struct
{
    /* block start global address */
    const uint32 address;

    /* block header start */
    const uint32 header;

    /* block length */
    const uint32 length;
    
    /* maximum program cycle */
    const uint8 maxProgAttempt;
    
} FL_BlockDescriptorType;

/** Segment list information of the block */
typedef struct
{
    /* Start global address of the segment in flash */
    uint32 address;
    
    /* Length of the segment */
    uint32 length;
    
} FL_SegmentInfoType;

/** Needed in interface to the security module. */
typedef struct
{
    /* number of segment */
    uint8 nrOfSegments;

    /* segments information */
    FL_SegmentInfoType segmentInfo[FL_MAX_SEGMENTS];
    
} FL_SegmentListType;

/** Logical block information for each block programmed to FLASH. */
typedef struct
{
    uint16 blkValid;
    
    uint16 blkProgAttempt;

    uint32 blkChecksum;
     
} FL_BlockHeader;

/** Finger print information saved in EEPROM. */
typedef struct
{
	uint8 areaType; /* application or calibration */

	uint8 fingerPrint[FL_FINGER_PRINT_LENGTH];
    
	uint16 programming_attempts;

	uint32 checksum; /* the checksum of the fingerprint item */

} FL_NvmInfoType;

/*=======[E X T E R N A L   D A T A]==========================================*/
/* information that shall be programmed to EEPROM */
extern FL_NvmInfoType FL_NvmInfo[FL_NUM_LOGICAL_BLOCKS];

/* logical block information that configured by user */
extern const FL_BlockDescriptorType FL_BlkInfo[FL_NUM_LOGICAL_BLOCKS];

/* global pointer to finger print area in FLASH/EEPROM */
extern const FL_NvmInfoType* FL_FingerPrint;

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

void FL_InitState(void);

uint16 FL_ReadDidF15B(uint8 *readData);
uint16 FL_ReadDidF180(uint8 *readData);
uint16 FL_ReadDidF18B(uint8 *readData);
uint16 FL_ReadDidF18C(uint8 *readData);
uint16 FL_ReadDidF192(uint8 *readData);
uint16 FL_ReadDidF1FA(uint8 *readData);

uint32 FL_ReadMemory(uint32 address, 
                     uint32 length,
                     uint8* data);
                     
uint8 FL_CheckProgPreCondition(uint8 * conditions);

FL_ResultType FL_WriteFingerPrint(uint8 *data,
                                  const uint16 length);
                                  
FL_ResultType FL_CheckSumRoutine(const uint8 *checkSum);
											           
FL_ResultType FL_EraseRoutine(const uint8 blockIndex);

FL_ResultType FL_DownloadRequestValid(const uint32 startAdd, 
									  const uint32 length);
													             
FL_ResultType FL_FlashProgramRegion(const uint32 destAddr,
									const uint8 *sourceBuff,
									const uint32 length);
										                 
FL_ResultType FL_ExitTransferData(void);

boolean FL_ServiceFinished(FL_ResultType *error);

void FL_MainFunction(void);

FL_ResultType  FL_FindFirstEmptyFlashInfo(uint8 *FL_NvmInfo_Index);
FL_ResultType  FL_FindLastBlockFlashInfo(uint8 *FL_NvmInfo_Index);
FL_ResultType  FL_DriverDownloaded(void);
FL_ResultType FL_WriteBlockHeaders(void);
FL_ResultType FL_MergeAppAndBootloader(void);

extern const FL_NvmInfoType FLASH_INFO;

#endif/* endof FL_H */


