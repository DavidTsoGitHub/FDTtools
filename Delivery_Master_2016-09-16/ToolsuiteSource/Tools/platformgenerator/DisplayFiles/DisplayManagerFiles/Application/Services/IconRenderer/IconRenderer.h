#ifndef ICON_RENDERER_H
#define ICON_RENDERER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Icon.h"
#include "LayerConfiguration.h"

/***************************** Exported Functions ****************************/
void IconRenderer_RenderIcon(const display_area_definition_t* area, const icon_t* icon, uint16 x, uint16 y, uint16 containerWidth, uint16 containerHeight, boolean hasFocus, uint8 fgColor, uint8 bgColor);

#endif
