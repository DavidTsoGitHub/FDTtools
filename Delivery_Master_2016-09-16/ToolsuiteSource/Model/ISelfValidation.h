#ifndef ISELFVALIDATION
#define ISELFVALIDATION

#include "ProjectObject.h"

class Project;

class ISelfValidation
{
public:
    virtual bool objectSelfValidation(Project* project, QStringList* errorList) = 0;
};

#endif // ISELFVALIDATION

