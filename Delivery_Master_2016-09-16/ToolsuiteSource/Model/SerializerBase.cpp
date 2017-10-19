#include "SerializerBase.h"
#include "Project.h"
#include "Logger.h"

SerializerBase::SerializerBase(const char* encoding) :
    m_encoding(encoding)
{
}

SerializerBase::~SerializerBase()
{
}

const char* SerializerBase::GetEncoding() const
{
    return m_encoding;
}


