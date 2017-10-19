#ifndef CONFIGFILEREADER_H
#define CONFIGFILEREADER_H

#include "DeserializerBase.h"

#include <QTextStream>
#include <QString>

class ConfigFileReader
{
public:
    ConfigFileReader(void);
    ~ConfigFileReader(void);

    DeserializerBase::DeserializerRespons Deserialize(DeserializerBase* deserializer, QString path);
    const QString Error() const;

private:
    QString m_error;
};

#endif // CONFIGFILEREADER_H
