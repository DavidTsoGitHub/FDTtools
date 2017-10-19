#ifndef FUNCTIONBLOCKINITIALIZATIONEXCEPTION_H
#define FUNCTIONBLOCKINITIALIZATIONEXCEPTION_H

#include "ExceptionBase.h"

class FunctionBlockInitializationException : public ExceptionBase
{
public:
    FunctionBlockInitializationException();
    explicit FunctionBlockInitializationException(QStringList innerExceptionStrings);
    explicit FunctionBlockInitializationException(qint32 functionBlockId);
    FunctionBlockInitializationException(qint32 functionBlockId, QStringList innerExceptionStrings);
};

#endif // FUNCTIONBLOCKINITIALIZATIONEXCEPTION_H
