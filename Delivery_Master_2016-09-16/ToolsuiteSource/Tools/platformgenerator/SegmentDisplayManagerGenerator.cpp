#include "SegmentDisplayManagerGenerator.h"
#include "Logger.h"

SegmentDisplayManagerGenerator::SegmentDisplayManagerGenerator(const SegmentDisplay& display, const BSPSegmentDisplay& bspDefinition) :
    m_display(display),
    m_bspDefinition(bspDefinition)
{
    m_SegmentDisplayManagerSourceConfigFile = QString(PlatformPaths::CreateServicePath("SegmentDisplayManager")).append("/SegmentDisplayManager_cfg.c");
    m_SegmentDisplayManagerHeaderConfigFile = QString(PlatformPaths::CreateServicePath("SegmentDisplayManager")).append("/SegmentDisplayManager_cfg.h");
}

SegmentDisplayManagerGenerator::~SegmentDisplayManagerGenerator()
{
}

void SegmentDisplayManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");

    LOG_INFO("Generating SegmentDisplayManager!");
    generateSegmentDisplayManagerSourceConfigurationFile();
    generateSegmentDisplayManagerHeaderConfigurationFile();
    createStaticSegmentDisplayManagerFiles();
}

void SegmentDisplayManagerGenerator::generateSegmentDisplayManagerSourceConfigurationFile()
{
    QStringList fileLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"SegmentDisplayManager_cfg.h\"");
    fileLines.append("#include \"GCL_SegmentDisplayManager.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");    
    fileLines.append("");
    fileLines.append("const uint8 SegmentDigitCount = SEGMENT_DIGIT_COUNT;");
    fileLines.append("const uint8 SegmentPatternCount = SEGMENT_PATTERN_COUNT;");
    fileLines.append("const uint8 DigitBitPattern[SEGMENT_DIGIT_COUNT][SEGMENT_PATTERN_COUNT] = ");
    fileLines.append("{");
    const QMap<QChar, const BSPSegmentDisplay::SegmentStatus*> characterSegmentMapping = m_bspDefinition.GetCharacterSegmentMapping();
    const QMap<quint8, const BSPSegmentDisplay::SegmentBitMapping*> segmentBitMapping = m_bspDefinition.GetSegmentBitMappings();

    QString characterLine("  /*");

    foreach (QChar character, characterSegmentMapping.keys())
    {
        characterLine.append(QString("%1").arg(character, 6));
    }
    characterLine.append(" */");

    fileLines.append(characterLine);

    QStringList bitPatterns;
    foreach (quint8 key, segmentBitMapping.keys())
    {
        QString line("    { ");

        const BSPSegmentDisplay::SegmentBitMapping* mapping = segmentBitMapping.value(key);

        QStringList bytes;
        foreach(const BSPSegmentDisplay::SegmentStatus* status, characterSegmentMapping.values())
        {
            bytes.append(getByteString(*mapping, *status));
        }

        line.append(bytes.join(", "));
        line.append(" }");
        bitPatterns.append(line);
    }
    fileLines.append(bitPatterns.join(",\n"));
    fileLines.append("};");
    fileLines.append("const uint8 SpecialSegmentsCount = SPECIAL_SEGMENT_COUNT;");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("/* SpecialSegments cannot be const:ed as the previous status is stored and updated in this array */");
    fileLines.append("SpecialSegment SpecialSegments[SPECIAL_SEGMENT_COUNT] = ");
    fileLines.append("{");
    QStringList specialSegmentDefinitions;
    foreach (QString specialSegment, m_bspDefinition.GetSpecialSegments())
    {
        QString specialSegmentDefinition("    { ");
        quint8 byteIndex;
        quint8 bitIndex;
        findBitAndByteIndexForSpecialSegment(specialSegment, &byteIndex, &bitIndex);
        specialSegmentDefinition.append(".byteIndex = " + QString::number(byteIndex) + ", ");
        specialSegmentDefinition.append(".bitIndex = " + QString::number(bitIndex) + ", ");
        specialSegmentDefinition.append(".gclReadFunction = GCL_Read_SegmentDisplayManager_" + specialSegment + "SegmentStatus");
        specialSegmentDefinition.append(" }");
        specialSegmentDefinitions.append(specialSegmentDefinition);
    }
    fileLines.append(specialSegmentDefinitions.join(",\n"));
    fileLines.append("};");
    fileLines.append("uint8 SegmentData[SEGMENT_DIGIT_COUNT] = { 0 };");
    fileLines.append("");

    AddGeneratedFile(m_SegmentDisplayManagerSourceConfigFile, fileLines.join("\n"));
}

