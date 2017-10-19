#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Font.h"
#include "LayerConfiguration.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    TEXT_LEFT_ALIGNED,
    TEXT_CENTER_ALIGNED,
    TEXT_RIGHT_ALIGNED
} TextAlignment;

/***************************** Exported Functions ****************************/
void TextRenderer_CalcTextBoundingBox(const wchar* txt, const font_t* font, uint16* width, uint16* height);
void TextRenderer_CalcBoundingBox(uint32 labelID, uint16* width, uint16* height);

void TextRenderer_RenderAlignedLabel(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor, TextAlignment alignment);
void TextRenderer_RenderLabel(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);
void TextRenderer_RenderText(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);

void TextRenderer_RenderLabelLeftAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);
void TextRenderer_RenderTextLeftAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);

void TextRenderer_RenderLabelCenterAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);
void TextRenderer_RenderTextCenterAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);

void TextRenderer_RenderLabelRightAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);
void TextRenderer_RenderTextRightAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor);

#endif
