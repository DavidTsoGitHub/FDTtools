#include "DeserializationExeception.h"

#include <QFileInfo>

DeserializationExeception::DeserializationExeception(QStringList innerExceptionStrings, QString message) :
    ExceptionBase("DeserializationExeception",
                  "Deserialization Error",
                  "Error deserializing file: " + message,
                  innerExceptionStrings)
{
}

DeserializationExeception::DeserializationExeception(const QFileInfo& fileInfo, QString message) :
    ExceptionBase("DeserializationExeception",
                  "Deserialization Error",
                  "Error deserializing file: " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ") " + message)
{
}

DeserializationExeception::DeserializationExeception(const QFileInfo& fileInfo, QStringList innerExceptionStrings, QString message) :
    ExceptionBase("DeserializationExeception",
                  "Deserialization Error",
                  "Error deserializing file: " + fileInfo.fileName() + " (" + fileInfo.absolutePath() + ") " + message ,
                  innerExceptionStrings)
{
}
