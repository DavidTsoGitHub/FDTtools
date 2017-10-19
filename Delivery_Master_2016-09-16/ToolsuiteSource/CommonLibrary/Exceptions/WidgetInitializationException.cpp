#include "WidgetInitializationException.h"

#include <QString>

WidgetInitializationException::WidgetInitializationException() :
    ExceptionBase("WidgetInitializationException",
                  "Widget Initialization Error.",
                  "Widgets has not been initialized.",
                  NULL)
{}

WidgetInitializationException::WidgetInitializationException(QStringList innerExceptionStrings) :
    ExceptionBase("WidgetInitializationException",
                  "Widget Initialization Error.",
                  "Widgets has not been initialized.",
                  innerExceptionStrings)
{}

WidgetInitializationException::WidgetInitializationException(QString message) :
    ExceptionBase("WidgetInitializationException",
                  "Widget Initialization Error.",
                  message,
                  NULL)
{}

WidgetInitializationException::WidgetInitializationException(QString message, QStringList innerExceptionStrings) :
    ExceptionBase("WidgetInitializationException",
                  "Widget Initialization Error.",
                  message,
                  innerExceptionStrings)
{}

