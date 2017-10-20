/*=======[I N C L U D E S]====================================================*/

#include "FlashLoader.h"

/*=======[E X T E R N A L   D A T A]==========================================*/

/* definition of available blocks
 * Block address (_application_start, _calibaration_start) are retrieved from the linker file
 * Symbols _application_header_start and _calibaration_header_start are definitions of where
 * the block area headers are positioned.
 * */
const FL_BlockDescriptorType FL_BlkInfo[FL_NUM_LOGICAL_BLOCKS] =
{
    /* start global address,       header address,                      length,     maximum attempt */
    {(uint32)&_application_start,  (uint32)&_application_header_start,  0x1E4000uL, 0x00u},
    {(uint32)&_calibaration_start, (uint32)&_calibaration_header_start, 0x1000000uL,0x00u}
};

/* definition of the finger print area in FLASH.
 * _fingerprint_flash_start is a symbol that is created in the lcf linker file. */
const FL_NvmInfoType* FL_FingerPrint = (FL_NvmInfoType*)&_fingerprint_flash_start;
