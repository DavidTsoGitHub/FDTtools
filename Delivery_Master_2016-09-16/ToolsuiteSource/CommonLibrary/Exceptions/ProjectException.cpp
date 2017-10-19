#include "ProjectException.h"

#include <QString>

ProjectException::ProjectException() :
    ExceptionBase("ProjectException",
                  "Project Error",
                  "Unexpected problems with the project occurred",
                  NULL)
{}

ProjectException::ProjectException(QStringList innerExceptionStrings) :
    ExceptionBase("ProjectException",
                  "Project Error",
                  "Unexpected problems with the project occurred",
                  innerExceptionStrings)
{}

ProjectException::ProjectException(QString message) :
    ExceptionBase("ProjectException",
                  "Project Error",
                  message,
                  NULL)
{}

ProjectException::ProjectException(QString message, QStringList innerExceptionStrings) :
    ExceptionBase("ProjectException",
                  "Project Error",
                  message,
                  innerExceptionStrings)
{}

