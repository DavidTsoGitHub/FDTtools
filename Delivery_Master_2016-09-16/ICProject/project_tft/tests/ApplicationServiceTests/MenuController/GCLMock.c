#include "GCLMock.h"
#include "GCL_MenuController.h"

static uint8 MenuControllerUp = 0;
static uint8 MenuControllerDown = 0;
static uint8 MenuControllerLeft = 0;
static uint8 MenuControllerRight = 0;
static uint8 MenuControllerSet = 0;
static uint8 MenuControllerClear = 0;
static uint8 currentLanguage = 0;
static uint16 vehSpd = 0;
static uint8 showDTC = 0;
static uint8 topMenuCancelCallCount = 0;

void GCLMock_Reset()
{
    MenuControllerUp = 0;
    MenuControllerDown = 0;
    MenuControllerLeft = 0;
    MenuControllerRight = 0;
    MenuControllerSet = 0;
    MenuControllerClear = 0;
    currentLanguage = 0;
    vehSpd = 0;
    showDTC = 0;
    topMenuCancelCallCount = 0;
}

void GCL_Read_MenuController_VehicleSpeed(uint16* value, uint8* status)
{
    *value = vehSpd;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_ShowDTC(uint8* value, uint8* status)
{
    *value = showDTC;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Up(uint8* value, uint8* status)
{
    *value = MenuControllerUp;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Down(uint8* value, uint8* status)
{
    *value = MenuControllerDown;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Left(uint8* value, uint8* status)
{
    *value = MenuControllerLeft;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Right(uint8* value, uint8* status)
{
    *value = MenuControllerRight;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Set(uint8* value, uint8* status)
{
    *value = MenuControllerSet;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_Clear(uint8* value, uint8* status)
{
    *value = MenuControllerClear;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_CurrentLanguage(uint8* value, uint8* status)
{
    *value = currentLanguage;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_MenuController_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = POWER_IGNITION_ON;
    *status = GCL_SIGNAL_OK;
}

void GCL_Write_MenuController_TopMenuCancel()
{
    topMenuCancelCallCount++;
}

void GCLMock_CurrentLanguage(uint8 languageId)
{
    currentLanguage = languageId;
}

void GCLMock_MenuButtonUp(uint8 buttonStatus)
{
    MenuControllerUp = buttonStatus;
    MenuController_Up_Indication();
}

void GCLMock_MenuButtonDown(uint8 buttonStatus)
{
    MenuControllerDown = buttonStatus;
    MenuController_Down_Indication();
}

void GCLMock_MenuButtonSet(uint8 buttonStatus)
{
    MenuControllerSet = buttonStatus;
    MenuController_Set_Indication();
}

void GCLMock_MenuButtonClear(uint8 buttonStatus)
{
    MenuControllerClear = buttonStatus;
    MenuController_Clear_Indication();
}

void GCLMock_VehicleSpeed(uint16 speed)
{
    vehSpd = speed;
}

uint8 GCLMock_TopMenuCancelCallCount()
{
    return topMenuCancelCallCount;
}


