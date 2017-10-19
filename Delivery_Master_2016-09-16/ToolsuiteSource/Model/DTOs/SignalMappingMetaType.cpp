#include "SignalMappingMetaType.h"

#include "SignalMapping.h"

SignalMappingMetaType::SignalMappingMetaType()
{

}

SignalMappingMetaType::SignalMappingMetaType(SignalMapping* signalMapping) :
    m_signalMapping(signalMapping)
{

}

SignalMapping*SignalMappingMetaType::GetSignalMapping() const
{
    return m_signalMapping;
}

void SignalMappingMetaType::SetSignalMapping(SignalMapping* signalMapping)
{
    m_signalMapping = signalMapping;
}

