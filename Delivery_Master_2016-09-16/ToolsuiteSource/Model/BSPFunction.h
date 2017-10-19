#ifndef BSPFUNCTION_H
#define BSPFUNCTION_H

#include "BSPFunctionArgument.h"

#include <QString>
#include <QMap>

class BSPFunction
{
public:
    typedef enum
    {
        Undefined,
        Source,
        Sink
    } GCLSignalTypeEnum;

    QString Name;
    QString Description;
    QString GCLSignalName;
    GCLSignalTypeEnum GCLSignalType;
    QString GCLSignalTypeName() const;
    QMap<qint32, BSPFunctionArgument> Arguments;
    QString ReturnType;

    BSPFunction();
};

#endif // BSPFUNCTION_H
