#application information
include(../../project.pri)

DEFINES += APP_VERSION=\\\"$${BSPEDITOR_VERSION_MAJOR}.$${BSPEDITOR_VERSION_MINOR}.$${BSPEDITOR_VERSION_STEPPING}\\\"
DEFINES += APP_NAME=\\\"$${BSPEDITOR_EXE_TITLE}\\\"
DEFINES += APP_BUILD=\\\"$${system("git rev-parse HEAD")}\\\"

win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date /T)_$$system(time /T)\\\"
else:!win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date \"+%y-%m-%d_%H:%M\")\\\"

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml xmlpatterns

TARGET = $${BSPEDITOR_EXE_TITLE}
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treenavigatormodel.cpp \
    treenavigatoritem.cpp \
    propertiesmodel.cpp \
    settingsdialog.cpp \
    treenavigatorfilter.cpp \
    bsptreefilter.cpp \
    bsprules.cpp \
    bspschemarules.cpp \
    bspcustomdelegate.cpp

HEADERS  += mainwindow.h \
    treenavigatormodel.h \
    treenavigatoritem.h \
    propertiesmodel.h \
    settingsdialog.h \
    treenavigatorfilter.h \
    bsptreefilter.h \
    bsprules.h \
    bspschemarules.h \
    bspcustomdelegate.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    bundled_schema.qrc
