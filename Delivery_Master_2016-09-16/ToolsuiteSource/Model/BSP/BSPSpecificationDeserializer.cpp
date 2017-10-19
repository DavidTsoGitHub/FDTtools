#include "BSPSpecificationDeserializer.h"

#include "BSPEnum.h"
#include "BSPDisplay.h"
#include "BSPFixedPointFloat.h"
#include "BSPFunction.h"
#include "BSPFunctionArgument.h"
#include "BSPMapping.h"
#include "BSPMemory.h"
#include "BSPRunnable.h"
#include "BSPSegmentDisplay.h"
#include "BSPSpecification.h"
#include "CalibrationParameter.h"
#include "Logger.h"

#include <QXmlStreamReader>

bool isEndOfElement(QXmlStreamReader& xmlReader, QString elementName)
{
    return (xmlReader.isEndElement() && xmlReader.name() == elementName);
}

BSPSpecificationDeserializer::BSPSpecificationDeserializer(BSPSpecification& bspSpecification) : DeserializerBase(NULL),
  m_bspSpecification(bspSpecification)
{}

DeserializerBase::DeserializerRespons BSPSpecificationDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    QStringList addedInitFunctions;

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "BSP")
    {
        m_error = "No BSP root element found in XML!";
        return ErrorTagNotFound;
    }
    else
    {
//        //if(!xmlReader.attributes().hasAttribute("AIVersion"))
//        //{
//            m_error = "No BSP AIVersion attribute found in XML!";
//            return ErrorAIVNotFound;
//        }
//        else if(xmlReader.attributes().value("AIVersion").toDouble() != QString(AI_VERSION).toDouble())
//        {
//            m_error = "Wrong BSP AIVersion attribute found in XML!";
//            return ErrorWrongAIV;
//        }
    }

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "BSPTypes")
            {
                xmlReader.readNext();
                while (!isEndOfElement(xmlReader, "BSPTypes"))
                {
                    if (xmlReader.name() == "BSPEnumerations")
                    {
                        xmlReader.readNext();
                        while (!isEndOfElement(xmlReader, "BSPEnumerations"))
                        {
                            if (xmlReader.name() == "BSPEnum")
                            {
                                BSPEnum* bspEnum = new BSPEnum();
                                bspEnum->Name = xmlReader.attributes().value("Name").toString();
                                bspEnum->Description = xmlReader.attributes().value("Description").toString();

                                while (!isEndOfElement(xmlReader, "BSPEnum"))
                                {
                                    if(xmlReader.name() == "Items")
                                    {
                                        while (!isEndOfElement(xmlReader, "Items"))
                                        {
                                            xmlReader.readNext();

                                            if (xmlReader.name() == "Item")
                                            {
                                                BSPEnumItem bspEnumItem;

                                                bspEnumItem.Value = xmlReader.attributes().value("Value").toInt();
                                                bspEnumItem.Identifier = xmlReader.attributes().value("Identifier").toString();
                                                bspEnumItem.Description = xmlReader.attributes().value("Description").toString();
                                                bspEnum->EnumItems.insert(bspEnumItem.Value, bspEnumItem);

                                                while(!isEndOfElement(xmlReader, "Item"))
                                                {
                                                    xmlReader.readNext();
                                                }
                                            }
                                        }
                                    }

                                    xmlReader.readNext();
                                }

                                m_bspSpecification.AddEnum(bspEnum);
                            }

                            xmlReader.readNext();
                        }
                    }
                    else if (xmlReader.name() == "BSPFixedPointFloats")
                    {
                        while (xmlReader.readNextStartElement())
                        {
                            if (xmlReader.name() == "BSPFixedPointFloat")
                            {
                                BSPFixedPointFloat* bspFpf = new BSPFixedPointFloat();

                                bspFpf->Name = xmlReader.attributes().value("Name").toString();
                                bspFpf->BaseType = xmlReader.attributes().value("BaseType").toString();
                                bspFpf->Unit = xmlReader.attributes().value("Unit").toString();
                                bspFpf->SignificantBits = xmlReader.attributes().value("SignificantBits").toInt();
                                bspFpf->FractionalBits = xmlReader.attributes().value("FractionalBits").toInt();
                                QString endianess = xmlReader.attributes().value("Endianess").toString();
                                if (endianess.compare("bigendian", Qt::CaseInsensitive) == 0)
                                {
                                    bspFpf->Endianess = BSPFixedPointFloat::BigEndian;
                                }
                                else if (endianess.compare("littleendian", Qt::CaseInsensitive) == 0)
                                {
                                    bspFpf->Endianess = BSPFixedPointFloat::LittleEndian;
                                }
                                else
                                {
                                    bspFpf->Endianess = BSPFixedPointFloat::UnknownEndian;
                                    LOG_WARNING("Unknown endianess for FixedPointFloat " + bspFpf->Name);
                                }

                                m_bspSpecification.AddFixedPointFloat(bspFpf);
                            }

                            xmlReader.readNext();
                        }
                    }

                    xmlReader.readNext();
                }
            }
            else if (xmlReader.name() == "BSPRunnables")
            {
                while (!isEndOfElement(xmlReader, "BSPRunnables"))
                {
                    xmlReader.readNext();

                    if (!xmlReader.isStartElement()) continue;
                    if (xmlReader.name() == "BSPRunnable")
                    {
                        QString init = xmlReader.attributes().value("Init").toString();
                        QString runnable = xmlReader.attributes().value("Runnable").toString();
                        quint32 cycleTime = xmlReader.attributes().value("CycleTime").toUInt();
                        quint32 initialDelay = xmlReader.attributes().value("InitialDelay").toUInt();

                        if (addedInitFunctions.contains(init))
                        {
                            init = "";
                        }
                        else
                        {
                            addedInitFunctions.append(init);
                        }

                        BSPRunnable* bspRunnable = new BSPRunnable(init, runnable, cycleTime, initialDelay);
                        m_bspSpecification.AddRunnable(bspRunnable);
                    }
                }
            }
            else if (xmlReader.name() == "BSPCalibrations")
            {
                while (!isEndOfElement(xmlReader, "BSPCalibrations"))
                {
                    xmlReader.readNext();

                    if (!xmlReader.isStartElement()) continue;
                    if (xmlReader.name() == "BSPCalibration")
                    {
                        QString name = xmlReader.attributes().value("Name").toString();
                        QString dataType = xmlReader.attributes().value("DataType").toString();
                        quint8 arrayLength = xmlReader.attributes().value("ArrayLength").toInt();
                        QString defaultValue = xmlReader.attributes().value("DefaultValue").toString();
                        QString description = xmlReader.attributes().value("Description").toString();

                        CalibrationParameter* cal = new CalibrationParameter();
                        cal->SetParameterName(name);
                        cal->SetDataType(dataType);
                        cal->SetArrayLength(arrayLength);
                        cal->SetDescription(description);

                        m_bspSpecification.AddCalibrationParameter(cal, defaultValue);
                    }
                }
            }
            else if (xmlReader.name() == "BSPFunctions")
            {
                xmlReader.readNext();
                while (!isEndOfElement(xmlReader, "BSPFunctions"))
                {
                    if (xmlReader.name() == "BSPFunction")
                    {
                        BSPFunction* function = parseFunction(xmlReader);
                        m_bspSpecification.AddFunction(function);
                    }
                    xmlReader.readNext();
                }
            }
            else if (xmlReader.name() == "BSPSegmentDisplay")
            {
                xmlReader.readNext();

                BSPSegmentDisplay* segmentDisplay = this->parseSegmentDisplay(xmlReader);
                if (segmentDisplay == NULL)
                {
                    m_error += "Failed to parse segment display section of BSP specification XML!";
                }
                else
                {
                    m_bspSpecification.SetSegmentDisplay(segmentDisplay);
                }
            }
            else if (xmlReader.name() == "BSPDisplay")
            {
                BSPDisplay* display = this->parseDisplay(xmlReader);
                if (display == NULL)
                {
                    m_error += "Failed to parse display section of BSP specification XML!";
                }
                else
                {
                    m_bspSpecification.SetDisplay(display);
                }
            }
            else if (xmlReader.name() == "BSPMemory")
            {
                BSPMemory* memory = this->parseMemory(xmlReader);
                if (memory == NULL)
                {
                    m_error += "Failed to parse memory section of BSP specification XML!";
                }
                else
                {
                    m_bspSpecification.SetMemory(memory);
                }
            }
        }
    }

    if(xmlReader.hasError() || !m_error.isEmpty())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "BSPSpecification XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of BSPSpecification XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in BSPSpecification XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    m_bspSpecification.ParseGCLSignals();

    return DeserializationOK;
}

