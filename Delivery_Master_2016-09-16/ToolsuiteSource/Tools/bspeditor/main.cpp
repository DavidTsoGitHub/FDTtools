#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("SwedSpot AB");
    app.setApplicationName("BSP Editor");
    app.setApplicationVersion(APP_VERSION);
    qputenv("QT_HASH_SEED", "0");
    MainWindow w;
    w.show();

    return app.exec();
}
