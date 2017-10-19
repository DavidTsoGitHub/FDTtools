#ifndef LAYOUTTRANSITIONTYPES_H
#define LAYOUTTRANSITIONTYPES_H

#include "GCLSignalSink.h"


//Defining namespace for these types
class LayoutTransitionTypes
{
public:
    typedef enum
    {
        EqualTo = 0,
        GreaterThan = 1,
        EqualOrGreaterThan = 2,
        LessThan = 3,
        EqualOrLessThan = 4
    } TransitionCompare;

    typedef enum
    {
        NONE = 0,
        AND = 1,
        XOR = 2
    } TransitionConnective;

/*    typedef enum
    {
        FIRST = 0,
        SECOND = 1
    } TransitionConditionOrder;*/

};

#endif // LAYOUTTRANSITIONTYPES_H

