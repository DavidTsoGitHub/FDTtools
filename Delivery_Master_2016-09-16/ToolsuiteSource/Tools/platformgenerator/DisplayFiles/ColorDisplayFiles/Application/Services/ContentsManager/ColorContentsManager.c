//lint -save
/********************************* Includes **********************************/
#include "LayerConfiguration.h"
#include "ContentsManager.h"
#include "ContentsManager_cfg.h"
#include "DisplayManager.h"
#include "DisplayManager_cfg.h"
#include "StandardTypes.h"
#include "LabelManager.h"
#include "UIWidget.h"
#include "Logger.h"
#include "systime.h"
#include "LayoutManager.h"
#include "GCL_LayoutManager.h"

/**************************** Constants and Types ****************************/
#define CONTENTSMANAGER_LOG_NAME "ContentsManager"
#define DCU_DISABLED     0u
#define DCU_ENABLED      1u
/********************************* Variables *********************************/
//extern const uint8 DisplayManager_ColorCount;
static const display_layout_definition_t* m_currentLayout = NULL;
static uint8 m_checkMessageAreaIndex = 255;
static uint8* passiveFrameBuffer;
static uint8* activeFrameBuffer;
static uint8 m_currentLanguage;
static sint8 m_currentLayoutID;
static layer_metadata_t ContentsManager_LayerMetadata[16];
static boolean m_showCheckMessage = FALSE;
static boolean m_languageUpdatePending = FALSE;

/************************* Local Function Prototypes *************************/
static void pushCurrentLayoutConfiguration(void);
static void setupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, BSP_DCU_BPPModes bpp, uint8* buffer, uint32 bufferOffset, uint32 bufferSize, uint8 opacity);

static void drawCurrentLayout(void);
static void refreshLayout(void);

static void drawArea(uint8 areaIndex);
static void refreshArea(uint8 areaIndex);

static void resetLayerConfiguration(void);
static void setupAreaFrameBuffers(void);
static void calculateWritableAreas(void);
static void calculateWritableArea(display_area_definition_t* area);
static void drawBorder(const display_area_definition_t *area);
static void setPixelInArea(const display_area_definition_t* area, uint16 x, uint16 y, uint8 colorID);
static void drawLineInArea(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID);
static void clearPixel(const display_area_definition_t *area, uint16 x, uint16 y, uint8 colorID);
static void clearRectangle(const display_area_definition_t *area, uint16 x, uint16 y, uint16 width, uint16 height, uint8 colorID);
static void dm_memset(uint8* dest, uint8 fill, uint32 count);
static uint16 getAbsoluteValue(sint16 value);
static boolean languageChanged(void);

/***************************** Exported Functions ****************************/
void ContentsManager_Init(void)
{
    m_currentLayoutID = -1;
    DisplayManager_Init();

    calculateWritableAreas();

    DisplayManager_GetFrameBuffers(&activeFrameBuffer, &passiveFrameBuffer);

    uint8 u8SignalStatus;
    GCL_Read_LayoutManager_CurrentLanguage(&m_currentLanguage, &u8SignalStatus);
    LabelManager_SetLanguage(m_currentLanguage);
}

void ContentsManager_Run(void)
{
    static sint8 newLayoutID = -1;

    if (LayoutManager_GetState() > DM_STATE_STARTUP_REQUESTED)
    {
        newLayoutID = LayoutManager_GetCurrentLayoutID();

        if (newLayoutID != m_currentLayoutID)
        {
            ContentsManager_SetCurrentLayout(newLayoutID);
        }
        else
        {
            refreshLayout();
            pushCurrentLayoutConfiguration();
        }
    }
}

void ContentsManager_SetLanguageAwaitingUpdate(uint8 newLanguage)
{
    if (newLanguage != m_currentLanguage)
    {
        m_languageUpdatePending = TRUE;
    }
}

boolean ContentsManager_GetLanguageAwaitingUpdate()
{
    return m_languageUpdatePending;
}

boolean ContentsManager_MenuLayoutActive(void)
{
    boolean active = FALSE;
    if(m_showCheckMessage == TRUE)
    {
        active = FALSE;
    }
#ifdef HAS_MENU
    else if (m_currentLayoutID == MENU_LAYOUT_INDEX)
    {
        active = TRUE;
    }
#endif
    else
    {
        ;//Do Nothing!
    }


    return active;
}

