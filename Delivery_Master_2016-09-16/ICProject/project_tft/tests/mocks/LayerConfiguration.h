#ifndef LAYERCONFIGURATION_H
#define LAYERCONFIGURATION_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
struct ui_widget_t;

typedef enum
{
    BPP8 = 3u,
    BPP16 = 4u
} bpp_e;

typedef struct
{
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    uint8  dirty;
} writable_area_definition_t;

typedef struct
{
    writable_area_definition_t writableArea;
    uint8 layerID;
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    uint8  border;
    uint8  widgetCount;
    uint32 bufferOffset;
    uint32 bufferSize;
    uint8* buffer;
    const struct ui_widget_t* widgets;
} display_area_definition_t;

typedef struct
{
    void (*draw)(display_area_definition_t* area, const struct ui_widget_t*);
    boolean (*refresh)(display_area_definition_t* area, const struct ui_widget_t*);
    void (*handleNext)(void);
    void (*handlePrevious)(void);
    void (*handleEnter)(void);
    void (*handleLeave)(void);
    boolean (*offerFocus)(const struct ui_widget_t*);
    void (*forceFocusRelease)(void);
    boolean (*focusFinished)(void);
} ui_widget_funcs;

typedef struct ui_widget_t
{
    uint32 widgetID;
    sint8 layerID;
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    boolean boxed;
    uint8 color;
    bpp_e bpp;
    const ui_widget_funcs* functions;
} ui_widget_t;


typedef struct
{
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    bpp_e  bpp;
    boolean isEnabled;
    boolean isFrameBufferContents;
    uint32 frameBufferOffset;
    uint32 dataSize;
    uint8* data;
} layer_configuration_t;

typedef struct
{
    layer_configuration_t configuration;
    boolean isUpdated;
} layer_metadata_t;

/***************************** Exported Functions ****************************/

#endif // LAYERCONFIGURATION_H
