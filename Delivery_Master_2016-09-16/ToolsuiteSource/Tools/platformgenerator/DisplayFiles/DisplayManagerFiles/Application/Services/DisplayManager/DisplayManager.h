#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "LayerConfiguration.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void DisplayManager_Init(void);
void DisplayManager_Run(void);

uint32 DisplayManager_GetFrameBuffers(uint8** activeBuffer, uint8** passiveBuffer);
void DisplayManager_ClearFrameBuffer(void);
void DisplayManager_ResetLayers(void);
void DisplayManager_SetLayersUpdated(void);
void DisplayManager_SetBackgroundColor(uint8 colorID);
void DisplayManager_SetDCUMode(uint8 mode);
void DisplayManager_ForceUpdateLayers(void);
void DisplayManager_ClearScreen(void);

void DisplayManager_BeginLayerConfiguration(void);
void DisplayManager_SetLayer(uint8 layerIndex, const layer_configuration_t* config);
void DisplayManager_SetLayerDisabled(uint8 layerIndex);
void DisplayManager_EndLayerConfiguration(void);

void DisplayManager_UpdateLayers(void);

#endif // DISPLAYMANAGER_H
