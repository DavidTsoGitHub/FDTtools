#include "ExceptionBase.h"

ExceptionBase::ExceptionBase(QString name, QString information, QString message, bool isFatal) : runtime_error(message.toStdString()),
    m_fatal(isFatal),
    m_innerExceptionStrings(QStringList()),
    m_information(information),
    m_message(message),
    m_name(name)
{}

ExceptionBase::ExceptionBase(QString name, QString information, QString message, QStringList innerExceptionStrings, bool isFatal) : runtime_error(message.toStdString()),
    m_fatal(isFatal),
    m_innerExceptionStrings(innerExceptionStrings),
    m_information(information),
    m_message(message),
    m_name(name)
{}

ExceptionBase::~ExceptionBase() throw()
{}

QStringList& ExceptionBase::InnerExceptionStrings()
{
    return m_innerExceptionStrings;
}

const QString& ExceptionBase::Information() const
{
    return m_information;
}

const QString& ExceptionBase::Message() const
{
    return m_message;
}

const QString& ExceptionBase::Name() const
{
    return m_name;
}

const bool& ExceptionBase::Fatal() const
{
    return m_fatal;
}

