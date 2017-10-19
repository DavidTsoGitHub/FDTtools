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

typedef enum
{
    STD_DATATYPE_UINT8,
    STD_DATATYPE_UINT16,
    STD_DATATYPE_UINT32,
    STD_DATATYPE_INT8,
    STD_DATATYPE_INT16,
    STD_DATATYPE_INT32,
    STD_DATATYPE_DYNAMICWIDGETVALUE
} DynamicValueType;

typedef union 
{
    uint8 U8;
    uint16 U16;
    uint32 U32;
    sint8 S8;
    sint16 S16;
    sint32 S32;
} DynamicValue;

typedef struct
{
    DynamicValueType DataType;
    DynamicValue Value;
    boolean Flashing;
    boolean Valid;
} DynamicWidgetValue;

typedef struct
{
    uint32 LabelID;
    uint8 Value;
} LabelMapping;

typedef struct
{
    uint8 Count;
    LabelMapping Mappings[];
} LabelMappings;

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

//Standard function return statuses

#define STD_OK 0
#define STD_NOK 1


#endif // STANDARDTYPES_H
