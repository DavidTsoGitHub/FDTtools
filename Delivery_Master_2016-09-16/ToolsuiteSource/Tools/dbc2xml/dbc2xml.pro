#application information
include(../../project.pri)

DEFINES += APP_VERSION=\\\"$${DBC2XML_VERSION_MAJOR}.$${DBC2XML_VERSION_MINOR}.$${DBC2XML_VERSION_STEPPING}\\\"
DEFINES += APP_NAME=\\\"$${DBC2XML_EXE_TITLE}\\\"
DEFINES += APP_BUILD=\\\"$${system("git rev-parse HEAD")}\\\"

win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date /T)_$$system(time /T)\\\"
else:!win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date \"+%y-%m-%d_%H:%M\")\\\"

QT       += core

QT       -= gui

TARGET = $${DBC2XML_EXE_TITLE}
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Model/release/ -lModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Model/debug/ -lModel
else:!win32: LIBS += -L$$OUT_PWD/../../Model/ -lModel

INCLUDEPATH += $$PWD/../../Model
DEPENDPATH += $$PWD/../../Model

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Model/release/libModel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Model/debug/libModel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Model/release/Model.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Model/debug/Model.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../../Model/libModel.a
