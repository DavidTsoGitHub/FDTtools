/********************************* Includes **********************************/
#include "ContentsManager.h"
#include "ContentsManager_cfg.h"
#include "DisplayManager.h"
#include "DisplayManager_cfg.h"
#include "StandardTypes.h"
#include "ResourceManager.h"
#include "LabelManager.h"
#include "UIWidget.h"
#include "Logger.h"
#include "systime.h"
#include "LayoutManager.h"

/**************************** Constants and Types ****************************/
#define CONTENTSMANAGER_LOG_NAME "ContentsManager"

#define ALL_PIXELS_OFF_IN_BYTE  0xFF
#define ALL_PIXELS_ON_IN_BYTE   0x00

/********************************* Variables *********************************/
static const display_layout_definition_t* currentLayout = NULL;
static uint8* frameBuffer = NULL;
static uint8 m_currentLanguage;
static sint8 m_currentLayoutID = -1;
static uint8 m_checkMessageAreaIndex = 255;
static boolean m_showCheckMessage = FALSE;
static boolean m_languageUpdatePending = FALSE;

static layer_metadata_t ContentsManager_LayerMetadata[1];

/************************* Local Function Prototypes *************************/
static void pushCurrentLayoutConfiguration(void);
static void setupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, BSP_DCU_BPPModes bpp, uint8* buffer, uint32 bufferOffset, uint32 bufferSize);

static void drawCurrentLayout(void);
static void refreshLayout(void);

static void drawArea(uint8 areaIndex);
static void refreshArea(uint8 areaIndex);
static void clearArea(uint8 areaIndex);

static void resetLayerConfiguration(void);
static void calculateWritableAreas(void);
static void calculateWritableArea(display_area_definition_t *area);
static void drawBorder(const display_area_definition_t* area);
static void setPixelInArea(const display_area_definition_t* area, uint16 x, uint16 y, uint8 colorID);
static void drawLineInArea(const display_area_definition_t *area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID);
static void clearPixel(const display_area_definition_t* area, uint16 x, uint16 y);
static void clearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height);

static boolean languageChanged(void);

/***************************** Exported Functions ****************************/
void ContentsManager_Init(void)
{
    calculateWritableAreas();

    DisplayManager_GetFrameBuffers(&frameBuffer, NULL);

    uint8 u8SignalStatus;
    GCL_Read_LayoutManager_CurrentLanguage(&m_currentLanguage, &u8SignalStatus);
    LabelManager_SetLanguage(m_currentLanguage);
}

