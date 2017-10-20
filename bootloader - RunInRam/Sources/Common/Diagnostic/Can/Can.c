/*=======[I N C L U D E S]====================================================*/
#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "ComStack_Types.h"
#include "Can.h"
#include "CanTp.h"
#include "Mcu.h"
#include "Appl.h"
#include "MPC5645S.h"

/*=======[M A C R O S]========================================================*/

/* Message box status defines */
#define CAN_MB_TX_ONCE              0xcu
#define CAN_MB_INACTIVE             0x8u
#define CAN_MB_RX                   0x4u
#define CAN_MB_ABORT                0x9u
#define CAN_MB_RX_OVERRUN           0x6u

#define CAN_ERRINT                  0x00000002uL
#define CAN_BUSOFF                  0x00000004uL

/* mcr register */
#define CAN_MCR_MDIS        0x7FFFFFFFuL
#define CAN_MCR_FRZACK      0x01000000uL
#define CAN_MCR_MBFEN       0x00010000uL
#define CAN_MCR_MAXMB       31u
#define CAN_MCR_NOTRDY      0x08000000uL

#define CAN_MCR_SOFTREST    0x02000000uL

/* control register */
#define CAN_CR_CLKSRC       0xFFFFDFFFuL
#define CAN_CR_RJW          0x00C00000uL
#define CAN_CR_SMP          0xFFFFFF7FuL     

/* Can Buffer */
#define CAN_IDE_STANDARD    0xFFDFFFFFuL
#define CAN_IDE_EXTENDED    0x00200000uL


/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/**=======[I N T E R N A L   D A T A]==========================================*/


/**=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/

STATIC void Can_TxConfirmMainFunc(void);
STATIC void Can_RxIndicationMainFunc(void);
STATIC void Can_ErrorMainFunc(void);
STATIC uint32 Can_FindBuffer(uint32 buffer_mask) ;


/**=======[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/

/*******************************************************************************/
/***
 * @brief			:Can_FindBuffer,find the buffer for rx or tx.  
 * 
 * Service ID		: <Can_FindBuffer>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
STATIC uint32 Can_FindBuffer(uint32 buffer_mask) 
{
    boolean exit_Flag       = FALSE;
    uint32  compare_lastbit = 0x00000001uL;
    uint8   count           = 0u;

    while ((count < 32u) && (exit_Flag == FALSE))
    {
        if(((uint32)(buffer_mask >> count) & compare_lastbit) == 0u)
        {
         /* if this bit is zero, then count next */
         count ++;
        }
        else
        {
         exit_Flag = TRUE;
        }
    }
    return count;
}


