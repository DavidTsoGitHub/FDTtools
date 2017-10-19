#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

#include "SerializerBase.h"

class Project;

class ProjectSerializer : public SerializerBase
{
public:
    explicit ProjectSerializer(Project& project);
    ~ProjectSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    Project& m_project;
};

#endif // PROJECTSERIALIZER_H
