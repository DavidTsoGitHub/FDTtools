//lint -save
/********************************* Includes **********************************/
#include "TextRenderer.h"
#include "ContentsManager.h"
#include "LabelManager.h"
#include "WCharDefines.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/
static void writeText(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerLimitWidth, uint16 containerLimitHeight, uint8 fgColor, uint8 bgColor);
static const glyph_t* getGlyph(wchar c, const font_t *const font);
static uint16 getAbsoluteValue(sint8 value);

/***************************** Exported Functions ****************************/
void TextRenderer_CalcTextBoundingBox(const wchar* txt, const font_t* font, uint16* width, uint16* height)
{
    *width = 0;
    *height = 0;
    uint16 maxWidth = 0;
    boolean firstGlyph = TRUE;

    uint16 maxY, minY;
    maxY = 0x0000u;
    minY = 0xFFFFu;

    while (*txt != WCHAR_EOL)
    {
        if ((*txt == WCHAR_BACKSLASH) && (txt[1] == WCHAR_N))
        {
            txt ++;
            txt ++; // Due to Misra not liking txt += 2;

            if (*width > maxWidth)
            {
                maxWidth = *width;
            }

            *width = 0;
        }
        const glyph_t* glyph = getGlyph(*txt, font);

        // If this is the first glyph, add its horizontal offset to the width
        if (firstGlyph == TRUE)
        {
            *width += getAbsoluteValue(glyph->offsetX);
            firstGlyph = FALSE;
        }

        // unless this is the final character, add the spacing to the next glyph to the bounding box width:
        if (txt[1] != WCHAR_EOL)
        {
            *width += getAbsoluteValue(glyph->deviceWidthX);
        }
        else
        {
            // this is the final character; add its width to the bounding box width:
            *width += glyph->width + getAbsoluteValue(glyph->offsetX);
        }

        // FIXME note that the device width along the Y axis is currently ignored.

        // if this character's height is greater than the current bounding box,
        // set the bounding box height to the same as this character's:
        if (getAbsoluteValue(glyph->offsetY) < minY)
        {
            minY = getAbsoluteValue(glyph->offsetY);
        }
        if ((glyph->height + getAbsoluteValue(glyph->offsetY)) > maxY)
        {
            maxY = (uint16)glyph->height + getAbsoluteValue(glyph->offsetY);
        }

        txt++;
    }

    if (*width < maxWidth)
    {
        *width = maxWidth;
    }

    *height = maxY - minY;
}

void TextRenderer_CalcBoundingBox(uint32 labelID, uint16* width, uint16* height)
{
    const label_t* label = LabelManager_GetLabel(labelID);
    const wchar* txt = label->text;
    const font_t* font = label->font;

    TextRenderer_CalcTextBoundingBox(txt, font, width, height);
}

void TextRenderer_RenderAlignedLabel(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor, TextAlignment alignment)
{
    const label_t* label = LabelManager_GetLabel(labelID);
    const font_t* font = label->font;

    uint16 alignedY = y;
    if (font->size < containerHeight)
    {
        alignedY = y + ((uint16)(containerHeight - font->size) / 2u);
    }

    if (label != NULL)
    {
        switch (alignment)
        {
            case TEXT_CENTER_ALIGNED:
                TextRenderer_RenderTextCenterAligned(area, label->text, label->font, x, alignedY, containerWidth, containerHeight, fgColor, bgColor);
                break;
            case TEXT_RIGHT_ALIGNED:
                TextRenderer_RenderTextRightAligned(area, label->text, label->font, x, alignedY, containerWidth, containerHeight, fgColor, bgColor);
                break;
            case TEXT_LEFT_ALIGNED:
            default:
                TextRenderer_RenderLabel(area, labelID, x, alignedY, containerWidth, containerHeight, fgColor, bgColor);
                break;
        }
    }
}