/*******************************************************************************/
/***
 * @brief			:Can initalise,config the registers.  
 * 
 * Service ID		: <Can_Init>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Can_Init(void) 
{
    uint8 tseg1     = 0u;
    uint8 tseg2     = 0u;
    uint8 brp       = 0u;  
    uint8 pduIndex  = 0u;
    uint8 mbIndex   = 0u;
    uint8 can_mbTmp;  
    
   	/* Time quantas */
    uint8 Can_controllerTimeQuanta;

    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

    /* enable the flexcan module*/
    Can_Ptr->MCR.R &= CAN_MCR_MDIS;
    
    /* set CAN to freeze mode */
    Can_Ptr->MCR.R |= (uint32)(1u << 30u);
    Can_Ptr->MCR.R |= (uint32)(1u << 28u);

    /* and wait for confirmation. */
	while (0u == (Can_Ptr->MCR.R & CAN_MCR_FRZACK ))
	{
	 /*check whether enter the freeze mode*/
	}

    /* Supv = 0*/
    Can_Ptr->CR.R &= 0xFF7FFFFFuL;

    Can_Ptr->MCR.R |= CAN_MCR_MBFEN;
     
    /* use osc clock */    
    Can_Ptr->CR.R &= CAN_CR_CLKSRC ;
    Can_Ptr->MCR.R |= (uint32) CAN_MCR_MAXMB;

    /* Set Ecr count */
    Can_Ptr->ECR.R = 0x00008137uL;

    
    /* Select the configured TSEG1 value. */
    tseg1 = (Can_ChannelCfg.CanControllerPropSeg + Can_ChannelCfg.CanControllerSeg1 + 2u);

    /* Select the configured TSEG2 value. */
    tseg2 = (Can_ChannelCfg.CanControllerSeg2 + 1u);
    
    Can_controllerTimeQuanta = 1u + tseg1 + tseg2;
    
    brp =  (uint8)(CLOCK_CRYSTAL_FREQ_UESD / ((uint32)(Can_ChannelCfg.CanControllerBaudRate) * (uint32)1000u * (uint32)(Can_controllerTimeQuanta)) - 1uL);
    Can_Ptr->CR.R  |= (uint32)(brp << 24u)
                   | (uint32)((Can_ChannelCfg.CanControllerSeg1) << 19u)
                   | (uint32)((Can_ChannelCfg.CanControllerSeg2) << 16u)
                   | (uint32)(Can_ChannelCfg.CanControllerPropSeg)
                   |  CAN_CR_RJW;

     Can_Ptr->CR.R &= CAN_CR_SMP; /**< One sample per bit */
          
     /* We need to initialize all the MB control and status registers */
     for (mbIndex = 0u; mbIndex<NUM_CAN_MB; mbIndex++) {
        Can_Ptr->BUF[mbIndex].CS.R = CAN_MB_INACTIVE << 24;
     }

     /* Config the Rx Buffer And Config the Filter*/
    for (pduIndex = 0u; pduIndex < CAN_RX_PDU_NUM; pduIndex++)
	{      
    	/* loop for multiplexed mailboxes, set as same as first */
        uint32 Can_mbMask_Low = *(&Can_Ptr->IFRL.R);
        uint32 Can_mbMask = ~Can_mbMask_Low  & Can_RxPdu[pduIndex].Can_RxMailBoxMask;
        
        while(Can_mbMask != 0u) 
        {
        	can_mbTmp = (uint8)Can_FindBuffer(Can_mbMask);

            Can_Ptr->BUF[can_mbTmp].CS.R = (uint32)(CAN_MB_RX << 24u);
			Can_Ptr->RXIMR[can_mbTmp].R = Can_RxPdu[pduIndex].CanFilterMaskRef;

			if (Can_RxPdu[pduIndex].standardFrame == TRUE)
            {
                Can_Ptr->BUF[can_mbTmp].CS.R &= CAN_IDE_STANDARD;
                Can_Ptr->BUF[can_mbTmp].ID.R = (uint32)(Can_RxPdu[pduIndex].canId << 18u);
			} 
            else
            {
                Can_Ptr->BUF[can_mbTmp].CS.R |= CAN_IDE_EXTENDED ;
                Can_Ptr->BUF[can_mbTmp].ID.R = (uint32)(Can_RxPdu[pduIndex].canId);
			}
            Can_mbMask &= (~((uint32)(1u << can_mbTmp)));

        }
     }

     /* Config the Tx Buffer */
    for (pduIndex = 0u; pduIndex < CAN_TX_PDU_NUM; pduIndex++)
	{
        uint32 Can_mbMask_Low = *(&Can_Ptr->IFRL.R);
        
        uint32 mbMask = ~Can_mbMask_Low  & Can_TxPdu[pduIndex].Can_TxMailBoxMask;
        
        while(mbMask != 0u) 
        {
        	can_mbTmp = (uint8)Can_FindBuffer(mbMask);
            Can_Ptr->BUF[can_mbTmp].CS.R = (uint32)(CAN_MB_INACTIVE << 24u);
            mbMask &= ~((uint32)(1u << can_mbTmp));

	    }
    }

    /* Set can out of initialization mode */
    Can_Ptr->MCR.R &= 0xAFFFFFFFuL;
    
	while (CAN_MCR_NOTRDY == (Can_Ptr->MCR.R & CAN_MCR_NOTRDY ))
	{
	 /* Wait CAN exit initialisation mode*/
	}

     return;
}

