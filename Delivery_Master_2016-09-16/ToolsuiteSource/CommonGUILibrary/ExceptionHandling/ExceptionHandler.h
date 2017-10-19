#ifndef EXCEPTIONHANDLER_H
#define EXCEPTIONHANDLER_H

#include "Exceptions/ExceptionBase.h"

class QStringList;

class ExceptionHandler
{

public:
    static void HandleUncaughtErrors(ExceptionBase& e, QStringList* errorMessage);
};

#endif // EXCEPTIONHANDLER_H
