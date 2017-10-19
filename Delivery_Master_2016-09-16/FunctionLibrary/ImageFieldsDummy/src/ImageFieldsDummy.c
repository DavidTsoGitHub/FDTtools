/********************************* Includes **********************************/
#include "ImageFieldsDummy.h"
#include "ImageFieldsDummy_cfg.h"
#include "GCL_ImageFieldsDummy.h"
#include "StandardTypes.h"

static uint8 m_lastChargingIndication;
static uint8 m_lastParkBrakeIndication;
static uint8 m_lastBrakePressureLowIndication;

static void writeImageIDsToGCL(void);

void ImageFieldsDummy_Init(void)
{
    m_lastChargingIndication = 0;
    m_lastParkBrakeIndication = 0;
    m_lastBrakePressureLowIndication = 0;
    GCL_Write_ImageFieldsDummy_Image1ID(0);
    GCL_Write_ImageFieldsDummy_Image2ID(0);
    GCL_Write_ImageFieldsDummy_Image3ID(0);
}

void ImageFieldsDummy_Run(void)
{
    uint8 u8SignalStatus;
    
    uint8 ChargingIndication;
    uint8 ParkBrakeIndication;
    uint8 BrakePressureLowIndication;

    GCL_Read_ImageFieldsDummy_ChargingIndication(&ChargingIndication, &u8SignalStatus);
    GCL_Read_ImageFieldsDummy_ParkBrakeIndication(&ParkBrakeIndication, &u8SignalStatus);
    GCL_Read_ImageFieldsDummy_BrakePressureLowIndication(&BrakePressureLowIndication, &u8SignalStatus);
    
    if((ChargingIndication != m_lastChargingIndication) || 
       (ParkBrakeIndication != m_lastParkBrakeIndication) || 
       (BrakePressureLowIndication != m_lastBrakePressureLowIndication))
    {
        m_lastBrakePressureLowIndication = BrakePressureLowIndication;
        m_lastChargingIndication = ChargingIndication;
        m_lastParkBrakeIndication = ParkBrakeIndication;
        
        writeImageIDsToGCL();
    }
}

static void writeImageIDsToGCL(void)
{
    uint32 labelIds[3];
    labelIds[0] = 0;
    labelIds[1] = 0;
    labelIds[2] = 0;
    uint8 labelIndex = 0;
    
    if (m_lastChargingIndication == 1U)
    {
        labelIds[labelIndex] = ImageID_Charging;
        labelIndex++;
    }
    
    if (m_lastParkBrakeIndication == 1U)
    {
        labelIds[labelIndex] = ImageID_ParkBrake;
        labelIndex++;
    }
    
    if (m_lastBrakePressureLowIndication == 1U)
    {
        labelIds[labelIndex] = ImageID_BrakePressureLow;
    }
    
    GCL_Write_ImageFieldsDummy_Image1ID(labelIds[0]);    
    GCL_Write_ImageFieldsDummy_Image2ID(labelIds[1]);
    GCL_Write_ImageFieldsDummy_Image3ID(labelIds[2]);
}