void ContentsManager_Run(void)
{
    static sint8 newLayoutID = -1;
    static boolean ignitionOn = FALSE;

    if (LayoutManager_GetState() > DM_STATE_STARTUP_REQUESTED)
    {
        ignitionOn = TRUE;
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
    else
    {
        if (ignitionOn == TRUE)
        {
            m_currentLayoutID = -1;
            ignitionOn = FALSE;
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

    if (m_showCheckMessage == TRUE)
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
    (void)layerID; // SW rendering == 1 layer
    ContentsManager_LayerMetadata[0].isUpdated = TRUE;
}

void ContentsManager_ClearDisplay(void)
{
    DisplayManager_ResetLayers();
}

void ContentsManager_ClearArea(const display_area_definition_t* area)
{
    //Logger_Info(CONTENTSMANAGER_LOG_NAME, "Clearing area %d", area->);

    clearRectangle(area,
                   area->writableArea.x,
                   area->writableArea.y,
                   area->writableArea.width,
                   area->writableArea.height);
}

void ContentsManager_ClearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height, uint8 colorID)
{
    UNUSED(colorID);
    clearRectangle(area, area->writableArea.x + x, area->writableArea.y + y, width, height);
}

void ContentsManager_DrawDot(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint8 colorID)
{
    uint16 xInFullArea = (area->writableArea.x - area->x) + xPos;
    uint16 yInFullArea = (area->writableArea.y - area->y) + yPos;

    setPixelInArea(area, xInFullArea, yInFullArea, colorID);
}

void ContentsManager_DrawLine(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID)
{
    uint16 xStartInFullArea = (area->writableArea.x - area->x) + xStart;
    uint16 xEndInFullArea = (area->writableArea.x - area->x) + xEnd;

    uint16 yStartInFullArea = (area->writableArea.y - area->y) + yStart;
    uint16 yEndInFullArea = (area->writableArea.y - area->y) + yEnd;

    drawLineInArea(area, xStartInFullArea, yStartInFullArea, xEndInFullArea, yEndInFullArea, colorID);
}

void ContentsManager_DrawBoxWithRoundCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    (void)fgColorID;
    (void)bgColorID;

    uint16 xEnd = xPos + width - 1u;
    uint16 yEnd = yPos + height - 1u;

    if (fill == TRUE)
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = (yPos + 2u); lineIndex <= (yEnd - 2u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos + 1u, lineIndex, xEnd - 1u, lineIndex, DM_PIXEL_STATUS_ON);
            }

            ContentsManager_DrawLine(area, xPos + 2u, yPos + 1u, xEnd - 2u, yPos + 1u, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd - 1u, xEnd - 2u, yEnd - 1u, DM_PIXEL_STATUS_ON);

            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 1u, yPos, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 1u, yEnd, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, DM_PIXEL_STATUS_OFF);
        }
        else
        {
            for (uint16 lineIndex = (yPos + 2u); lineIndex <= (yEnd - 2u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, DM_PIXEL_STATUS_ON);
            }

            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 2u, yEnd, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos + 1u, yPos + 1u, xEnd - 1u, yPos + 1u, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos + 1u, yEnd - 1u, xEnd - 1u, yEnd - 1u, DM_PIXEL_STATUS_ON);
        }
    }
    else
    {
        if (invertBorder == TRUE)
        {
            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, DM_PIXEL_STATUS_OFF);

            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, DM_PIXEL_STATUS_OFF);

            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 1u, yEnd, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, DM_PIXEL_STATUS_OFF);

            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, DM_PIXEL_STATUS_OFF);
        }
        else
        {
            ContentsManager_DrawLine(area, xPos + 2u, yPos, xEnd - 2u, yPos, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawDot(area, xPos + 1u, yPos + 1u, DM_PIXEL_STATUS_ON);

            ContentsManager_DrawLine(area, xPos, yPos + 2u, xPos, yEnd - 2u, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawDot(area, xPos + 1u, yEnd - 1u, DM_PIXEL_STATUS_ON);

            ContentsManager_DrawLine(area, xPos + 2u, yEnd, xEnd - 2u, yEnd, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawDot(area, xEnd - 1u, yEnd - 1u, DM_PIXEL_STATUS_ON);

            ContentsManager_DrawLine(area, xEnd, yPos + 2u, xEnd, yEnd - 2u, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawDot(area, xEnd - 1u, yPos + 1u, DM_PIXEL_STATUS_ON);
        }
    }
}

void ContentsManager_DrawBoxWithHardCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    (void)fgColorID;
    (void)bgColorID;

    uint16 xEnd = xPos + width - 1u;
    uint16 yEnd = yPos + height - 1u;

    if (fill == TRUE)
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = (yPos + 1u); lineIndex <= (yEnd - 1u); ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos + 1u, lineIndex, xEnd - 1u, lineIndex, DM_PIXEL_STATUS_ON);
            }

            ContentsManager_DrawLine(area, xPos, yPos, xEnd, yPos, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xPos, yPos, xPos, yEnd, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xPos, yEnd, xEnd, yEnd, DM_PIXEL_STATUS_OFF);
            ContentsManager_DrawLine(area, xEnd, yPos, xEnd, yEnd, DM_PIXEL_STATUS_OFF);
        }
        else
        {
            for (uint16 lineIndex = yPos; lineIndex <= yEnd; ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, DM_PIXEL_STATUS_ON);
            }
        }
    }
    else
    {
        if (invertBorder == TRUE)
        {
            for (uint16 lineIndex = yPos; lineIndex <= yEnd; ++lineIndex)
            {
                ContentsManager_DrawLine(area, xPos, lineIndex, xEnd, lineIndex, DM_PIXEL_STATUS_OFF);
            }
        }
        else
        {
            ContentsManager_DrawLine(area, xPos, yPos, xEnd, yPos, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos, yPos, xPos, yEnd, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xPos, yEnd, xEnd, yEnd, DM_PIXEL_STATUS_ON);
            ContentsManager_DrawLine(area, xEnd, yPos, xEnd, yEnd, DM_PIXEL_STATUS_ON);
        }
    }
}

