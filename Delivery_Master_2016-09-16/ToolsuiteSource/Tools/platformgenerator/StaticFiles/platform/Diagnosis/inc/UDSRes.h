/*****************************************************************/
/*                                                               */
/* FILE        : UDSRes.h                                        */
/* DATE        : 2015-11-30                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : macros defination, global variables and         */
/*               function prototypes of UDSRes module            */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#ifndef UDS_RES_H
#define UDS_RES_H

/* address mode */
#define PHYTHICAL_ADDRESSING       0u
#define FUNCTIONAL_ADDRESSING      1u

/* safty mode */
#define DEFAULT_SECURITY_LEVEL     0u
#define DIAGNOSIS_SECURITY_LEVEL   1u

#define DEFAULT_SESSION            0x01u
#define PROGRAM_SESSION            0x02u
#define EXTEND_SESSION             0x03u

/* negative response code */
#define     PR       0x00u     /* positive response, NOTE: this response code shall NOT be used for negative response */
#define    SNS       0x11u     /* service not supported */
#define   SFNS       0x12u     /* sub-functin not supported */
#define IMLOIF       0x13u     /* incorrect Message Length Or Invalid Format */
#define    CNC       0x22u     /* Condition not correct */
#define    RSE       0x24u     /* Request Sequence Error */
#define   ROOR       0x31u     /* Request out of range */
#define    SAD       0x33u     /* security Access Denied */
#define     IK       0x35u     /* invalid Key */
#define   ENOA       0x36u     /* excedding the maximum number of access attempts */
#define  RTDNE       0x37u     /* request time delay not expired */
#define    TDS       0x71u     /* transfer Data Suspended */
#define    GPF       0x72u     /* general Programming Failure */
#define   WBSC       0x73u     /* wrong Block Sequence Counter */
#define  RCRRP       0x78u     /* request Correctly Received - Response Pending */

/* definitions of routine control service */
#define    IDLE      0u
#define    BUSY      1u

/* DTC format */
#define ISO15031_6   0x00u
#define ISO14229_1   0x01u
#define SAEJ1939_73  0x02u
#define ISO11992_4   0x03u

extern uint16 u16g_AddressingMode;

/* function prototypes */
extern void UDS_Init(void);
extern void UDS_Entry(const uint8 u8_RxBuff[], uint32 u32_UDSDataLen);
extern void UDS_Run(void);
#endif
