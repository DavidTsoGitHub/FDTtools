#ifndef TEXTRENDERERMOCK_H
#define TEXTRENDERERMOCK_H

#include "TextRenderer.h"
#include <time.h>

void TextRendererMock_Reset(void);

int TextRendererMock_RenderLabel_CallCount();
clock_t TextRendererMock_RenderLabel_FirstCallTimeStamp();
clock_t TextRendererMock_RenderLabel_LastCallTimeStamp();

int TextRendererMock_RenderText_CallCount(void);
clock_t TextRendererMock_RenderText_FirstCallTimeStamp(void);
clock_t TextRendererMock_RenderText_LastCallTimeStamp(void);

int TextRendererMock_CalcBoundingBox_CallCount(void);
clock_t TextRendererMock_CalcBoundingBox_FirstCallTimeStamp(void);
clock_t TextRendererMock_CalcBoundingBox_LastCallTimeStamp(void);

#endif