void ContentsManager_SetLayerDirty(sint8 layerID)
{
    for(uint8 areaIndex = 0u; areaIndex < m_currentLayout->areaCount; ++areaIndex)
    {
        display_area_definition_t* area = &m_currentLayout->areas[areaIndex];
        if (area->layerID == layerID)
        {
            uint8* tmpBuffer = area->activeBuffer;
            area->activeBuffer = area->passiveBuffer;
            area->passiveBuffer = tmpBuffer;

            ContentsManager_LayerMetadata[area->layerID].configuration.data = area->activeBuffer;
        }
    }

    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
}

void ContentsManager_SetLayerEnabled(uint8 layerID, boolean enabled)
{
    ContentsManager_LayerMetadata[layerID].configuration.isEnabled = enabled;
    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
}

void ContentsManager_ClearDisplay(void)
{
    DisplayManager_ResetLayers();
    DisplayManager_SetLayersUpdated();
}

void ContentsManager_ClearArea(const display_area_definition_t* area)
{
    Logger_Info(CONTENTSMANAGER_LOG_NAME, "Clearing area in layer %d", area->layerID);

    dm_memset(area->passiveBuffer, area->bgColor, area->bufferSize);
    dm_memset(area->activeBuffer, area->bgColor, area->bufferSize);
}

void ContentsManager_ClearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height, uint8 colorID )
{
    clearRectangle(area, x, y, width, height, colorID);
}

void ContentsManager_DrawDot(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint8 colorID)
{
    uint16 xInFullArea = area->writableArea.x + xPos;
    uint16 yInFullArea = area->writableArea.y + yPos;

    setPixelInArea(area, xInFullArea, yInFullArea, colorID);
}

void ContentsManager_DrawLine(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID)
{
    uint16 xStartInFullArea = area->writableArea.x + xStart;
    uint16 xEndInFullArea = area->writableArea.x + xEnd;

    uint16 yStartInFullArea = area->writableArea.y + yStart;
    uint16 yEndInFullArea = area->writableArea.y + yEnd;

    drawLineInArea(area, xStartInFullArea, yStartInFullArea, xEndInFullArea, yEndInFullArea, colorID);
}

void ContentsManager_DrawBoxWithRoundCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    uint8 fgColor = fgColorID;
    uint8 bgColor = bgColorID;
    uint16 xEnd = xPos + width - 1u;
    uint16 yEnd = yPos + height - 1u;

    if (fill == TRUE)
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = (yPos + 2u); lineIndex <= (yEnd - 2u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos + 1u, lineIndex, xEnd - 1u, lineIndex, fgColor);
            }

            ContentsManager_DrawLine(area, xPos + 2u, yPos + 1u, xEnd - 2u, yPos + 1u, fgColor);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd - 1u, xEnd - 2u, yEnd - 1u, fgColor);

            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 1u, yPos, bgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, bgColor);
            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, bgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, bgColor);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 1u, yEnd, bgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, bgColor);
            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, bgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, bgColor);
        }
        else
        {
            for (uint16 lineIndex = (yPos + 2u); lineIndex <= (yEnd - 2u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, fgColor);
            }

            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, fgColor);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 2u, yEnd, fgColor);
            ContentsManager_DrawLine(area, xPos + 1u, yPos + 1u, xEnd - 1u, yPos + 1u, fgColor);
            ContentsManager_DrawLine(area, xPos + 1u, yEnd - 1u, xEnd - 1u, yEnd - 1u, fgColor);
        }
    }
    else
    {
        if (invertBorder == TRUE)
        {
            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, bgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, bgColor);

            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, bgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, bgColor);

            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 1u, yEnd, bgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, bgColor);

            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, bgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, bgColor);
        }
        else
        {
            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, fgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, fgColor);

            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, fgColor);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, fgColor);

            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 2u, yEnd, fgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, fgColor);

            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, fgColor);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, fgColor);
        }
    }
}

