#include "jdp.h"
#include "SIUL.h"
#include "DCU.h"
#include "DCU_IF.h"

extern void GFX_ReDrawAllowed(void);
extern void GFX_RequestUpdateSettings(void);

void DCU_IF_Init(void)
{
    DCU.INTMASK.R = 0xFFFFF;
    DCU.INTMASK.B.MLSBFVS = 0x00;
    DCU.INTMASK.B.MVSBLANK = 0x00;
}


// Interupt handler for DCU interupts
void DCU_ISR(void)
{
    // if( DCU.INTSTATUS.B.VSYNC != 0u )
    // {
    //     if (DCU_CallbackVSYNC != NULL)
    //     {
    //         DCU_CallbackVSYNC();
    //     }
    //     DCU.INTSTATUS.R = 0x000001u;
    // }
    if( DCU.INTSTATUS.B.LSBFVS != 0u )
    {
        GFX_ReDrawAllowed();
        DCU.INTSTATUS.R = 0x000004u;
    }
    if( DCU.INTSTATUS.B.VSBLANK != 0u )
    {
        //GFX_RequestUpdateSettings();
        DCU.INTSTATUS.R = 0x000008u;
    }
   
    
    // if( DCU.INTSTATUS.B.PROGEND != 0u )
    // {   
    //     if (DCU_CallbackPROGD != NULL)
    //     {
    //         DCU_CallbackPROGD( );
    //     }
    //     DCU.INTSTATUS.R = 0x00000400u;
    // }
}

void DCU_IF_SetupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, uint8 bpp, uint8 transparency, uint8 clutOffset, uint8* buffer, boolean enabled)
{
    if(transparency != 255)
    {
        DCU.LAYER[layerID].CTRLDESCL4.B.AB = 0x2;
    }
    else
    {
        DCU.LAYER[layerID].CTRLDESCL4.B.AB = 0x0;
    }

    DCU.LAYER[layerID].CTRLDESCL1.B.HEIGHT = height;
    DCU.LAYER[layerID].CTRLDESCL1.B.WIDTH = width;
    DCU.LAYER[layerID].CTRLDESCL2.B.POSY = y;
    DCU.LAYER[layerID].CTRLDESCL2.B.POSX = x;
    DCU.LAYER[layerID].CTRLDESCL3.R = (uint32_t) buffer;
    DCU.LAYER[layerID].CTRLDESCL4.B.LUOFFS = clutOffset;
    DCU.LAYER[layerID].CTRLDESCL4.B.BPP = bpp;
    DCU.LAYER[layerID].CTRLDESCL4.B.TRANS = transparency;
    DCU.LAYER[layerID].CTRLDESCL4.B.EN = (enabled == TRUE) ? 1 : 0;

    DCU.LAYER[layerID].CTRLDESCL4.B.BB = 1u;
}

void DCU_IF_SetCLUT(uint8 colorCount, uint32* clutData)
{
    DCU_Modeswitch(0);
    for (uint8 i = 0u; i < colorCount; ++i)
    {
        DCU.CLUT[i] = clutData[i];
    }
    DCU_Modeswitch(1);
}

void DCU_IF_SetBackgroundColor(uint8 clutID)
{
    DCU.BGND.R = DCU.CLUT[clutID];
}

void DCU_IF_SetChromaKey(uint8 layerID, uint32 startColor, uint32 endColor)
{
    //Setting an hysteres on each color component of 1
    uint8 rMax = (uint8)((0x00FF0000u & endColor)>>16u);
    uint8 rMin = (uint8)((0x00FF0000u & startColor)>>16u);
    uint8 gMax = (uint8)((0x0000FF00u & endColor)>>8u);
    uint8 gMin = (uint8)((0x0000FF00u & startColor)>>8u);
    uint8 bMax = (uint8)(0x000000FFu & endColor);
    uint8 bMin = (uint8)(0x000000FFu & startColor);

    DCU.LAYER[layerID].CTRLDESCL5.B.CKMAXR = rMax;
    DCU.LAYER[layerID].CTRLDESCL6.B.CKMINR = rMin;
    DCU.LAYER[layerID].CTRLDESCL5.B.CKMAXG = gMax;
    DCU.LAYER[layerID].CTRLDESCL6.B.CKMING = gMin;
    DCU.LAYER[layerID].CTRLDESCL5.B.CKMAXB = bMax;
    DCU.LAYER[layerID].CTRLDESCL6.B.CKMINB = bMin;
}

void DCU_IF_SetNumberOfLayersToBlend(uint8 layerCount)
{
    if ((layerCount >= 2) && (layerCount <= 4))
    {
        DCU.DCUMODE.B.BLEND_ITER = layerCount;
    }
}

void DCU_IF_SetLayerBuffer(uint8 layerID, uint8* buffer)
{
    DCU.LAYER[layerID].CTRLDESCL3.R = (uint32_t) buffer;
}

void DCU_IF_SetLayerEnabled(uint8 layerID, boolean status)
{
    DCU.LAYER[layerID].CTRLDESCL4.B.EN = status;
}

void DCU_IF_SetDCUMode(uint8 mode)
{
    DCU_Modeswitch(mode);
}
