#ifndef PROGRAMNOTBUILTEXCEPTION_H
#define PROGRAMNOTBUILTEXCEPTION_H

#include "ExceptionBase.h"

class QFileInfo;

class FileNotExistException : public ExceptionBase
{
public:
    FileNotExistException();
    explicit FileNotExistException(QStringList innerExceptionStrings);
    explicit FileNotExistException(QFileInfo fileInfo);
    FileNotExistException(QFileInfo fileInfo, QStringList innerExceptionStrings);
};

#endif // PROGRAMNOTBUILTEXCEPTION_H