void ContentsManager_DrawBoxWithHardCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    uint8 fgColor = fgColorID;
    uint8 bgColor = bgColorID;
    uint16 xEnd = xPos + width - 1u;
    uint16 yEnd = yPos + height - 1u;

    if (fill == TRUE)
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = (yPos + 1u); lineIndex <= (yEnd - 1u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos + 1u, lineIndex, xEnd - 1u, lineIndex, fgColor);
            }

            ContentsManager_DrawLine(area, xPos, yPos, xEnd, yPos, bgColor);
            ContentsManager_DrawLine(area, xPos, yPos, xPos, yEnd, bgColor);
            ContentsManager_DrawLine(area, xPos, yEnd, xEnd, yEnd, bgColor);
            ContentsManager_DrawLine(area, xEnd, yPos, xEnd, yEnd, bgColor);
        }
        else
        {
            for (uint16 lineIndex = yPos; lineIndex <= yEnd; ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, fgColor);
            }
        }
    }
    else
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = yPos; lineIndex <= yEnd; ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, bgColor);
            }
        }
        else
        {
            ContentsManager_DrawLine(area, xPos, yPos, xEnd, yPos, fgColor);
            ContentsManager_DrawLine(area, xPos, yPos, xPos, yEnd, fgColor);
            ContentsManager_DrawLine(area, xPos, yEnd, xEnd, yEnd, fgColor);
            ContentsManager_DrawLine(area, xEnd, yPos, xEnd, yEnd, fgColor);
        }
    }
}

void ContentsManager_DrawTriangle(const display_area_definition_t* area, dm_triangle_direction_t triangleDirection, uint16 xStart, uint16 yStart, uint16 width, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    uint8 fgColor = fgColorID;
    uint8 bgColor = bgColorID;
    uint16 thisX = xStart;
    uint16 yEnd = 0;
    uint16 yPos = 0;
    uint16 xPos = 0;
    uint16 nbrOfPixels = 0;
    uint16 yTip = 0;
    uint16 compensate = 0;

    if ((width % 2u) != 0u)
    {
        nbrOfPixels = 0u;
        compensate = 0u;
    }
    else
    {
        nbrOfPixels = 1u;
        compensate = 1u;
    }

    switch (triangleDirection)
    {
        case DM_TRIANGLE_DIRECTION_UP:
            thisX = thisX + (width / 2u) - compensate;
            yEnd = yStart + (width / 2u) + (width % 2u);

            if (fill == TRUE)
            {
                if (invertBorder == TRUE)
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, bgColor);
                    for (yPos = (yStart + 1u); yPos < (yEnd - 1u); ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, bgColor);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, bgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                }
                else
                {
                    for (yPos = yStart; yPos < yEnd; ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
            }
            else
            {
                if (invertBorder == TRUE)
                {
                    for (yPos = yStart; yPos < yEnd; ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
                else
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, fgColor);
                    for (yPos = (yStart + 1u); yPos < (yEnd - 1u); ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, fgColor);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, fgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                }
            }
            break;

        case DM_TRIANGLE_DIRECTION_DOWN:
            thisX = thisX + (width / 2u) - compensate;
            yEnd = yStart + (width / 2u) + (width % 2u);

            --yEnd;
            --yStart;

            if (fill == TRUE)
            {
                if (invertBorder == TRUE)
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, bgColor);
                    for (yPos = (yEnd - 1u); yPos > (yStart + 1u); --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, bgColor);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, bgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                }
                else
                {
                    for (yPos = yEnd; yPos > yStart; --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
            }
            else
            {
                if (invertBorder == TRUE)
                {
                    for (yPos = yEnd; yPos > yStart; --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
                else
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, fgColor);
                    for (yPos = (yEnd - 1u); yPos > (yStart + 1u); --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, bgColor);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, fgColor);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, fgColor);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, fgColor);
                }
            }
            break;

        case DM_TRIANGLE_DIRECTION_LEFT:
            break;

        case DM_TRIANGLE_DIRECTION_RIGHT:
            yEnd = yStart + width - compensate;
            yTip = (uint16)(yStart + (uint16)((width >> 1u) + ((compensate == 0u) ? (uint16)1u : (uint16)0u)) - 1u);
            xPos = thisX;
            nbrOfPixels = 1u;

            if (fill == TRUE)
            {
                if (invertBorder == TRUE)
                {
                    ContentsManager_DrawLine(area, xPos, yStart, (xPos + nbrOfPixels), yStart, bgColor);
                    ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), (xPos + nbrOfPixels), (yEnd - nbrOfPixels), bgColor);
                    ++nbrOfPixels;

                    for(yPos = (yStart + 1u); yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, (xPos + 1u), yPos, ((xPos + nbrOfPixels) - (uint16)1u), yPos, fgColor);
                        ContentsManager_DrawLine(area, (xPos + 1u), (yEnd - nbrOfPixels), ((xPos + nbrOfPixels) - (uint16)1u), (yEnd - nbrOfPixels), fgColor);
                        ContentsManager_DrawDot(area, xPos, yPos, bgColor);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, yPos, bgColor);
                        ContentsManager_DrawDot(area, xPos, (yEnd - nbrOfPixels), bgColor);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, (yEnd - nbrOfPixels), bgColor);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, (xPos + 1u), yTip, ((xPos + nbrOfPixels) - (uint16)1u), yTip, fgColor);
                    ContentsManager_DrawDot(area, xPos, yTip, bgColor);
                    ContentsManager_DrawDot(area, xPos + nbrOfPixels, yTip, bgColor);
                }
                else
                {
                    for(yPos = yStart; yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, xPos, yPos, xPos + nbrOfPixels, yPos, fgColor);
                        ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), xPos + nbrOfPixels, (yEnd - nbrOfPixels), fgColor);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, xPos, yTip, xPos + nbrOfPixels, yTip, fgColor);
                }
            }
            else
            {
                if (invertBorder == TRUE)
                {
                    for(yPos = yStart; yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, xPos, yPos, xPos + nbrOfPixels, yPos, bgColor);
                        ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), xPos + nbrOfPixels, (yEnd - nbrOfPixels), bgColor);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, xPos, yTip, xPos + nbrOfPixels, yTip, bgColor);
                }
                else
                {
                    ContentsManager_DrawLine(area, xPos, yStart, (xPos + nbrOfPixels), yStart, fgColor);
                    ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), (xPos + nbrOfPixels), (yEnd - nbrOfPixels), fgColor);
                    ++nbrOfPixels;

                    for(yPos = (yStart + 1u); yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, (xPos + 1u), yPos, ((xPos + nbrOfPixels) - (uint16)1u), yPos, bgColor);
                        ContentsManager_DrawLine(area, (xPos + 1u), (yEnd - nbrOfPixels), ((xPos + nbrOfPixels) - (uint16)1u), (yEnd - nbrOfPixels), bgColor);
                        ContentsManager_DrawDot(area, xPos, yPos, fgColor);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, yPos, fgColor);
                        ContentsManager_DrawDot(area, xPos, (yEnd - nbrOfPixels), fgColor);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, (yEnd - nbrOfPixels), fgColor);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, (xPos + 1u), yTip, ((xPos + nbrOfPixels) - (uint16)1u), yTip, bgColor);
                    ContentsManager_DrawDot(area, xPos, yTip, fgColor);
                    ContentsManager_DrawDot(area, xPos + nbrOfPixels, yTip, fgColor);
                }
            }
            break;

        default:
            Logger_Error(CONTENTSMANAGER_LOG_NAME, "Unknown triangle direction");
            break;
    }
}

