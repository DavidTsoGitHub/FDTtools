#include "GCLSignalSourceMetaType.h"

GCLSignalSourceMetaType::GCLSignalSourceMetaType()
{

}

GCLSignalSourceMetaType::GCLSignalSourceMetaType(GCLSignalSource* signalSource) :
    m_signalSource(signalSource)
{}

GCLSignalSource* GCLSignalSourceMetaType::GetSignalSource() const
{
    return m_signalSource;
}

void GCLSignalSourceMetaType::SetSignalSource(GCLSignalSource* signalSource)
{
    m_signalSource = signalSource;
}