BSPSegmentDisplay* BSPSpecificationDeserializer::parseSegmentDisplay(QXmlStreamReader &xml)
{
    BSPSegmentDisplay* segmentDisplay = new BSPSegmentDisplay();

    while (!xml.atEnd() && !(xml.isEndElement() && xml.name() == "BSPSegmentDisplay"))
    {
        xml.readNext();
        if (!xml.isStartElement()) continue;

        if (xml.name() == "CharacterMappings")
        {
            while (!xml.atEnd() && !(xml.isEndElement() && xml.name() == "CharacterMappings"))
            {
                xml.readNext();
                if (!xml.isStartElement()) continue;
                if (xml.name() == "CharacterMapping")
                {
                    QString character = xml.attributes().value("Char").toString();
                    QString segments = xml.attributes().value("Segments").toString();
                    segmentDisplay->AddCharacterSegmentMapping(character.at(0), segments);
                }
            }
        }
        else if (xml.name() == "SpecialSegments")
        {
            while (!xml.atEnd() && !(xml.isEndElement() && xml.name() == "SpecialSegments"))
            {
                xml.readNext();
                if (!xml.isStartElement()) continue;
                if (xml.name() == "SpecialSegment")
                {
                    QString segmentName = xml.attributes().value("ID").toString();
                    segmentDisplay->AddSpecialSegment(segmentName);
                }
            }
        }
        else if (xml.name() == "SegmentBitMappings")
        {
            while (!xml.atEnd() && !(xml.isEndElement() && xml.name() == "SegmentBitMappings"))
            {
                xml.readNext();
                if (!xml.isStartElement()) continue;
                if (xml.name() == "SegmentBitMapping")
                {
                    quint8 byteIndex = xml.attributes().value("Byte").toUInt();
                    BSPSegmentDisplay::SegmentBitMapping* segmentBitMapping = new BSPSegmentDisplay::SegmentBitMapping();

                    segmentBitMapping->Bit0 = xml.attributes().value("Bit0").toString();
                    segmentBitMapping->Bit1 = xml.attributes().value("Bit1").toString();
                    segmentBitMapping->Bit2 = xml.attributes().value("Bit2").toString();
                    segmentBitMapping->Bit3 = xml.attributes().value("Bit3").toString();
                    segmentBitMapping->Bit4 = xml.attributes().value("Bit4").toString();
                    segmentBitMapping->Bit5 = xml.attributes().value("Bit5").toString();
                    segmentBitMapping->Bit6 = xml.attributes().value("Bit6").toString();
                    segmentBitMapping->Bit7 = xml.attributes().value("Bit7").toString();

                    segmentDisplay->AddSegmentBitMapping(byteIndex, segmentBitMapping);
                }
            }
        }
    }

    return segmentDisplay;
}

