#ifndef GCLMOCK_H
#define GCLMOCK_H

#include "Platform_Types.h"

void GCLMock_Reset(void);

void GCLMock_CurrentLanguage(uint8 languageId);

void GCLMock_MenuButtonUp(uint8 buttonStatus);
void GCLMock_MenuButtonDown(uint8 buttonStatus);
void GCLMock_MenuButtonLeft(uint8 buttonStatus);
void GCLMock_MenuButtonRight(uint8 buttonStatus);
void GCLMock_MenuButtonSet(uint8 buttonStatus);
void GCLMock_MenuButtonClear(uint8 buttonStatus);
void GCLMock_VehicleSpeed(uint16 speed);
uint8 GCLMock_TopMenuCancelCallCount();

#endif