void ContentsManager_SetCurrentLayout(sint8 layoutID)
{
    m_currentLayoutID = layoutID;
    m_checkMessageAreaIndex = 255;

    if (m_currentLayoutID == -1)
    {
        ContentsManager_ClearDisplay();
        DisplayManager_SetLayersUpdated();
    }
    else
    {
        m_currentLayout = &ContentsManager_Layouts[layoutID];
        DisplayManager_SetDCUMode(DCU_DISABLED);
        resetLayerConfiguration();

        // Since we are setting a new layout, the framebuffers need to be initialized
        setupAreaFrameBuffers();

        for (uint8 areaId = 0; areaId < m_currentLayout->areaCount; ++areaId)
        {
            const display_area_definition_t* area = &m_currentLayout->areas[areaId];

            if(area->isCheckMessageArea == TRUE)
            {
                m_checkMessageAreaIndex = areaId;
            }

            for (uint8 i = 0; i < area->widgetCount; ++i)
            {
                const ui_widget_t* widget = &area->widgets[i];
                if (widget->layerID != area->layerID)
                {
                    setupLayer((uint8)widget->layerID, widget->x, widget->y, widget->width, widget->height, widget->bpp, NULL, 0, 0, 255);
                    ContentsManager_LayerMetadata[(uint8)widget->layerID].configuration.isEnabled = FALSE;
                    ContentsManager_LayerMetadata[(uint8)widget->layerID].configuration.isFrameBufferContents = FALSE;
                }
            }

            if (area->layerID != -1)
            {
                setupLayer((uint8)area->layerID, area->x, area->y, area->width, area->height, DCU_8_BPP, area->activeBuffer, area->bufferOffset, area->bufferSize, 255);
                if (area->isCheckMessageArea == TRUE)
                {
                    ContentsManager_LayerMetadata[(uint8)area->layerID].configuration.isEnabled = FALSE;
                }
                else
                {
                    ContentsManager_LayerMetadata[(uint8)area->layerID].configuration.isEnabled = TRUE;
                }
            }
        }

        DisplayManager_SetBackgroundColor(m_currentLayout->backgroundColorID);

        drawCurrentLayout();
        if (m_currentLayout->focusedWidget != NULL)
        {
            LayoutManager_SetWidgetFocus(m_currentLayout->focusedWidget, TRUE);
        }

        pushCurrentLayoutConfiguration();
        DisplayManager_ForceUpdateLayers();
        DisplayManager_SetDCUMode(DCU_ENABLED);
    }
}

