#ifndef CAN_CFG_H
#define CAN_CFG_H


/*=======[I N C L U D E S]====================================================*/


/*=======[M A C R O S]========================================================*/

/** select controller  */
/* @type:define range:CAN_CONTROLLER_A~CAN_CONTROLLER_C note:NONE */
#define CAN_CONTROLLER_USED  CAN_0

/* define CAN TX and RX PDU number */
/* @type:uint8 range:1~255 note:reference to num of Can_TxPdu */
#define CAN_TX_PDU_NUM	    1u

/* @type:uint8 range:1~255 note:reference to num of Can_RxPdu */
#define CAN_RX_PDU_NUM	    1u

#endif /* endof CAN_CFG_H */


