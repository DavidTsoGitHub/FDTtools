#ifndef DCM_H
#define DCM_H

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "CanTp.h"

/*=======[M A C R O S]========================================================*/

/** Negtive response code */
typedef uint8 Dcm_NrcType;
#define DCM_E_GENERAL_REJECT                                0x10u
#define DCM_E_SERVICE_NOT_SUPPORTED                         0x11u
#define DCM_E_SUBFUNC_NOT_SUPPORTED                         0x12u
#define DCM_E_INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT    0x13u
#define DCM_E_BUSY_REPEAT_REQUEST                           0x21u
#define DCM_E_CONDITION_NOT_CORRECT                         0x22u
#define DCM_E_REQUEST_SEQUENCE_ERROR                        0x24u
#define DCM_E_REQUEST_OUT_OF_RANGE                          0x31u
#define DCM_E_SECURITY_ACCESS_DENIED                        0x33u
#define DCM_E_INVALID_KEY                                   0x35u
#define DCM_E_EXCEEDED_NUMBER_OF_ATTEMPTS                   0x36u
#define DCM_E_REQUIRED_TIME_DELAY_NOT_EXPIRED               0x37u
#define DCM_E_UPLOAD_DOWNLOAD_NOT_ACCEPTED                  0x70u
#define DCM_E_TRANSFER_DATA_SUSPENDED                       0x71u
#define DCM_E_GENERAL_PROGRAMMING_FAILURE                   0x72u
#define DCM_E_WRONG_BLOCK_SEQUENCE_COUNTER                  0x73u
#define DCM_E_PENDING                                       0x78u
#define DCM_E_SUBFUNC_NOT_SUPPORTED_INACTIVE_SESSION        0x7Eu
#define DCM_E_SERVICE_NOT_SUPPORTED_INACTIVE_SESSION        0x7Fu
#define DCM_E_VOLTAGE_TOO_HIGH                              0x92u
#define DCM_E_VOLTAGE_TOO_LOW                               0x93u

/** SID negative response */
#define DCM_RSP_SID_NRC 0x7Fu

/** if response needed */
#define DCM_RSP_REQUIRED        0x00u
#define DCM_RSP_NOT_REQUIRED    0x80u
#define DCM_RSP_CLEAR_REQUIRED  0x7Fu

/** service 10 diagnosticSessionType */
typedef uint8 Dcm_SessionType;
#define DCM_SESSION_DEFAULT     0x01u
#define DCM_SESSION_PROGRAMMING 0x02u
#define DCM_SESSION_EXTEND      0x03u

/** service 11 resetType */
typedef uint8 Dcm_ResetType;
#define DCM_HARD_RESET  0x01u

/** service 31 routine control type */
typedef uint8 Dcm_RoutineControlType;
#define DCM_START_ROUTINE   0x01u

/** service 28 communication control */
typedef uint8 Dcm_ComControlType;
#define DCM_ENABLE_RXANDTX      0x00u
#define DCM_ENABLE_RXANDDISTX   0x01u

typedef uint8 Dcm_CommunicationType;
#define DCM_NORMAL_COM_MESSAGES 0x01u

/** service 85 DTC setting type */
typedef uint8 Dcm_DTCSettingType;
#define DCM_DTC_SETTING_ON  0x01u
#define DCM_DTC_SETTING_OFF 0x02u

/** service 27 security access lock type */
typedef uint8 Dcm_SecurityType;
#define DCM_SECURITY_LOCKED     0x00u
#define DCM_SECURITY_LEV1       0x01u
#define DCM_SECURITY_LEV2       0x02u

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
/** Dcm Buffer status*/
typedef enum
{
    DCM_BUFF_FREE,
     
    DCM_BUFF_FOR_TP,
	
    DCM_BUFF_FOR_SERVICE
	
} Dcm_BuffStatusType;

/** struct of DCM Buffer type*/
typedef struct
{
    /* status of this buffer */
    Dcm_BuffStatusType buffStatus;

    /* PduId of this buffer */
    PduIdType pduId;

    /* Pdu Data of this buffer */
    PduInfoType pduInfo;
    
} Dcm_BuffType;


/*=======[E X T E R N A L   D A T A]==========================================*/


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

void Dcm_Init( void );

void Dcm_ProgramInit(void);

void Dcm_MainFunction( void );

CanTp_BufReq_RetType Dcm_ProvideRxBuffer(PduIdType pduId,
                                         PduLengthType pduLen,
										 PduInfoType **sduInfo );

void Dcm_RxIndication(PduIdType pduId,
                      NotifResultType result);

CanTp_BufReq_RetType Dcm_ProvideTxBuffer(PduIdType pduId,
                                         PduInfoType **sduInfo);

void Dcm_TxConfirmation(PduIdType pduId,
                        NotifResultType result);

Dcm_SessionType Dcm_GetSessionMode(void);

Dcm_SecurityType Dcm_GetSecurityLevel(void);

void Dcm_ResetResponse(void);
void Dcm_DefaultSessionResponse(void);
void Dcm_SendRsp(void);

#endif/* endof DCM_H */