void ContentsManager_SetLayerFrameBuffer(uint8 layerID, uint8* buffer)
{
    ContentsManager_LayerMetadata[layerID].configuration.data = buffer;
    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
}

boolean ContentsManager_ValidImageSize(uint8 layerID, uint16 width, uint16 height)
{
    boolean validImageSize = TRUE;

    if ((ContentsManager_LayerMetadata[layerID].configuration.width != width) || (ContentsManager_LayerMetadata[layerID].configuration.height != height))
    {
        validImageSize = FALSE;
    }

    return validImageSize;
}

void ContentsManager_SetLayerOpacity(uint8 layerID, uint8 opacity)
{
    ContentsManager_LayerMetadata[layerID].configuration.opacity = opacity;
    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
}

void ContentsManager_SetLayerPosition(uint8 layerID, uint16 x, uint16 y)
{
#ifdef DISPLAY_ROTATED
    ContentsManager_LayerMetadata[layerID].configuration.x = y;
    ContentsManager_LayerMetadata[layerID].configuration.y = DisplayManager_DisplayHeight - (x + ContentsManager_LayerMetadata[layerID].configuration.height);
#else
    ContentsManager_LayerMetadata[layerID].configuration.x = x;
    ContentsManager_LayerMetadata[layerID].configuration.y = y;
#endif
    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
}

void ContentsManager_SetLayerRelativePosition(uint8 layerID, sint16 x, sint16 y)
{
    uint16 absX = getAbsoluteValue(x);
    uint16 absY = getAbsoluteValue(y);

#ifdef DISPLAY_ROTATED
    if ((ContentsManager_LayerMetadata[layerID].configuration.originX < absY) || ((DisplayManager_DisplayHeight - ContentsManager_LayerMetadata[layerID].configuration.height) < absX))
    {
        return;
    }

    if (y < 0)
    {
        ContentsManager_LayerMetadata[layerID].configuration.x = ContentsManager_LayerMetadata[layerID].configuration.originX - absY;
    }
    else
    {
        ContentsManager_LayerMetadata[layerID].configuration.x = ContentsManager_LayerMetadata[layerID].configuration.originX + absY;
    }

    if (x < 0)
    {
        ContentsManager_LayerMetadata[layerID].configuration.y = ContentsManager_LayerMetadata[layerID].configuration.originY + absX;
    }
    else
    {
        ContentsManager_LayerMetadata[layerID].configuration.y = ContentsManager_LayerMetadata[layerID].configuration.originY - absX;
    }
#else
    if ((ContentsManager_LayerMetadata[layerID].configuration.originX < absX) || (ContentsManager_LayerMetadata[layerID].configuration.originY < absY))
    {
        return;
    }

    if (x < 0)
    {
        ContentsManager_LayerMetadata[layerID].configuration.x = ContentsManager_LayerMetadata[layerID].configuration.originX + absX;
    }
    else
    {
        ContentsManager_LayerMetadata[layerID].configuration.x = ContentsManager_LayerMetadata[layerID].configuration.originX - absX;
    }

    if (y < 0)
    {
        ContentsManager_LayerMetadata[layerID].configuration.y = ContentsManager_LayerMetadata[layerID].configuration.originY - absY;
    }
    else
    {
        ContentsManager_LayerMetadata[layerID].configuration.y = ContentsManager_LayerMetadata[layerID].configuration.originY + absY;
    }
#endif
    ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;

}

