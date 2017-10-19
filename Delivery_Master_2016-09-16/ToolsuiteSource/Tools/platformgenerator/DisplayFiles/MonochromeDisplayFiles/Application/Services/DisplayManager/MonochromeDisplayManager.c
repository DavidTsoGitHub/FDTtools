/********************************* Includes **********************************/
#include "DisplayManager.h"
#include "DisplayManager_cfg.h"
#include "StandardTypes.h"
#include "BSP.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    IDLE,
    APPLICATION_LAYER_UPDATING,
    UPDATING_DCU_SETTINGS
} state_e;

/********************************* Variables *********************************/
static state_e displayManagerState = IDLE;

static uint8* frameBuf = (uint8*)0;

/************************* Local Function Prototypes *************************/
static void dm_memset (uint8* dest, uint8 fill, uint32 count);

/***************************** Exported Functions ****************************/
void DisplayManager_Init(void)
{
    BSP_Get_Framebuffer(&frameBuf);
    
    DisplayManager_ClearFrameBuffer();
    BSP_Service_FramebufferUpdated();
}

void DisplayManager_Run(void)
{
    DisplayManager_UpdateLayers();
}

uint32 DisplayManager_GetFrameBuffers(uint8** frameBuffer, uint8** notUsedByMono)
{
    (void)notUsedByMono;
    *frameBuffer = frameBuf;
    return BUFFER_SIZE;
}

void DisplayManager_ClearFrameBuffer(void)
{
   dm_memset(frameBuf, 0xFF, BUFFER_SIZE);
}

void DisplayManager_ResetLayers(void)
{
    DisplayManager_BeginLayerConfiguration();

    DisplayManager_ClearFrameBuffer();

    for (uint8 layerIndex = 0u; layerIndex < DisplayManager_LayerCount; ++layerIndex)
    {
        DisplayManager_PassiveLayerConfig[layerIndex].configuration.isEnabled = FALSE;
        DisplayManager_PassiveLayerConfig[layerIndex].isUpdated = TRUE;
    }

    DisplayManager_EndLayerConfiguration();
}

void DisplayManager_BeginLayerConfiguration(void)
{
    while (displayManagerState != IDLE) {}

    displayManagerState = APPLICATION_LAYER_UPDATING;
}

void DisplayManager_SetLayer(uint8 layerIndex, const layer_configuration_t* config)
{
    DisplayManager_PassiveLayerConfig[layerIndex].configuration = *config;
    DisplayManager_PassiveLayerConfig[layerIndex].isUpdated = TRUE;
}

void DisplayManager_EndLayerConfiguration(void)
{
    displayManagerState = IDLE;
}

void DisplayManager_UpdateLayers()
{
    if (displayManagerState != IDLE)
    {
        return;
    }
    else
    {
        displayManagerState = UPDATING_DCU_SETTINGS;
    }

    boolean dirty = FALSE;
    
    for(uint8 layerIndex = 0u; layerIndex < DisplayManager_LayerCount; ++layerIndex)
    {
        if (DisplayManager_PassiveLayerConfig[layerIndex].isUpdated == FALSE)
        {
            continue;
        }
        
        dirty = TRUE;
        DisplayManager_PassiveLayerConfig[layerIndex].isUpdated = FALSE;
    }
    
    if (dirty == TRUE)
    {
        BSP_Service_FramebufferUpdated();
    }

    displayManagerState = IDLE;
}

/****************************** Local Functions ******************************/
static void dm_memset (uint8* dest, uint8 fill, uint32 count)
{
    for (uint32 i = 0u; i < count; i++)
    {
        dest[i] = fill;
    }
}
