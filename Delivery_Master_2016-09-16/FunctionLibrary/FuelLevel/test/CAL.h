#ifndef CAL_H
#define CAL_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    CAL_OK,
    CAL_NOK
} CAL_STATUS;

typedef enum
{
    FUEL_WARNINGTHRESHOLD,
    FUEL_WARNINGTOLERANCE
} calibration_parameter_id_t;

typedef struct
{
    uint16 Offset;
    uint8 Length;
} calibration_parameter_t;

/***************************** Exported Functions ****************************/
void CAL_Init(void);

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value);

void CALStub_SetThreshold(uint8 value);
void CALStub_SetTolerance(uint8 value);

#endif // CAL_H
