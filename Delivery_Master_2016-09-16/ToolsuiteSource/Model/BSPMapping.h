#ifndef BSPMAPPING_H
#define BSPMAPPING_H

#include "BSPFunction.h"
#include "IndexToSignalMapping.h"

#include <QString>
#include <QList>

class BSPMapping
{
public:

    static const quint8 UnknownIndex;

    BSPMapping();

    QString Type;
    QString FunctionName;
    QList<IndexToSignalMapping*> IndexToSignalMappings;

    quint8 GetIndexFromSignalName(QString signalName) const ;
};

#endif // BSPMAPPING_H
