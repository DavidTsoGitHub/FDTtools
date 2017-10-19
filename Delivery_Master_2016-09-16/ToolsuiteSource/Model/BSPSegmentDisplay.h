#ifndef BSPSEGMENTDISPLAY
#define BSPSEGMENTDISPLAY

#include <QMap>
#include <QStringList>

class BSPSegmentDisplay
{
public:
    struct SegmentStatus
    {
        bool A;
        bool B;
        bool C;
        bool D;
        bool E;
        bool F;
        bool G;
    };

    struct SegmentBitMapping
    {
        QString Bit0;
        QString Bit1;
        QString Bit2;
        QString Bit3;
        QString Bit4;
        QString Bit5;
        QString Bit6;
        QString Bit7;
    };

    BSPSegmentDisplay();
    ~BSPSegmentDisplay();

    const QMap<QChar, const SegmentStatus*>& GetCharacterSegmentMapping() const;
    const QStringList& GetSpecialSegments() const;
    const QMap<quint8, const SegmentBitMapping*>& GetSegmentBitMappings() const;

    void AddCharacterSegmentMapping(QChar character, QString segments);
    void AddSpecialSegment(QString segmentName);
    void AddSegmentBitMapping(quint8 byteIndex, const SegmentBitMapping* segmentBitMapping);

private:
    QMap<QChar, const SegmentStatus*> m_characterSegmentMapping;
    QMap<quint8, const SegmentBitMapping*> m_segmentBitMappings;
    QStringList m_specialSegments;
};

#endif // BSPSEGMENTDISPLAY