void ContentsManager_ShowCheckMessage(boolean show)
{
    m_showCheckMessage = show;

    if((m_showCheckMessage == TRUE) && (m_checkMessageAreaIndex != 255u))
    {
        const display_area_definition_t* area = &m_currentLayout->areas[m_checkMessageAreaIndex];
        if (area->layerID != -1)
        {
            ContentsManager_LayerMetadata[area->layerID].configuration.isEnabled = TRUE;
        }

        for (uint8 widgetIndex = 0; widgetIndex < area->widgetCount; ++widgetIndex)
        {
            ContentsManager_LayerMetadata[area->widgets[widgetIndex].layerID].configuration.isEnabled = TRUE;
        }

        drawArea(m_checkMessageAreaIndex);

        Logger_Info(CONTENTSMANAGER_LOG_NAME, "Showing CheckMessage");
    }
    else if(m_checkMessageAreaIndex != 255u)
    {
        const display_area_definition_t* area = &m_currentLayout->areas[m_checkMessageAreaIndex];
        if (area->layerID != -1)
        {
            ContentsManager_LayerMetadata[area->layerID].configuration.isEnabled = FALSE;
            ContentsManager_LayerMetadata[area->layerID].isUpdated = TRUE;
        }

        for (uint8 widgetIndex = 0; widgetIndex < area->widgetCount; ++widgetIndex)
        {
            ContentsManager_LayerMetadata[area->widgets[widgetIndex].layerID].configuration.isEnabled = FALSE;
            ContentsManager_LayerMetadata[area->widgets[widgetIndex].layerID].isUpdated = TRUE;
        }

        Logger_Info(CONTENTSMANAGER_LOG_NAME, "Clearing CheckMessage");
    }
    else
    {
        ; // Do nothing
    }
}

/****************************** Local Functions ******************************/
static void pushCurrentLayoutConfiguration(void)
{
    DisplayManager_BeginLayerConfiguration();
    boolean updateLayout = FALSE;

    for (uint8 layerID = 0u; layerID < DisplayManager_LayerCount; ++layerID)
    {
        layer_metadata_t* layer = &ContentsManager_LayerMetadata[layerID];
        if (layer->isUpdated == FALSE)
        {
            continue;
        }
        else
        {
            updateLayout = TRUE;
            DisplayManager_SetLayer(layerID, &layer->configuration);
            layer->isUpdated = FALSE;
        }
    }

    if (updateLayout == TRUE)
    {
        DisplayManager_SetLayersUpdated();
    }
    DisplayManager_EndLayerConfiguration();
}

static void setupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, BSP_DCU_BPPModes bpp, uint8* buffer, uint32 bufferOffset, uint32 bufferSize, uint8 opacity)
{
    ContentsManager_LayerMetadata[layerID].configuration.bpp = bpp;
    ContentsManager_LayerMetadata[layerID].configuration.data = buffer;
    ContentsManager_LayerMetadata[layerID].configuration.isFrameBufferContents = TRUE;
    //ContentsManager_LayerMetadata[layerID].configuration.isEnabled = TRUE;
    ContentsManager_LayerMetadata[layerID].configuration.frameBufferOffset = bufferOffset;
    ContentsManager_LayerMetadata[layerID].configuration.dataSize = bufferSize;
    ContentsManager_LayerMetadata[layerID].configuration.opacity = opacity;

#ifdef DISPLAY_ROTATED
    ContentsManager_LayerMetadata[layerID].configuration.x = y;
    ContentsManager_LayerMetadata[layerID].configuration.y = DisplayManager_DisplayHeight - (x + width);
    ContentsManager_LayerMetadata[layerID].configuration.width = height;
    ContentsManager_LayerMetadata[layerID].configuration.height = width;
#else
    ContentsManager_LayerMetadata[layerID].configuration.x = x;
    ContentsManager_LayerMetadata[layerID].configuration.y = y;
    ContentsManager_LayerMetadata[layerID].configuration.width = width;
    ContentsManager_LayerMetadata[layerID].configuration.height = height;
#endif

    ContentsManager_LayerMetadata[layerID].configuration.originX = ContentsManager_LayerMetadata[layerID].configuration.x;
    ContentsManager_LayerMetadata[layerID].configuration.originY = ContentsManager_LayerMetadata[layerID].configuration.y;
}

