#ifndef DESERIALIZATIONEXECEPTION_H
#define DESERIALIZATIONEXECEPTION_H

#include "ExceptionBase.h"

class QFileInfo;

class DeserializationExeception : public ExceptionBase
{
public:
    explicit DeserializationExeception(QStringList innerExceptionStrings, QString message = QString());
    explicit DeserializationExeception(const QFileInfo& fileInfo, QString message = QString());
    DeserializationExeception(const QFileInfo& fileInfo, QStringList innerExceptionStrings, QString message = QString());
};

#endif // DESERIALIZATIONEXECEPTION_H
