#include "MPC5606_DCU_BPPModes.h"

#include <QMap>
#include "Project.h"
#include "UIWidgetBase.h"
#include "BSPEnum.h"

MPC5606_DCU_BPPModes::MPC5606_DCU_BPPModes(Project* project) :
    IBSP_DCU_BPPModes(),
    m_bspDCUBPPModes(project->GetBSPSpecification().GetBSPDCUBPPModes())
{
    for (int i = 0; i < UIWidgetBase::BPP_COUNT; i++)
    {
        UIWidgetBase::BitplaneDefinition_t bppEnumVal = static_cast<UIWidgetBase::BitplaneDefinition_t>(i);
        DCU_BPP_Item bppItem;
        switch (bppEnumVal)
        {
            case UIWidgetBase::BPP_1:
            {
                bppItem.bitsPerPixel = 1;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_1_BPP");
                break;
            }
            case UIWidgetBase::BPP_2:
            {
                bppItem.bitsPerPixel = 2;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_2_BPP");
                break;
            }
            case UIWidgetBase::BPP_4:
            {
                bppItem.bitsPerPixel = 4;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_4_BPP");
                break;
            }
            case UIWidgetBase::BPP_8:
            {
                bppItem.bitsPerPixel = 8;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_8_BPP");
                break;
            }
            case UIWidgetBase::BPP_16_565:
            {
                bppItem.bitsPerPixel = 16;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_16_BPP_565");
                break;
            }
            case UIWidgetBase::BPP_24:
            {
                bppItem.bitsPerPixel = 24;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_24_BPP");
                break;
            }
            case UIWidgetBase::BPP_32:
            {
                bppItem.bitsPerPixel = 32;
                bppItem.bspEnumItem = getBSPEnumRow("DCU_32_BPP_8888");
                break;
            }
            default:
                break;
        }

        m_bppMap.insert(bppEnumVal, bppItem);
    }
}

BSPEnumItem MPC5606_DCU_BPPModes::getBSPEnumRow(QString identifier)
{
    foreach (BSPEnumItem bspEnumItem, m_bspDCUBPPModes->EnumItems.values())
    {
        if (bspEnumItem.Identifier.compare(identifier) == 0)
        {
            return bspEnumItem;
        }
    }

    return BSPEnumItem();
}

quint8 MPC5606_DCU_BPPModes::GetBitsPerPixelForMode(UIWidgetBase::BitplaneDefinition_t independentBPPDef)
{
    return m_bppMap.value(independentBPPDef).bitsPerPixel;
}

const QString MPC5606_DCU_BPPModes::GetBitsPerPixelForModeString(UIWidgetBase::BitplaneDefinition_t independentBPPDef)
{
    return m_bppMap.value(independentBPPDef).bspEnumItem.Identifier;
}

BSPEnumItem MPC5606_DCU_BPPModes::GetDCUBPPFlag(UIWidgetBase::BitplaneDefinition_t independentBPPDef)
{
    return m_bppMap.value(independentBPPDef).bspEnumItem;
}

