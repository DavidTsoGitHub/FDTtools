/********************************* Includes **********************************/
#include "IconRenderer.h"
#include "ContentsManager.h"

/***************************** Exported Functions ****************************/

void IconRenderer_RenderIcon(const display_area_definition_t* area, const icon_t* icon, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, boolean hasFocus, uint8 fgColor, uint8 bgColor)
{
    if (icon != NULL)
    {
        uint16 width = icon->width;
        uint16 bytesPerLine = icon->bytesPerLine;
        uint32 byteCount = (uint32)icon->bytesPerLine*icon->height;
        uint16 xPosTmp = x;

        sint16 tmp = ((sint16)containerWidth - (sint16)icon->width) / 2;
        if(tmp > 0)
        {
            xPosTmp = xPosTmp - (uint16)tmp;
        }
        else
        {
            xPosTmp = xPosTmp + (uint16)tmp;
        }
        uint16 xPos = xPosTmp;

        uint16 yPos;
        tmp = ((sint16)containerHeight - (sint16)icon->height) / 2;
        if(tmp > 0)
        {
            yPos = y - (uint16)tmp;
        }
        else
        {
            yPos = y + (uint16)tmp;
        }


        const uint8* bytes = icon->bitmap;

        for(uint16 i = 0U; i < byteCount; i += bytesPerLine)
        {
            for(uint16 j = 0U; j < width; ++j)
            {
                uint16 bitPosition = j%8U;
                uint16 arrayPosition = i+(j/8U);
                uint8 value = (bytes[arrayPosition] >> bitPosition) & 0x01U;

                uint8 pixelStatus;
                if (fgColor < 2U)
                {
                    if(hasFocus == TRUE)
                    {
                        pixelStatus = (value == 0x00U) ? fgColor : bgColor;
                    }
                    else
                    {
                        pixelStatus = (value == 0x00U) ? bgColor : fgColor;
                    }
                }
                else
                {
                    if(hasFocus == TRUE)
                    {
                        pixelStatus = (value == 0x00U) ? fgColor : bgColor;
                    }
                    else
                    {
                        pixelStatus = (value == 0x00U) ? bgColor : fgColor;
                    }
                }

                ContentsManager_DrawDot(area, xPos++, yPos, pixelStatus);
            }
            ++yPos;
            xPos = xPosTmp;
        }
    }
}
