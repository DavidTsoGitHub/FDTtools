#include "ObjectNotFoundException.h"

#include <QString>

ObjectNotFoundException::ObjectNotFoundException() :
    ExceptionBase("ObjectNotFoundException",
                  "Object could not be found.",
                  "Object could not be found.",
                  NULL)
{
}

ObjectNotFoundException::ObjectNotFoundException(QStringList innerExceptionStrings) :
    ExceptionBase("ObjectNotFoundException",
                  "Object could not be found.",
                  "Object could not be found.",
                  innerExceptionStrings)
{
}

ObjectNotFoundException::ObjectNotFoundException(QString message) :
    ExceptionBase("ObjectNotFoundException",
                  "Object could not be found.",
                  message,
                  NULL)
{
}

ObjectNotFoundException::ObjectNotFoundException(QString message, QStringList innerExceptionStrings) :
    ExceptionBase("ObjectNotFoundException",
                  "Object could not be found.",
                  message,
                  innerExceptionStrings)
{
}
