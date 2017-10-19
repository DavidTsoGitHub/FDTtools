#application information
include(../../project.pri)

DEFINES += APP_VERSION=\\\"$${PLATFORMGENERATOR_VERSION_MAJOR}.$${PLATFORMGENERATOR_VERSION_MINOR}.$${PLATFORMGENERATOR_VERSION_STEPPING}\\\"
DEFINES += APP_NAME=\\\"$${PLATFORMGENERATOR_EXE_TITLE}\\\"
DEFINES += APP_BUILD=\\\"$${system("git rev-parse HEAD")}\\\"

win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date /T)_$$system(time /T)\\\"
else:!win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date \"+%y-%m-%d_%H:%M\")\\\"

QT       += gui widgets

TARGET = $${PLATFORMGENERATOR_EXE_TITLE}
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

CONFIG += precompile_header
PRECOMPILED_HEADER += precompiled.h

TEMPLATE = app

SOURCES += \
    main.cpp \
    FDTPlatformGenerator.cpp \
    FileWriter.cpp \
    MakefileCreator.cpp \
    FontConverter.cpp \
    PlatformPaths.cpp \
    FileGeneratorBase.cpp \
    GeneratedFile.cpp \
    IconConverter.cpp \
    ImageConverter.cpp

HEADERS += \
    FDTPlatformGenerator.h \
    IFileWriter.h \
    FileWriter.h \
    MakefileCreator.h \
    FontConverter.h \
    PlatformPaths.h \
    FileGeneratorBase.h \
    GeneratedFile.h \
    IconConverter.h \
    precompiled.h \
    MenuFiles/Application/Services/Menu/MenuControllerInformation.h \
    ImageConverter.h

include(FileGenerators.pri)

RESOURCES += \
    StaticFiles.qrc

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../CommonLibrary/release/ -lCommonLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../CommonLibrary/debug/ -lCommonLibrary
else:!win32:LIBS += -L$$OUT_PWD/../../CommonLibrary/ -lCommonLibrary

INCLUDEPATH += $$PWD/../../CommonLibrary
DEPENDPATH += $$PWD/../../CommonLibrary

