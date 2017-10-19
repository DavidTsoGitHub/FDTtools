#include "ExceptionHandler.h"

#include "Logger.h"

#include <QStringList>

void ExceptionHandler::HandleUncaughtErrors(ExceptionBase& e, QStringList* errorMessage)
{
    errorMessage->append(e.Message());
    errorMessage->append(e.InnerExceptionStrings());

    LOG_ERROR(errorMessage->join('\n'));
}
