#include "ConfigFileReader.h"
#include "Logger.h"
#include "Exceptions/CannotOpenFileException.h"
#include "Exceptions/DeserializationExeception.h"

ConfigFileReader::ConfigFileReader()
{}

ConfigFileReader::~ConfigFileReader()
{}

DeserializerBase::DeserializerRespons ConfigFileReader::Deserialize(DeserializerBase* deserializer, QString path)
{
    LOG_DEBUG("Entering");
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        QString errorMessage("Error opening file:\n" + file.fileName() + "\n\nError: " + file.errorString());
        LOG_ERROR(errorMessage);
        m_error.append(errorMessage + "\n");
        return DeserializerBase::ErrorNoFile;
    }

    QTextStream xml(&file);

    if (deserializer->GetEncoding() != NULL)
    {
        xml.setCodec(deserializer->GetEncoding());
    }

    DeserializerBase::DeserializerRespons response = deserializer->Deserialize(xml.readAll());

    if (response != DeserializerBase::DeserializationOK)
    {
        QString errorMessage("Error deserializing xml file:\n" + file.fileName() + "\n\nError: " + deserializer->Error());
        LOG_ERROR(errorMessage);
        m_error.append(errorMessage + "\n");
    }

    file.close();
    return response;
}

const QString ConfigFileReader::Error() const
{
    return m_error;
}

