#ifndef DCM_CFG_H
#define DCM_CFG_H

#include "Gpt_Cfg.h" 


/*=======[M A C R O S]========================================================*/


/** main receive buffer size */
/* @type:uint16 range:1~4095 note:NONE */
#define DCM_RX_BUF_SIZE    2048u

/** main send buffer size */
/* @type:uint16 range:1~4095 note:NONE */
#define DCM_TX_BUF_SIZE    40u

/** DCM pduId use for CanTp */
/* @type:define range:NONE note:reference to CanTp_TxSdu->txDcmId */
#define DCM_TX_PDU_ID       0xAAu

/* @type:define range:NONE note:reference to CanTp_RxSdu->rxDcmId */
#define DCM_RX_PHY_PDU_ID   0xCCu

/* @type:define range:NONE note:reference to CanTp_RxSdu->rxDcmId */
#define DCM_RX_FUNC_PDU_ID  0xFFu

/** DCM protocol time */
/* @type:define range:1~65535 note:unit ms */
#define DCM_P2MAX_TIME  40u/* ms */

/* @type:define range:1~65535 note:unit ms */
#define DCM_P2SMAX_TIME 5000u //4000u/* ms */

/* @type:define range:1~65535 note:unit ms */
#define DCM_P3MAX_TIME  5000u/* ms */

/** DCM main task tick time */
/* @type:define range:NONE note:auto generate */
#define DCM_MAIN_TICK   GPT_PERIOD_TIME

/** pending send attemt num */
/* @type:define range:1~65535 note:NONE */
#define DCM_PENDING_ATTEMPT_NUM     10u

/** DCM security access time */
/* security access timeout on boot */
/* @type:define range:0~65535 note:unit ms */
#define DCM_SECURITY_TIME_ON_BOOT   0u

/* security access timeout when failed */
/* @type:define range:0~65535 note:unit ms */
#define DCM_SECURITY_TIME           10000u

/* security access attempt max num */
/* @type:define range:0~65535 note:NONE */
#define DCM_SECURITY_ATTEMPT_NUM    10u

/** session supportted num */
/* @type:define range:NONE note:auto generate */
#define DCM_SESSION_SUPPORT_NUM    3

/** security supportted num */
/* @type:define range:NONE note:auto generate */
#define DCM_SECURITY_SUPPORT_NUM    3

/** service table num */
/* @type:define range:1~255 note:reference to Dcm_ServiceTable */
#define DCM_SERVICE_NUM         12

/** session mode num */
/* @type:define range:1~3 note:reference to Dcm_SessionRow */
#define DCM_SESSION_NUM         3

/** reset mode num */
/* @type:define range:1~255 note:reference to Dcm_ResetRow */
#define DCM_RESET_NUM           2

/** read data ID num */
/* @type:define range:0~255 note:reference to Dcm_ReadDidRow */
#define DCM_READDID_NUM         6

/** security level num,not include locked level */
/* @type:define range:1~2 note:reference to Dcm_SecurityRow */
#define DCM_SECURITY_NUM        1

/** communication control num */
/* @type:define range:0~255 note:reference to Dcm_ComControlRow */
#define DCM_COM_CONTROL_NUM     2

/** write data ID num */
/* @type:define range:1~255 note:reference to Dcm_WriteDidRow */
#define DCM_WRITEDID_NUM        1

/** routine control ID num */
/** write data ID num */
/* @type:define range:1~255 note:reference to Dcm_RoutineControlRow */
#define DCM_ROUTINE_CONTROL_NUM 6

/** DTC settin mode num */
/* @type:define range:0~255 note:reference to Dcm_DTCSettingRow */
#define DCM_DTC_SET_NUM         2

#endif/* endof DCM_CFG_H */




