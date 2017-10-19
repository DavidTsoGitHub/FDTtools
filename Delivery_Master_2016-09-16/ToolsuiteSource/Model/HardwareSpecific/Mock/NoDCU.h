#ifndef NODCU_H
#define NODCU_H

#include "IBSP_DCU_BPPModes.h"

class NoDCU : public IBSP_DCU_BPPModes
{
public:
    NoDCU();

    // IBSP_DCU_BPPModes interface
public:
    quint8 GetBitsPerPixelForMode(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
    const QString GetBitsPerPixelForModeString(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
    BSPEnumItem GetDCUBPPFlag(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
};

#endif // NODCU_H