void TextRenderer_RenderLabel(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    const label_t* label = LabelManager_GetLabel(labelID);

    if (label != NULL)
    {
        TextRenderer_RenderText(area, label->text, label->font, x, y, containerWidth, containerHeight, fgColor, bgColor);
    }
}

void TextRenderer_RenderLabelLeftAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    TextRenderer_RenderLabel(area, labelID, x, y, containerWidth, containerHeight, fgColor, bgColor);
}

void TextRenderer_RenderTextLeftAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    TextRenderer_RenderText(area, txt, font, x, y, containerWidth, containerHeight, fgColor, bgColor);
}

void TextRenderer_RenderLabelCenterAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    const label_t* label = LabelManager_GetLabel(labelID);

    if (label != NULL)
    {
        TextRenderer_RenderTextCenterAligned(area, label->text, label->font, x, y, containerWidth, containerHeight, fgColor, bgColor);
    }
}

void TextRenderer_RenderTextCenterAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    uint16 height = 0, width = 0;

    TextRenderer_CalcTextBoundingBox(txt, font, &width, &height);

    uint16 centerAlignedX = 0u;
    uint16 containerLimitWidth = 0u;

    if (width < containerWidth)
    {
        centerAlignedX = x + ((uint16)(containerWidth - width) / 2u);
        containerLimitWidth = centerAlignedX + containerWidth - 5u;
    }
    else
    {
        centerAlignedX = x + 5u;
        containerLimitWidth = centerAlignedX + containerWidth - 10u;
    }

    uint16 containerLimitHeight = y + containerHeight;
    writeText(area, txt, font, centerAlignedX, y, containerLimitWidth, containerLimitHeight, fgColor, bgColor);
}

void TextRenderer_RenderLabelRightAligned(const display_area_definition_t* area, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    const label_t* label = LabelManager_GetLabel(labelID);

    if (label != NULL)
    {
        TextRenderer_RenderTextRightAligned(area, label->text, label->font, x, y, containerWidth, containerHeight, fgColor, bgColor);
    }
}

void TextRenderer_RenderTextRightAligned(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    uint16 height = 0, width = 0;

    TextRenderer_CalcTextBoundingBox(txt, font, &width, &height);

    uint16 rightAlignedX = 0u;
    uint16 containerLimitWidth = 0u;

    if (width < (containerWidth - 10u))
    {
        rightAlignedX = x + containerWidth - (width + 5u);
        containerLimitWidth = rightAlignedX + containerWidth - 5u;
    }
    else
    {
        rightAlignedX = x + 5u;
        containerLimitWidth = rightAlignedX + containerWidth - 10u;
    }

    uint16 containerLimitHeight = y + containerHeight;
    writeText(area, txt, font, rightAlignedX, y, containerLimitWidth, containerLimitHeight, fgColor, bgColor);
}

void TextRenderer_RenderText(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, uint8 fgColor, uint8 bgColor)
{
    uint16 containerLimitWidth = x + containerWidth - 5u;
    uint16 containerLimitHeight = y + containerHeight;

    writeText(area, txt, font, (x + 5u), y, containerLimitWidth, containerLimitHeight, fgColor, bgColor);
}

