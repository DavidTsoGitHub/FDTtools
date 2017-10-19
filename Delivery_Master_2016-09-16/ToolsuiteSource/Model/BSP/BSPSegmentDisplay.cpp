#include "BSPSegmentDisplay.h"

BSPSegmentDisplay::BSPSegmentDisplay()
{
}

BSPSegmentDisplay::~BSPSegmentDisplay()
{
}

const QMap<QChar, const BSPSegmentDisplay::SegmentStatus*>& BSPSegmentDisplay::GetCharacterSegmentMapping() const
{
    return m_characterSegmentMapping;
}

const QStringList& BSPSegmentDisplay::GetSpecialSegments() const
{
    return m_specialSegments;
}

const QMap<quint8, const BSPSegmentDisplay::SegmentBitMapping*>& BSPSegmentDisplay::GetSegmentBitMappings() const
{
    return m_segmentBitMappings;
}

void BSPSegmentDisplay::AddCharacterSegmentMapping(QChar character, QString activeSegments)
{
    SegmentStatus* segmentStatus = new SegmentStatus();
    segmentStatus->A = activeSegments.contains("A");
    segmentStatus->B = activeSegments.contains("B");
    segmentStatus->C = activeSegments.contains("C");
    segmentStatus->D = activeSegments.contains("D");
    segmentStatus->E = activeSegments.contains("E");
    segmentStatus->F = activeSegments.contains("F");
    segmentStatus->G = activeSegments.contains("G");

    m_characterSegmentMapping.insert(character, segmentStatus);
}

void BSPSegmentDisplay::AddSpecialSegment(QString segmentName)
{
    m_specialSegments.append(segmentName);
}

void BSPSegmentDisplay::AddSegmentBitMapping(quint8 byteIndex, const SegmentBitMapping* segmentBitMapping)
{
    m_segmentBitMappings.insert(byteIndex, segmentBitMapping);
}
