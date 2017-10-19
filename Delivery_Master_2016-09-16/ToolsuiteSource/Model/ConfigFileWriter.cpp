#include "ConfigFileWriter.h"

#include <QFile>
#include <QTextStream>

ConfigFileWriter::ConfigFileWriter()
{
}

bool ConfigFileWriter::WriteToFile(SerializerBase& serializer, const QString& fileName, QString& errorText)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
    {
        errorText = file.errorString();
        return false;
    }

    QTextStream fileStream(&file);
    const char* codec = serializer.GetEncoding();
    if (codec != NULL)
    {
        fileStream.setCodec(codec);
    }

    QString content = serializer.Serialize();
    fileStream << content;
    fileStream.flush();
    file.close();

    return true;
}

