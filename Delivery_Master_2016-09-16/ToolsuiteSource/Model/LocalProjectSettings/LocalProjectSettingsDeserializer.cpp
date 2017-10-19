#include "LocalProjectSettingsDeserializer.h"

#include "Logger.h"
#include "Project.h"

#include <QString>

LocalProjectSettingsDeserializer::LocalProjectSettingsDeserializer(Project& project) :
    DeserializerBase(NULL),
    m_project(project)
{}

LocalProjectSettingsDeserializer::~LocalProjectSettingsDeserializer()
{}

DeserializerBase::DeserializerRespons LocalProjectSettingsDeserializer::Deserialize(const QString &xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "LocalSettings")
    {
        m_error = "No LocalSettings root element found in XML!";
        return ErrorTagNotFound;
    }

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement() && xmlReader.name() != "LocalSettings")
        {
            if (xmlReader.name() == "Simulator")
            {
                QString compiler = xmlReader.attributes().value("Compiler").toString();
                m_project.GetTargetSettings().SetSimulatorCompilerName(compiler);

            }
            else if (xmlReader.name() == "TargetHardware")
            {
                QString compiler = xmlReader.attributes().value("Compiler").toString();
                m_project.GetTargetSettings().SetTargetCompilerName(compiler);
            }
            else if (xmlReader.name() == "ActiveBuildTarget")
            {
                QString activeBuildTarget = xmlReader.attributes().value("Target").toString();
                if (activeBuildTarget == "Simulator")
                {
                    m_project.GetTargetSettings().SetActiveTarget(TargetSettings::Simulator);
                }
                else if (activeBuildTarget == "TargetHW")
                {
                    m_project.GetTargetSettings().SetActiveTarget(TargetSettings::TargetHW);
                }
                else
                {
                    m_project.GetTargetSettings().SetActiveTarget(TargetSettings::Simulator);
                }
            }
            else
            {
                LOG_WARNING(QString("Unknown element %1 in LocalSettings").arg(xmlReader.name().toString()));
            }
        }

        xmlReader.readNext();
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "LocalSettings XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
                return ErrorIncorrectStructure;
                break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of LocalSettings XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
                return ErrorIncorrectStructure;
                break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in LocalSettings XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
                return ErrorIncorrectStructure;
                break;
            case QXmlStreamReader::NoError:
            case QXmlStreamReader::CustomError:
                //Same behavior as default
            default:
                m_error = xmlReader.errorString();
                return ErrorUnknown;
                break;
        }
        return ErrorUnknown;
    }

    return DeserializationOK;
}
