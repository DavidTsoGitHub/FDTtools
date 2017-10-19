#include "ProjectPostInitDeserializer.h"

#include "FunctionBlock.h"
#include "LabelMappings.h"
#include "Logger.h"
#include "Project.h"

#include <QString>
#include <QXmlStreamReader>

ProjectPostInitDeserializer::ProjectPostInitDeserializer(Project* project) : DeserializerBase(NULL),
    m_project(project)
{}

ProjectPostInitDeserializer::~ProjectPostInitDeserializer()
{}

DeserializerBase::DeserializerRespons ProjectPostInitDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    while (!(xmlReader.name() == "Project" && xmlReader.isEndElement()) && !xmlReader.atEnd())
    {
        if (!xmlReader.isStartElement())
        {
            xmlReader.readNext();
            continue;
        }

        if (xmlReader.name() == "FunctionBlocks")
        {
            parseFunctionBlocks(xmlReader, *m_project);
        }

        xmlReader.readNext();
    }

    return DeserializationOK;
}

void ProjectPostInitDeserializer::parseFunctionBlocks(QXmlStreamReader& xml, Project& project)
{
    if (xml.name() == "FunctionBlocks" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "FunctionBlocks" && xml.isEndElement()) && !xml.atEnd())
    {

        if (xml.name() == "FunctionBlock" && xml.isStartElement())
        {

            int functionBlockID = xml.attributes().value("ID").toInt();

            while (!(xml.name() == "FunctionBlock" && xml.isEndElement()))
            {
                xml.readNext();

                if (xml.name() == "LabelMappingsParameters")
                {

                    FunctionBlock* functionBlock = project.GetFunctionBlocks().GetFunctionBlock(functionBlockID);

                    if (functionBlock == NULL)
                    {
                        LOG_ERROR("Failed to read LabelMappings for FunctionBlock ID:"  + QString::number(functionBlockID));
                        continue;
                    }


                    while (!(xml.name() == "LabelMappingsParameters" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "LabelMappings")
                        {
                            QString labelMappingsName = xml.attributes().value("Name").toString();
                            LabelMappings* labelMappings = functionBlock->GetLabelMappingsParameter(labelMappingsName);

                            if (labelMappings == NULL)
                            {
                                LOG_ERROR("Failed to read LabelMappings for FunctionBlock ID:"  + QString::number(functionBlock->GetID()));
                                continue;
                            }

                            while (!(xml.name() == "LabelMappings" && xml.isEndElement()))
                            {
                                xml.readNext();

                                if (xml.isStartElement() && xml.name() == "LabelMapping")
                                {
                                    quint32 labelID = xml.attributes().value("LabelID").toInt();
                                    Label* label = project.GetLabelDefinition().GetLabel(labelID);
                                    labelMappings->AddMappedLabel(label);
                                }
                            }
                        }
                    }
                }
            }
        }

        xml.readNext();
    }
}