// REFRESH ------------------------------------------------------------------------------------------------------------

static void refreshLayout(void)
{
    if(languageChanged() == TRUE)
    {
        drawCurrentLayout();
        m_languageUpdatePending = FALSE;
    }
    else
    {
        for(uint8 areaIndex = 0u; areaIndex < m_currentLayout->areaCount; ++areaIndex)
        {
            refreshArea(areaIndex);
        }
    }
}

static void refreshArea(uint8 areaIndex)
{
    display_area_definition_t* area = &m_currentLayout->areas[areaIndex];

    if((area->isCheckMessageArea == FALSE) || ((area->isCheckMessageArea == TRUE) && (m_showCheckMessage == TRUE)))
    {
        boolean needsRedraw = FALSE;
        for (uint8 widgetIndex = 0u; widgetIndex < area->widgetCount; ++widgetIndex)
        {
            const ui_widget_t* widget = &area->widgets[widgetIndex];
            needsRedraw |= UIWidget_NeedsRedraw(widget);
        }

        if (needsRedraw == TRUE)
        {
            for (uint8 widgetIndex = 0u; widgetIndex < area->widgetCount; ++widgetIndex)
            {
                const ui_widget_t* widget = &area->widgets[widgetIndex];
                UIWidget_Redraw(area, widget);
                ContentsManager_LayerMetadata[widget->layerID].isUpdated = TRUE;
            }

            if (area->border > 0u)
            {
                drawBorder(area);
            }

            if (area->layerID != -1)
            {
                uint8* tmpBuffer = area->activeBuffer;
                area->activeBuffer = area->passiveBuffer;
                area->passiveBuffer = tmpBuffer;

                ContentsManager_LayerMetadata[area->layerID].configuration.data = area->activeBuffer;
            }
        }
    }
    else
    {
        ;//Do nothing
    }
}

// DRAW ---------------------------------------------------------------------------------------------------------------

static void drawCurrentLayout()
{
    for (uint8 areaIndex = 0u; areaIndex < m_currentLayout->areaCount; areaIndex++)
    {
        drawArea(areaIndex);
    }
}

static void drawArea(uint8 areaIndex)
{
    display_area_definition_t* area = &m_currentLayout->areas[areaIndex];

    ContentsManager_ClearArea(area);

    if((area->isCheckMessageArea == FALSE) || ((area->isCheckMessageArea == TRUE) && (m_showCheckMessage == TRUE)))
    {

        for (uint8 widgetIndex = 0u; widgetIndex < area->widgetCount; ++widgetIndex)
        {
            const ui_widget_t* widget = &area->widgets[widgetIndex];
            UIWidget_Draw(area, widget);
            ContentsManager_LayerMetadata[widget->layerID].isUpdated = TRUE;
        }

        if (area->border > 0u)
        {
            drawBorder(area);
        }

        if (area->layerID != -1)
        {
            uint8* tmpBuffer = area->activeBuffer;
            area->activeBuffer = area->passiveBuffer;
            area->passiveBuffer = tmpBuffer;

            ContentsManager_LayerMetadata[area->layerID].configuration.data = area->activeBuffer;
        }
    }
    else
    {
        ;//Do nothing
    }
}

// --------------------------------------------------------------------------------------------------------------------

static void calculateWritableAreas(void)
{
    for (sint8 i = 0; i < CONTENTS_MANAGER_LAYOUT_COUNT; ++i)
    {
        for (uint8 j = 0; j < ContentsManager_Layouts[i].areaCount; ++j)
        {
            display_area_definition_t* area = &ContentsManager_Layouts[i].areas[j];
            calculateWritableArea(area);
        }
    }
}

static void calculateWritableArea(display_area_definition_t* area)
{
    uint16 gap = 0u;
    if (area->border > 0u)
    {
        gap = 2u;
    }

    area->writableArea.x = gap;
    area->writableArea.y = gap;
    area->writableArea.width = (area->width - gap) - gap;
    area->writableArea.height = (area->height - gap) - gap;
}

