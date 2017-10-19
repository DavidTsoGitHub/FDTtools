#ifndef BSP_DCU_BPPMODES
#define BSP_DCU_BPPMODES

#include "UIWidgetBase.h"
class BSPEnumItem;

class IBSP_DCU_BPPModes
{
public:
    virtual quint8 GetBitsPerPixelForMode(UIWidgetBase::BitplaneDefinition_t independentBPPDef) = 0;
    virtual const QString GetBitsPerPixelForModeString(UIWidgetBase::BitplaneDefinition_t independentBPPDef) = 0;
    virtual BSPEnumItem GetDCUBPPFlag(UIWidgetBase::BitplaneDefinition_t independentBPPDef) = 0;
};

#endif // BSP_DCU_BPPMODES

