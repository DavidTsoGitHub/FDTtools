#ifndef LAYERCONFIGURATION_H
#define LAYERCONFIGURATION_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "BSP_Types.h"

/**************************** Constants and Types ****************************/
struct ui_widget_t;

typedef enum
{
    EQUALS_TO = 0,
    GREATER_THAN = 1,
    EQUALS_OR_GREATER_THAN = 2,
    LESS_THAN = 3,
    EQUALS_OR_LESS_THAN = 4
} transition_condition_t;

typedef enum
{
    NONE,
    AND,
    XOR
} transition_connective_t;

typedef enum
{
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_SET,
    BUTTON_CLEAR
} transition_buttons_t;

typedef enum
{
    DO_NOTHING,
    RUN_ONE_CYCLE,
    SHOW_DEFAULT
} animation_on_load_behavior_t;

typedef enum
{
    START_FROM_BEGINNING,
    START_FROM_DEFAULT_FRAME,
    CONTINUE_FROM_CURRENT_FRAME
} animation_on_trigger_active_behavior_t;

typedef enum
{
    STOP,
    STOP_AT_END_OF_CYCLE,
    STOP_AT_DEFAULT_FRAME,
    CONTINUE
} animation_on_trigger_inactive_behavior_t;

typedef struct
{
    animation_on_load_behavior_t onLoadBehavior;
    animation_on_trigger_active_behavior_t onTriggerActiveBehavior;
    animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior;
} animation_triggers_properties_t;

typedef struct
{
    void (*firstConditionGCLSink)(DynamicWidgetValue* value, uint8* signalStatus);
    transition_condition_t firstConditionCompareOperator;
    sint32 firstConditionCompareValue;
    transition_connective_t connective;
    void (*secondConditionGCLSink)(DynamicWidgetValue* value, uint8* signalStatus);
    transition_condition_t secondConditionCompareOperator;
    sint32 secondConditionCompareValue;
} signal_transition_t;

typedef struct
{
    transition_buttons_t button;
    sint8 layoutID;
} button_transition_t;

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
    uint32 animationID;
    uint8 layerID;
    sint16 step;
    boolean incrementStep;
    sint16 index;
    sint16 startIndex;
    sint16 endIndex;
    boolean triggerActive;
    boolean animationUpdated;
    boolean lastIndication;
    boolean allreadyTriggered;
    boolean animationRunning;
    boolean onLoadRunOneCycleRunning;
} animation_mapping_t;

typedef struct
{
    writable_area_definition_t writableArea;
    sint8 layerID;
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    uint8  border;
    uint8 fgColor;
    uint8 bgColor;
    uint8  widgetCount;
    uint32 bufferOffset;
    uint32 bufferSize;
    boolean isCheckMessageArea;
    uint8* activeBuffer;
    uint8* passiveBuffer;
    const struct ui_widget_t* widgets;
    animation_mapping_t* animation1;
    animation_mapping_t* animation2;
} display_area_definition_t;

typedef struct
{
    void (*draw)(const display_area_definition_t* area, const struct ui_widget_t*);
    boolean (*needsRedraw)(const struct ui_widget_t*);
    void (*redraw)(const display_area_definition_t* area, const struct ui_widget_t*);
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
    uint8 fgColor;
    uint8 bgColor;
    BSP_DCU_BPPModes bpp;
    const ui_widget_funcs* functions;
    animation_mapping_t* animation1;
    animation_mapping_t* animation2;
} ui_widget_t;

typedef struct
{
    uint16 originX;
    uint16 originY;
    uint16 x;
    uint16 y;
    uint16 width;
    uint16 height;
    BSP_DCU_BPPModes  bpp;
    boolean isEnabled;
    boolean isFrameBufferContents;
    uint32 frameBufferOffset;
    uint32 dataSize;
    uint8* data;
    uint8 opacity;
} layer_configuration_t;

typedef struct
{
    layer_configuration_t configuration;
    boolean isUpdated;
} layer_metadata_t;

/***************************** Exported Functions ****************************/

#endif // LAYERCONFIGURATION_H
