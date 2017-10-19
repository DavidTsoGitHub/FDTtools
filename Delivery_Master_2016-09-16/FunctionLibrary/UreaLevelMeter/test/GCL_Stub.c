#include "GCL_UreaLevelMeter.h"

// Define variables to hold stubbed GCL signals
static uint8 m_CatalystTankLevel = 0;
static uint8 m_CatalystTankLevelSignalStatus = 0;
static uint8 m_ReadCatalystTankLevelCallCount = 0;

static PowerMode m_PowerMode = POWER_LOW;
static uint8 m_PowerModeSignalStatus = 0;

static uint8 m_Segment0 = 0;
static uint8 m_Segment1 = 0;
static uint8 m_Segment2 = 0;
static uint8 m_Segment3 = 0;

void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    m_CatalystTankLevel = 0;
    m_CatalystTankLevelSignalStatus = 0;
    m_ReadCatalystTankLevelCallCount = 0;
    m_PowerMode = POWER_LOW;
    m_PowerModeSignalStatus = 0;
    m_Segment0 = 0;
    m_Segment1 = 0;
    m_Segment2 = 0;
    m_Segment3 = 0;
}

void GCLStub_SetCatalystTankLevel(uint8 catalystTankLevel, uint8 signalStatus)
{
    m_CatalystTankLevel = catalystTankLevel;
    m_CatalystTankLevelSignalStatus = signalStatus;
}

uint8 GCLStub_GetReadCatalystTankLevelCallCount(void)
{
    return m_ReadCatalystTankLevelCallCount;
}

void GCLStub_SetPowerMode(PowerMode powerMode, uint8 signalStatus)
{
    m_PowerMode = powerMode;
    m_PowerModeSignalStatus = signalStatus;
}

uint8 GCLStub_GetSegment0(void)
{
    return m_Segment0;
}

uint8 GCLStub_GetSegment1(void)
{
    return m_Segment1;
}

uint8 GCLStub_GetSegment2(void)
{
    return m_Segment2;
}

uint8 GCLStub_GetSegment3(void)
{
    return m_Segment3;
}

void GCL_Read_UreaLevelMeter_CatalystTankLevel(uint8* value, uint8* status)
{
    ++m_ReadCatalystTankLevelCallCount;
    *value = m_CatalystTankLevel;
    *status = m_CatalystTankLevelSignalStatus;
}

void GCL_Read_UreaLevelMeter_PowerMode(PowerMode* value, uint8* status)
{
    *value = m_PowerMode;
    *status = m_PowerModeSignalStatus;
}

void GCL_Write_UreaLevelMeter_Segment0(uint8 value)
{
    m_Segment0 = value;
}

void GCL_Write_UreaLevelMeter_Segment1(uint8 value)
{
    m_Segment1 = value;
}

void GCL_Write_UreaLevelMeter_Segment2(uint8 value)
{
    m_Segment2 = value;
}

void GCL_Write_UreaLevelMeter_Segment3(uint8 value)
{
    m_Segment3 = value;
}
