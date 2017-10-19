#ifndef OBJECTNOTFOUNDEXCEPTION_H
#define OBJECTNOTFOUNDEXCEPTION_H

#include "ExceptionBase.h"

class QString;

class ObjectNotFoundException : public ExceptionBase
{
public:
    ObjectNotFoundException();
    explicit ObjectNotFoundException(QStringList innerExceptionStrings);
    explicit ObjectNotFoundException(QString message);
    ObjectNotFoundException(QString message, QStringList innerExceptionStrings);
};

#endif // OBJECTNOTFOUNDEXCEPTION_H
