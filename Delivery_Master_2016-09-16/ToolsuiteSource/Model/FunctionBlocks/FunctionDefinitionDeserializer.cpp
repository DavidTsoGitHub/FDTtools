#include "FunctionDefinitionDeserializer.h"
#include "Logger.h"
#include "ConfigurationParameter.h"
#include "CalibrationParameter.h"

FunctionDefinitionDeserializer::FunctionDefinitionDeserializer(QString functionDefinitionXml) :
    m_xmlString(functionDefinitionXml)
{
}

bool FunctionDefinitionDeserializer::Deserialize(FunctionDefinition& functionDefinition)
{
    LOG_DEBUG("Entering");
    if (m_xmlString.isNull() || m_xmlString.isEmpty())
    {
        m_error = "XML string is empty!";
        return false;
    }

    bool functionDefinitionElementFound = false;
    QXmlStreamReader xml(m_xmlString);

    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.name().compare(QString("FunctionDefinition")) != 0) continue;

        functionDefinitionElementFound = true;
        parseFunctionDefinition(xml, functionDefinition);
    }

    if (!functionDefinitionElementFound)
    {
        m_error = "No FunctionDefinition element found in XML!";
        return false;
    }

    if (xml.hasError() || !m_error.isEmpty())
    {
        switch (xml.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "FunctionDefinition XML not well formed! Line:" +  QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of FunctionDefinition XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in FunctionDefinition XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xml.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return false;
    }

    return true;
}