void ContentsManager_DrawTriangle(const display_area_definition_t* area, dm_triangle_direction_t triangleDirection, uint16 xStart, uint16 yStart, uint16 width, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID)
{
    (void)fgColorID;
    (void)bgColorID;

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
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, DM_PIXEL_STATUS_OFF);
                    for (yPos = (yStart + 1u); yPos < (yEnd - 1u); ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, DM_PIXEL_STATUS_OFF);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                }
                else
                {
                    for (yPos = yStart; yPos < yEnd; ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
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
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
                else
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, DM_PIXEL_STATUS_ON);
                    for (yPos = (yStart + 1u); yPos < (yEnd - 1u); ++yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, DM_PIXEL_STATUS_ON);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
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
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, DM_PIXEL_STATUS_OFF);
                    for (yPos = (yEnd - 1u); yPos > (yStart + 1u); --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, DM_PIXEL_STATUS_OFF);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                }
                else
                {
                    for (yPos = yEnd; yPos > yStart; --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
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
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                }
                else
                {
                    ContentsManager_DrawLine(area, thisX, yStart, thisX + nbrOfPixels, yStart, DM_PIXEL_STATUS_ON);
                    for (yPos = (yEnd - 1u); yPos > (yStart + 1u); --yPos)
                    {
                        ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, thisX - 1u, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, thisX + nbrOfPixels + 1u, yPos, DM_PIXEL_STATUS_ON);
                        --thisX;
                        nbrOfPixels = nbrOfPixels + 2u;
                    }
                    ContentsManager_DrawLine(area, thisX, yPos, thisX + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
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
                    ContentsManager_DrawLine(area, xPos, yStart, (xPos + nbrOfPixels), yStart, DM_PIXEL_STATUS_OFF);
                    ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), (xPos + nbrOfPixels), (yEnd - nbrOfPixels), DM_PIXEL_STATUS_OFF);
                    ++nbrOfPixels;

                    for(yPos = (yStart + 1u); yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, (xPos + 1u), yPos, ((xPos + nbrOfPixels) - (uint16)1u), yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawLine(area, (xPos + 1u), (yEnd - nbrOfPixels), ((xPos + nbrOfPixels) - (uint16)1u), (yEnd - nbrOfPixels), DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, xPos, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, xPos, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_OFF);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, (xPos + 1u), yTip, ((xPos + nbrOfPixels) - (uint16)1u), yTip, DM_PIXEL_STATUS_ON);
                    ContentsManager_DrawDot(area, xPos, yTip, DM_PIXEL_STATUS_OFF);
                    ContentsManager_DrawDot(area, xPos + nbrOfPixels, yTip, DM_PIXEL_STATUS_OFF);
                }
                else
                {
                    for(yPos = yStart; yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, xPos, yPos, xPos + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), xPos + nbrOfPixels, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_ON);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, xPos, yTip, xPos + nbrOfPixels, yTip, DM_PIXEL_STATUS_ON);
                }
            }
            else
            {
                if (invertBorder == TRUE)
                {
                    for(yPos = yStart; yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, xPos, yPos, xPos + nbrOfPixels, yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), xPos + nbrOfPixels, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_OFF);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, xPos, yTip, xPos + nbrOfPixels, yTip, DM_PIXEL_STATUS_OFF);
                }
                else
                {
                    ContentsManager_DrawLine(area, xPos, yStart, (xPos + nbrOfPixels), yStart, DM_PIXEL_STATUS_ON);
                    ContentsManager_DrawLine(area, xPos, (yEnd - nbrOfPixels), (xPos + nbrOfPixels), (yEnd - nbrOfPixels), DM_PIXEL_STATUS_ON);
                    ++nbrOfPixels;

                    for(yPos = (yStart + 1u); yPos < yTip; ++yPos)
                    {
                        ContentsManager_DrawLine(area, (xPos + 1u), yPos, ((xPos + nbrOfPixels) - (uint16)1u), yPos, DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawLine(area, (xPos + 1u), (yEnd - nbrOfPixels), ((xPos + nbrOfPixels) - (uint16)1u), (yEnd - nbrOfPixels), DM_PIXEL_STATUS_OFF);
                        ContentsManager_DrawDot(area, xPos, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, yPos, DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, xPos, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_ON);
                        ContentsManager_DrawDot(area, xPos + nbrOfPixels, (yEnd - nbrOfPixels), DM_PIXEL_STATUS_ON);
                        ++nbrOfPixels;
                    }

                    ContentsManager_DrawLine(area, (xPos + 1u), yTip, ((xPos + nbrOfPixels) - (uint16)1u), yTip, DM_PIXEL_STATUS_OFF);
                    ContentsManager_DrawDot(area, xPos, yTip, DM_PIXEL_STATUS_ON);
                    ContentsManager_DrawDot(area, xPos + nbrOfPixels, yTip, DM_PIXEL_STATUS_ON);
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
    currentLayout = &ContentsManager_Layouts[layoutID];

    resetLayerConfiguration();

    const display_area_definition_t* area = &currentLayout->areas[0];
    setupLayer(0u, area->x, area->y, area->width, area->height, DCU_8_BPP, area->activeBuffer, area->bufferOffset, area->bufferSize);

    drawCurrentLayout();
    if (currentLayout->focusedWidget != NULL)
    {
        LayoutManager_SetWidgetFocus(currentLayout->focusedWidget, TRUE);
    }

    pushCurrentLayoutConfiguration();
}

void ContentsManager_SetLayerOpacity(uint8 layerID, uint8 opacity)
{
    UNUSED(layerID);
    UNUSED(opacity);
}

void ContentsManager_SetLayerPosition(uint8 layerID, uint16 x, uint16 y)
{
    UNUSED(layerID);
    UNUSED(x);
    UNUSED(y);
}

void ContentsManager_SetLayerRelativePosition(uint8 layerID, sint16 x, sint16 y)
{
    UNUSED(layerID);
    UNUSED(x);
    UNUSED(y);
}

void ContentsManager_ShowCheckMessage(boolean show)
{
    m_showCheckMessage = show;

    if((m_showCheckMessage == TRUE) && (m_checkMessageAreaIndex != 255u))
    {
        drawArea(m_checkMessageAreaIndex);

        Logger_Info(CONTENTSMANAGER_LOG_NAME, "Showing CheckMessage");
    }
    else if(m_checkMessageAreaIndex != 255u)
    {
        drawCurrentLayout();
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
    for (uint8 layerID = 0; layerID < DisplayManager_LayerCount; ++layerID)
    {
        layer_metadata_t* layer = &ContentsManager_LayerMetadata[layerID];
        if (layer->isUpdated == FALSE)
        {
            continue;
        }

        DisplayManager_SetLayer(layerID, &layer->configuration);
        layer->isUpdated = FALSE;
    }
    DisplayManager_EndLayerConfiguration();
}

static void setupLayer(uint8 layerID, uint16 x, uint16 y, uint16 width, uint16 height, BSP_DCU_BPPModes bpp, uint8* buffer, uint32 bufferOffset, uint32 bufferSize)
{
    ContentsManager_LayerMetadata[layerID].configuration.bpp = bpp;
    ContentsManager_LayerMetadata[layerID].configuration.data = buffer;
    ContentsManager_LayerMetadata[layerID].configuration.isFrameBufferContents = TRUE;
    ContentsManager_LayerMetadata[layerID].configuration.isEnabled = TRUE;
    ContentsManager_LayerMetadata[layerID].configuration.frameBufferOffset = bufferOffset;
    ContentsManager_LayerMetadata[layerID].configuration.dataSize = bufferSize;

#ifdef DISPLAY_ROTATED
    ContentsManager_LayerMetadata[layerID].configuration.x = y;
    ContentsManager_LayerMetadata[layerID].configuration.y = ContentsManager_DisplayHeight - (x + width);
    ContentsManager_LayerMetadata[layerID].configuration.width = height;
    ContentsManager_LayerMetadata[layerID].configuration.height = width;
#else
    ContentsManager_LayerMetadata[layerID].configuration.x = x;
    ContentsManager_LayerMetadata[layerID].configuration.y = y;
    ContentsManager_LayerMetadata[layerID].configuration.width = width;
    ContentsManager_LayerMetadata[layerID].configuration.height = height;
#endif
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
        for(uint8 areaIndex = 0u; areaIndex < currentLayout->areaCount; ++areaIndex)
        {
            refreshArea(areaIndex);
        }
    }
}

static void refreshArea(uint8 areaIndex)
{
    const display_area_definition_t* area = &currentLayout->areas[areaIndex];
    if((area->isCheckMessageArea == FALSE) || ((area->isCheckMessageArea == TRUE) && (m_showCheckMessage == TRUE)))
    {
        boolean needsRedraw = FALSE;
        for (uint8 widgetIndex = 0u; widgetIndex < area->widgetCount; ++widgetIndex)
        {
            const ui_widget_t* widget = &area->widgets[widgetIndex];
            needsRedraw = UIWidget_NeedsRedraw(widget);
            if (needsRedraw == TRUE)
            {
                break;
            }
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

            if(m_showCheckMessage == TRUE)
            {
                drawArea(m_checkMessageAreaIndex);
            }
        }
    }
}

// DRAW ---------------------------------------------------------------------------------------------------------------

static void drawCurrentLayout()
{
    m_checkMessageAreaIndex = 255u;
    DisplayManager_ClearFrameBuffer();
    for (uint8 areaIndex = 0u; areaIndex < currentLayout->areaCount; areaIndex++)
    {
        const display_area_definition_t* area = &currentLayout->areas[areaIndex];
        if(area->isCheckMessageArea == TRUE)
        {
            m_checkMessageAreaIndex = areaIndex;
        }
        drawArea(areaIndex);
    }
}

static void drawArea(uint8 areaIndex)
{
    const display_area_definition_t* area = &currentLayout->areas[areaIndex];

    if((area->isCheckMessageArea == FALSE) || ((area->isCheckMessageArea == TRUE) && (m_showCheckMessage == TRUE)))
    {
        if (area->isCheckMessageArea == TRUE)
        {
            clearArea(areaIndex);
        }

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

    area->writableArea.x = area->x + gap;
    area->writableArea.y = area->y + gap;
    area->writableArea.width = (area->width - gap) - gap;
    area->writableArea.height = (area->height - gap) - gap;
}

static void drawBorder(const display_area_definition_t* area)
{
    uint16 xStart = 0;
    uint16 xEnd = area->width - 1u;
    uint16 yStart = 0;
    uint16 yEnd = area->height - 1u;

    drawLineInArea(area, xStart + 2u, yStart, xEnd - 2u, yStart, DM_PIXEL_STATUS_ON);
    setPixelInArea(area, xStart + 1u, yStart + 1u, DM_PIXEL_STATUS_ON);

    drawLineInArea(area, xStart, yStart + 2u, xStart, yEnd - 2u, DM_PIXEL_STATUS_ON);
    setPixelInArea(area, xStart + 1u, yEnd - 1u, DM_PIXEL_STATUS_ON);

    drawLineInArea(area, xStart + 2u, yEnd, xEnd - 2u, yEnd, DM_PIXEL_STATUS_ON);
    setPixelInArea(area, xEnd - 1u, yEnd - 1u, DM_PIXEL_STATUS_ON);

    drawLineInArea(area, xEnd, yStart + 2u, xEnd, yEnd - 2u, DM_PIXEL_STATUS_ON);
    setPixelInArea(area, xEnd - 1u, yStart + 1u, DM_PIXEL_STATUS_ON);
}

static void drawLineInArea(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID)
{
    uint8 state = colorID;

    if (state > 1u)
    {
        state = DM_PIXEL_STATUS_ON;
    }

    if ((xStart == xEnd) && (yStart == yEnd))
    {
        setPixelInArea(area, xStart, yStart, state);
    }
    if (xStart == xEnd)
    {
        for(uint16 i = yStart; i <= yEnd; ++i)
        {
            setPixelInArea(area, xStart, i, state);
        }
    }
    else if (yStart == yEnd)
    {
        uint16 yStartInFullArea = area->y + yStart;
        uint16 xStartInFullArea = area->x + xStart;

        uint8* frameBufPosition = &frameBuffer[(DisplayManager_BytesPerLine*yStartInFullArea) + (xStartInFullArea >> 3)];

        uint16 lineLength = (xEnd - xStart) + (uint16)1u;

        uint16 prePixelsCount = 8u - (xStartInFullArea % 8u);
        if (prePixelsCount == 8u)
        {
            prePixelsCount = 0u;
        }
        else
        {
            ++frameBufPosition;
        }

        prePixelsCount = (prePixelsCount > lineLength) ? lineLength : prePixelsCount;
        uint16 pixels = lineLength;

        for(uint16 i = 0u; i < prePixelsCount; ++i)
        {
            setPixelInArea(area, xStart + i, yStart, state);
            --pixels;
        }

        uint8 pixelValue = (state == DM_PIXEL_STATUS_ON) ? (uint8)ALL_PIXELS_ON_IN_BYTE : (uint8)ALL_PIXELS_OFF_IN_BYTE;
        while(pixels > 7u)
        {
            *frameBufPosition = pixelValue;
            ++frameBufPosition;
            pixels = pixels - 8u;
        }

        for(uint8 i = (uint8)pixels; i > 0u; --i)
        {
            setPixelInArea(area, (uint16)(xStart + lineLength) - (uint16)i, yStart, state);
            --pixels;
        }
    }
    else
    {
        Logger_Info(CONTENTSMANAGER_LOG_NAME, "No support for diagonal lines");
    }
}

static void setPixelInArea(const display_area_definition_t* area, uint16 x, uint16 y, uint8 colorID)
{
    uint8 state = colorID;

    if (state > 1u)
    {
        state = DM_PIXEL_STATUS_ON;
    }

    if ((x <= area->width) && (y <= area->height))
    {
        uint16 displayX = area->x + x;
        uint16 displayY = area->y + y;
        uint8* byte = &frameBuffer[((displayY * DisplayManager_BytesPerLine) + (displayX >> 3))];

        uint8 mask = 0x01u << (displayX % 8u);

        if (state == DM_PIXEL_STATUS_OFF)
        {
            *byte |= mask;
        }
        else
        {
            *byte &= ~mask;
        }
    }

}

static void clearPixel(const display_area_definition_t* area, uint16 x, uint16 y)
{
    (void)area;
    uint8* byte = &frameBuffer[(y * DisplayManager_BytesPerLine) + (x >> 3)];

    uint8 mask = 0x01u << (x % 8u);

    *byte |= mask;
}

static void resetLayerConfiguration(void)
{
    for (uint8 layerID = 0; layerID < DisplayManager_LayerCount; ++layerID)
    {
        ContentsManager_LayerMetadata[layerID].configuration.isEnabled = FALSE;
        ContentsManager_LayerMetadata[layerID].isUpdated = TRUE;
    }
}

static void clearArea(uint8 areaIndex)
{
    const display_area_definition_t* area = &currentLayout->areas[areaIndex];
    clearRectangle(area, area->x, area->y, area->width, area->height);
}

static void clearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height)
{
    uint16 firstLine = y;
    uint16 lastLine = y + height;

    uint16 prePixelsCount = 8u - (x % 8u);
    if (prePixelsCount == 8u)
    {
        prePixelsCount = 0u;
    }

    prePixelsCount = (prePixelsCount > width) ? width : prePixelsCount;

    for(uint16 line = firstLine; line < lastLine; ++line)
    {
        uint8* frameBufPosition = &frameBuffer[(DisplayManager_BytesPerLine*line) + (x >> 3)];
        if (prePixelsCount > 0u)
        {
            ++frameBufPosition;
        }

        uint16 pixels = width;

        for(uint16 i = 0; i < prePixelsCount; ++i)
        {
            clearPixel(area, x + i, line);
            --pixels;
        }

        while(pixels > 7u)
        {
            *frameBufPosition = ALL_PIXELS_OFF_IN_BYTE;
            ++frameBufPosition;
            pixels = pixels - 8u;
        }

        for(uint16 i = pixels; i > 0u; --i)
        {
            clearPixel(area, (x + width) - i, line);
            --pixels;
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
