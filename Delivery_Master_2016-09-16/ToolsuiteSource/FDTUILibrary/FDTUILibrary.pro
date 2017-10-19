#-------------------------------------------------
#
# Project created by QtCreator 2015-08-03T12:51:23
#
#-------------------------------------------------

QT       += core widgets

TARGET = FDTUILibrary
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

include(Helpers/Helpers.pri)

SOURCES +=

HEADERS +=
unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../FDTUI/Dialogs

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Model/release/ -lModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Model/debug/ -lModel
else:!win32: LIBS += -L$$OUT_PWD/../Model/ -lModel

INCLUDEPATH += $$PWD/../Model
DEPENDPATH += $$PWD/../Model

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Model/release/libModel.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Model/debug/libModel.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Model/release/Model.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Model/debug/Model.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../Model/libModel.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Logger/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Logger/debug/ -lLogger
else:!win32: LIBS += -L$$OUT_PWD/../Logger/ -lLogger

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
