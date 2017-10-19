#ifndef PROJECTDESERIALIZER_H
#define PROJECTDESERIALIZER_H

class Project;

class QString;
class QXmlStreamReader;

class ProjectDeserializer
{

public:
    /*!
     * \brief Deserializes Project XML
     * \param projectXml The Project XML to deserialize
     * \param result true if the deserialization was successful, false otherwise
     * \param errorText Contains the error text if the deserialization fails
     * \return A pointer to a Project object
     */
    static Project* Deserialize(QString projectXml, bool* result, QString* errorText);

private:
    ProjectDeserializer() {}

    static void parseProjectElement(QXmlStreamReader& xml, Project* project);
    static void parseFunctionBlocks(QXmlStreamReader& xml, Project* project);
    static void parseJ1939(QXmlStreamReader&xml, Project* project);
};

#endif // PROJECTDESERIALIZER_H
