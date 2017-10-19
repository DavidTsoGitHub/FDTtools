#ifndef MENUDEFINITIONDESERIALIZER_H
#define MENUDEFINITIONDESERIALIZER_H

#include <QString>

class MenuDefinition;
class MenuItem;
class Project;
class QXmlStreamReader;

class MenuDefinitionDeserializer
{
public:
    explicit MenuDefinitionDeserializer(const QString& xml);
    ~MenuDefinitionDeserializer();

    bool Deserialize(MenuDefinition& menuDefinition, Project& project);
    const QString Error() const;

private:
    const QString m_xml;
    QString m_error;
    quint32 m_startupMenuID;

    MenuItem* parseMenuItem(QXmlStreamReader& xmlReader, Project& project, MenuDefinition& menuDefinition);
};

#endif // MENUDEFINITIONDESERIALIZER_H
