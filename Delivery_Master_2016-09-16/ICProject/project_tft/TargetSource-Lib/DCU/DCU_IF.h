#include "StandardTypes.h"

void DCU_IF_Init(void);
void DCU_IF_SetupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 transparency, uint8 clutOffset, uint8* buffer, boolean enabled);
void DCU_IF_SetCLUT(uint8 colorCount, uint32* clutData);
void DCU_IF_SetNumberOfLayersToBlend(uint8 layerCount);
void DCU_IF_SetLayerBuffer(uint8 layerID, uint8* buffer);
void DCU_IF_SetLayerEnabled(uint8 layerID, boolean status);
