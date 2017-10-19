#ifndef CANNOTOPENFILEEXCEPTION_H
#define CANNOTOPENFILEEXCEPTION_H

#include "ExceptionBase.h"

class QFileInfo;

class CannotOpenFileException : public ExceptionBase
{
public:
    CannotOpenFileException(void);
    explicit CannotOpenFileException(QStringList innerExceptionStrings);
    explicit CannotOpenFileException(const QFileInfo& fileInfo);
    CannotOpenFileException(const QFileInfo& fileInfo, QStringList innerExceptionStrings);
};

#endif // CANNOTOPENFILEEXCEPTION_H
