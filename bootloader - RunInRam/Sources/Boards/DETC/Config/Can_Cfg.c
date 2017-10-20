/*=======[I N C L U D E S]====================================================*/
#include "ComStack_Types.h"
#include "Can.h"
#include "Can_Cfg.h"


/*=======[M A C R O S]========================================================*/



/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/** Can Channel Configuration*/
const CanChannelCfgType Can_ChannelCfg=
{
   	/* BaudRate . kbps*/
    /* @type:uint16 range:1~1000 kbps,note:NONE */
	250u,
	
	/* Prop_Seg*/
    /* @type:uint8 range:1~14 note:NONE */
	7u,
	
	/* Phase seg1*/
    /* @type:uint8 range:1~14 note:(Prop_Seg + Phase seg1) should less than 14 */
	4u,
	
	/* Phase seg2*/
    /* @type:uint8 range:1~7 note:NONE */
    1u,
};


/** Can transmit Pdu configuration*/
const CanTxPduType Can_TxPdu[CAN_TX_PDU_NUM] = 
{
	{
        /* tx pduId */
        /* @type:uint16 range:0x0000~0xFFFF note:auto generate */
        0x01u,
        
        /* canId */
        /* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
		//0x260u,
		0x18DAF117,
		/* standardFrame or not */
        /* @type:boolean range:TRUE,FALSE note:NONE */
		FALSE,
		//TRUE,
        /* Can_TxMailBoxMask,buffer 1-31 can be selected*/
        /* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
        0x00000200uL,  

	},
};

/** Can receive Pdu configuration*/
const CanRxPduType Can_RxPdu[CAN_RX_PDU_NUM] = 
{
	{
        /* rx pduId */
        /* @type:uint16 range:0x0000~0xFFFF note:auto generate */
        0x11u,
        
        /* canId */
        /* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
		//0x240u,
        0x18DA17F1,
		/* standardFrame or not */
        /* @type:boolean range:TRUE,FALSE note:NONE */
        FALSE,
		//TRUE,
        /* Can_RxMailBoxMask,buffer 1-31 can be selected */
        /* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
        0x00000100uL,
        
        /* CanFilterMaskRef */
        /* @type:uint32 range:0x00000000~0xFFFFFFFF note:NONE */
        0xFFFFFFFFuL,

	},

};

