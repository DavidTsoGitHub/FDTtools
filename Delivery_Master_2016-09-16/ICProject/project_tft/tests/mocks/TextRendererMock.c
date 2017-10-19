#include "TextRendererMock.h"

static int renderLabelCallCount = 0;
static clock_t renderLabelFirstCallTimeStamp = -1;
static clock_t renderLabelLastCallTimeStamp = -1;

static int renderTextCallCount = 0;
static clock_t renderTextFirstCallTimeStamp = -1;
static clock_t renderTextLastCallTimeStamp = -1;

static int calcBoundingBoxCallCount = 0;
static clock_t calcBoundingBoxFirstCallTimeStamp = -1;
static clock_t calcBoundingBoxLastCallTimeStamp = -1;

void TextRendererMock_Reset()
{
    renderLabelCallCount = 0;
    renderLabelFirstCallTimeStamp = -1;
    renderLabelLastCallTimeStamp = -1;

    renderTextCallCount = 0;
    renderTextFirstCallTimeStamp = -1;
    renderTextLastCallTimeStamp = -1;

    calcBoundingBoxCallCount = 0;
    calcBoundingBoxFirstCallTimeStamp = -1;
    calcBoundingBoxLastCallTimeStamp = -1;
}

/**** renderLabel ****/
void TextRenderer_RenderLabel(uint8 areaID, uint32 labelID, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, boolean inverted)
{
    if (renderLabelFirstCallTimeStamp < 0)
    {
        renderLabelFirstCallTimeStamp = clock();
    }
    renderLabelLastCallTimeStamp = clock();
    ++renderLabelCallCount;
}

int TextRendererMock_RenderLabel_CallCount()
{
    return renderLabelCallCount;
}

clock_t TextRendererMock_RenderLabel_FirstCallTimeStamp()
{
    return renderLabelFirstCallTimeStamp;
}

clock_t TextRendererMock_RenderLabel_LastCallTimeStamp()
{
    return renderLabelLastCallTimeStamp;
}

/**** renderText ****/
void TextRenderer_RenderText(uint8 areaID, const wchar* txt, const font_t* font, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, boolean inverted)
{
    if (renderTextFirstCallTimeStamp < 0)
    {
        renderTextFirstCallTimeStamp = clock();
    }
    renderTextLastCallTimeStamp = clock();
    ++renderTextCallCount;
}

int TextRendererMock_RenderText_CallCount()
{
    return renderTextCallCount;
}

clock_t TextRendererMock_RenderText_FirstCallTimeStamp()
{
    return renderTextFirstCallTimeStamp;
}

clock_t TextRendererMock_RenderText_LastCallTimeStamp()
{
    return renderTextLastCallTimeStamp;
}

/**** calcBoundingBox ****/

void calcBoundingBox(uint32 labelID, uint16* width, uint16* height)
{
    if (calcBoundingBoxFirstCallTimeStamp < 0)
    {
        calcBoundingBoxFirstCallTimeStamp = clock();
    }
    calcBoundingBoxLastCallTimeStamp = clock();
    ++calcBoundingBoxCallCount;
}

int TextRendererMock_CalcBoundingBox_CallCount()
{
    return calcBoundingBoxCallCount;
}

clock_t TextRendererMock_CalcBoundingBox_FirstCallTimeStamp(void)
{
    return calcBoundingBoxFirstCallTimeStamp;
}

clock_t TextRendererMock_CalcBoundingBox_LastCallTimeStamp(void)
{
    return calcBoundingBoxLastCallTimeStamp;
}