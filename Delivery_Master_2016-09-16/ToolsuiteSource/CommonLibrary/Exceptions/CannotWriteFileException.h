#ifndef CANNOTWRITEFILEEXCEPTION_H
#define CANNOTWRITEFILEEXCEPTION_H

#include "ExceptionBase.h"

class QFileInfo;
class QString;

class CannotWriteFileException : public ExceptionBase
{
public:
    CannotWriteFileException();
    explicit CannotWriteFileException(QStringList innerExceptionStrings);
    explicit CannotWriteFileException(QFileInfo& fileInfo);
    CannotWriteFileException(QFileInfo& fileInfo, QStringList innerExceptionStrings);
    CannotWriteFileException(QFileInfo& fileInfo, QString errorMessage);
    CannotWriteFileException(QFileInfo& fileInfo, QString errorMessage, QStringList innerExceptionStrings);
};

#endif // CANNOTWRITEFILEEXCEPTION_H
