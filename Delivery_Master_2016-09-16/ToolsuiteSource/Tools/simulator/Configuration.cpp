#include "Configuration.h"
#include "Logger.h"

#include <QFile>
#include <QXmlStreamReader>

Configuration::Configuration(QString path)
{
    readConfiguration(path);
}

const QList<TellTaleConfiguration>&Configuration::GetTellTales() const
{
    return m_tellTales;
}

void Configuration::readConfiguration(QString path)
{
    QFile configurationFile(path);
    if (!configurationFile.open(QFile::ReadOnly))
    {
        LOG_WARNING("Configuration file error: " + configurationFile.errorString());
        return;
    }

    QXmlStreamReader xml(configurationFile.readAll());

    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.name().compare(QString("SimulatorConfig")) != 0) continue;

        parseConfigurationElement(xml);
    }

    if (xml.hasError())
    {
        switch (xml.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                LOG_ERROR("SimulatorConfig XML not well formed! Line:" +  QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber()));
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                LOG_ERROR("Premature end of SimulatorConfig XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber()));
            break;
            case QXmlStreamReader::UnexpectedElementError:
                LOG_ERROR("Unexpected element in SimulatorConfig XML! Line:" + QString::number(xml.lineNumber()) + " Col:" + QString::number(xml.columnNumber()));
            break;
            case QXmlStreamReader::CustomError:
                LOG_ERROR(xml.errorString());
            break;
            case QXmlStreamReader::NoError:
            break;
        }
    }
}

void Configuration::parseConfigurationElement(QXmlStreamReader& xml)
{
    while (!(xml.name() == "SimulatorConfig" && xml.isEndElement()) && !xml.atEnd())
    {
        xml.readNext();

        if (!xml.isStartElement()) continue;

        if (xml.name() == "TellTales")
        {
            while (!(xml.name() == "TellTales" && xml.isEndElement()) && !xml.atEnd())
            {
                xml.readNext();
                if (xml.isStartElement() && xml.name() == "TellTale")
                {
                    TellTaleConfiguration configuration;
                    bool conversionOk;
                    configuration.Index = xml.attributes().value("Index").toInt(&conversionOk, 16);
                    configuration.ActiveColor = QColor(xml.attributes().value("ActiveColor").toString());
                    configuration.BitmapPath = xml.attributes().value("BitmapPath").toString();

                    if (!conversionOk)
                    {
                        LOG_WARNING("Failed to parse TellTale configuration, Index attribute value couldn't be parsed to a hexadecimal value: " + xml.attributes().value("Index").toString());
                        continue;
                    }
                    else if (!QFile(configuration.BitmapPath).exists())
                    {
                        LOG_WARNING("Failed to parse TellTale configuration, bitmap file not found: " + configuration.BitmapPath);
                        continue;
                    }
                    else if (!configuration.ActiveColor.isValid())
                    {
                        LOG_WARNING("Failed to parse TellTale configuration, active color name unknown: "  + configuration.ActiveColor.name());
                        continue;
                    }

                    m_tellTales.append(configuration);
                }
            }
        }
    }
}

