#ifndef ANIMATIONDESERIALIZER
#define ANIMATIONDESERIALIZER

#include "AnimationDefinition.h"
#include "DeserializerBase.h"

#include <QString>
#include <QXmlStreamReader>

class AnimationDeserializer : public DeserializerBase
{
public:
    explicit AnimationDeserializer(AnimationDefinition& imageDefinition);
    ~AnimationDeserializer(void);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
   AnimationDefinition& m_animationDefinition;
};

#endif // ANIMATIONDESERIALIZER

