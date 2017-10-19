#include "GCLMock.h"
#include "GCL_SegmentDisplayManager.h"

static uint32 m_displayValue = 0;
static PowerMode m_powerMode = POWER_IGNITION_OFF;
static uint8 m_t1SegmentState = 0;
static uint8 m_dotSegmentState = 0;

void GCLMock_Reset()
{
    m_displayValue = 0;
    m_powerMode = POWER_IGNITION_OFF;
    m_t1SegmentState = 0;
    m_dotSegmentState = 0;
}

void GCLMock_SetDisplayValue(uint32 displayValue)
{
    m_displayValue = displayValue;
}

void GCLMock_SetPowerMode(PowerMode powerMode)
{
    m_powerMode = powerMode;
}

void GCLMock_SetDotSegmentOn()
{
    m_dotSegmentState = 1;
}

void GCLMock_SetDotSegmentOff()
{
    m_dotSegmentState = 0;
}

void GCLMock_SetT1SegmentOn()
{
    m_t1SegmentState = 1;
}

void GCLMock_SetT1SegmentOff()
{
    m_t1SegmentState = 0;
}

void GCL_Read_SegmentDisplayManager_DisplayValue(uint32* value, uint8* status)
{
    *value = m_displayValue;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_SegmentDisplayManager_PowerMode(PowerMode* value, uint8* status)
{
    *value = m_powerMode;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_SegmentDisplayManager_T1SegmentStatus(uint8* value, uint8* status)
{
    *value = m_t1SegmentState;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_SegmentDisplayManager_DOTSegmentStatus(uint8* value, uint8* status)
{
    *value = m_dotSegmentState;
    *status = GCL_SIGNAL_OK;
}

