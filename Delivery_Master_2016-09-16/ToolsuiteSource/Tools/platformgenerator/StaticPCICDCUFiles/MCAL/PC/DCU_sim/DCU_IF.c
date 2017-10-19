#include "DCU_IF.h"
#include "PC_BSP.h"

void DCU_IF_Init(void)
{
}

// Interupt handler for DCU interupts
void DCU_ISR(void)
{
    //if( DCU.INTSTATUS.B.LSBFVS != 0u )
    //{
        //GFX_ReDrawAllowed();
    //}
}

void DCU_IF_SetupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 opacity, uint8 clutOffset, uint8* buffer, boolean enabled)
{
    PCIC_SetLayer(layerID, x, y, width, height, bpp, opacity, clutOffset, buffer, enabled);
}

void DCU_IF_SetCLUT(uint8 colorCount, uint32* clutData)
{
    PCIC_SetCLUT(colorCount, clutData);
}

void DCU_IF_SetBackgroundColor(uint8 clutID)
{
    PCIC_SetBackgroundColor(clutID);
}

void DCU_IF_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor)
{
    PCIC_SetChromaKey(layerID, startColor, endColor);
}

void DCU_IF_SetNumberOfLayersToBlend(uint8 layerCount)
{
    if ((layerCount >= 2) && (layerCount <= 4))
    {
        //DCU.DCUMODE.B.BLEND_ITER = layerCount;
    }
}

/*void DCU_IF_SetLayerBuffer(uint8 layerID, uint8* buffer)
{
    PCIC_SetLayerBuffer(layerID, buffer);

    //DCU.LAYER[layerID].CTRLDESCL3.R = (uint32_t) buffer;
}*/

void DCU_IF_SetLayerEnabled(uint8 layerID, boolean status)
{
    PCIC_SetLayerEnabled(layerID, status);

    //DCU.LAYER[layerID].CTRLDESCL4.B.EN = status;
}

void DCU_IF_SetDCUMode(uint8 mode)
{
    (void)mode;
}
