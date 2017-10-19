#include "ProjectSerializer.h"

#include "FunctionBlock.h"
#include "Logger.h"
#include "Project.h"

#include <QXmlStreamWriter>

ProjectSerializer::ProjectSerializer(Project& project) : SerializerBase(NULL),
    m_project(project)
{}

ProjectSerializer::~ProjectSerializer()
{}

QString ProjectSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    xmlWriter.writeStartElement("Project");
    xmlWriter.writeAttribute("Name", m_project.GetProjectName());

    xmlWriter.writeStartElement("BSP");
    xmlWriter.writeAttribute("SpecificationPath", m_project.GetBSPSpecificationPath());
    xmlWriter.writeEndElement(); // Project.BSP

    if (!m_project.GetCANSpecificationPath().isEmpty())
    {
        xmlWriter.writeStartElement("CAN");
        xmlWriter.writeAttribute("ClusterECUName", m_project.GetCANClusterECUName());
        xmlWriter.writeAttribute("SpecificationPath", m_project.GetCANSpecificationPath());

        CANSpecification::DriverParameters driverParameters = m_project.GetCANSpecification().GetDriverParameters();
        xmlWriter.writeAttribute("PRESDIV", QString::number(driverParameters.PRESDIV));
        xmlWriter.writeAttribute("RJW", QString::number(driverParameters.RJW));
        xmlWriter.writeAttribute("PSEG1", QString::number(driverParameters.PSEG1));
        xmlWriter.writeAttribute("PSEG2", QString::number(driverParameters.PSEG2));
        xmlWriter.writeAttribute("PROPSEG", QString::number(driverParameters.PROPSEG));

        xmlWriter.writeEndElement(); // Project.CAN
    }

    xmlWriter.writeStartElement("DIAG");
    xmlWriter.writeAttribute("USDTFuncReqID", QString::number(m_project.GetCANSpecification().USDTFuncReqID()));
    xmlWriter.writeAttribute("USDTReqID", QString::number(m_project.GetCANSpecification().USDTReqID()));
    xmlWriter.writeAttribute("USDTRespID", QString::number(m_project.GetCANSpecification().USDTRespID()));
    xmlWriter.writeEndElement(); // Project.DIAG

    xmlWriter.writeStartElement("J1939");
    xmlWriter.writeStartElement("SF_Mailbox");
    xmlWriter.writeAttribute("ID", QString::number(m_project.GetCANSpecification().J1939SFID()));
    xmlWriter.writeAttribute("Mask", QString::number(m_project.GetCANSpecification().J1939SFMask()));
    xmlWriter.writeEndElement(); // SF_Mailbox
    xmlWriter.writeStartElement("FF_Mailbox");
    xmlWriter.writeAttribute("ID", QString::number(m_project.GetCANSpecification().J1939FFID()));
    xmlWriter.writeAttribute("Mask", QString::number(m_project.GetCANSpecification().J1939FFMask()));
    xmlWriter.writeEndElement(); // FF_Mailbox
    xmlWriter.writeStartElement("CF_Mailbox");
    xmlWriter.writeAttribute("ID", QString::number(m_project.GetCANSpecification().J1939CFID()));
    xmlWriter.writeAttribute("Mask", QString::number(m_project.GetCANSpecification().J1939CFMask()));
    xmlWriter.writeEndElement(); // CF_Mailbox
    xmlWriter.writeEndElement(); // Project.J1939

    xmlWriter.writeStartElement("TargetSettings");
    QString endianessString = m_project.GetTargetSettings().GetTargetEndianess() == TargetSettings::BigEndian ? "BigEndian" : "LittleEndian";
    xmlWriter.writeAttribute("Endianess", endianessString);
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("FunctionBlocks");
    foreach (FunctionBlock* block, m_project.GetFunctionBlocks().List())
    {
        xmlWriter.writeStartElement("FunctionBlock");
        xmlWriter.writeAttribute("ID", QString::number(block->GetID()));
        xmlWriter.writeAttribute("PathInLib", block->GetPathInFunctionLibrary());
        if (block->GetFunctionDefinition().ConfigurationParameters().count() > 0)
        {
            xmlWriter.writeStartElement("ConfigurationParameters");
            foreach (const ConfigurationParameter* configurationParameter, block->GetFunctionDefinition().ConfigurationParameters())
            {
                xmlWriter.writeStartElement("ConfigurationParameter");
                xmlWriter.writeAttribute("Name", configurationParameter->GetParameterName());
                xmlWriter.writeAttribute("Value", block->GetConfigurationParameterValue(configurationParameter->GetParameterName()));
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.ConfigurationParameters.ConfigurationParameter
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.ConfigurationParameters
        }

        if (block->GetFunctionDefinition().CalibrationParameters().count() > 0)
        {
            xmlWriter.writeStartElement("CalibrationParameters");
            foreach (const CalibrationParameter* calibrationParameter, block->GetFunctionDefinition().CalibrationParameters())
            {
                xmlWriter.writeStartElement("CalibrationParameter");
                xmlWriter.writeAttribute("Name", calibrationParameter->GetParameterName());
                xmlWriter.writeAttribute("DefaultValue", block->GetCalibrationParameterDefaultValue(calibrationParameter->GetParameterName()));
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.CalibrationParameters.CalibrationParameter
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.CalibrationParameters
        }

        if (block->GetFunctionDefinition().LabelMappingsParameters().count() > 0)
        {
            xmlWriter.writeStartElement("LabelMappingsParameters");
            foreach (LabelMappings* labelMappingsParameter, block->GetFunctionDefinition().LabelMappingsParameters())
            {
                xmlWriter.writeStartElement("LabelMappings");
                xmlWriter.writeAttribute("Name", labelMappingsParameter->GetLabelMappingsName());
                foreach (Label* label, labelMappingsParameter->GetMappedLabels())
                {
                    xmlWriter.writeStartElement("LabelMapping");
                    xmlWriter.writeAttribute("Index", QString::number(labelMappingsParameter->GetMappedLabels().indexOf(label)));
                    xmlWriter.writeAttribute("LabelID", QString::number(label->GetID()));
                    xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.LabelMappingsParameters.LabelMappings.LabelMapping
                }
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.LabelMappingsParameters.LabelMappings
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.LabelMappingsParameters
        }

        if (block->GetFunctionDefinition().FaultDefinitions().count() > 0)
        {
            xmlWriter.writeStartElement("DTCParameters");
            foreach (const FaultDefinition dtc, block->GetFunctionDefinition().FaultDefinitions())
            {
                xmlWriter.writeStartElement("DTCParameter");
                xmlWriter.writeAttribute("Name", dtc.Name);
                xmlWriter.writeAttribute("Description", dtc.Description);
                xmlWriter.writeAttribute("DTC_ID", QString::number(block->GetDTCParameterID(dtc.Name)));
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.DTCParameters.DTCParameter
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.DTCParameters
        }

        if (block->GetFunctionDefinition().IconResources().count() > 0)
        {
            xmlWriter.writeStartElement("IconResources");
            foreach (const IconResource iconResource, block->GetFunctionDefinition().IconResources())
            {
                xmlWriter.writeStartElement("IconResource");
                xmlWriter.writeAttribute("Name", iconResource.Name);
                xmlWriter.writeAttribute("IconID", QString::number(block->GetIconResourceID(iconResource.Name)));
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.IconResources.IconResource
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.IconResources
        }

        if (block->GetFunctionDefinition().ImageResources().count() > 0)
        {
            xmlWriter.writeStartElement("ImageResources");
            foreach (const ImageResource imageResource, block->GetFunctionDefinition().ImageResources())
            {
                xmlWriter.writeStartElement("ImageResource");
                xmlWriter.writeAttribute("Name", imageResource.Name);
                xmlWriter.writeAttribute("ImageID", QString::number(block->GetImageResourceID(imageResource.Name)));
                xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.ImageResources.ImageResource
            }
            xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock.ImageResources
        }

        xmlWriter.writeEndElement(); // Project.FunctionBlocks.FunctionBlock
    }
    xmlWriter.writeEndElement(); // Project.FunctionBlocks

    xmlWriter.writeEndElement(); // Project

    xmlWriter.writeEndDocument();

    return xmlString;
}
