#include "ProjectInitializationException.h"

#include <QString>

ProjectInitializationException::ProjectInitializationException(QStringList innerExceptionStrings) :
    ExceptionBase("ProjectInitializationException",
                  "Project Initialization Error.",
                  "Project was not able to initialize.",
                  innerExceptionStrings)
{}

ProjectInitializationException::ProjectInitializationException(QString message) :
    ExceptionBase("ProjectInitializationException",
                  "Project Initialization Error.",
                  message,
                  NULL)
{}

ProjectInitializationException::ProjectInitializationException(QString message, QStringList innerExceptionStrings) :
    ExceptionBase("ProjectInitializationException",
                  "Project Initialization Error.",
                  message,
                  innerExceptionStrings)
{}