void FunctionDefinitionDeserializer::parseFunctionDefinition(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "FunctionDefinition" && xml.isStartElement())
    {
        functionDefinition.SetName(xml.attributes().value("Name").toString());
        xml.readNext();
    }

    while (!(xml.name() == "FunctionDefinition" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "Tasks")
        {
            parseTasks(xml, functionDefinition);
        }
        else if (xml.name() == "SignalInterface")
        {
            parseSignalInterface(xml, functionDefinition);
        }
        else if (xml.name() == "ConfigurationParameters")
        {
            parseConfigurationParameters(xml, functionDefinition);
        }
        else if (xml.name() == "CalibrationParameters")
        {
            parseCalibrationParameters(xml, functionDefinition);
        }
        else if (xml.name() == "LabelMappingsParameters")
        {
            parseLabelMappingsParameters(xml, functionDefinition);
        }
        else if (xml.name() == "FaultDefinitions")
        {
            parseFaultDefinitions(xml, functionDefinition);
        }
        else if (xml.name() == "UIResources")
        {
            parseUIResources(xml, functionDefinition);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseTasks(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "Tasks" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "Tasks" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "Task")
        {
            parseTask(xml, functionDefinition);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseTask(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "Task" && xml.isStartElement())
    {
        xml.readNext();
    }

    Task* task = new Task();

    while (!(xml.name() == "Task" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "InitFunction")
        {
            task->InitFunction = xml.readElementText();
        }
        else if (xml.name() == "Runnable")
        {
            task->Runnable = xml.readElementText();
        }
        else if (xml.name() == "StartupDelay")
        {
            task->StartupDelay = xml.readElementText().toInt();
        }
        else if (xml.name() == "Periodicity")
        {
            task->Periodicity = xml.readElementText().toInt();
        }

        xml.readNext();
    }

    functionDefinition.AddTask(task);
}

void FunctionDefinitionDeserializer::parseSignalInterface(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "SignalInterface" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "SignalInterface" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "InputSignals")
        {
            parseInputSignals(xml, functionDefinition);
        }
        else if (xml.name() == "OutputSignals")
        {
            parseOutputSignals(xml, functionDefinition);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseInputSignals(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "InputSignals" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "InputSignals" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "Signal" && xml.isStartElement())
        {
            QString signalName;
            QString dataType;
            bool updateNotification = false;
            bool mandatory = true;

            foreach (QXmlStreamAttribute attribute, xml.attributes())
            {
                if (attribute.name() == "Name")
                {
                    signalName = attribute.value().toString();
                }
                else if (attribute.name() == "DataType")
                {
                    dataType = attribute.value().toString();
                }
                else if (attribute.name() == "UpdateNotification")
                {
                    if (attribute.value().compare("true", Qt::CaseInsensitive) == 0)
                    {
                        updateNotification = true;
                    }
                    else
                    {
                        updateNotification = false;
                    }
                }
                else if (attribute.name() == "Mandatory")
                {
                    if (attribute.value().compare("true", Qt::CaseInsensitive) == 0)
                    {
                        mandatory = true;
                    }
                    else
                    {
                        mandatory = false;
                    }
                }
                else
                {
                    LOG_WARNING("Unknown attribute '" + attribute.name().toString() + "' in input signal for function block '" + functionDefinition.GetName() + "'");
                }
            }

            GCLSignalSink* sink = new GCLSignalSink(signalName, functionDefinition.GetName(), dataType, updateNotification, mandatory);
            functionDefinition.AddGCLSignalSink(sink);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseOutputSignals(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "OutputSignals" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "OutputSignals" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "Signal" && xml.isStartElement())
        {
            QString signalName;
            QString dataType;

            foreach (QXmlStreamAttribute attribute, xml.attributes())
            {
                if (attribute.name() == "Name")
                {
                    signalName = attribute.value().toString();
                }
                else if (attribute.name() == "DataType")
                {
                    dataType = attribute.value().toString();
                }
                else
                {
                    LOG_WARNING("Unknown attribute '" + attribute.name().toString() + "' in output signal for function block '" + functionDefinition.GetName() + "'");
                }
            }

            GCLSignalSource* source = new GCLSignalSource(signalName, functionDefinition.GetName(), dataType);

            functionDefinition.AddGCLSignalSource(source);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseConfigurationParameters(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "ConfigurationParameters" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "ConfigurationParameters" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "ConfigurationParameter" && xml.isStartElement())
        {
            ConfigurationParameter* configurationParameter = new ConfigurationParameter();

            configurationParameter->SetDescriptiveName(xml.attributes().value("DescriptiveName").toString());
            configurationParameter->SetParameterName(xml.attributes().value("ParameterName").toString());
            configurationParameter->SetDescription(xml.attributes().value("Description").toString());
            configurationParameter->SetDataType(xml.attributes().value("DataType").toString());
            if (xml.attributes().hasAttribute("ArrayLength"))
            {
                quint32 length = xml.attributes().value("ArrayLength").toUInt();

                if (length > 255 || length == 0)
                {
                    m_error += QString("Invalid array size %1 in configuration parameter %2").arg(configurationParameter->GetArrayLength()).arg(configurationParameter->GetParameterName());
                }
                else
                {
                    configurationParameter->SetArrayLength(length);
                }
            }
            else
            {
                configurationParameter->SetArrayLength(1);
            }

            functionDefinition.AddConfigurationParameter(configurationParameter);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseCalibrationParameters(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "CalibrationParameters" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "CalibrationParameters" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "CalibrationParameter" && xml.isStartElement())
        {
            CalibrationParameter* calibrationParameter = new CalibrationParameter();

            calibrationParameter->SetDescriptiveName(xml.attributes().value("DescriptiveName").toString());
            calibrationParameter->SetParameterName(xml.attributes().value("ParameterName").toString());
            calibrationParameter->SetDescription(xml.attributes().value("Description").toString());
            calibrationParameter->SetDataType(xml.attributes().value("DataType").toString());
            if (xml.attributes().hasAttribute("ArrayLength"))
            {
                quint32 length = xml.attributes().value("ArrayLength").toUInt();

                if (length > 255 || length == 0)
                {
                    m_error += QString("Invalid array size %1 in calibration parameter %2").arg(calibrationParameter->GetArrayLength()).arg(calibrationParameter->GetParameterName());
                }
                else
                {
                    calibrationParameter->SetArrayLength(length);
                }
            }
            else
            {
                calibrationParameter->SetArrayLength(1);
            }

            functionDefinition.AddCalibrationParameter(calibrationParameter);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseLabelMappingsParameters(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "LabelMappingsParameters" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "LabelMappingsParameters" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "LabelMappings" && xml.isStartElement())
        {
            LabelMappings* labelMappingsParameter = new LabelMappings();

            labelMappingsParameter->SetDescriptiveName(xml.attributes().value("DescriptiveName").toString());
            labelMappingsParameter->SetLabelMappingsName(xml.attributes().value("LabelMappingsName").toString());
            labelMappingsParameter->SetDescription(xml.attributes().value("Description").toString());

            functionDefinition.AddLabelMappings(labelMappingsParameter);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseFaultDefinitions(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "FaultDefinitions" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "FaultDefinitions" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "FaultDefinition" && xml.isStartElement())
        {
            QString name = xml.attributes().value("Name").toString();
            QString description = xml.attributes().value("Description").toString();

            functionDefinition.AddFaultDefinition(name, description);
        }

        xml.readNext();
    }
}

void FunctionDefinitionDeserializer::parseUIResources(QXmlStreamReader& xml, FunctionDefinition& functionDefinition)
{
    if (xml.name() == "UIResources" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "UIResources" && xml.isEndElement()) && !xml.atEnd())
    {
        xml.readNext();
        if (!xml.isStartElement()) continue;

        if (xml.name() == "Icons")
        {
            while (!(xml.name() == "Icons" && xml.isEndElement()) && !xml.atEnd())
            {
                xml.readNext();

                if (!xml.isStartElement()) continue;
                if (xml.name() == "Icon")
                {
                    QString name = xml.attributes().value("Name").toString();
                    if (!name.isEmpty())
                    {
                        functionDefinition.AddIconResource(name);
                    }
                }
            }
        }

        if (xml.name() == "Images")
        {
            while (!(xml.name() == "Images" && xml.isEndElement()) && !xml.atEnd())
            {
                xml.readNext();

                if (!xml.isStartElement()) continue;
                if (xml.name() == "Image")
                {
                    QString name = xml.attributes().value("Name").toString();
                    if (!name.isEmpty())
                    {
                        functionDefinition.AddImageResource(name);
                    }
                }
            }
        }
    }
}


const QString FunctionDefinitionDeserializer::Error(void) const
{
    return m_error;
}
