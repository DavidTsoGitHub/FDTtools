#ifndef SIMULATORGENERALEXCEPTION_H
#define SIMULATORGENERALEXCEPTION_H

#include "ExceptionBase.h"

class SimulatorGeneralException : public ExceptionBase
{
public:
    explicit SimulatorGeneralException(bool fatal);
    SimulatorGeneralException(QStringList innerExceptionStrings, bool fatal);
    SimulatorGeneralException(QString message, bool fatal);
    SimulatorGeneralException(QString message, QStringList innerExceptionStrings, bool fatal);
};

#endif // SIMULATORGENERALEXCEPTION_H
