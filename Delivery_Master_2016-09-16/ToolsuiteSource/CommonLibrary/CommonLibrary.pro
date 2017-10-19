#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T13:00:14
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = CommonLibrary
TEMPLATE = lib
CONFIG += staticlib c++11

include(Exceptions/Exceptions.pri)
include(RegEx/RegEx.pri)
include(XMLUpgrader/XMLUpgrader.pri)

DEFINES += COMMONLIBRARY_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}
