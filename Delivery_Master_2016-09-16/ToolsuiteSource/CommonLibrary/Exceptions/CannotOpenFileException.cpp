#include "CannotOpenFileException.h"

#include <QFileInfo>

CannotOpenFileException::CannotOpenFileException() :
    ExceptionBase("CannotOpenFileException",
                  "Can not open file",
                  "Error opening file")
{
}

CannotOpenFileException::CannotOpenFileException(QStringList innerExceptionStrings) :
    ExceptionBase("CannotOpenFileException",
                  "Can not open file",
                  "Error opening file",
                  innerExceptionStrings)
{
}

CannotOpenFileException::CannotOpenFileException(const QFileInfo& fileInfo) :
    ExceptionBase("CannotOpenFileException",
                  "Can not open file",
                  "Error opening file: " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")")
{
}

CannotOpenFileException::CannotOpenFileException(const QFileInfo& fileInfo, QStringList innerExceptionStrings) :
    ExceptionBase("CannotOpenFileException",
                  "Can not open file",
                  "Error opening file: " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")" ,
                  innerExceptionStrings)
{
}