/*******************************************************************************/
/***
 * @brief			:Can Deinit,config the can registers to power on default value.  
 * 
 * Service ID		: <Can_Deinit>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Can_Deinit(void) 
{
    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

    /* Clear Can registers */
    Can_Ptr->MCR.R |= CAN_MCR_SOFTREST;

    return;     
}
/*******************************************************************************/
/**
 * @brief			:to transmit the messages.  
 *                   
 * Service ID		: <Can_Transmit>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
Std_ReturnType Can_Transmit(const uint16 pduId, const PduInfoType *PduInfoPtr)
{
    Std_ReturnType Can_transmitRet = E_NOT_OK;
    uint16 pduIndex;	
    uint16 timer;   
    uint32 Can_mbNr;

    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

	for (pduIndex = 0u; pduIndex < CAN_TX_PDU_NUM; pduIndex++)
	{
        if (pduId == Can_TxPdu[pduIndex].pduId)
        {	

            /* Get all free TX mboxes */
            uint32 Can_mbMask_Low = *(&Can_Ptr->IFRL.R);
            uint32 Can_iflag = ~Can_mbMask_Low  & Can_TxPdu[pduIndex].Can_TxMailBoxMask;

            if (0u != Can_iflag)
            {
                /* find mb number*/
                Can_mbNr = (uint32)Can_FindBuffer(Can_iflag);               
                Can_Ptr->BUF[Can_mbNr].CS.R = 0u;
                Can_Ptr->BUF[Can_mbNr].ID.R = 0u;
                
                /*Setup message box type*/
                if (Can_TxPdu[pduIndex].standardFrame == TRUE) 
                {
                    Can_Ptr->BUF[Can_mbNr].CS.R &= CAN_IDE_STANDARD;
                } 
                else 
                {
                    Can_Ptr->BUF[Can_mbNr].CS.R |= CAN_IDE_EXTENDED;

                } 

                /* Hold the transmit buffer inactive */
                Can_Ptr->BUF[Can_mbNr].CS.R |= (uint32)(CAN_MB_INACTIVE << 24u);

                if (Can_TxPdu[pduIndex].standardFrame == TRUE)
                {
                    Can_Ptr->BUF[Can_mbNr].ID.R = (uint32)(Can_TxPdu[pduIndex].canId << 18u) ;
                    Can_Ptr->BUF[Can_mbNr].ID.R &= (~(uint32)(1u << 29u));
                }
                else
                {
                    /* Write 29-bit MB IDs, Set Local Priority */
                    Can_Ptr->BUF[Can_mbNr].ID.R = ((uint32)(Can_TxPdu[pduIndex].canId) | (uint32)(1u << 29u));
                }


                (void)Appl_Memset((uint8 *)&Can_Ptr->BUF[Can_mbNr].DATA.B, 0u, 8u);
                (void)Appl_Memcpy((uint8 *)&Can_Ptr->BUF[Can_mbNr].DATA.B, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

                /* SRR = 1 ,RTR = 0,*/
                Can_Ptr->BUF[Can_mbNr].CS.R |= ((uint32)(1u << 22u) |(uint32)((PduInfoPtr->SduLength) << 16u));
                
                Can_Ptr->BUF[Can_mbNr].CS.R |= (uint32)(CAN_MB_TX_ONCE << 24u);/* Write tx once code */

                
                timer = (uint16)(Can_Ptr->TIMER.R); /* Unlock Message buffers */
                
                Can_transmitRet = E_OK;

            }
        }

	
    }        
	return Can_transmitRet;
}


/*******************************************************************************/
/***
 * @brief			: Confrim the tranmitted message,get the pdu,send to Cantp .  
 *                   
 * Service ID		: <Can_TxConfirmMainFunc>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
STATIC  void Can_TxConfirmMainFunc(void) 
{ 
    uint8 Can_mbNr;   
    uint16 pduIndex;    
    uint32 Can_mbMask;
    
    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

    uint32 Can_mbMask_Low = *(&Can_Ptr->IFRL.R);

     for (pduIndex = 0u; pduIndex < CAN_TX_PDU_NUM; pduIndex++)
    {
        Can_mbMask = (Can_mbMask_Low & Can_TxPdu[pduIndex].Can_TxMailBoxMask);

        while(Can_mbMask != 0u)
        {
            Can_mbNr = (uint8)Can_FindBuffer(Can_mbMask);
         
            /* Clear interrupt and mark txBox as Free*/
            if (Can_mbNr >= 32u) 
            {
                Can_Ptr->IFRH.R |= (uint32) (1u << (Can_mbNr - 32u));
            } 
            else 
            {
                Can_Ptr->IFRL.R |= (uint32)(1u << Can_mbNr);
            }                
            
		    /* callback */
			CanTp_TxConfirmation(Can_TxPdu[pduIndex].pduId);
            
            Can_mbMask &= (uint32)(~(1u << Can_mbNr));

        }

    }

    return;  
   	
}

