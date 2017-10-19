#include "NoDCU.h"

#include "BSPEnumItem.h"

NoDCU::NoDCU()
{
}

quint8 NoDCU::GetBitsPerPixelForMode(UIWidgetBase::BitplaneDefinition_t /*independentBPPDef*/)
{
    return 0;
}

const QString NoDCU::GetBitsPerPixelForModeString(UIWidgetBase::BitplaneDefinition_t /*independentBPPDef*/)
{
    return QString();
}

BSPEnumItem NoDCU::GetDCUBPPFlag(UIWidgetBase::BitplaneDefinition_t /*independentBPPDef*/)
{
    return BSPEnumItem();
}
