INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/ExceptionBase.h \
    $$PWD/CannotOpenFileException.h \
    $$PWD/DeserializationExeception.h \
    $$PWD/FunctionBlockInitializationException.h \
    $$PWD/CannotWriteFileException.h \
    $$PWD/FileNotExistException.h \
    $$PWD/SimulatorGeneralException.h \
    $$PWD/ObjectNotFoundException.h \
    $$PWD/ProjectException.h \
    $$PWD/BuildException.h \
    $$PWD/ProjectInitializationException.h \
    $$PWD/WidgetInitializationException.h

SOURCES += \
    $$PWD/ExceptionBase.cpp \
    $$PWD/CannotOpenFileException.cpp \
    $$PWD/DeserializationExeception.cpp \
    $$PWD/FunctionBlockInitializationException.cpp \
    $$PWD/CannotWriteFileException.cpp \
    $$PWD/FileNotExistException.cpp \
    $$PWD/SimulatorGeneralException.cpp \
    $$PWD/ObjectNotFoundException.cpp \
    $$PWD/ProjectException.cpp \
    $$PWD/BuildException.cpp \
    $$PWD/ProjectInitializationException.cpp \
    $$PWD/WidgetInitializationException.cpp
