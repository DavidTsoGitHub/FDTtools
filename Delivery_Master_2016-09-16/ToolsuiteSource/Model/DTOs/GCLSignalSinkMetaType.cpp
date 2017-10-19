#include "GCLSignalSinkMetaType.h"

GCLSignalSinkMetaType::GCLSignalSinkMetaType() :
    m_signalSink(NULL)
{

}

GCLSignalSinkMetaType::GCLSignalSinkMetaType(GCLSignalSink* signalSink):
    m_signalSink(signalSink)
{

}

GCLSignalSink* GCLSignalSinkMetaType::GetSignalSink() const
{
    return m_signalSink;
}

void GCLSignalSinkMetaType::SetSignalSink(GCLSignalSink* signalSink)
{
    m_signalSink = signalSink;
}

