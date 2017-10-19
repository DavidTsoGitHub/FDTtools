#application information
include(../project.pri)

DEFINES += APP_VERSION=\\\"$${FDTUI_VERSION_MAJOR}.$${FDTUI_VERSION_MINOR}.$${FDTUI_VERSION_STEPPING}\\\"
DEFINES += APP_NAME=\\\"$${FDTUI_EXE_TITLE}\\\"
DEFINES += APP_BUILD=\\\"$${system("git rev-parse HEAD")}\\\"
DEFINES += AI_VERSION=\\\"$${APPLICATION_INTERFACE_VERSION}\\\"

win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date /T)_$$system(time /T)\\\"
else:!win32: DEFINES += APP_BUILD_DATE=\\\"$$system(date \"+%y-%m-%d_%H:%M\")\\\"

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

CONFIG(release, debug|release): DEFINES += NDEBUG

RC_FILE = FDT.rc
RC_ICONS = detc.ico

TARGET = $${FDTUI_EXE_TITLE}
TEMPLATE = app

CONFIG += precompile_header c++11
PRECOMPILED_HEADER += precompiled.h

HEADERS += \
    precompiled.h \
    DisplayErrorMessagesHandler.h

SOURCES += main.cpp \
    DisplayErrorMessagesHandler.cpp

RESOURCES += \
    ToolIcons.qrc

include(CreateProjectWizard/CreateProjectWizard.pri)
include(Splash/Splash.pri)
include(MainWindow/MainWindow.pri)
include(SignalMappingCanvas/SignalMappingCanvas.pri)
include(Dialogs/Dialogs.pri)
include(LabelEditor/LabelEditor.pri)
include(LayoutEditor/LayoutEditor.pri)
include(SinksOverview/SinksOverview.pri)
include(SourcesOverview/SourcesOverview.pri)
include(PropertiesWidgets/PropertiesWidgets.pri)
include(ProjectBrowser/ProjectBrowser.pri)
include(MenuEditor/MenuEditor.pri)
include(CustomWidgets/CustomWidgets.pri)
include(CreateFunctionBlockWizard/CreateFunctionBlockWizard.pri)
include(IconManager/IconManager.pri)
include(ToolSettings/ToolSettings.pri)
include(DiagEditor/DiagEditor.pri)
include(SchedulerAnalysis/SchedulerAnalysis.pri)
include(CustomDelegates/CustomDelegates.pri)
include(ImageManager/ImageManager.pri)
include(AnimationEditor/AnimationEditor.pri)
include(CheckMessageEditor/CheckMessageEditor.pri)

#-------------------------------------------------
HELPFILE = $$shell_path($$PWD/Tutorial.chm)
ICONFILE = $$shell_path($$PWD/detc.ico)
OUTPUT_FOLDER = $$shell_path($$OUT_PWD)
OUTPUT_RELEASE_FOLDER = $$shell_path($$OUT_PWD/release)
OUTPUT_DEBUG_FOLDER = $$shell_path($$OUT_PWD/debug)

copyfiles.commands = $(COPY_DIR) $$HELPFILE $$OUTPUT_FOLDER && \
                     $(COPY_DIR) $$ICONFILE $$OUTPUT_FOLDER && \
                     $(COPY_DIR) $$HELPFILE $$OUTPUT_RELEASE_FOLDER && \
                     $(COPY_DIR) $$ICONFILE $$OUTPUT_RELEASE_FOLDER && \
                     $(COPY_DIR) $$HELPFILE $$OUTPUT_DEBUG_FOLDER && \
                     $(COPY_DIR) $$ICONFILE $$OUTPUT_DEBUG_FOLDER

first.depends = $(first) copyfiles
export(first.depends)
export(copyfiles.commands)

QMAKE_EXTRA_TARGETS += first copyfiles

DISTFILES += \
    Tutorial.chm \
    detc.ico

#-------------------------------------------------
#
# Libraries
#
#-------------------------------------------------
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FDTUILibrary/release/ -lFDTUILibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FDTUILibrary/debug/ -lFDTUILibrary
else:!win32: LIBS += -L$$OUT_PWD/../FDTUILibrary/ -lFDTUILibrary

INCLUDEPATH += $$PWD/../FDTUILibrary
DEPENDPATH += $$PWD/../FDTUILibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/release/libFDTUILibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/debug/libFDTUILibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/release/FDTUILibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/debug/FDTUILibrary.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../FDTUILibrary/libFDTUILibrary.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommonGUILibrary/release/ -lCommonGUILibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommonGUILibrary/debug/ -lCommonGUILibrary
else:!win32: LIBS += -L$$OUT_PWD/../CommonGUILibrary/ -lCommonGUILibrary

INCLUDEPATH += $$PWD/../CommonGUILibrary
DEPENDPATH += $$PWD/../CommonGUILibrary

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

FORMS +=
