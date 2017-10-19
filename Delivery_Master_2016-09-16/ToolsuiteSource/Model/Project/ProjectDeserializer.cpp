#include "ProjectDeserializer.h"

#include "FunctionBlock.h"
#include "Logger.h"
#include "Project.h"

#include <QString>
#include <QXmlStreamReader>

Project* ProjectDeserializer::Deserialize(QString projectXml, bool* result, QString* errorString)
{
    LOG_DEBUG("Entering");
    if (projectXml.isNull() || projectXml.isEmpty())
    {
        *errorString = "XML string is empty!";
        *result = false;
        return NULL;
    }

    bool xmlHasProjectElement = false;

    Project* project = new Project();

    QXmlStreamReader xml(projectXml);
    while (!xml.atEnd())
    {
        /* We're only interested in the Project element */
        xml.readNext();
        if (xml.name().compare(QString("Project")) != 0) continue;

        xmlHasProjectElement = true;

        parseProjectElement(xml,project);
    }

    if (!xmlHasProjectElement)
    {
        *errorString = "No Project element found in XML!";
        *result = false;
        return NULL;
    }

    if (xml.hasError())
    {
        switch (xml.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                *errorString = "Project XML not well formed! Line:" +  QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                *errorString = "Premature end of Project XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                *errorString = "Unexpected element in Project XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                *errorString = xml.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        *result = false;
        return NULL;
    }

    *errorString = "";
    *result = true;
    return project;
}

void ProjectDeserializer::parseProjectElement(QXmlStreamReader& xml, Project* project)
{
    project->SetProjectName(xml.attributes().value("Name").toString());

    while (!(xml.name() == "Project" && xml.isEndElement()) && !xml.atEnd())
    {
        if (!xml.isStartElement())
        {
            xml.readNext();
            continue;
        }

        if (xml.name() == "BSP")
        {
            project->SetBSPSpecificationPath(xml.attributes().value("SpecificationPath").toString());
        }
        else if (xml.name() == "CAN")
        {
            project->SetCANClusterECUName(xml.attributes().value("ClusterECUName").toString());
            project->SetCANSpecificationPath(xml.attributes().value("SpecificationPath").toString());

            CANSpecification::DriverParameters driverParameters;
            driverParameters.PRESDIV = xml.attributes().value("PRESDIV").toUShort();
            driverParameters.PROPSEG = xml.attributes().value("PROPSEG").toUShort();
            driverParameters.PSEG1 = xml.attributes().value("PSEG1").toUShort();
            driverParameters.PSEG2 = xml.attributes().value("PSEG2").toUShort();
            driverParameters.RJW = xml.attributes().value("RJW").toUShort();

            project->GetCANSpecification().SetDriverParameters(driverParameters);

        }
        else if (xml.name() == "DIAG")
        {
            project->GetCANSpecification().SetUSDTFuncReqID(xml.attributes().value("USDTFuncReqID").toUInt());
            project->GetCANSpecification().SetUSDTReqID(xml.attributes().value("USDTReqID").toUInt());
            project->GetCANSpecification().SetUSDTRespID(xml.attributes().value("USDTRespID").toUInt());

        }
        else if (xml.name() == "J1939")
        {
            parseJ1939(xml,project);
        }
        else if (xml.name() == "TargetSettings")
        {
            QString endianessString = xml.attributes().value("Endianess").toString();
            if (endianessString.compare("BigEndian") == 0)
            {
                project->GetTargetSettings().SetTargetEndianess(TargetSettings::BigEndian);
            }
            else
            {
                project->GetTargetSettings().SetTargetEndianess(TargetSettings::LittleEndian);
            }
        }
        else if (xml.name() == "FunctionBlocks")
        {
            parseFunctionBlocks(xml, project);
        }

        xml.readNext();
    }
}

void ProjectDeserializer::parseFunctionBlocks(QXmlStreamReader& xml, Project* project)
{
    if (xml.name() == "FunctionBlocks" && xml.isStartElement())
    {
        xml.readNext();
    }

    while (!(xml.name() == "FunctionBlocks" && xml.isEndElement()) && !xml.atEnd())
    {

        if (xml.name() == "FunctionBlock" && xml.isStartElement())
        {
            FunctionBlock* functionBlock = new FunctionBlock();

            functionBlock->SetID(xml.attributes().value("ID").toInt());
            functionBlock->SetPathInFunctionLibrary(xml.attributes().value("PathInLib").toString());

            while (!(xml.name() == "FunctionBlock" && xml.isEndElement()))
            {
                xml.readNext();

                if (xml.name() == "ConfigurationParameters")
                {
                    while (!(xml.name() == "ConfigurationParameters" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "ConfigurationParameter")
                        {
                            QString parameterName = xml.attributes().value("Name").toString();
                            QString parameterValue = xml.attributes().value("Value").toString();


                            if (parameterName.isEmpty() || parameterValue.isEmpty())
                            {
                                LOG_ERROR("Failed to read ConfigurationParameter for FunctionBlock ID:" + QString::number(functionBlock->GetID()) + " Parameter Name:" + parameterName + " Value: " + parameterValue);
                                continue;
                            }

                            functionBlock->SetConfigurationParameter(parameterName, parameterValue);
                        }
                    }
                }
                else if (xml.name() == "CalibrationParameters")
                {
                    while (!(xml.name() == "CalibrationParameters" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "CalibrationParameter")
                        {
                            QString parameterName = xml.attributes().value("Name").toString();
                            QString parameterDefaultValue = xml.attributes().value("DefaultValue").toString();

                            if (parameterName.isEmpty() || parameterDefaultValue.isEmpty())
                            {
                                LOG_ERROR("Failed to read CalibrationParameter for FunctionBlock ID:" + QString::number(functionBlock->GetID()) + " Parameter Name:" + parameterName + " Value: " + parameterDefaultValue);
                                continue;
                            }

                            functionBlock->SetCalibrationParameterDefaultValue(parameterName, parameterDefaultValue);
                        }
                    }
                }
                else if (xml.name() == "DTCParameters")
                {
                    while (!(xml.name() == "DTCParameters" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "DTCParameter")
                        {
                            QString resourceName = xml.attributes().value("Name").toString();
                            bool numberConversionOk;
                            QString dtcIDString = xml.attributes().value("DTC_ID").toString();
                            quint32 dtcID = dtcIDString.toInt(&numberConversionOk, 0);

                            if (!numberConversionOk || resourceName.isEmpty())
                            {
                                LOG_ERROR("Failed to read DTCParameter for FunctionBlock ID:" + QString::number(functionBlock->GetID()) + " Resource Name:" + resourceName + " DTC_ID: " + dtcIDString);
                                continue;
                            }

                            functionBlock->SetDTCParameterID(resourceName, dtcID);
                        }
                    }
                }
                else if (xml.name() == "IconResources")
                {
                    while (!(xml.name() == "IconResources" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "IconResource")
                        {
                            QString resourceName = xml.attributes().value("Name").toString();
                            bool numberConversionOk;
                            QString iconIDString = xml.attributes().value("IconID").toString();
                            quint32 iconID = iconIDString.toInt(&numberConversionOk, 0);

                            if (!numberConversionOk || resourceName.isEmpty())
                            {
                                LOG_ERROR("Failed to read IconResource for FunctionBlock ID:" + QString::number(functionBlock->GetID()) + " Resource Name:" + resourceName + " IconID: " + iconIDString);
                                continue;
                            }

                            functionBlock->SetIconResourceID(resourceName, iconID);
                        }
                    }
                }
                else if (xml.name() == "ImageResources")
                {
                    while (!(xml.name() == "ImageResources" && xml.isEndElement()))
                    {
                        xml.readNext();
                        if (xml.isStartElement() && xml.name() == "ImageResource")
                        {
                            QString resourceName = xml.attributes().value("Name").toString();
                            bool numberConversionOk;
                            QString imageIDString = xml.attributes().value("ImageID").toString();
                            quint32 imageID = imageIDString.toInt(&numberConversionOk, 0);

                            if (!numberConversionOk || resourceName.isEmpty())
                            {
                                LOG_ERROR("Failed to read ImageResource for FunctionBlock ID:" + QString::number(functionBlock->GetID()) + " Resource Name:" + resourceName + " ImageID: " + imageIDString);
                                continue;
                            }

                            functionBlock->SetImageResourceID(resourceName, imageID);
                        }
                    }
                }
            }

            project->AddFunctionBlock(functionBlock);
        }

        xml.readNext();
    }
}

void ProjectDeserializer::parseJ1939(QXmlStreamReader &xml, Project* project)
{
    if (xml.name() == "J1939" && xml.isStartElement() && !xml.atEnd() )
    {
        xml.readNext();
    }
    while (!(xml.name() == "J1939" && xml.isEndElement()) && !xml.atEnd())
    {
        if (xml.name() == "SF_Mailbox" && xml.isStartElement() )
        {
            project->GetCANSpecification().SetJ1939SFID(xml.attributes().value("ID").toUInt());
            project->GetCANSpecification().SetJ1939SFMask(xml.attributes().value("Mask").toUInt());

        }
        else if (xml.name() == "FF_Mailbox" && xml.isStartElement() )
        {
            project->GetCANSpecification().SetJ1939FFID(xml.attributes().value("ID").toUInt());
            project->GetCANSpecification().SetJ1939FFMask(xml.attributes().value("Mask").toUInt());

        }
        else if (xml.name() == "CF_Mailbox" && xml.isStartElement() )
        {
            project->GetCANSpecification().SetJ1939CFID(xml.attributes().value("ID").toUInt());
            project->GetCANSpecification().SetJ1939CFMask(xml.attributes().value("Mask").toUInt());

        }
        xml.readNext();
    }
}
