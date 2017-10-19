//lint -save
//lint -esym(528,can_disableCanAInt) [Function is part of the CAN API]
//lint -e923 [Supression of conversion from platform register to uint32]
//lint -e9078 [Supression of conversion from platform register to uint32]

#include <jdp.h>
#include "CanDrv5606S.h"
#include "can_driver.h"
#include "CAN_IF.h"
#include "Platform_Types.h"

//For CAN transceiver
#include <SIUL.h>

#define TRANSCEIVERMODE_NORMAL      0u
//#define TRANSCEIVERMODE_LOWPOWER  1u

#define UNUSED(v) (void)v

static uint32 u32CANIntMaskLow = 0u;
static uint32 u32CANIntMaskHigh = 0u;
static CAN_MODES mbCanMode[CAN_MAX_MB];
static boolean bRxOnly = FALSE;

static void can_receive(uint8 u8ControllerId, uint8 u8Mb, uint32 *id, uint8 *data, uint8 *datalen);
static uint32 can_getCanId(uint8 u8ControllerId, uint8 mb);
static void can_disableCanAInt(void);
static void can_enableCanAInt(void);
static void BSP_SetTransceiverMode(uint8 mode);
static void canInitPins(void);

static volatile struct FLEXCAN_tag* CANREG_PTR(uint8 u8ControllerID)
{
    volatile struct FLEXCAN_tag* retval = (volatile struct FLEXCAN_tag*) 0;
    switch(u8ControllerID)
    {
        case 0:
            retval = &CAN_0;
            break;
        default:
            retval = NULL;
            break;
    }
    return retval;
}


//Called from BSP_Init() or moved to RappId configuration.
static void canInitPins(void)
{
    //Set transceiver PIN configuration
    SIU.PCR[PJ11].R = 0x0200u;
    
    //configure CAN0 pins
    SIU.PCR[16].R = 0x0624u; /* MPC56xxB: Config port B0 as CAN0TX, open drain */
    SIU.PCR[17].R = 0x0500u;
    SIU.PSMI[0].R = 0x00u;

}

//Shall be implemented in the BSP...
static void BSP_SetTransceiverMode(uint8 mode)
{
    SIU.GPDO[PJ11].B.PDO = mode;
}


void CAN_InitController( uint8 u8ControllerId, uint32 u32TimingConfig)
{
    uint8 i;
    volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);
    
    if(CAN_X != NULL)
    {
        (*CAN_X).MCR.R = MPC_FREEZE_ENABLE_BUFFERS;
        (*CAN_X).MCR.B.BCC = 1u;         //Enable individual mask registers
        (*CAN_X).MCR.B.SRXDIS = 1u;      //Disable self reception

        (*CAN_X).CR.R = u32TimingConfig;
       
        (*CAN_X).CR.B.BOFFMSK = 1u; // Bus off interupt enabled.
        (*CAN_X).CR.B.ERRMSK = 1u;  // ERROR interupt enabled.
        
        (*CAN_X).CR.B.BOFFREC = 0u; // Automatic Bus Off Recovery mode enabled.
        (*CAN_X).ESR.B.BOFFINT = 1u;        // Clear busoff int

        for (i = 0; i < CAN_MAX_MB; ++i)
        {
            (*CAN_X).BUF[i].CS.B.CODE = CODE_MS_BUF_RX_INACTIVE; /* Inactivate all message buffers */
        }
    }
    
    bRxOnly = FALSE;
    
    //Configure the MCU Pins for CAN and Transceiver control.
    
    canInitPins();
    
}


void CAN_Start(uint8 u8ControllerId)
{
    can_enableCanAInt();
    BSP_SetTransceiverMode(TRANSCEIVERMODE_NORMAL);
    
    volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);

    if(CAN_X != NULL)
    {
        (*CAN_X).MCR.B.FRZ = 0u;        //Leave freeze mode
        (*CAN_X).MCR.B.HALT = 0u;   //Remove halt mode
    }
}

void CAN_Stop(uint8 u8ControllerId)
{
    volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);

    if(CAN_X != NULL)
    {
        (*CAN_X).MCR.B.FRZ = 1u;        //Enter freeze mode
        (*CAN_X).MCR.B.HALT = 1u;   //Halt CAN controller
    }
}

void CAN_Run(void)
{

}

void CAN_ConfigMB(uint8 controllerId, uint8 mailBox, CAN_MODES mode, uint32 canID, uint32 canMask)
{
    volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(controllerId);
    
    if ( (mailBox < CAN_MAX_MB) && (CAN_X != NULL))
    {
        mbCanMode[mailBox] = mode;
    
        if( mode != CAN_MODE_DISABLED )
        {
            
            if ( canID > 0x7FFu ) //larger than 11 bits..
            {
                (*CAN_X).BUF[mailBox].CS.B.IDE = 1u; // set extended ID flag
                (*CAN_X).BUF[mailBox].ID.R = canID;
            }
            else
            {
                (*CAN_X).BUF[mailBox].CS.B.IDE = 0u; // set to standard message
                (*CAN_X).BUF[mailBox].ID.R = canID;
            }
            
            
            switch (mode)
            {
            case CAN_MODE_RX:
                /* Set the individual mask register */
                (*CAN_X).RXIMR[mailBox].R = canMask;
                
                /* Set code for rx buffer */
                (*CAN_X).BUF[mailBox].CS.B.CODE = CODE_MS_BUF_RX_EMPTY;
                break;
                
            case CAN_MODE_TX:
                /* Set code for tx buffer */
                (*CAN_X).BUF[mailBox].CS.B.CODE = CODE_MS_BUF_TX_INACTIVE;
                break;
            default:
                // do nothing
                break;
            }
            
            //Enable Interrupt handler for MB
            if (mailBox < 32u)
            {
                u32CANIntMaskLow |= (0x00000001uL << mailBox);
            }
            else
            {
                u32CANIntMaskHigh |= (0x00000001uL << ( mailBox-32u) );
            }
        }
    }
}

