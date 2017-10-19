#ifndef CONFIGFILEWRITER_H
#define CONFIGFILEWRITER_H

#include "SerializerBase.h"
#include <QString>

class ConfigFileWriter
{
public:
    static bool WriteToFile(SerializerBase& serializer, const QString& m_fileName, QString& errorText);

private:
    ConfigFileWriter();
};

#endif // FILEWRITER_H
