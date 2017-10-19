#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    POWER_LOW,
    POWER_IGNITION_OFF,
    POWER_IGNITION_ON,
    // POWER_INVALID should always be last in the enum
    // to enable comparisons like "if (powerMode >= POWER_INVALID) { // handle unknown mode... }
    POWER_INVALID
} PowerMode;

typedef struct
{
    union 
    {
        uint8 U8;
        uint16 U16;
        uint32 U32;
        sint8 S8;
        sint16 S16;
        sint32 S32;
    } Value;
    boolean Flashing;
    boolean Valid;
} DynamicWidgetValue;

typedef struct
{
    uint16 year;
    uint8  month;
    uint8  day;
    uint8  hour;
    uint8  minute;
    uint8  second;
} dateTime_t;

/* Diagnostic interface defintions */
typedef enum
{
    DTC_ACTIVE,
    DTC_INACTIVE
} DTCStatus;

typedef struct
{
     uint32 u32DTCNumber;
     DTCStatus Status;
} tDiagReport;

#endif // STANDARDTYPES_H