/****************************** Local Functions  *****************************/
static void writeText(const display_area_definition_t* area, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerLimitWidth, uint16 containerLimitHeight, uint8 fgColor, uint8 bgColor)
{
    uint16 startX = 0, startY = 0;
    uint16 thisX = x, thisY = y;
    uint8 mask;
    uint8 color = 0;

    while (*txt != WCHAR_EOL)
    {
        if ((*txt == WCHAR_BACKSLASH) && (txt[1] == WCHAR_N))
        {
            thisY = (uint16)(thisY + (uint16)font->size + (uint16)font->linespacing);
            thisX = x;
            txt++;
            txt++; // Due to Misra not liking txt += 2;
        }
        const glyph_t* glyph = getGlyph(*txt, font);

        boolean isFinalCharacter = (txt[1] == WCHAR_EOL) ? TRUE : FALSE;

        // starting (upper left) coordinates for this glyph:
        if (glyph->offsetX < 0)
        {
            startX = (uint16)thisX - getAbsoluteValue(glyph->offsetX);
        }
        else
        {
            startX = (uint16)thisX + getAbsoluteValue(glyph->offsetX);
        }

        if (glyph->offsetY < 0)
        {
            startY = (uint16)((thisY + font->size) - (glyph->height - getAbsoluteValue(glyph->offsetY)));
        }
        else
        {
            startY = (uint16)((thisY + font->size) - (glyph->height + getAbsoluteValue(glyph->offsetY)));
        }

        // iterate over the lines in this glyph:
        for (uint16 h = 0u; h < glyph->height; h++)
        {
            // iterate over the bytes in this line:
            uint16 bytes = ((uint16)glyph->width >> 3u);
            if ((glyph->width % 8u) > 0u)
            {
                bytes += 1u;
            }
            uint16 currentLinePixelX = 0u;
            uint8 currentPixelValue;

            for (uint16 b = 0u; b < bytes; b++)
            {
                mask = 0x80;

                // iterate over the pixels in this byte:
                for (uint16 pixelInCurrentByte = 0u; pixelInCurrentByte < ((b == (bytes - 1u)) ? (glyph->width - (b * 8u)) : 8u); pixelInCurrentByte++)
                {
                    currentPixelValue = glyph->bitmap[(h * bytes) + b] & mask;

                    color = (currentPixelValue > 0u) ? fgColor : bgColor;

                    if (((startX + currentLinePixelX) < containerLimitWidth) && ((startY + h) < containerLimitHeight))
                    {
                        ContentsManager_DrawDot(area, (uint16)(startX + currentLinePixelX), startY + h, color);
                    }
                    currentLinePixelX++;

                    mask >>= 1;
                }

                // unless this is the final character, clear the space between this glyph
                // and the next too:
                if (isFinalCharacter == FALSE)
                {
                    for (uint16 pixelInGlyph = glyph->width; pixelInGlyph < getAbsoluteValue(glyph->deviceWidthX); pixelInGlyph++)
                    {
                        if (((startX + currentLinePixelX) < containerLimitWidth) && ((startY + h) < containerLimitHeight))
                        {
                            ContentsManager_DrawDot(area, startX + pixelInGlyph, startY + h, bgColor);
                        }
                    }
                }
            }
        }

        // unless this was the final character, advance the x coordinate by this glyph's
        // device width along the x axis:
        if (isFinalCharacter == FALSE)
        {
            thisX += getAbsoluteValue(glyph->deviceWidthX);
        }

        ++txt;
    }
}

static const glyph_t* getGlyph(wchar c, const font_t* const font)
{
    const glyph_t* returnValue = &font->emptyGlyph;
    uint16 min = 0;
    uint16 max = font->numGlyphs - 1u;

    while (max >= min)
    {
        if((0u == max) && (0u == min))
        {
            if (font->glyphs[0].encoding == (uint16) c)
            {
                returnValue = &(font->glyphs[0]);
            }
            break;
        }

        uint16 mid = min + ((uint16)(max - min) / 2u);

        if (font->glyphs[mid].encoding == (uint16) c)
        {
            returnValue = &(font->glyphs[mid]);
            break;
        }
        else if (font->glyphs[mid].encoding < (uint16) c)
        {
            min = mid + 1U;
            min = (min == (uint16)UINT16_MAX__) ? (uint16)UINT16_MAX__ : min;
        }
        else
        {
            max = mid - 1U;
            max = (max == (uint16)UINT16_MAX__) ? 0u : max;
        }
    }

    return returnValue;
}

static uint16 getAbsoluteValue(sint8 value)
{
    //lint --e(9033) --e(571) [Convert signed to unsigned, used for absolute coordinate calculations] [- '' -]
    return (uint16)((value < 0) ? (0 - value) : value);
}

//lint -restore