void CAN_ISR(uint8 u8ControllerId, uint8 u8Mb)
{
    if( (u8Mb >= CAN_MAX_MB) || (u8ControllerId != 0u) ) 
    {
        // do nothing
    }
    else
    {
        if(mbCanMode[u8Mb] == CAN_MODE_RX)
        {
            uint8 u8Buffer[8];
            uint8 u8DataLen;
            uint32 u32CanId;

            //Get data and notify CAN_IF...
            can_receive(u8ControllerId, u8Mb, &u32CanId, u8Buffer, &u8DataLen);
            CAN_IF_RxIndication(u8ControllerId, u8Mb, u32CanId, u8Buffer, u8DataLen);
        }
        else if(mbCanMode[u8Mb]== CAN_MODE_TX)
        {
            CAN_IF_TxConfirmation(can_getCanId(u8ControllerId,u8Mb), u8Mb, u8ControllerId);
        }
        else
        {
            //do nothing
        }
    }
}

void CAN_RxOnly()
{
    bRxOnly = TRUE;
}

void CAN_RxAndTx()
{
    bRxOnly = FALSE;
}

void CAN_Write(uint8 u8ControllerId, uint8 u8Mb, uint32 u32PduId, const uint8* Data, uint8 u8DataLength)
{
    uint8 i;
    uint32 extid;
    
    if (bRxOnly == FALSE)
    {
        volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);
    
        if(CAN_X != NULL)
        {
            //Clear any current transmit
            //(*CAN_X).BUF[u8Mb].CS.B.CODE = CODE_MS_BUF_TX_INACTIVE;      //Inactivate MB to cancel current TX requests
            if(u32PduId != 0u)
            {
                extid = (*CAN_X).BUF[u8Mb].CS.B.IDE;
                if (extid == 0u)
                {
                    /* Set the id for standard id */
                    (*CAN_X).BUF[u8Mb].ID.B.STD_ID = (uint16)u32PduId;
                }
                else
                {
                    /* Set the id for extended id */
                    (*CAN_X).BUF[u8Mb].ID.R = u32PduId;
                 }
            }
            (*CAN_X).BUF[u8Mb].CS.B.RTR = 0u; // Data frame, not remote Tx request frame
            (*CAN_X).BUF[u8Mb].CS.B.LENGTH = u8DataLength;
            for (i = 0; i < u8DataLength; i++)
            {
                (*CAN_X).BUF[u8Mb].DATA.B[i] = Data[i]; // Data to be transmitted
            }
            
            (*CAN_X).BUF[u8Mb].CS.B.SRR = 1u; // Tx frame (not req'd for standard frame)
            (*CAN_X).BUF[u8Mb].CS.B.CODE = CODE_MS_BUF_TX_TRANSMIT;
        }
    }
}



static void can_receive(uint8 u8ControllerId, uint8 u8Mb, uint32 *id, uint8 *data, uint8 *datalen)
{
    uint32 rxCode = 0u;
    uint8 i;
    uint32 timer;
    uint8 len;
    uint32 extid;
    
    volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);

    if(CAN_X != NULL)
    {
        rxCode =(*CAN_X).BUF[u8Mb].CS.B.CODE;
        UNUSED(rxCode);
        
        extid = (*CAN_X).BUF[u8Mb].CS.B.IDE;
        
        if (extid > 0u)
        {
            *id = (*CAN_X).BUF[u8Mb].ID.R;          
        }
        else
        {
            *id = (*CAN_X).BUF[u8Mb].ID.B.STD_ID;           
        }
        
        len = (uint8)(*CAN_X).BUF[u8Mb].CS.B.LENGTH;
        *datalen = len;
        
        for (i = 0; i < len; i++)
        {
            data[i] = (*CAN_X).BUF[u8Mb].DATA.B[i];
        }

        timer = (*CAN_X).TIMER.R;
        UNUSED(timer);
        
        if (u8Mb < 32u)
        {
            (*CAN_X).IFRL.R = 0x00000001uL << (u8Mb);
        }
        else 
        {
            (*CAN_X).IFRH.R = 0x00000001uL << (u8Mb - 32u);
        }
    }
}


static uint32 can_getCanId(uint8 u8ControllerId, uint8 mb)
{
    uint32 retVal = 0;
    const volatile struct FLEXCAN_tag *CAN_X = CANREG_PTR(u8ControllerId);
    if(!CAN_X)
    {
        retVal = 0;
    }
    else
    {
        retVal = (*CAN_X).BUF[mb].ID.R;
    }
    return retVal;
}

static void can_disableCanAInt(void)
{
	CAN_0.IMRL.R = 0u;
	CAN_0.IMRH.R = 0u;
}

static void can_enableCanAInt(void)
{
	CAN_0.IMRL.R = u32CANIntMaskLow;
	CAN_0.IMRH.R = u32CANIntMaskHigh;
}

//lint -restore
