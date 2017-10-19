#ifndef ICONDESERIALIZER_H
#define ICONDESERIALIZER_H

#include "IconDefinition.h"
#include "DeserializerBase.h"

#include <QString>

class IconDeserializer : public DeserializerBase
{
public:
    explicit IconDeserializer(IconDefinition& iconDefinition);
    ~IconDeserializer(void);

    const QList<QString> &GetMissingImageFilesList() const;

private:
    IconDefinition& m_iconDefinition;
    QList<QString> m_missingIconFilesList;
    bool verifyFileExistence(const QString &filename);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);
};

#endif // ICONDESERIALIZER_H
