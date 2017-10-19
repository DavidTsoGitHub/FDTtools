#ifndef BSPSPECIFICATIONDESERIALIZER_H
#define BSPSPECIFICATIONDESERIALIZER_H

#include "DeserializerBase.h"

class BSPDisplay;
class BSPFunction;
class BSPFunctionArgument;
class BSPMapping;
class BSPMemory;
class BSPSegmentDisplay;
class BSPSpecification;

class QString;
class QXmlStreamReader;

class BSPSpecificationDeserializer : public DeserializerBase
{
public:
    explicit BSPSpecificationDeserializer(BSPSpecification& bspSpecification);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& data);

private:
    BSPSpecification& m_bspSpecification;

    BSPFunction* parseFunction(QXmlStreamReader& xml);
    BSPSegmentDisplay* parseSegmentDisplay(QXmlStreamReader& xml);
    BSPDisplay* parseDisplay(QXmlStreamReader &xml);
    BSPMemory *parseMemory(QXmlStreamReader &xml);
    BSPFunctionArgument parseFunctionArg(QXmlStreamReader& xml);
    BSPMapping* parseFunctionMapping(QXmlStreamReader& xml);

};

#endif // BSPSPECIFICATIONDESERIALIZER_H
