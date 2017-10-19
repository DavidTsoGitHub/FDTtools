#ifndef FUNCTIONDEFINITIONDESERIALIZER_H
#define FUNCTIONDEFINITIONDESERIALIZER_H

#include "FunctionDefinition.h"
#include <QString>
#include <QXmlStreamReader>

class FunctionDefinitionDeserializer
{
public:
    explicit FunctionDefinitionDeserializer(QString functionDefinitionXml);

    bool Deserialize(FunctionDefinition& functionDefinition);
    const QString Error(void) const;

private:
    QString m_xmlString;
    QString m_error;

    void parseFunctionDefinition(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseTasks(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseTask(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseSignalInterface(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseInputSignals(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseOutputSignals(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseConfigurationParameters(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseCalibrationParameters(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    void parseLabelMappingsParameters(QXmlStreamReader &xml, FunctionDefinition &functionDefinition);
    void parseFaultDefinitions(QXmlStreamReader &xml, FunctionDefinition &functionDefinition);
    void parseUIResources(QXmlStreamReader& xml, FunctionDefinition& functionDefinition);
    static void parseDTCParameters(QXmlStreamReader &xml, FunctionDefinition &functionDefinition);
};

#endif // FUNCTIONDEFINITIONDESERIALIZER_H
