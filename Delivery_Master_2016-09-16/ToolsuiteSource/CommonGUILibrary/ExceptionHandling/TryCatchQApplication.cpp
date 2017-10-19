#include "TryCatchQApplication.h"

#include "ExceptionHandler.h"
#include "Logger.h"

#include <QObject>

TryCatchQApplication::TryCatchQApplication(int& argc, char* argv[]) : QApplication(argc, argv)
{
}

TryCatchQApplication::~TryCatchQApplication()
{}

bool TryCatchQApplication::notify(QObject* receiver, QEvent* event)
{
    QStringList* errorMessages = new QStringList();
    int result = EXIT_FAILURE;
    bool fatal = false;

    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (ExceptionBase& e)
    {
        ExceptionHandler::HandleUncaughtErrors(e, errorMessages);

        if (errorMessages->count() > 0)
        {
            emit signal_OnNewErrorMessages(e.Information(), errorMessages);
        }

        fatal = e.Fatal();

        if (!fatal)
        {
            emit signal_OnRecoverbleException();
        }

        result = -1;
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
        fatal = true;
        result = -2;
    }

    if (fatal)
    {
        delete errorMessages;
        exit(result);
    }

    return false;
}
