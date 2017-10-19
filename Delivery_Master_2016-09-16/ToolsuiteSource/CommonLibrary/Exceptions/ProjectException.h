#ifndef PROJECTEXCEPTION_H
#define PROJECTEXCEPTION_H

#include "ExceptionBase.h"

class QString;

class ProjectException : public ExceptionBase
{
public:
    ProjectException();
    explicit ProjectException(QStringList innerExceptionStrings);
    explicit ProjectException(QString message);
    ProjectException(QString message, QStringList innerExceptionStrings);
};

#endif // PROJECTEXCEPTION_H
