#include "MainWindow.h"
#include "CreateProjectWizard.h"
#include "Splash.h"
#include "Exceptions/ExceptionBase.h"
#include "ExceptionHandling/TryCatchQApplication.h"
#include "ExceptionHandling/ExceptionHandler.h"
#include "Logger.h"
#include "DisplayErrorMessagesHandler.h"
#include "Helpers/ProjectController.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QStringList>
#include <QMessageBox>


static void showSplashScreen(void);
static void splashScreenAccepted(void);
static void splashScreenRejected(void);

static Splash* m_splashScreen;
static QString m_filePath;
static MainWindow* m_mainWindow;

void errorMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    switch (type)
    {
        case QtDebugMsg:
            fprintf(stdout, "Debug: %s %s\n", context.function, qUtf8Printable(msg));
            fflush(stdout);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s %s\n", context.function, qUtf8Printable(msg));
            fflush(stderr);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s %s\n", context.function, qUtf8Printable(msg));
            fflush(stderr);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s %s\n", context.function, qUtf8Printable(msg));
            fflush(stderr);
            abort();
    }
}

int main(int argc, char* argv[])
{
    qInstallMessageHandler(errorMessageHandler);

    TryCatchQApplication application(argc, argv);
    application.setApplicationVersion(APP_VERSION);
    application.setApplicationName(APP_NAME);

    m_mainWindow = new MainWindow();

    QStringList* errorMessages = new QStringList();
    int result = EXIT_FAILURE;
    bool fatal = false;

    DisplayErrorMessagesHandler* displayErrorMessagesHandler = new DisplayErrorMessagesHandler();
    QObject::connect(&application , &TryCatchQApplication::signal_OnNewErrorMessages, displayErrorMessagesHandler, &DisplayErrorMessagesHandler::slot_DisplayErrorMessage);

    try
    {
        QStringList args = application.arguments();

        if (args.length() == 1)
        {
            showSplashScreen();
        }
        else
        {
            if (args.count() > 2)
            {
                m_mainWindow->SetFunctionLibraryPath(args[2]);
            }

            const QFileInfo projectFile(args[1]);
            if (!projectFile.exists())
            {
                showSplashScreen();
            }
            else
            {
                m_mainWindow->RestoreWindowStateAndGeometry();
                m_mainWindow->OpenProject(projectFile);
            }
        }
    }
    catch (ExceptionBase& e)
    {
        ExceptionHandler::HandleUncaughtErrors(e, errorMessages);
        displayErrorMessagesHandler->DisplayErrorMessage(e.Information(), errorMessages);

        fatal = e.Fatal();
        result = -1;
    }
    catch (std::exception& e)
    {
        LOG_ERROR(e.what());
        fatal = true;
        result = -2;
    }

    if (!fatal)
    {
        result = application.exec();
    }


    if (result != 0)
    {
        QMessageBox::warning(NULL, "Fatal exception", "The program received a fatal error and will now exit.", QMessageBox::Ok);
    }

    delete displayErrorMessagesHandler;
    delete m_mainWindow;
    delete errorMessages;
    return result;
}

void showSplashScreen()
{
    m_splashScreen = new Splash();
    QObject::connect(m_splashScreen, &Splash::accepted, splashScreenAccepted);
    QObject::connect(m_splashScreen, &Splash::rejected, splashScreenRejected);
    m_splashScreen->show();
}

void splashScreenAccepted()
{
    QFileInfo projectFile(m_splashScreen->GetFilePath());
    m_splashScreen->close();
    delete m_splashScreen;

    m_mainWindow->RestoreWindowStateAndGeometry();
    m_mainWindow->OpenProject(projectFile);
}

void splashScreenRejected()
{
    m_splashScreen->close();
    delete m_splashScreen;

    m_mainWindow->RestoreWindowStateAndGeometry();
}
