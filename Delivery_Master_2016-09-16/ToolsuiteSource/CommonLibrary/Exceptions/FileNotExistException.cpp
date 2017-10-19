#include "FileNotExistException.h"

#include <QFileInfo>

FileNotExistException::FileNotExistException() :
    ExceptionBase("FileNotExistException",
                  "File not found",
                  "File not found",
                  NULL)
{
}

FileNotExistException::FileNotExistException(QStringList innerExceptionStrings) :
    ExceptionBase("FileNotExistException",
                  "File not found",
                  "File not found",
                  innerExceptionStrings)
{
}

FileNotExistException::FileNotExistException(QFileInfo fileInfo) :
    ExceptionBase("FileNotExistException",
                  "File not found",
                  fileInfo.fileName() + " was not found (" + fileInfo.absolutePath() + ")" ,
                  NULL)
{
}

FileNotExistException::FileNotExistException(QFileInfo fileInfo, QStringList innerExceptionStrings) :
    ExceptionBase("FileNotExistException",
                  "File not found",
                  fileInfo.fileName() + " was not found (" + fileInfo.absolutePath() + ")" ,
                  innerExceptionStrings)
{
}
