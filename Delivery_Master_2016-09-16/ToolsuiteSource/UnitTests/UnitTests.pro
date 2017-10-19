#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T10:05:03
#
#-------------------------------------------------

QT       += testlib core gui widgets

TARGET = UnitTests
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


CONFIG += precompile_header
PRECOMPILED_HEADER += precompiled.h

include(ProjectTests/ProjectTests.pri)
include(CANTests/CANTests.pri)
include(BSPTests/BSPTests.pri)
include(FunctionBlockTests/FunctionBlockTests.pri)
include(MenuDefinitionTests/MenuDefinitionTests.pri)
include(LayoutTests/LayoutTests.pri)
include(SignalMappingTests/SignalMappingTests.pri)
include(CompilerTests/CompilerTests.pri)

SOURCES += \
    testmain.cpp   

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    autotest.h \  
    precompiled.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FDTUILibrary/release/ -lFDTUILibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FDTUILibrary/debug/ -lFDTUILibrary
else:!win32: LIBS += -L$$OUT_PWD/../FDTUILibrary/ -lFDTUILibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/release/libFDTUILibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/debug/libFDTUILibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/release/FDTUILibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/debug/FDTUILibrary.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/libFDTUILibrary.a

INCLUDEPATH += $$PWD/../FDTUILibrary
DEPENDPATH += $$PWD/../FDTUILibrary

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


