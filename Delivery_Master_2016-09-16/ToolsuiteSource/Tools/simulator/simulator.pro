#application information
include(../../project.pri)

DEFINES += APP_VERSION=\\\"$${SIMULATOR_VERSION_MAJOR}.$${SIMULATOR_VERSION_MINOR}.$${SIMULATOR_VERSION_STEPPING}\\\"
DEFINES += APP_NAME=\\\"$${SIMULATOR_EXE_TITLE}\\\"
DEFINES += APP_BUILD=\\\"$${system("git rev-parse HEAD")}\\\"

win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date /T)_$$system(time /T)\\\"
else:!win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date \"+%y-%m-%d_%H:%M\")\\\"


QT       += core gui

QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $${SIMULATOR_EXE_TITLE}
TEMPLATE = app

CONFIG += precompile_header c++11
PRECOMPILED_HEADER += precompiled.h

SOURCES += main.cpp\
        mainwindow.cpp \
    comthread.cpp \
    function.cpp \
    SpeedGauge.cpp \
    DisplayErrorMessagesHandler.cpp \
    UreaLevel.cpp \
    MemoryTableWidget.cpp \
    HexByteDelegate.cpp \
    TellTale.cpp \
    Configuration.cpp \
    FlowLayout.cpp \
    PassiveBuzzer.cpp \
    ActiveBuzzer.cpp \
    FuelGauge.cpp \
    DCU.cpp \
    DisplayWindow.cpp

HEADERS  += mainwindow.h \
    comthread.h \
    function.h \
    SpeedGauge.h \
    DisplayErrorMessagesHandler.h \
    UreaLevel.h \
    MemoryTableWidget.h \
    HexByteDelegate.h \
    TellTale.h \
    Configuration.h \
    FlowLayout.h \
    PassiveBuzzer.h \
    ActiveBuzzer.h \
    FuelGauge.h \
    precompiled.h \
    DCU.h \
    DisplayWindow.h

FORMS    += mainwindow.ui \
    PassiveBuzzer.ui \
    ActiveBuzzer.ui \
    DisplayWindow.ui

TELLTALES = $$shell_path($$PWD/Telltales)
TELLTALES_OUTPUT_FOLDER = $$shell_path($$OUT_PWD/Telltales)

win32:  TELLTALES_RELEASE_FOLDER = $$shell_path($$OUT_PWD/release/Telltales)
win32:  TELLTALES_DEBUG_FOLDER = $$shell_path($$OUT_PWD/debug/Telltales)

!win32: TELLTALES_RELEASE_FOLDER = $$shell_path($$OUT_PWD/Telltales)
!win32: TELLTALES_DEBUG_FOLDER = $$shell_path($$OUT_PWD/Telltales)

SIMULATOR_CONFIG = $$shell_path($$PWD/simulator_cfg.xml)
CONFIG_OUTPUT_FOLDER = $$shell_path($$OUT_PWD/simulator_cfg.xml)

win32: CONFIG_RELEASE_FOLDER = $$shell_path($$OUT_PWD/release/simulator_cfg.xml)
win32: CONFIG_DEBUG_FOLDER = $$shell_path($$OUT_PWD/debug/simulator_cfg.xml)

!win32:CONFIG_RELEASE_FOLDER = $$shell_path($$OUT_PWD/simulator_cfg.xml)
!win32:CONFIG_DEBUG_FOLDER = $$shell_path($$OUT_PWD/simulator_cfg.xml)

copytelltales.commands = $(COPY_DIR) $$TELLTALES $$TELLTALES_OUTPUT_FOLDER && \
                         $(COPY_DIR) $$TELLTALES $$TELLTALES_RELEASE_FOLDER && \
                         $(COPY_DIR) $$TELLTALES $$TELLTALES_DEBUG_FOLDER
copyconfig.commands = $(COPY) $$SIMULATOR_CONFIG $$CONFIG_OUTPUT_FOLDER && \
                      $(COPY) $$SIMULATOR_CONFIG $$CONFIG_RELEASE_FOLDER && \
                      $(COPY) $$SIMULATOR_CONFIG $$CONFIG_DEBUG_FOLDER

first.depends = $(first) copytelltales copyconfig
export(first.depends)
export(copytelltales.commands)
export(copyconfig.commands)

QMAKE_EXTRA_TARGETS += first copytelltales copyconfig

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Model/release/ -lModel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Model/debug/ -lModel
else:!win32: LIBS += -L$$OUT_PWD/../../Model/ -lModel

INCLUDEPATH += $$PWD/../../Model
DEPENDPATH += $$PWD/../../Model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../CommonLibrary/release/ -lCommonLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../CommonLibrary/debug/ -lCommonLibrary
else:!win32: LIBS += -L$$OUT_PWD/../../CommonLibrary/ -lCommonLibrary

INCLUDEPATH += $$PWD/../../CommonLibrary
DEPENDPATH += $$PWD/../../CommonLibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../CommonGUILibrary/release/ -lCommonGUILibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../CommonGUILibrary/debug/ -lCommonGUILibrary
else:!win32: LIBS += -L$$OUT_PWD/../../CommonGUILibrary/ -lCommonGUILibrary

INCLUDEPATH += $$PWD/../../CommonGUILibrary
DEPENDPATH += $$PWD/../../CommonGUILibrary

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Logger/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Logger/debug/ -lLogger
else:!win32: LIBS += -L$$OUT_PWD/../../Logger/ -lLogger

INCLUDEPATH += $$PWD/../../Logger
DEPENDPATH += $$PWD/../../Logger

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Logger/release/libLogger.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Logger/debug/libLogger.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Logger/release/Logger.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../Logger/debug/Logger.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../../Logger/libLogger.a

RESOURCES +=

DISTFILES += \
    simulator_cfg.xml \
    telltales/M01.bmp \
    telltales/A01.bmp \
    telltales/F04.bmp \
    telltales/A16L.bmp \
    telltales/A16R.bmp \
    telltales/F03.bmp \
    telltales/K10.bmp \
    telltales/B02.bmp \
    telltales/B01.bmp \
    faceplate.png
