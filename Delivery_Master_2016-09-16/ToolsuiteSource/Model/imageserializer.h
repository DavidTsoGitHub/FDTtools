#ifndef IMAGESERIALIZER
#define IMAGESERIALIZER

#include "SerializerBase.h"
#include "ImageDefinition.h"

class ImageSerializer : public SerializerBase
{
public:
    explicit ImageSerializer(ImageDefinition& imageDefinition);
    ~ImageSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    ImageDefinition& m_imageDefinition;
};

#endif // IMAGESERIALIZER