BSPDisplay* BSPSpecificationDeserializer::parseDisplay(QXmlStreamReader &xml)
{
    BSPDisplay* display = new BSPDisplay();

    if (xml.name() == "BSPDisplay")
    {
        quint16 width = xml.attributes().value("Width").toInt();
        quint16 height = xml.attributes().value("Height").toInt();
        quint32 availableMemory = xml.attributes().value("AvailableMemory").toInt();
        QString useGRAM = xml.attributes().value("UseGRAM").toString();
        quint16 layers = xml.attributes().value("NumberOfLayers").toInt();
        QString renderingType = xml.attributes().value("RenderingType").toString();
        QString renderingOrientation = xml.attributes().value("RenderingOrientation").toString();

        display->SetWidth(width);
        display->SetHeight(height);
        display->SetAvailableMemory(availableMemory);
        display->SetUseGRAM( (useGRAM.compare("true", Qt::CaseInsensitive) == 0) ? true : false );
        display->SetNumberOfLayers(layers);

        if (renderingType.compare("SW", Qt::CaseInsensitive) == 0)
        {
            display->SetRenderingType(BSPDisplay::SW);
            display->SetBpp(1);
        }
        else if (renderingType.compare("HW", Qt::CaseInsensitive) == 0)
        {
            bool requiredBPPParamsInSpec = false;
            foreach (const BSPEnum* bspEnum, m_bspSpecification.Enums())
            {
                if (bspEnum->Name.compare("BSP_DCU_BPPModes") == 0) requiredBPPParamsInSpec = true;
            }

            if (!requiredBPPParamsInSpec) return NULL;
            display->SetRenderingType(BSPDisplay::HW);
            display->SetBpp(8);
        }
        else
        {
            return NULL;
        }

        if (renderingOrientation.compare("Normal", Qt::CaseInsensitive) == 0)
        {
            display->SetRenderingOrientation(BSPDisplay::Normal);
        }
        else if (renderingOrientation.compare("CCW", Qt::CaseInsensitive) == 0)
        {
            display->SetRenderingOrientation(BSPDisplay::CounterClockWise);
        }
        else
        {
            return NULL;
        }
    }



    return display;
}

