#ifndef CAN_TP_H
#define CAN_TP_H

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"
#include "Can.h"
#include "CanTp_Cfg.h"


/*=======[M A C R O S]========================================================*/

	

/*=======[T Y P E   D E F I N I T I O N S]====================================*/

typedef BufReq_ReturnType CanTp_BufReq_RetType;

typedef enum 
{
	CANTP_EXTENDED, 
	CANTP_STANDARD
} CanTp_Address_FormatType;



typedef struct 
{
	/* 		  rxPduId			rxDcmId		            */
	/* Can ------------->  CanTp ------------->  DCM */
	/*        txFcPduId                                 */
	/*        <------------                             */
	const	uint16 rxDcmId;
	const	PduIdType rxPduId;
	const	CanTp_Address_FormatType addressingFormat;
	const	uint8 nSa;
	const	PduIdType txFcPduId;
} CanTp_RxSduType;


typedef struct 
{

	/* 		  txPduId			txDcmId		             */	
	/* Can <-------------  CanTp <-------------  DCM  */
	/*        rxFcPduId                         	   	 */
	/*        ------------>                              */
	const PduIdType txDcmId; 		
	const PduIdType txPduId; 
	const PduIdType rxFcPduId;
} CanTp_TxSduType; 





/*=======[E X T E R N A L   D A T A]==========================================*/

extern const CanTp_RxSduType CanTp_RxSdu[CANTP_RX_CHANNEL_NUM];
extern const CanTp_TxSduType CanTp_TxSdu[CANTP_TX_CHANNEL_NUM];



/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

 void CanTp_Transmit(const PduIdType CanTp_txSduId, const PduInfoType *const CanTp_txInfoPtr);
 void CanTp_RxIndication(const PduIdType CanTp_pduId, const PduInfoType *const CanTp_objRxPtr);
 void CanTp_TxConfirmation(const PduIdType CanTp_pduId);
 void CanTp_MainFunction(void);
 void CanTp_Init(void);
 
#endif



