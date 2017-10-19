#ifndef BUILDEXCEPTION_H
#define BUILDEXCEPTION_H

#include "ExceptionBase.h"

class QString;

class BuildException : public ExceptionBase
{
public:
    BuildException();
    explicit BuildException(QStringList innerExceptionStrings);
    explicit BuildException(QString message);
    BuildException(QString message, QStringList innerExceptionStrings);
};

#endif // BUILDEXCEPTION_H
