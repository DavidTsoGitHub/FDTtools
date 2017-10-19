/********************************* Includes **********************************/
#include "WarningIconFields.h"
#include "WarningIconFields_cfg.h"
#include "GCL_WarningIconFields.h"
#include "StandardTypes.h"

static uint8 m_lastChargingIndication;
static uint8 m_lastParkBrakeIndication;
static uint8 m_lastBrakePressureLowIndication;
static uint8 m_lastEngineOilPressureIndication;

static void writeIconIDsToGCL(void);

void WarningIconFields_Init(void)
{
    m_lastChargingIndication = 0;
    m_lastParkBrakeIndication = 0;
    m_lastBrakePressureLowIndication = 0;
    m_lastEngineOilPressureIndication = 0;
    GCL_Write_WarningIconFields_Icon1ID(0);
    GCL_Write_WarningIconFields_Icon2ID(0);
    GCL_Write_WarningIconFields_Icon3ID(0);
    GCL_Write_WarningIconFields_Icon4ID(0);
}

void WarningIconFields_Run(void)
{
    uint8 u8SignalStatus;
    uint8 ChargingIndication;
    uint8 ParkBrakeIndication;
    uint8 BrakePressureLowIndication;
    uint8 EngineOilPressureIndication;

    GCL_Read_WarningIconFields_ChargingIndication(&ChargingIndication, &u8SignalStatus);
    GCL_Read_WarningIconFields_ParkBrakeIndication(&ParkBrakeIndication, &u8SignalStatus);
    GCL_Read_WarningIconFields_BrakePressureLowIndication(&BrakePressureLowIndication, &u8SignalStatus);
    GCL_Read_WarningIconFields_EngineOilPressureIndication(&EngineOilPressureIndication, &u8SignalStatus);
    
    if((ChargingIndication != m_lastChargingIndication) || 
       (ParkBrakeIndication != m_lastParkBrakeIndication) || 
       (BrakePressureLowIndication != m_lastBrakePressureLowIndication) || 
       (EngineOilPressureIndication != m_lastEngineOilPressureIndication))
    {
        m_lastBrakePressureLowIndication = BrakePressureLowIndication;
        m_lastChargingIndication = ChargingIndication;
        m_lastEngineOilPressureIndication = EngineOilPressureIndication;
        m_lastParkBrakeIndication = ParkBrakeIndication;
        
        writeIconIDsToGCL();
    }
}

static void writeIconIDsToGCL(void)
{
    uint32 labelIds[4];
    labelIds[0] = 0;
    labelIds[1] = 0;
    labelIds[2] = 0;
    labelIds[3] = 0;
    uint8 labelIndex = 0;
    
    if (m_lastChargingIndication == 1U)
    {
        labelIds[labelIndex] = IconID_Charging;
        labelIndex++;
    }
    
    if (m_lastParkBrakeIndication == 1U)
    {
        labelIds[labelIndex] = IconID_ParkBrake;
        labelIndex++;
    }
    
    if (m_lastBrakePressureLowIndication == 1U)
    {
        labelIds[labelIndex] = IconID_BrakePressureLow;
        labelIndex++;
    }
    
    if (m_lastEngineOilPressureIndication == 1U)
    {
        labelIds[labelIndex] = IconID_EngineOilPressure;
    }

    GCL_Write_WarningIconFields_Icon1ID(labelIds[0]);    
    GCL_Write_WarningIconFields_Icon2ID(labelIds[1]);
    GCL_Write_WarningIconFields_Icon3ID(labelIds[2]);
    GCL_Write_WarningIconFields_Icon4ID(labelIds[3]);
}
