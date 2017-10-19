#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#define CPU_TYPE_8       8
#define CPU_TYPE_16      16
#define CPU_TYPE_32      32

#define MSB_FIRST        0
#define LSB_FIRST        1

#define HIGH_BYTE_FIRST  0
#define LOW_BYTE_FIRST   1

#define CPU_TYPE         CPU_TYPE_32
#define CPU_BIT_ORDER    LSB_FIRST
#define CPU_BYTE_ORDER   LOW_BYTE_FIRST

#ifndef TRUE
#define TRUE ((boolean)1)
#endif

#ifndef FALSE
#define FALSE ((boolean)0)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned char         boolean;       /*        TRUE..FALSE          */
typedef signed char           sint8;         /*        -128..127            */
typedef unsigned char         uint8;         /*           0..255            */
typedef signed short          sint16;        /*      -32768..32767          */
typedef unsigned short        uint16;        /*           0..65535          */
typedef signed int           sint32;        /* -2147483648..2147483647     */
typedef unsigned int         uint32;        /*           0..4294967295     */

typedef signed char           sint8_least;
typedef unsigned char         uint8_least;
typedef signed short          sint16_least;
typedef unsigned short        uint16_least;
typedef signed int           sint32_least;
typedef unsigned int         uint32_least;

typedef float                 float32;
typedef double                float64;

typedef unsigned short        wchar;

#define SINT8_MIN__ (-128)
#define SINT8_MAX__ (127)
#define UINT8_MIN__ (0)
#define UINT8_MAX__ (255)

#define SINT16_MIN__ (-32768)
#define SINT16_MAX__ (32767)
#define UINT16_MIN__ (0)
#define UINT16_MAX__ (65535)

#define SINT32_MIN__ (-2147483648)
#define SINT32_MAX__ (2147483647)
#define UINT32_MIN__ (0)
#define UINT32_MAX__ (4294967295)

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
    STD_DATATYPE_INT32
} DynamicLabelValueType;

typedef union 
    {
        uint8 U8;
        uint16 U16;
        uint32 U32;
        sint8 S8;
        sint16 S16;
        sint32 S32;
    } value_t;

typedef struct
{
    value_t Value;
    DynamicLabelValueType DataType;
    boolean Flashing;
    boolean Valid;
} DynamicLabelValue;

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

#endif // STANDARDTYPES_H
