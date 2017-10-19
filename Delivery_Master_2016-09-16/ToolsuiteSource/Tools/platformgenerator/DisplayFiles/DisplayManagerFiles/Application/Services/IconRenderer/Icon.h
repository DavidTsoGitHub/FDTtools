#ifndef ICON_H
#define ICON_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
typedef struct {
    uint16 width;
    uint16 height;
    uint8 bitDepth;
    uint8 bitPlanes;
    uint16 bytesPerLine;
    const uint8* bitmap;
} icon_t;

#endif
