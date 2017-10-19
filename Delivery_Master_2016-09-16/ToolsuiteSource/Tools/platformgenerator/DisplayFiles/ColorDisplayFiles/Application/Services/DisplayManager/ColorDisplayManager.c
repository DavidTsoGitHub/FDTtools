/********************************* Includes **********************************/
#include "DisplayManager.h"
#include "DisplayManager_cfg.h"
#include "StandardTypes.h"
#include "GFX.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    IDLE,
    APPLICATION_LAYER_UPDATING,
    UPDATING_DCU_SETTINGS
} state_e;

/********************************* Variables *********************************/
static state_e displayManagerState = IDLE;
static boolean m_layersUpdated = FALSE;

#ifndef __WIN32
#pragma push /* Save the current state */
#pragma force_active on
#pragma section data_type ".__gram_data" ".__gram_data"
__declspec (section ".__gram_data")
#endif
static uint8 activeFrameBuffer[BUFFER_SIZE];
static uint8 passiveFrameBuffer[BUFFER_SIZE];
#ifndef __WIN32
#pragma pop
#endif

/************************* Local Function Prototypes *************************/
static void dm_memset (uint8* dest, uint8 fill, uint32 count);

/***************************** Exported Functions ****************************/
void DisplayManager_Init(void)
{
    GFX_SetupColorPalette(DisplayManager_ColorCount, DisplayManager_CLUT);

    for(uint8 layerIndex = 0u; layerIndex < DisplayManager_LayerCount; ++layerIndex)
    {
        GFX_SetChromaKey(layerIndex, DisplayManager_CLUT[DisplayManager_ChromaKeyStartColorID], DisplayManager_CLUT[DisplayManager_ChromaKeyEndColorID]);
    }

    DisplayManager_ClearFrameBuffer();
}

void DisplayManager_Run(void)
{
    // Intentionally left empty. DisplayManager_UpdateLayers is called by DCU interrupt
}

uint32 DisplayManager_GetFrameBuffers(uint8** activeBuffer, uint8** passiveBuffer)
{
    *activeBuffer = activeFrameBuffer;
    *passiveBuffer = passiveFrameBuffer;

    return BUFFER_SIZE;
}

void DisplayManager_ClearFrameBuffer(void)
{
    dm_memset(activeFrameBuffer, DisplayManager_ChromaKeyStartColorID, BUFFER_SIZE);
    dm_memset(passiveFrameBuffer, DisplayManager_ChromaKeyStartColorID, BUFFER_SIZE);
}

void DisplayManager_ResetLayers(void)
{
    DisplayManager_BeginLayerConfiguration();

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
    if (m_layersUpdated == TRUE)
    {
        if (displayManagerState != IDLE)
        {
            return;
        }
        else
        {
            displayManagerState = UPDATING_DCU_SETTINGS;
        }

        for(uint8 layerIndex = 0u; layerIndex < DisplayManager_LayerCount; ++layerIndex)
        {
            if (DisplayManager_PassiveLayerConfig[layerIndex].isUpdated == FALSE)
            {
                continue;
            }

            const layer_metadata_t* passiveLayerMetadata = &DisplayManager_PassiveLayerConfig[layerIndex];
            GFX_SetupLayer(layerIndex, passiveLayerMetadata->configuration);
            DisplayManager_PassiveLayerConfig[layerIndex].isUpdated = FALSE;
        }

        displayManagerState = IDLE;
        m_layersUpdated = FALSE;
    }
}

void DisplayManager_SetLayersUpdated(void)
{
    m_layersUpdated = TRUE;
}

void DisplayManager_SetBackgroundColor(uint8 colorID)
{
    GFX_SetBackgroundColor(colorID);
}

void DisplayManager_SetDCUMode(uint8 mode)
{
    GFX_SetDCUMode(mode);
}

void DisplayManager_ForceUpdateLayers(void)
{
    m_layersUpdated = TRUE;
    DisplayManager_UpdateLayers();
}

/****************************** Local Functions ******************************/

static void dm_memset (uint8* dest, uint8 fill, uint32 count)
{
    for (uint32 i = 0u; i < count; i++)
    {
        dest[i] = fill;
    }
}
