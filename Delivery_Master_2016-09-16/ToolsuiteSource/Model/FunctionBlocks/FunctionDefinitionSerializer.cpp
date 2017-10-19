#include "FunctionDefinitionSerializer.h"

#include <QXmlStreamWriter>
#include "Logger.h"

QString FunctionDefinitionSerializer::Serialize(const FunctionDefinition& functionDefinition)
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("FunctionDefinition");
    xmlWriter.writeAttribute("Name", functionDefinition.GetName());

    xmlWriter.writeStartElement("Tasks");
    foreach (const Task* task, functionDefinition.Tasks())
    {
        xmlWriter.writeStartElement("Task");
        xmlWriter.writeTextElement("InitFunction", task->InitFunction);
        xmlWriter.writeTextElement("Runnable", task->Runnable);
        xmlWriter.writeTextElement("StartupDelay", QString::number(task->StartupDelay));
        xmlWriter.writeTextElement("Periodicity", QString::number(task->Periodicity));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // Tasks

    xmlWriter.writeStartElement("SignalInterface");

    xmlWriter.writeStartElement("InputSignals");
    foreach (const GCLSignalSink* inputSignal, functionDefinition.Sinks())
    {
        xmlWriter.writeStartElement("Signal");
        xmlWriter.writeAttribute("Name", inputSignal->SignalName());
        xmlWriter.writeAttribute("DataType", inputSignal->DataType());
        xmlWriter.writeAttribute("UpdateNotification", inputSignal->HasUpdateNotification() ? "True" : "False");
        xmlWriter.writeAttribute("Mandatory", inputSignal->IsMandatory() ? "True" : "False");
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // InputSignals

    xmlWriter.writeStartElement("OutputSignals");
    foreach (const GCLSignalSource* outputSignal, functionDefinition.Sources())
    {
        xmlWriter.writeStartElement("Signal");
        xmlWriter.writeAttribute("Name", outputSignal->SignalName());
        xmlWriter.writeAttribute("DataType", outputSignal->DataType());
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // OutputSignals

    xmlWriter.writeEndElement(); // SignalInterface

    xmlWriter.writeStartElement("ConfigurationParameters");
    foreach (const ConfigurationParameter* configurationParameter, functionDefinition.ConfigurationParameters())
    {
        xmlWriter.writeStartElement("ConfigurationParameter");
        xmlWriter.writeAttribute("DescriptiveName", configurationParameter->GetDescriptiveName());
        xmlWriter.writeAttribute("ParameterName", configurationParameter->GetParameterName());
        xmlWriter.writeAttribute("Description", configurationParameter->GetDescription());
        xmlWriter.writeAttribute("DataType", configurationParameter->GetDataType());
        xmlWriter.writeAttribute("ArrayLength", QString::number(configurationParameter->GetArrayLength()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // ConfigurationParameters

    xmlWriter.writeStartElement("CalibrationParameters");
    foreach (const CalibrationParameter* calibrationParameter, functionDefinition.CalibrationParameters())
    {
        xmlWriter.writeStartElement("CalibrationParameter");
        xmlWriter.writeAttribute("DescriptiveName", calibrationParameter->GetDescriptiveName());
        xmlWriter.writeAttribute("ParameterName", calibrationParameter->GetParameterName());
        xmlWriter.writeAttribute("Description", calibrationParameter->GetDescription());
        xmlWriter.writeAttribute("DataType", calibrationParameter->GetDataType());
        xmlWriter.writeAttribute("ArrayLength", QString::number(calibrationParameter->GetArrayLength()));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // CalibrationParameters

    xmlWriter.writeStartElement("LabelMappingsParameters");
    foreach (LabelMappings* labelMappingsParameter, functionDefinition.LabelMappingsParameters())
    {
        xmlWriter.writeStartElement("LabelMappings");
        xmlWriter.writeAttribute("DescriptiveName", labelMappingsParameter->GetDescriptiveName());
        xmlWriter.writeAttribute("LabelMappingsName", labelMappingsParameter->GetLabelMappingsName());
        xmlWriter.writeAttribute("Description", labelMappingsParameter->GetDescription());
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // LabelMappingsParameters

    xmlWriter.writeStartElement("FaultDefinitions");
    foreach (FaultDefinition faultDefinition, functionDefinition.FaultDefinitions())
    {
        xmlWriter.writeStartElement("FaultDefinition");
        xmlWriter.writeAttribute("Name", faultDefinition.Name);
        xmlWriter.writeAttribute("Description", faultDefinition.Description);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // FaultDefinitions

    xmlWriter.writeStartElement("UIResources");

    xmlWriter.writeStartElement("Icons");
    foreach (IconResource iconResource, functionDefinition.IconResources())
    {
        xmlWriter.writeStartElement("Icon");
        xmlWriter.writeAttribute("Name", iconResource.Name);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // UIResources.Icons

    xmlWriter.writeStartElement("Images");
    foreach (ImageResource imageResource, functionDefinition.ImageResources())
    {
        xmlWriter.writeStartElement("Image");
        xmlWriter.writeAttribute("Name", imageResource.Name);
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement(); // UIResources.Images

    xmlWriter.writeEndElement(); // UIResources

    xmlWriter.writeEndElement(); // FunctionDefinition

    return xmlString;
}
