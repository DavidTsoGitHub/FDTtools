#ifndef ANIMATIONSERIALIZER
#define ANIMATIONSERIALIZER

#include "SerializerBase.h"
#include "AnimationDefinition.h"

#include <QXmlStreamWriter>

class AnimationSerializer : public SerializerBase
{
public:
    explicit AnimationSerializer(AnimationDefinition& imageDefinition);
    ~AnimationSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    AnimationDefinition& m_animationDefinition;

    void writeCommonAttributes(QXmlStreamWriter& xmlWriter, const Animation& animation);
};

#endif // ANIMATIONSERIALIZER

