#ifndef LAYOUTDEFINITIONDESERIALIZER_H
#define LAYOUTDEFINITIONDESERIALIZER_H

#include "DeserializerBase.h"

class MenuDefinition;
class LayoutDefinition;
class Project;

class LayoutDefinitionDeserializer : public DeserializerBase
{
public:
    explicit LayoutDefinitionDeserializer(LayoutDefinition& layoutDefinition, Project &project);
    ~LayoutDefinitionDeserializer();

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    LayoutDefinition& m_layoutDefinition;
    Project& m_project;
    MenuDefinition* parseMenuDefinition(QString menuDefinitionPath);
};

#endif // LAYOUTDEFINITIONDESERIALIZER_H
