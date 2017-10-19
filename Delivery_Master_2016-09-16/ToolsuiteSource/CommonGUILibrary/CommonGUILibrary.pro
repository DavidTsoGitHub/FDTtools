#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T11:45:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CommonGUILibrary
TEMPLATE = lib
CONFIG += staticlib

include(ExceptionHandling/ExceptionHandling.pri)

DEFINES += COMMONGUILIBRARY_LIBRARY

SOURCES +=

HEADERS +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Logger/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Logger/debug/ -lLogger
else:!win32: LIBS += -L$$OUT_PWD/../Logger/-lLogger

INCLUDEPATH += $$PWD/../Logger
DEPENDPATH += $$PWD/../Logger

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/release/libLogger.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/debug/libLogger.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/release/Logger.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/debug/Logger.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../Logger/libLogger.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommonLibrary/release/ -lCommonLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommonLibrary/debug/ -lCommonLibrary
else:!win32: LIBS += -L$$OUT_PWD/../CommonLibrary/ -lCommonLibrary

INCLUDEPATH += $$PWD/../CommonLibrary
DEPENDPATH += $$PWD/../CommonLibrary
