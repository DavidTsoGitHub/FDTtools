#ifndef FONT_H
#define FONT_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
typedef struct {
    uint16 encoding;
    sint8 offsetX;
    sint8 offsetY;
    uint8 width;
    uint8 height;
    sint8 deviceWidthX;
    sint8 deviceWidthY;
    uint8 boundingWidth;
    uint8 boundingHeight;
    const unsigned char* bitmap;
} glyph_t;

typedef struct {
    uint8 size;
    uint8 linespacing;
    uint16 numGlyphs;
    glyph_t emptyGlyph;
    glyph_t glyphs[];
} font_t;

#endif
