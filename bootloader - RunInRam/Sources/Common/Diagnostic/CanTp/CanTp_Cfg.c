/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "CanTp.h"
#include "CanTp_Cfg.h"


/*=======[E X T E R N A L   D A T A]==========================================*/
const CanTp_RxSduType CanTp_RxSdu[CANTP_RX_CHANNEL_NUM]=
{	
 
	{	
	 	/* rxDcmId */
        /* @type:uint16 range:0x0000~0xFFFF note:auto generate */
		0xCCu,
		
		/* rxPduId */
        /* @type:uint16 range:0x0000~0xFFFF note:reference to Can_RxPdu->pduId */
		0x11u, 
		
		/* addressingFormat */
        /* @type:ENUM range:NONE note:NONE */
		CANTP_STANDARD,	
		//CANTP_EXTENDED,	 
		
		/* nSa */
        /* @type:uint8 range:0x00~0xFF note:NONE */
		0x00u,
		
		/* txFcPduId */
        /* @type:uint16 range:0x0000~0xFFFF note:reference to Can_TxPdu->pduId */
	 	0x01u	 
	},

}; 	

const CanTp_TxSduType CanTp_TxSdu[CANTP_TX_CHANNEL_NUM]=
{	
	
	{	
	 	/* txDcmId */
        /* @type:uint16 range:0x0000~0xFFFF note:auto generate */
		0xAAu, 
		
		/* txPduId */
        /* @type:uint16 range:0x0000~0xFFFF note:reference to Can_TxPdu->pduId */
		0x01u,
		
		/* rxFcPduId */
        /* @type:uint16 range:0x0000~0xFFFF note:reference to Can_RxPdu->pduId */
		0x11u, 
	},
}; 



