﻿/*
 * platform types for PC-compatible architectures (x86)
 */
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

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

#endif  /* PLATFORM_TYPES_H */
