#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

/*****************************************************************************
**                      Include Section                                     **
*****************************************************************************/

#include "Std_Types.h"                  /* standard AUTOSAR types           */

/*****************************************************************************
**                      Global Symbols                                      **
*****************************************************************************/

/*
  COMTYPE002 File version information
*/
#define COMSTACKTYPE_AR_MAJOR_VERSION 2
#define COMSTACKTYPE_AR_MINOR_VERSION 0
#define COMSTACKTYPE_AR_PATCH_VERSION 0

/* COMTYPE018 General return codes for NotifResultType */
#define NTFRSLT_OK           0x00 
#define NTFRSLT_E_NOT_OK     0x01 
#define NTFRSLT_E_TIMEOUT_A  0x02 
#define NTFRSLT_E_TIMEOUT_Bs 0x03 
#define NTFRSLT_E_TIMEOUT_Cr 0x04
#define NTFRSLT_E_WRONG_SN   0x05
#define NTFRSLT_E_INVALID_FS 0x06
#define NTFRSLT_E_UNEXP_PDU  0x07
#define NTFRSLT_E_WFT_OVRN   0x08
#define NTFRSLT_E_NO_BUFFER  0x09

/* COMTYPE021 General return codes for BusTrcvErrorType */
#define BUSTRCV_NO_ERROR     0x00
#define BUSTRCV_E_ERROR      0x01


/*****************************************************************************
**                      Global Data Types (ECU dependent)                   **
*****************************************************************************/

/* Chapter 8.1.1.1 */
typedef uint8        PduIdType;     /* Type of PDU ID.
                                    Allowed ranges: uint8 .. uint16 */

/* Chapter 8.1.1.2 */
typedef uint16       PduLengthType; /* Type of PDU Length.
                                    Allowed ranges: uint8 .. uint32 */

/* Chapter 8.1.1.5 */
typedef uint8        NotifResultType; 

/* Chapter 8.1.1.6 */
typedef uint8        BusTrcvErrorType;

typedef uint16       NetworkHandleType;

/*****************************************************************************
**                      Global Data Types (ECU independent)                 **
*****************************************************************************/

/* Chapter 8.1.1.3 */
typedef struct                        
{
         uint8          *SduDataPtr;
         PduLengthType  SduLength;
} PduInfoType;                        /* Basic information about a PDU of any 
                                      type*/
/* Chapter 8.1.1.4 */
typedef enum                          
{
         BUFREQ_OK,
         BUFREQ_E_NOT_OK,
         BUFREQ_E_BUSY,
         BUFREQ_E_OVFL
} BufReq_ReturnType;                 /* result of a buffer request          */


/*****************************************************************************
**                      Global Data                                         **
*****************************************************************************/
/* intentionally left open */

/*****************************************************************************
**                      Global Functions                                    **
*****************************************************************************/
/* intentionally left open */

#endif /* COMSTACK_TYPES_H */
