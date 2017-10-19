/********************************* Includes **********************************/
#include "GFX.h"
#include "DCU_IF.h"

extern void DisplayManager_UpdateLayers(void);

/**************************** Constants and Types ****************************/
/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void GFX_SetupLayer(uint8 layerID, layer_configuration_t layerConfig)
{
    DCU_IF_SetupLayer(layerID, layerConfig.x, layerConfig.y, layerConfig.width, layerConfig.height, (uint8)layerConfig.bpp, layerConfig.opacity, 0, layerConfig.data, layerConfig.isEnabled);
}

void GFX_SetupColorPalette(uint8 colorCount, uint32* colorData)
{
    DCU_IF_SetCLUT(colorCount, colorData);
}

void GFX_SetBackgroundColor(uint8 colorID)
{
    DCU_IF_SetBackgroundColor(colorID);
}

void GFX_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor)
{
    DCU_IF_SetChromaKey(layerID, startColor, endColor);
}

void GFX_SetBlendLayers(uint8 layerCount)
{
    DCU_IF_SetNumberOfLayersToBlend(layerCount);
}

/*void GFX_UpdateFB(uint8 layerID, uint8* buffer)
{
    DCU_IF_SetLayerBuffer(layerID, buffer);
}*/

void GFX_ReDrawAllowed()
{
    DisplayManager_UpdateLayers();
}

void GFX_RequestUpdateSettings()
{
    // DisplayManager_UpdateLayerSettings();
}

void GFX_SetDCUMode(uint8 mode)
{
    DCU_IF_SetDCUMode(mode);
}

/*
void GFX_SetLayerEnabled(uint8 layerID, boolean status)
{
    DCU_IF_SetLayerEnabled(layerID, status);
}
*/
