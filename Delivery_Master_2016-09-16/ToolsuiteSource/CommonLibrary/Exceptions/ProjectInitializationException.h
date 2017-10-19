#ifndef PROJECTINITIALIZATIONEXCEPTION_H
#define PROJECTINITIALIZATIONEXCEPTION_H

#include "ExceptionBase.h"

class QString;

class ProjectInitializationException : public ExceptionBase
{
public:
    explicit ProjectInitializationException(QStringList innerExceptionStrings);
    explicit ProjectInitializationException(QString message);
    ProjectInitializationException(QString message, QStringList innerExceptionStrings);
};

#endif // PROJECTINITIALIZATIONEXCEPTION_H
