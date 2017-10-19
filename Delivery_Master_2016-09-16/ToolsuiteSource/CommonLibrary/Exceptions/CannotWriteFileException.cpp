#include "CannotWriteFileException.h"

#include <QFileInfo>
#include <QString>

CannotWriteFileException::CannotWriteFileException() :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create file!")
{
}

CannotWriteFileException::CannotWriteFileException(QStringList innerExceptionStrings) :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create file!",
                  innerExceptionStrings)
{
}

CannotWriteFileException::CannotWriteFileException(QFileInfo& fileInfo) :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")")
{
}

CannotWriteFileException::CannotWriteFileException(QFileInfo& fileInfo, QStringList innerExceptionStrings) :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")" ,
                  innerExceptionStrings)
{
}

CannotWriteFileException::CannotWriteFileException(QFileInfo& fileInfo, QString errorMessage) :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")\nError message: " + errorMessage)
{
}

CannotWriteFileException::CannotWriteFileException(QFileInfo& fileInfo, QString errorMessage, QStringList innerExceptionStrings) :
    ExceptionBase("CannotWriteFileException",
                  "Can not write to file",
                  "Couldn't create " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ")\nError message: " + errorMessage,
                  innerExceptionStrings)
{
}
