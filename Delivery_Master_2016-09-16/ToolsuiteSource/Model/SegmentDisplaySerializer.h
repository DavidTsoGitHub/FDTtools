#ifndef SEGMENTDISPLAYSERIALIZER
#define SEGMENTDISPLAYSERIALIZER

#include "SerializerBase.h"

class SegmentDisplay;
class QString;

class SegmentDisplaySerializer : public SerializerBase
{
public:
    explicit SegmentDisplaySerializer(SegmentDisplay& displayDefinition);
    ~SegmentDisplaySerializer();

    QString Serialize();

private:
    SegmentDisplay& m_displayDefinition;
};

#endif // SEGMENTDISPLAYSERIALIZER