/*******************************************************************************/
/***
 * @brief			: Handle the Can Message receive and indication to Cantp .  
 *                   
 * Service ID		: <Can_RxIndicationMainFunc>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
STATIC void Can_RxIndicationMainFunc(void)
{
    uint16 pduIndex; 
    uint8  Can_mbNr;
    uint32 rx_CanId ;
    uint8  length;
    uint8  data_count;
    uint8  RxData[8u];
    PduInfoType Can_receivePdu;
     
    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

    uint32 Can_mbMask_Low = *(&Can_Ptr->IFRL.R);
    uint32 Can_iFlag =  Can_mbMask_Low;

    for (pduIndex = 0u; pduIndex < CAN_RX_PDU_NUM; pduIndex++)
    {
        while(0u != (Can_iFlag & Can_RxPdu[pduIndex].Can_RxMailBoxMask))
        {	         
             /* Find mailbox */
            Can_mbNr = (uint8)Can_FindBuffer(Can_iFlag & Can_RxPdu[pduIndex].Can_RxMailBoxMask);

            Can_iFlag ^= (uint32)(1u << Can_mbNr);

            /* activate the internal lock with a read*/
            (void) Can_Ptr->BUF[Can_mbNr].CS.R;

            if (0u != (Can_Ptr->BUF[Can_mbNr].CS.R  & (uint32)(1u << 21u)))
            {
                /* Extended Id */
                rx_CanId = Can_Ptr->BUF[Can_mbNr].ID.R;
                rx_CanId |= 0x80000000uL; /* need check */ 
            }
            else
            {
                /* standard Id */
                rx_CanId = (uint32)((Can_Ptr->BUF[Can_mbNr].ID.R << 3u) >> 21u);
            }


            length = ((uint8)((Can_Ptr->BUF[Can_mbNr].CS.R) >> 16u)) & 0x0Fu;

            for (data_count = 0u; data_count < length; data_count++)
            {
               RxData[data_count] = Can_Ptr->BUF[Can_mbNr].DATA.B[data_count];
            }

            /* get the data */
            Can_receivePdu.SduDataPtr = RxData;

            /* get the length */
            Can_receivePdu.SduLength = (uint16)length;

            /* Call Back */
            CanTp_RxIndication(Can_RxPdu[pduIndex].pduId, &Can_receivePdu);
      
            /* unlock MB (dummy read timer) */
            (void) Can_Ptr->TIMER.R;

            /* Clear interrupt and mark txBox as Free*/
            if (Can_mbNr >= 32u) 
            {
                Can_Ptr->IFRH.R |= (uint32)(1u << (Can_mbNr - 32u));
            } 
            else 
            {
                Can_Ptr->IFRL.R |= (uint32)(1u << Can_mbNr);
            }

        }
    }


	return;	
}




/*******************************************************************************/
/***
 * @brief			: Handle the Can Message Error .  
 *                   
 * Service ID		: <Can_ErrorMainFunc>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
STATIC void Can_ErrorMainFunc(void)
{
    uint32  Can_errorMask;

    /* Get The Base Address */
    volatile struct FLEXCAN_tag *Can_Ptr = &CAN_CONTROLLER_USED;

    /* Clear bits 16-23 by read */
    Can_errorMask = Can_Ptr->ESR.R;

    if((0u != (Can_errorMask & CAN_ERRINT) ) || (0u != (Can_errorMask & CAN_BUSOFF)))
    {
        /* check the Bus */
	    /* bus off occured ,or error occured,clear the flag */
        Can_Ptr->ESR.R = 0x0003FFFFuL;

    } 
   
    return;

}

/*******************************************************************************/
/***
 * @brief			: The Can Commication Main Fcuntion.  
 *                   
 * Service ID		: <Can_MainFunc>
 * Sync/Async		: <Synchronous>
 * Reentrancy		: <Reentrant>
 * @param[in]		: <NONE>
 * @param[out]	    : <NONE>
 * @param[in/out]	: <NONE>
 * @return		    : <NONE>    
 */
/*******************************************************************************/
void Can_MainFunc(void)
{
    Can_TxConfirmMainFunc();
    Can_RxIndicationMainFunc();
    Can_ErrorMainFunc();
    return;
}

