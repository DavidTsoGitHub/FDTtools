#include "SimulatorGeneralException.h"

SimulatorGeneralException::SimulatorGeneralException(bool fatal) :
    ExceptionBase("SimulatorGeneralException",
                  "Simulator Error",
                  "Simulator stopped working!",
                  fatal)
{
}

SimulatorGeneralException::SimulatorGeneralException(QStringList innerExceptionStrings, bool fatal) :
    ExceptionBase("SimulatorGeneralException",
                  "Simulator Error",
                  "Simulator stopped working!",
                  innerExceptionStrings,
                  fatal)
{
}

SimulatorGeneralException::SimulatorGeneralException(QString message, bool fatal) :
    ExceptionBase("SimulatorGeneralException",
                  "Simulator Error",
                  "Simulator stopped working! Reason: " + message,
                  fatal)
{
}

SimulatorGeneralException::SimulatorGeneralException(QString message, QStringList innerExceptionStrings, bool fatal) :
    ExceptionBase("SimulatorGeneralException",
                  "Simulator Error",
                  "Simulator stopped working! Reason: " + message,
                  innerExceptionStrings,
                  fatal)
{
}
