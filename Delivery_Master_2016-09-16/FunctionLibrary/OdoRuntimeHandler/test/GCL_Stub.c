#include "GCL_OdoRuntimeHandler.h"
#include "GCL_Stub.h"

// Stubbed GCL Signal variables
// Signals read by Function Block
static uint32 m_OdoValue = 0;
static uint8 m_OdoValueSignalStatus = GCL_SIGNAL_OK;
static uint16 m_RuntimeValue = 0;
static uint8 m_RuntimeValueSignalStatus = GCL_SIGNAL_OK;
static boolean m_ShowNext = 0;
static uint8 m_ShowNextSignalStatus = GCL_SIGNAL_OK;

// Signals written by Function Block
static uint32 m_Value = 0;
static boolean m_ShowDot = 0;
static boolean m_ShowHourGlass = 0;


void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    m_OdoValue = 0;
    m_OdoValueSignalStatus = GCL_SIGNAL_OK;
    m_RuntimeValue = 0;
    m_RuntimeValueSignalStatus = GCL_SIGNAL_OK;
    m_ShowNext = 0;
    m_ShowNextSignalStatus = GCL_SIGNAL_OK;
    m_Value = 0;
    m_ShowDot = 0;
    m_ShowHourGlass = 0;

}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCLStub_Set_OdoValue(uint32 value, uint8 signalStatus)
{
    m_OdoValue = value;
    m_OdoValueSignalStatus = signalStatus;
}

void GCLStub_Set_RuntimeValue(uint16 value, uint8 signalStatus)
{
    m_RuntimeValue = value;
    m_RuntimeValueSignalStatus = signalStatus;
}

void GCLStub_Set_ShowNext(boolean value, uint8 signalStatus)
{
    m_ShowNext = value;
    m_ShowNextSignalStatus = signalStatus;
}

void GCLStub_Indicate_ShowNext(void)
{
    OdoRuntimeHandler_ShowNext_Indication();
}

uint32 GCLStub_Get_Value(void)
{
    return m_Value;
}

boolean GCLStub_Get_ShowDot(void)
{
    return m_ShowDot;
}

boolean GCLStub_Get_ShowHourGlass(void)
{
    return m_ShowHourGlass;
}



// GCL Functions
void GCL_Read_OdoRuntimeHandler_OdoValue(uint32* value, uint8* status)
{
    *value = m_OdoValue;
    *status = m_OdoValueSignalStatus;
}

void GCL_Read_OdoRuntimeHandler_RuntimeValue(uint16* value, uint8* status)
{
    *value = m_RuntimeValue;
    *status = m_RuntimeValueSignalStatus;
}

void GCL_Read_OdoRuntimeHandler_ShowNext(boolean* value, uint8* status)
{
    *value = m_ShowNext;
    *status = m_ShowNextSignalStatus;
}

void GCL_Write_OdoRuntimeHandler_Value(uint32 value)
{
    m_Value = value;
}

void GCL_Write_OdoRuntimeHandler_ShowDot(boolean value)
{
    m_ShowDot = value;
}

void GCL_Write_OdoRuntimeHandler_ShowHourGlass(boolean value)
{
    m_ShowHourGlass = value;
}