BSPMemory* BSPSpecificationDeserializer::parseMemory(QXmlStreamReader &xml)
{
    BSPMemory* memory = new BSPMemory();

    if (xml.name() == "BSPMemory")
    {
        bool status = false;
        quint32 internalMemorySize = xml.attributes().value("InternalMemorySize").toULong(&status, 16);
        quint32 externalMemorySize = xml.attributes().value("ExternalMemorySize").toULong(&status, 16);
        quint32 externalMemoryAddress = xml.attributes().value("ExternalMemoryAddress").toULong(&status, 16);

        memory->SetInternalMemorySize(internalMemorySize);
        memory->SetExternalMemorySize(externalMemorySize);
        memory->SetExternalMemoryAddress(externalMemoryAddress);
    }
    else
    {
        return NULL;
    }

    return memory;
}

BSPFunction* BSPSpecificationDeserializer::parseFunction(QXmlStreamReader& xml)
{
    BSPFunction* bspFunction = new BSPFunction();

    QString mappingType;
    while (!xml.atEnd() && !(xml.isEndElement() && xml.name() == "BSPFunction"))
    {
        if(xml.name() == "BSPFunction")
        {
            bspFunction->Name = xml.attributes().value("FunctionName").toString();
            bspFunction->GCLSignalName = xml.attributes().value("GCLSignalName").toString();

            QString gclSignalType = xml.attributes().value("GCLSignalType").toString();
            if (gclSignalType.compare("sink", Qt::CaseInsensitive) == 0)
            {
                bspFunction->GCLSignalType = BSPFunction::Sink;
            }
            else if (gclSignalType.compare("source", Qt::CaseInsensitive) == 0)
            {
                bspFunction->GCLSignalType = BSPFunction::Source;
            }
            else
            {
                bspFunction->GCLSignalType = BSPFunction::Undefined;
            }

            bspFunction->Description = xml.attributes().value("Description").toString();
            bspFunction->ReturnType = xml.attributes().value("Return").toString();

            mappingType = xml.attributes().value("MappingType").toString();
        }

        xml.readNext();
        if (xml.isStartElement())
        {
            if(xml.name() == "Argument")
            {
                BSPFunctionArgument item = this->parseFunctionArg(xml);
                bspFunction->Arguments.insert(item.Order, item);
            }
            else if(xml.name() == "IndexToSignalMappings")
            {
                BSPMapping* bspMapping = parseFunctionMapping(xml);
                bspMapping->Type = mappingType;
                bspMapping->FunctionName = bspFunction->Name;
                m_bspSpecification.AddMapping(bspMapping);
            }
        }
    }

    return bspFunction;
}

BSPFunctionArgument BSPSpecificationDeserializer::parseFunctionArg(QXmlStreamReader& xml)
{
    BSPFunctionArgument argItem;
    if(xml.attributes().hasAttribute("Order"))
    {
        argItem.Order = xml.attributes().value("Order").toInt();
    }

    argItem.Name = xml.attributes().value("ArgumentName").toString();
    argItem.Type = xml.attributes().value("ArgumentType").toString();

    while(!xml.atEnd() && !(xml.isEndElement() && xml.name() == "Argument"))
    {
        xml.readNext();
    }

    return argItem;
}

BSPMapping* BSPSpecificationDeserializer::parseFunctionMapping(QXmlStreamReader &xml)
{
    BSPMapping* bspMapping = new BSPMapping();
    while (!isEndOfElement(xml, "IndexToSignalMappings"))
    {
        if (xml.isStartElement() && xml.name() == "IndexToSignalMapping")
        {
            IndexToSignalMapping* indexToSignalMapping = new IndexToSignalMapping();
            indexToSignalMapping->GCLSignal = xml.attributes().value("GCLSignal").toString();
            indexToSignalMapping->ID = xml.attributes().value("ID").toInt(NULL, 0);
            indexToSignalMapping->Description = xml.attributes().value("Description").toString();
            bspMapping->IndexToSignalMappings.append(indexToSignalMapping);
        }
        xml.readNext();
    }

    return bspMapping;
}
