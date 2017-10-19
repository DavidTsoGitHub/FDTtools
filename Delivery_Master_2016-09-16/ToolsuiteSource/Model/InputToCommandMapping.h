#ifndef INPUTTOACTIONMAPPING
#define INPUTTOACTIONMAPPING

#include <QString>

class GCLSignalSink;

typedef enum
{
    CommandNone,
    CommandNext,
    CommandPrevious,
    CommandEnter,
    CommandLeave
} MenuCommandID;

typedef enum
{
    InputUp,
    InputDown,
    InputLeft,
    InputRight,
    InputSet,
    InputClear
} MenuInput;

typedef struct
{
    const GCLSignalSink* GCLSignal;
    MenuInput Input;
    MenuCommandID CommandID;
} InputToCommandMapping;

#endif // INPUTTOACTIONMAPPING

