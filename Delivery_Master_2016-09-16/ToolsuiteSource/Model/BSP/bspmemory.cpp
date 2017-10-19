#include "BSPMemory.h"

BSPMemory::BSPMemory() :
    m_internalMemorySize(0),
    m_externalMemorySize(0),
    m_externalMemoryAddress(0)
{

}

void BSPMemory::SetInternalMemorySize(quint64 size)
{
    m_internalMemorySize = size;
}

quint32 BSPMemory::GetInternalMemorySize() const
{
    return m_internalMemorySize;
}

void BSPMemory::SetExternalMemorySize(quint32 size)
{
    m_externalMemorySize = size;
}

quint32 BSPMemory::GetExternalMemorySize() const
{
    return m_externalMemorySize;
}

void BSPMemory::SetExternalMemoryAddress(quint32 address)
{
    m_externalMemoryAddress = address;
}

quint32 BSPMemory::GetExternalMemoryAddress() const
{
    return m_externalMemoryAddress;
}
