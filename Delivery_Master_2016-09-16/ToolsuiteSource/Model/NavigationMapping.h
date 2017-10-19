#ifndef NAVIGATIONMAPPING
#define NAVIGATIONMAPPING

#include "InputToCommandMapping.h"
#include <QString>

typedef struct
{
    MenuCommandID CommandID;
    QString TargetMenuItemID;
} NavigationCommand;

#endif // NAVIGATIONMAPPING

