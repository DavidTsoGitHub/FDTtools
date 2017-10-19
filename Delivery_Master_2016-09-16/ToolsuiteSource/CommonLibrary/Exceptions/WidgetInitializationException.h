#ifndef WIDGETINITIALIZATIONEXCEPTION_H
#define WIDGETINITIALIZATIONEXCEPTION_H

#include "ExceptionBase.h"

class QString;

class WidgetInitializationException : public ExceptionBase
{
public:
    WidgetInitializationException();
    explicit WidgetInitializationException(QStringList innerExceptionStrings);
    explicit WidgetInitializationException(QString message);
    WidgetInitializationException(QString message, QStringList innerExceptionStrings);
};

#endif // WIDGETINITIALIZATIONEXCEPTION_H
