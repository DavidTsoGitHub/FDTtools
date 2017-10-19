#ifndef IMAGEDESERIALIZER
#define IMAGEDESERIALIZER

#include "ImageDefinition.h"
#include "DeserializerBase.h"

#include <QString>

class ImageDeserializer : public DeserializerBase
{
public:
    explicit ImageDeserializer(ImageDefinition& imageDefinition);
    ~ImageDeserializer(void);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);
    const QList<QString>& GetMissingImageFilesList() const;

private:
   ImageDefinition& m_imageDefinition;
   QList<QString> m_missingImageFilesList;

   bool verifyFileExistence(const QString& filename);
};

#endif // IMAGEDESERIALIZER

