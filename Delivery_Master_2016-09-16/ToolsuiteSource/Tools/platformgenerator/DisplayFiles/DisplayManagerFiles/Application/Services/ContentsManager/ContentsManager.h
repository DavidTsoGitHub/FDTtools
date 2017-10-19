#ifndef CONTENTSMANAGER_H
#define CONTENTSMANAGER_H

/********************************* Includes **********************************/
#include "ContentsManager_cfg.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    DM_STATE_OFF,
    DM_STATE_STARTUP_REQUESTED,
    DM_STATE_STARTUP,
    DM_STATE_RUN
} dm_state_t;

typedef enum
{
    DM_TRIANGLE_DIRECTION_UP,
    DM_TRIANGLE_DIRECTION_DOWN,
    DM_TRIANGLE_DIRECTION_LEFT,
    DM_TRIANGLE_DIRECTION_RIGHT
} dm_triangle_direction_t;

#define DM_PIXEL_STATUS_OFF     0x00u
#define DM_PIXEL_STATUS_ON      0x01u

/***************************** Exported Functions ****************************/
void ContentsManager_Init(void);
void ContentsManager_Run(void);

dm_state_t ContentsManager_GetState(void);
boolean ContentsManager_MenuLayoutActive(void);

void ContentsManager_SetLayerDirty(sint8 layerID);
void ContentsManager_SetLayerEnabled(uint8 layerID, boolean enabled);

void ContentsManager_ClearDisplay(void);

void ContentsManager_ClearArea(const display_area_definition_t* area);
void ContentsManager_ClearRectangle(const display_area_definition_t* area, uint16 x, uint16 y, uint16 width, uint16 height, uint8 colorID);

void ContentsManager_DrawDot(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint8 colorID);
void ContentsManager_DrawLine(const display_area_definition_t* area, uint16 xStart, uint16 yStart, uint16 xEnd, uint16 yEnd, uint8 colorID);

void ContentsManager_DrawBoxWithRoundCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID);
void ContentsManager_DrawBoxWithHardCorners(const display_area_definition_t* area, uint16 xPos, uint16 yPos, uint16 width, uint16 height, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID);

void ContentsManager_DrawTriangle(const display_area_definition_t* area, dm_triangle_direction_t triangleDirection, uint16 xStart, uint16 yStart, uint16 width, boolean fill, boolean invertBorder, uint8 fgColorID, uint8 bgColorID);

void ContentsManager_SetCurrentLayout(sint8 layoutID);

void ContentsManager_SetLayerFrameBuffer(uint8 layerID, uint8 *buffer);

boolean ContentsManager_ValidImageSize(uint8 layerID, uint16 width, uint16 height);

void ContentsManager_SetLayerOpacity(uint8 layerID, uint8 opacity);
void ContentsManager_SetLayerPosition(uint8 layerID, uint16 x, uint16 y);
void ContentsManager_SetLayerRelativePosition(uint8 layerID, sint16 x, sint16 y);

void ContentsManager_ShowCheckMessage(boolean show);

void ContentsManager_SetLanguageAwaitingUpdate(uint8 newLanguage);
boolean ContentsManager_GetLanguageAwaitingUpdate(void);

#endif // CONTENTSMANAGER_H
