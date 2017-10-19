#include "BSPFunction.h"


BSPFunction::BSPFunction()
{
    GCLSignalType = Undefined;
}

QString BSPFunction::GCLSignalTypeName() const
{
    switch (GCLSignalType)
    {
        case Sink:
            return "Sink";
        case Source:
            return "Source";
        case Undefined:
        default:
            return "Undefined";
    }
}
