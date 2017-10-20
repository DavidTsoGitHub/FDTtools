#ifndef CAN_H
#define CAN_H


/*=======[I N C L U D E S]====================================================*/
#include "Can_Cfg.h"


/*=======[M A C R O S]========================================================*/
#define NUM_CAN_MB		64

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
 
typedef struct 
{
    /* CAN BaudRate ,kbps*/
    const uint16 CanControllerBaudRate;
    /* Prop Seg */
    const uint8 CanControllerPropSeg;
    /* Phase seg1 */
    const uint8 CanControllerSeg1;
    /* phase seg2 */
    const uint8 CanControllerSeg2;
 
} CanChannelCfgType;

typedef struct 
{
	const uint16 pduId;	
	const uint32 canId;
	const boolean standardFrame;
    /* Used for Tx Mail Box select */
    uint32 Can_TxMailBoxMask;

} CanTxPduType;

typedef struct 
{
	const uint16 pduId;	
	const uint32 canId;
    const boolean standardFrame;
    /* Used for Rx Mail Box select */
    uint32 Can_RxMailBoxMask;
    /* Can Rx Individual Mask registers */
    const uint32 CanFilterMaskRef; 

} CanRxPduType;

 

/*=======[E X T E R N A L   D A T A]==========================================*/

/** Can Channel configurations,initialised at Can_Cfg.c, called at Can.c*/
extern const CanChannelCfgType Can_ChannelCfg;

/** Can Transmit Pdu,initialised at Can_Cfg.c, called by Can_Transmit and Can_TxConfirmMainFunc*/
extern const CanTxPduType Can_TxPdu[CAN_TX_PDU_NUM];

/** Can Received Pdu,initialised at Can_Cfg.c, called by Can_RxIndicationMainFunc*/
extern const CanRxPduType Can_RxPdu[CAN_RX_PDU_NUM];


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

 void Can_Init(void);
 void Can_Deinit(void);
 Std_ReturnType Can_Transmit(const uint16 pduId, const PduInfoType *PduInfoPtr);
 void Can_MainFunc(void);



#endif /* endof CAN_H */
