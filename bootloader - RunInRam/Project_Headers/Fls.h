/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 * Contact: <contact@arccore.com>
 *
 * You may ONLY use this file:
 * 1)if you have a valid commercial ArcCore license and then in accordance with
 * the terms contained in the written license agreement between you and ArcCore,
 * or alternatively
 * 2)if you follow the terms found in GNU General Public License version 2 as
 * published by the Free Software Foundation and appearing in the file
 * LICENSE.GPL included in the packaging of this file or here
 * <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>
 *-------------------------------- Arctic Core -----------------------------*/

/** @reqSettings DEFAULT_SPECIFICATION_REVISION=4.1.2 */
/** @tagSettings DEFAULT_ARCHITECTURE=PPC|TMS570|MPC5607B|MPC5645S|ZYNQ */

#ifndef FLS_H_
#define FLS_H_

/** @req SWS_Fls_00248 */
#include "Std_Types.h"

typedef void (*flashCbType)( void );

#define FLS_VENDOR_ID                       60u
#define FLS_MODULE_ID                       92u
#define FLS_SW_MAJOR_VERSION                2u
#define FLS_SW_MINOR_VERSION                0u
#define FLS_SW_PATCH_VERSION                0u
#define FLS_AR_RELEASE_MAJOR_VERSION        4u
#define FLS_AR_RELEASE_MINOR_VERSION        1u
#define FLS_AR_RELEASE_PATCH_VERSION        2u

// Development errors
/** @req SWS_Fls_00004 */
#define FLS_E_PARAM_CONFIG			0x01u
#define FLS_E_PARAM_ADDRESS 		0x02u
#define FLS_E_PARAM_LENGTH 			0x03u
#define FLS_E_PARAM_DATA 			0x04u
#define FLS_E_UNINIT				0x05u
#define FLS_E_BUSY 					0x06u
#define FLS_E_VERIFY_ERASE_FAILED	0x07u
#define FLS_E_VERIFY_WRITE_FAILED	0x08u
#define FLS_E_TIMEOUT               0x09u
#define FLS_E_PARAM_POINTER         0x0Au

/* ArcCore extra errors */
#define FLS_E_INVALID_AREA          0x10u
#define FLS_E_UNEXPECTED_EXECUTION  0x11u

// Service id's for fls functions
#define FLS_INIT_ID					0x00
#define FLS_ERASE_ID				0x01
#define FLS_WRITE_ID				0x02
#define FLS_CANCEL_ID				0x03
#define FLS_GET_STATUS_ID			0x04
#define FLS_GET_JOB_RESULT_ID		0x05
#define FLS_MAIN_FUNCTION_ID		0x06
#define FLS_READ_ID					0x07
#define FLS_COMPARE_ID				0x08
#define FLS_SET_MODE_ID				0x09
#define FLS_GET_VERSION_INFO_ID		0x10

// Used as address offset from the configured flash base address to access a certain
// flash memory area.
/** @req SWS_Fls_00369 */
/** @req SWS_Fls_00216 */
typedef uint32 Fls_AddressType;

// Specifies the number of bytes to read/write/erase/compare
//
// Note!
// Shall be the same type as Fls_AddressType because of
// arithmetic operations. Size depends on target platform and
// flash device.
/** @req SWS_Fls_00370 */
typedef uint32 Fls_LengthType;

/** @req SWS_Fls_00309 */
#if !defined(FLS_INCLUDE_FILE)
#include "Fls_Cfg.h"

/**
 * Initializes the Flash Driver.
 *
 * @param ConfigPtr Pointer to flash driver configuration set.
 */
/** @req SWS_Fls_00249 */
void Fls_Init(const Fls_ConfigType *ConfigPtr);

/**
 * Erases flash sector(s).
 *
 * @param TargetAddress Target address in flash memory.
 *                      This address offset will be
 *						added to the flash memory base address.
 *						Min.: 0
 *                      Max.: FLS_SIZE - 1
 *
 * @param Length 		Number of bytes to erase
 *                      Min.: 1
 *                      Max.: FLS_SIZE - TargetAddress
 *
 * @param cb	 		Callback function for erase during waiting for erase to finish
 *						Usually for a watchdog timer handler
 *
 * @return E_OK: erase command has been accepted
 *         E_NOT_OK: erase command has not been accepted
 */

/** @req SWS_Fls_00250 */
Std_ReturnType Fls_Erase(Fls_AddressType TargetAddress, Fls_LengthType Length, flashCbType cb);

/** @req SWS_Fls_00251 */
Std_ReturnType Fls_Write(Fls_AddressType TargetAddress,
        const uint8 *SourceAddressPtr, Fls_LengthType Length);

#if ( FLS_CANCEL_API == STD_ON )
/** @req SWS_Fls_00252 */
void Fls_Cancel( void );
#endif

#if ( FLS_GET_STATUS_API == STD_ON )
/** @req SWS_Fls_00253 */
MemIf_StatusType Fls_GetStatus(void);
#endif

#if ( FLS_GET_JOB_RESULT_API == STD_ON )
/** @req SWS_Fls_00254 */
MemIf_JobResultType Fls_GetJobResult(void);
#endif

/** @req SWS_Fls_00255 */
/** @req SWS_Fls_00269 */
void Fls_MainFunction(void);

/** @req SWS_Fls_00256 */
Std_ReturnType Fls_Read(Fls_AddressType SourceAddress, uint8 *TargetAddressPtr,
        Fls_LengthType Length);

#if ( FLS_COMPARE_API == STD_ON )
/** @req SWS_Fls_00257 */
Std_ReturnType Fls_Compare(Fls_AddressType SourceAddress,
        const uint8 *TargetAddressPtr, Fls_LengthType Length);
#endif

#if ( FLS_SET_MODE_API == STD_ON )
/** @req SWS_Fls_00258 */
/** @req SWS_Fls_00187 */
void Fls_SetMode(MemIf_ModeType Mode);
#endif

#if ( FLS_VERSION_INFO_API == STD_ON )
/** @req SWS_Fls_00259 */
void Fls_GetVersionInfo(Std_VersionInfoType *VersioninfoPtr);
#endif

#if defined(CFG_ZYNQ)
typedef enum {
    FLS_JOB_NONE, FLS_JOB_COMPARE, FLS_JOB_ERASE, FLS_JOB_READ, FLS_JOB_WRITE,
}Fls_Arc_JobType;
/**
 * Gets values of internal variables for debugging
 * @param modstat
 * @param moduleMode
 * @param jobResult
 * @param jobPending
 * @param jobAddr
 * @param jobLen
 * @param jobPointer
 */
void Fls_Arc_Internal(MemIf_StatusType *modstat, MemIf_ModeType *moduleMode,
        MemIf_JobResultType *jobResult,Fls_Arc_JobType *jobPending,
        Fls_AddressType *jobAddr,Fls_LengthType *jobLen, const uint8** jobPointer);
#endif
#else
#include FLS_INCLUDE_FILE
#endif

#endif /*FLS_H_*/
