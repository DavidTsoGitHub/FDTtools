#ifndef MPC5606_DCU_BPPMODES_H
#define MPC5606_DCU_BPPMODES_H

#include "IBSP_DCU_BPPModes.h"
#include "BSPEnumItem.h"

template <class Key, class Value> class QMap;
class BSPEnum;
class UIWidgetBase;
class Project;

class MPC5606_DCU_BPPModes : public IBSP_DCU_BPPModes
{
public:
    struct DCU_BPP_Item {
        BSPEnumItem bspEnumItem;
        quint8 bitsPerPixel;
    };

    explicit MPC5606_DCU_BPPModes(Project* project);

private:
    QMap<UIWidgetBase::BitplaneDefinition_t, DCU_BPP_Item> m_bppMap;
    const BSPEnum* m_bspDCUBPPModes;

    BSPEnumItem getBSPEnumRow(QString identifier);

    // IBSP_DCU_BPPModes interface
public:
    quint8 GetBitsPerPixelForMode(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
    const QString GetBitsPerPixelForModeString(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
    BSPEnumItem GetDCUBPPFlag(UIWidgetBase::BitplaneDefinition_t independentBPPDef);
};

#endif // MPC5606_DCU_BPPMODES_H
