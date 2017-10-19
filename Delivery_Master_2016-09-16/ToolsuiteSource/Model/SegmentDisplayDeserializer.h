#ifndef SEGMENTDISPLAYDESERIALIZER
#define SEGMENTDISPLAYDESERIALIZER

#include <QString>

class SegmentDisplay;

class SegmentDisplayDeserializer
{
public:
    explicit SegmentDisplayDeserializer(const QString& displayXml);
    ~SegmentDisplayDeserializer();

    bool Deserialize(SegmentDisplay& displayDefinition);
    const QString Error() const;

private:
    const QString& m_displayXml;
    QString m_error;
};


#endif // SEGMENTDISPLAYDESERIALIZER