void SegmentDisplayManagerGenerator::generateSegmentDisplayManagerHeaderConfigurationFile()
{
    QStringList fileLines;

    fileLines.append("#ifndef SEGMENTDISPLAYMANAGER_CFG_H_");
    fileLines.append("#define SEGMENTDISPLAYMANAGER_CFG_H_");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("        uint8 byteIndex;");
    fileLines.append("        uint8 bitIndex;");
    fileLines.append("        uint8 lastStatus;");
    fileLines.append("        void (*gclReadFunction)(uint8* segmentStatus, uint8* signalStatus);");
    fileLines.append("    } SpecialSegment;");
    fileLines.append("");
    fileLines.append("#define SEGMENT_DIGIT_COUNT " + QString::number(m_bspDefinition.GetSegmentBitMappings().count()) + "U");
    fileLines.append("#define SEGMENT_PATTERN_COUNT " + QString::number(m_bspDefinition.GetCharacterSegmentMapping().count()) + "U");
    fileLines.append("#define SPECIAL_SEGMENT_COUNT " + QString::number(m_bspDefinition.GetSpecialSegments().count()) + "U");
    fileLines.append("");
    fileLines.append("extern const uint8 SegmentDigitCount;");
    fileLines.append("extern const uint8 DigitBitPattern[SEGMENT_DIGIT_COUNT][SEGMENT_PATTERN_COUNT];");
    fileLines.append("extern const uint8 SegmentPatternCount;");
    fileLines.append("extern const uint8 SpecialSegmentsCount;");
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("extern SpecialSegment SpecialSegments[SPECIAL_SEGMENT_COUNT];");
    fileLines.append("extern uint8 SegmentData[SEGMENT_DIGIT_COUNT];");
    fileLines.append("");
    fileLines.append("#endif // SEGMENTDISPLAYMANAGER_CFG_H_");
    fileLines.append("");

    AddGeneratedFile(m_SegmentDisplayManagerHeaderConfigFile, fileLines.join("\n"));
}

void SegmentDisplayManagerGenerator::createStaticSegmentDisplayManagerFiles()
{
    AddFilesFromResourcePath(":/SegmentDisplayManagerFiles");
}

QString SegmentDisplayManagerGenerator::getByteString(const BSPSegmentDisplay::SegmentBitMapping& mapping, const BSPSegmentDisplay::SegmentStatus& status)
{
    QString result("0x%1");
    quint8 value = 0;

    bool bit0 = getBitStatus(mapping.Bit0, status);
    bool bit1 = getBitStatus(mapping.Bit1, status);
    bool bit2 = getBitStatus(mapping.Bit2, status);
    bool bit3 = getBitStatus(mapping.Bit3, status);
    bool bit4 = getBitStatus(mapping.Bit4, status);
    bool bit5 = getBitStatus(mapping.Bit5, status);
    bool bit6 = getBitStatus(mapping.Bit6, status);
    bool bit7 = getBitStatus(mapping.Bit7, status);

    if (bit0) value |= (0x01 << 0);
    if (bit1) value |= (0x01 << 1);
    if (bit2) value |= (0x01 << 2);
    if (bit3) value |= (0x01 << 3);
    if (bit4) value |= (0x01 << 4);
    if (bit5) value |= (0x01 << 5);
    if (bit6) value |= (0x01 << 6);
    if (bit7) value |= (0x01 << 7);

    return result.arg(value, 2, 16, QChar('0'));
}

bool SegmentDisplayManagerGenerator::getBitStatus(QString segmentName, const BSPSegmentDisplay::SegmentStatus& status)
{
    if (segmentName == "A") return status.A;
    else if (segmentName == "B") return status.B;
    else if (segmentName == "C") return status.C;
    else if (segmentName == "D") return status.D;
    else if (segmentName == "E") return status.E;
    else if (segmentName == "F") return status.F;
    else if (segmentName == "G") return status.G;
    return false;
}

void SegmentDisplayManagerGenerator::findBitAndByteIndexForSpecialSegment(QString specialSegmentName, quint8* byteIndex, quint8* bitIndex)
{
    foreach (quint8 key, m_bspDefinition.GetSegmentBitMappings().keys())
    {
        const BSPSegmentDisplay::SegmentBitMapping* bitMapping = m_bspDefinition.GetSegmentBitMappings().value(key);

        if (bitMapping->Bit0 == specialSegmentName)
        {
            *bitIndex = 0;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit1 == specialSegmentName)
        {
            *bitIndex = 1;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit2 == specialSegmentName)
        {
            *bitIndex = 2;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit3 == specialSegmentName)
        {
            *bitIndex = 3;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit4 == specialSegmentName)
        {
            *bitIndex = 4;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit5 == specialSegmentName)
        {
            *bitIndex = 5;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit6 == specialSegmentName)
        {
            *bitIndex = 6;
            *byteIndex = key;
            return;
        }
        else if (bitMapping->Bit7 == specialSegmentName)
        {
            *bitIndex = 7;
            *byteIndex = key;
            return;
        }
    }

    *bitIndex = 0;
    *byteIndex = 0;
}
