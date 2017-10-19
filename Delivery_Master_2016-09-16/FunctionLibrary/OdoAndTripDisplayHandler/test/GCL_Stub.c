#include "GCL_OdoAndTripDisplayHandler.h"
#include "GCL_Stub.h"

// Stubbed GCL Signal variables
// Signals read by Function Block
static uint8 m_ShowNext = 0;
static uint8 m_ShowNextSignalStatus = GCL_SIGNAL_OK;
static uint32 m_OdoValue = 0;
static uint8 m_OdoValueSignalStatus = GCL_SIGNAL_OK;
static uint32 m_TripValue = 0;
static uint8 m_TripValueSignalStatus = GCL_SIGNAL_OK;
static uint16 m_InstantaneousFuelConsumption = 0;
static uint8 m_InstantaneousFuelConsumptionSignalStatus = GCL_SIGNAL_OK;
static uint32 m_TripFuelConsumption = 0;
static uint8 m_TripFuelConsumptionSignalStatus = GCL_SIGNAL_OK;

// Signals written by Function Block
static wchar* m_TripFieldText = 0;
static wchar* m_OdoAndInstFuelFieldText = 0;


void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    m_ShowNext = 0;
    m_ShowNextSignalStatus = GCL_SIGNAL_OK;
    m_OdoValue = 0;
    m_OdoValueSignalStatus = GCL_SIGNAL_OK;
    m_TripValue = 0;
    m_TripValueSignalStatus = GCL_SIGNAL_OK;
    m_InstantaneousFuelConsumption = 0;
    m_InstantaneousFuelConsumptionSignalStatus = GCL_SIGNAL_OK;
    m_TripFuelConsumption = 0;
    m_TripFuelConsumptionSignalStatus = GCL_SIGNAL_OK;
    m_TripFieldText = 0;
    m_OdoAndInstFuelFieldText = 0;

}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCLStub_Set_ShowNext(uint8 value, uint8 signalStatus)
{
    m_ShowNext = value;
    m_ShowNextSignalStatus = signalStatus;
}

void GCLStub_Indicate_ShowNext(void)
{
    OdoAndTripDisplayHandler_ShowNextTrigger1_Indication();
}

void GCLStub_Set_OdoValue(uint32 value, uint8 signalStatus)
{
    m_OdoValue = value;
    m_OdoValueSignalStatus = signalStatus;
}

void GCLStub_Set_TripValue(uint32 value, uint8 signalStatus)
{
    m_TripValue = value;
    m_TripValueSignalStatus = signalStatus;
}

void GCLStub_Set_InstantaneousFuelConsumption(uint16 value, uint8 signalStatus)
{
    m_InstantaneousFuelConsumption = value;
    m_InstantaneousFuelConsumptionSignalStatus = signalStatus;
}

void GCLStub_Set_TripFuelConsumption(uint32 value, uint8 signalStatus)
{
    m_TripFuelConsumption = value;
    m_TripFuelConsumptionSignalStatus = signalStatus;
}

wchar* GCLStub_Get_TripFieldText(void)
{
    return m_TripFieldText;
}

wchar* GCLStub_Get_OdoAndInstFuelFieldText(void)
{
    return m_OdoAndInstFuelFieldText;
}



// GCL Functions
void GCL_Read_OdoAndTripDisplayHandler_ShowNextTrigger1(uint8* value, uint8* status)
{
    *value = m_ShowNext;
    *status = m_ShowNextSignalStatus;
}

void GCL_Read_OdoAndTripDisplayHandler_OdoValue(uint32* value, uint8* status)
{
    *value = m_OdoValue;
    *status = m_OdoValueSignalStatus;
}

void GCL_Read_OdoAndTripDisplayHandler_TripValue(uint32* value, uint8* status)
{
    *value = m_TripValue;
    *status = m_TripValueSignalStatus;
}

void GCL_Read_OdoAndTripDisplayHandler_InstantaneousFuelConsumption(uint16* value, uint8* status)
{
    *value = m_InstantaneousFuelConsumption;
    *status = m_InstantaneousFuelConsumptionSignalStatus;
}

void GCL_Read_OdoAndTripDisplayHandler_TripFuelConsumption(uint32* value, uint8* status)
{
    *value = m_TripFuelConsumption;
    *status = m_TripFuelConsumptionSignalStatus;
}

void GCL_Write_OdoAndTripDisplayHandler_TripFieldText(wchar* value)
{
    m_TripFieldText = value;
}

void GCL_Write_OdoAndTripDisplayHandler_OdoAndInstFuelFieldText(wchar* value)
{
    m_OdoAndInstFuelFieldText = value;
}


