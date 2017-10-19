#ifndef IMAGE_H
#define IMAGE_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
typedef struct {
    uint16 width;
    uint16 height;
    uint8 bitDepth;
    uint8 bitPlanes;
    uint16 bytesPerLine;
    const uint16* bitmap;
} image_t;

#endif
