#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QColor>
#include <QList>
#include <QXmlStreamReader>

typedef struct
{
    int Index;
    QString BitmapPath;
    QColor ActiveColor;
} TellTaleConfiguration;

class Configuration
{
public:
    explicit Configuration(QString path = QString("simulator_cfg.xml"));

    const QList<TellTaleConfiguration>& GetTellTales() const;

private:
    QList<TellTaleConfiguration> m_tellTales;


    void readConfiguration(QString path);
    void parseConfigurationElement(QXmlStreamReader& xml);
};

#endif // CONFIGURATION_H
