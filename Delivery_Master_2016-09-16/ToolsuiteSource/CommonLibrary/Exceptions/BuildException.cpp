#include "BuildException.h"

#include <QString>

BuildException::BuildException() :
    ExceptionBase("BuildException",
                  "Build Error",
                  "Unexpected problems while building the project occurred")
{}

BuildException::BuildException(QStringList innerExceptionStrings) :
    ExceptionBase("BuildException",
                  "Build Error",
                  "Unexpected problems while building the project occurred",
                  innerExceptionStrings)
{}

BuildException::BuildException(QString message) :
    ExceptionBase("BuildException",
                  "Build Error",
                  message)
{}

BuildException::BuildException(QString message, QStringList innerExceptionStrings) :
    ExceptionBase("BuildException",
                  "Build Error",
                  message,
                  innerExceptionStrings)
{}
