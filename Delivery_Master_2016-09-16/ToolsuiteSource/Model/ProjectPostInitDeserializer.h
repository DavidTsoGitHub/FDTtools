#ifndef PROJECTPOSTINITDESERIALIZER_H
#define PROJECTPOSTINITDESERIALIZER_H

#include "DeserializerBase.h"

class Project;

class QString;
class QXmlStreamReader;

class ProjectPostInitDeserializer : public DeserializerBase
{
public:
    explicit ProjectPostInitDeserializer(Project* project);
    ~ProjectPostInitDeserializer();

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    Project* m_project;

    void parseFunctionBlocks(QXmlStreamReader& xml, Project& project);
};

#endif // PROJECTPOSTINITDESERIALIZER_H
