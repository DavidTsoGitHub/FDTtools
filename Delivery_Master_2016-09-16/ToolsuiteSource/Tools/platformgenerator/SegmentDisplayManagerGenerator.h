#ifndef SEGMENTDISPLAYMANAGERGENERATOR_H
#define SEGMENTDISPLAYMANAGERGENERATOR_H

#include "BSPSegmentDisplay.h"
#include "FileGeneratorBase.h"
#include "Project.h"
#include "SegmentDisplay.h"

class SegmentDisplayManagerGenerator : public FileGeneratorBase
{
public:
    explicit SegmentDisplayManagerGenerator(const SegmentDisplay& display, const BSPSegmentDisplay& bspDefinition);
    ~SegmentDisplayManagerGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    const SegmentDisplay& m_display;
    const BSPSegmentDisplay& m_bspDefinition;

    QString m_SegmentDisplayManagerSourceConfigFile;
    QString m_SegmentDisplayManagerHeaderConfigFile;

    void generateSegmentDisplayManagerSourceConfigurationFile(void);
    void generateSegmentDisplayManagerHeaderConfigurationFile(void);
    void createStaticSegmentDisplayManagerFiles(void);
    QString getByteString(const BSPSegmentDisplay::SegmentBitMapping& mapping, const BSPSegmentDisplay::SegmentStatus& status);
    bool getBitStatus(QString segmentName, const BSPSegmentDisplay::SegmentStatus& status);
    void findBitAndByteIndexForSpecialSegment(QString specialSegmentName, quint8* byteIndex, quint8* bitIndex);
};

#endif // SEGMENTDISPLAYMANAGERGENERATOR_H