static void drawBorder(const display_area_definition_t* area)
{
    uint16 xStart = 0;
    uint16 xEnd = area->width - 1u;
    uint16 yStart = 0;
    uint16 yEnd = area->height - 1u;

    drawLineInArea(area, xStart + 2u, yStart, xEnd - 2u, yStart, area->fgColor);
    setPixelInArea(area, xStart + 1u, yStart + 1u, area->fgColor);

    drawLineInArea(area, xStart, yStart + 2u, xStart, yEnd - 2u, area->fgColor);
    setPixelInArea(area, xStart + 1u, yEnd - 1u, area->fgColor);

    drawLineInArea(area, xStart + 2u, yEnd, xEnd - 2u, yEnd, area->fgColor);
    setPixelInArea(area, xEnd - 1u, yEnd - 1u, area->fgColor);

    drawLineInArea(area, xEnd, yStart + 2u, xEnd, yEnd - 2u, area->fgColor);
    setPixelInArea(area, xEnd - 1u, yStart + 1u, area->fgColor);
}

static void drawLineInArea(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID)
{
    if ((xStart == xEnd) && (yStart == yEnd))
    {
        setPixelInArea(area, xStart, yStart, colorID);
    }

    if (xStart == xEnd)
    {
        for(uint16 i = yStart; i <= yEnd; ++i)
        {
            setPixelInArea(area, xStart, i, colorID);
        }
    }
    else if (yStart == yEnd)
    {
        for(uint16 i = xStart; i <= xEnd; ++i)
        {
            setPixelInArea(area, i, yStart, colorID);
        }
    }
    else
    {
        Logger_Info(CONTENTSMANAGER_LOG_NAME, "No support for diagonal lines");
    }
}

static void setPixelInArea(const display_area_definition_t *area, uint16 x, uint16 y, uint8 colorID)
{
    uint16 posX = x;
    uint16 posY = y;

#ifdef DISPLAY_ROTATED
    uint16 xTemp = posX;
    posX = posY;
    posY = (uint16)((uint16)area->width - (uint16)1u) - xTemp;
    area->passiveBuffer[(posY * area->height) + posX] = colorID;
#else
    area->passiveBuffer[(posY * area->width) + posX] = colorID;
#endif

}

static void clearPixel(const display_area_definition_t* area, uint16 x, uint16 y, uint8 colorID)
{
    uint16 posX = x;
    uint16 posY = y;

#ifdef DISPLAY_ROTATED
    uint16 xTemp = posX;
    posX = posY;
    posY = (uint16)( (uint16)area->width - (uint16)1u ) - xTemp;
    area->passiveBuffer[(posY * area->height) + posX] = colorID;
#else
    area->passiveBuffer[(posY * area->width) + posX] = colorID;
#endif

}

static void resetLayerConfiguration(void)
{
    for (uint8 layerID = 0; layerID < DisplayManager_LayerCount; ++layerID)
    {
        ContentsManager_LayerMetadata[layerID].configuration.isEnabled = FALSE;
        ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
    }
}

static void setupAreaFrameBuffers(void)
{
    DisplayManager_ClearFrameBuffer();
    for (uint8 areaIndex = 0u; areaIndex < m_currentLayout->areaCount; ++areaIndex)
    {
        display_area_definition_t* area = &m_currentLayout->areas[areaIndex];
        area->passiveBuffer = &passiveFrameBuffer[area->bufferOffset];
        area->activeBuffer = &activeFrameBuffer[area->bufferOffset];
    }
}

static void clearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height, uint8 colorID)
{
    for (uint16 row = x; row < (x + width); ++row)
    {
        for (uint16 col = y; col < (y + height); ++col)
        {
            clearPixel(area, row, col, colorID);
        }
    }
}

static boolean languageChanged(void)
{
    uint8 u8CurrentLanguage, u8SignalStatus;
    boolean returnValue = FALSE;

    GCL_Read_LayoutManager_CurrentLanguage(&u8CurrentLanguage, &u8SignalStatus);

    if (u8CurrentLanguage != m_currentLanguage)
    {
        m_currentLanguage = u8CurrentLanguage;
        LabelManager_SetLanguage(m_currentLanguage);
        returnValue = TRUE;
    }

    return returnValue;
}

static void dm_memset(uint8* dest, uint8 fill, uint32 count)
{
    for (uint32 i = 0u; i < count; i++)
    {
        dest[i] = fill;
    }
}

static uint16 getAbsoluteValue(sint16 value)
{
    //lint --e(9033) [Convert signed to unsigned, used for absolute coordinate calculations] --e(571) [- '' -]
    return (uint16)((value < 0) ? (0 - value) : value);
}

//lint -restore
