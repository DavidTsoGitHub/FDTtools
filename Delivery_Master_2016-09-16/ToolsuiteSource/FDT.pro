TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    CommonLibrary \
    Logger \
    CommonGUILibrary \
    Model \
    FDTUILibrary \
    Tools \
    UnitTests \
    FDTUI

CommonGUILibrary.depends = Logger CommonLibrary
Model.depends = Logger CommonLibrary
FDTPlatformGenerator.depends = Model CommonLibrary
FDTUILibrary.depends = Logger Model CommonLibrary
UnitTests.depends = Model CommonLibrary FDTUILibrary Tools
Tools.depends = Logger FDTPlatformGenerator Model CommonLibrary CommonGUILibrary
FDTUI.depends = Logger Model Tools CommonGUILibrary CommonLibrary FDTUILibrary
