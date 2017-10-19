#include "mainwindow.h"
#include "ExceptionHandling/ExceptionHandler.h"
#include "ExceptionHandling/TryCatchQApplication.h"
#include "Exceptions/ExceptionBase.h"
#include "Logger.h"
#include "DisplayErrorMessagesHandler.h"

#include <QCommandLineParser>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    TryCatchQApplication a(argc, argv);
    a.setOrganizationName("SwedSpot AB");
    a.setApplicationName("BSP Simulator");
    a.setApplicationVersion(APP_VERSION);

    QCommandLineParser clParser;
    clParser.setApplicationDescription("Simulator");
    clParser.addHelpOption();
    clParser.addPositionalArgument("bspspecification", QCoreApplication::translate("main", "The BSP Specification XML file for the project"));

    // parse the command line
    clParser.process(a);

    if (clParser.positionalArguments().length() != 1)
    {
        clParser.showHelp(0);
    }

    QString bspSpecification = clParser.positionalArguments().at(0);

    qint32 result = EXIT_FAILURE;
    bool fatal = false;

    MainWindow* w = NULL;

    DisplayErrorMessagesHandler displayErrorMessagesHandler;
    QObject::connect(&a, &TryCatchQApplication::signal_OnNewErrorMessages, &displayErrorMessagesHandler, &DisplayErrorMessagesHandler::slot_DisplayErrorMessage);

    QStringList errorMessages;

    try
    {
        w = new MainWindow(bspSpecification);
        w->Initialize();
    }
    catch (ExceptionBase& e)
    {
        ExceptionHandler::HandleUncaughtErrors(e, &errorMessages);
        displayErrorMessagesHandler.DisplayErrorMessage(e.Information(), &errorMessages);

        fatal = e.Fatal();
        result = -1;
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
        fatal = true;
        result = -2;
    }

    if (!fatal && w != NULL)
    {
        w->show();
        result = a.exec();
    }

    if (result != 0)
    {
        QMessageBox::warning(NULL, "Fatal exception", "The program received a fatal error and will now exit.", QMessageBox::Ok);
    }

    return result;
}

