#include "FunctionBlockInitializationException.h"

FunctionBlockInitializationException::FunctionBlockInitializationException() :
ExceptionBase("FunctionBlockInitializationException",
              "Function Block Initialization Error",
              "Initialization failed for Function Block.")
{

}

FunctionBlockInitializationException::FunctionBlockInitializationException(QStringList innerExceptionStrings) :
    ExceptionBase("FunctionBlockInitializationException",
                  "Function Block Initialization Error",
                  "Initialization failed for Function Block.",
                  innerExceptionStrings)
{

}

FunctionBlockInitializationException::FunctionBlockInitializationException(qint32 functionBlockId) :
    ExceptionBase("FunctionBlockInitializationException",
                  "Function Block Initialization Error",
                  "Initialization failed for Function Block with ID: " + QString::number(functionBlockId) + ".")
{

}

FunctionBlockInitializationException::FunctionBlockInitializationException(qint32 functionBlockId, QStringList innerExceptionStrings) :
    ExceptionBase("FunctionBlockInitializationException",
                  "Function Block Initialization Error",
                  "Initialization failed for Function Block with ID: " + QString::number(functionBlockId) + ".",
                  innerExceptionStrings)
{

}

