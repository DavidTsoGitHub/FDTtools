#include "StandardTypes.h"
#include "LayerConfiguration.h"

typedef void (*UpdateCallback)(void);

void GFX_UpdateFB(uint8 areaID, uint8* buffer);
void GFX_SetupLayer(uint8 areaID, layer_configuration_t layerConfig);
void GFX_SetupColorPalette(uint8 colorCount, uint32* colorData);
void GFX_SetBackgroundColor(uint8 colorID);
void GFX_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor);
void GFX_SetBlendLayers(uint8 layerCount);
void GFX_RequestUpdateSettings(void);
void GFX_ReDrawAllowed(void);
void GFX_SetAreaEnabled(uint8 areaID, boolean status);
void GFX_SetDCUMode(uint8 mode);
