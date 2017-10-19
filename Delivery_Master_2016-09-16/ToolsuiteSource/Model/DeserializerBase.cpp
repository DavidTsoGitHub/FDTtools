#include "DeserializerBase.h"

#include "Project.h"
#include "Logger.h"

#include <QMessageBox>
#include <QString>
#include <QXmlStreamReader>

DeserializerBase::DeserializerBase(const char* encoding) :
    m_encoding(encoding)
{
}

DeserializerBase::~DeserializerBase()
{
}

const char* DeserializerBase::GetEncoding() const
{
    return m_encoding;
}

const QString DeserializerBase::Error() const
{
    return m_error;
}

bool DeserializerBase::isEndOfElement(QXmlStreamReader& xmlReader, QString elementName)
{
    return (xmlReader.isEndElement() && xmlReader.name() == elementName);
}
