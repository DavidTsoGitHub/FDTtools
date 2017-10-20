#ifndef SECM_H
#define SECM_H

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"
#include "FlashLoader.h"

/*=======[M A C R O S]========================================================*/

/** CRC buffer length */
#define SECM_CRC_BUFFER_LEN 100

#define SECM_OK 0x00u
#define SECM_NOT_OK 0x01u

/** CRC step */
#define CRC_INIT	0x00u
#define CRC_COMPUTE	0x01u
#define CRC_FINALIZE	0x02u

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** return type for SecM module */
typedef uint8 SecM_StatusType;

/** type for Seed */
typedef uint32 SecM_WordType;

/** struct type for Seed */
typedef struct
{
    SecM_WordType seedX;
    
    SecM_WordType seedY;
    
} SecM_SeedType;

/** type for Key */
typedef SecM_WordType SecM_KeyType;


/** type for Crc value */
typedef uint32 SecM_CRCType;

/** struct type for Crc */
typedef struct
{
    /* current CRC vaule */
    SecM_CRCType currentCRC;
    
    /* CRC step */
    uint8 crcState;
    
    /* CRC buffer point */
    const uint8 *crcSourceBuffer;
    
    /* CRC length */
    uint16 crcByteCount;
    
} SecM_CRCParamType;


/** struct type for verify parameter list */
typedef struct
{
    /* segment list for block */
    FL_SegmentListType *segmentList;
    
    /* Crc value transfered by UDS */
    const uint8 *verificationData;
    
    /* Crc value totle */
    SecM_CRCType crcTotle;
    
} SecM_VerifyParamType;


/*=======[E X T E R N A L   D A T A]==========================================*/
/** CRC buffer */
extern uint8 SecM_CrcDataBuffer[SECM_CRC_BUFFER_LEN];


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
SecM_StatusType SecM_GenerateSeed(SecM_SeedType *seed);

SecM_StatusType SecM_ComputeKey(SecM_SeedType seed, 
                                SecM_WordType k, 
                                SecM_KeyType *key);

SecM_StatusType SecM_CompareKey(SecM_KeyType key, 
                                SecM_SeedType seed);

SecM_StatusType SecM_ComputeCRC(SecM_CRCParamType *crcParam);

SecM_StatusType SecM_Verification(SecM_VerifyParamType *verifyParam);

uint32 SeedToKey (uint16 num_rounds, uint32 seed) ;

#endif/* endof SECM_H */

