#include "GCL_WarningIconFields.h"
#include "GCL_Stub.h"

static uint8 ChargingIndication = 0;
static uint8 ParkBrakeIndication = 0;
static uint8 BrakePressureLowIndication = 0;
static uint8 EngineOilPressureIndication = 0;

static uint32 Icon1ID = 0;
static uint32 Icon2ID = 0;
static uint32 Icon3ID = 0;
static uint32 Icon4ID = 0;


void GCLStub_Reset(void)
{
    ChargingIndication = 0;
    ParkBrakeIndication = 0;
    BrakePressureLowIndication = 0;
    EngineOilPressureIndication = 0;

    Icon1ID = 0;
    Icon2ID = 0;
    Icon3ID = 0;
    Icon4ID = 0;
}

void GCLStub_Reset(void);


void GCLStub_SetChargingIndication(uint8 value)
{
    ChargingIndication = value;
}

void GCLStub_SetParkBrakeIndication(uint8 value)
{
    ParkBrakeIndication = value;
}

void GCLStub_SetBrakePressureLowIndication(uint8 value)
{
    BrakePressureLowIndication = value;
}

void GCLStub_SetEngineOilPressureIndication(uint8 value)
{
    EngineOilPressureIndication = value;
}

// Define getters to inspect the signals that are written by the application block
uint32 GCLStub_GetIcon1ID(void)
{
    return Icon1ID;
}

uint32 GCLStub_GetIcon2ID(void)
{
    return Icon2ID;
}

uint32 GCLStub_GetIcon3ID(void)
{
    return Icon3ID;
}

uint32 GCLStub_GetIcon4ID(void)
{
    return Icon4ID;
}

void GCL_Read_WarningIconFields_ChargingIndication(uint8* value, uint8* status)
{
    *value = ChargingIndication;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_WarningIconFields_ParkBrakeIndication(uint8* value, uint8* status)
{
    *value = ParkBrakeIndication;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_WarningIconFields_BrakePressureLowIndication(uint8* value, uint8* status)
{
    *value = BrakePressureLowIndication;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_WarningIconFields_EngineOilPressureIndication(uint8* value, uint8* status)
{
    *value = EngineOilPressureIndication;
    *status = GCL_SIGNAL_OK;
}

void GCL_Write_WarningIconFields_Icon1ID(uint32 iconID)
{
    Icon1ID = iconID;
}

void GCL_Write_WarningIconFields_Icon2ID(uint32 iconID)
{
    Icon2ID = iconID;
}

void GCL_Write_WarningIconFields_Icon3ID(uint32 iconID)
{
    Icon3ID = iconID;
}

void GCL_Write_WarningIconFields_Icon4ID(uint32 iconID)
{
    Icon4ID = iconID;
}
