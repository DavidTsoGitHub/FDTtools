#ifndef CAL_H
#define CAL_H

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"


/*=======[E X T E R N A L   D A T A]==========================================*/
extern const uint32 Cal_Crc32Tab[256];

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
void Cal_Crc32Init(uint32 *curCrc);

void Cal_ComputeCrc32(uint32 *curCrc, const uint8 *buf, const uint32 size);

void Cal_CrcFinalize(uint32 *curCrc);
    
#endif/* endof CAL_H */

