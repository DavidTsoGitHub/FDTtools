#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T16:25:02

QT       += core widgets
TARGET = Model
TEMPLATE = lib
CONFIG += staticlib

DEFINES += AI_VERSION=\\\"$${APPLICATION_INTERFACE_VERSION}\\\"

CONFIG += precompile_header c++11
PRECOMPILED_HEADER += precompiled.h

HEADERS += \
    precompiled.h \
    GraphicsItemBase.h \
    BSPEnum.h \
    BSPEnumItem.h \
    BSPFixedPointFloat.h \
    BSPFunction.h \
    BSPFunctionArgument.h \
    BSPMapping.h \
    BSPRunnable.h \
    BSPSpecification.h \
    BSPSpecificationDeserializer.h \
    CANMessage.h \
    CANSignal.h \
    DBCParser.h \
    MessageAttributes.h \
    CANSpecification.h \
    CANSpecificationDeserializer.h \
    CheckMessagesDeserializer.h \
    CheckMessagesDefinition.h \
    CheckMessageItem.h \
    CheckMessage.h \
    CheckMessagesSerializer.h \
    DisplayArea.h \
    FunctionBlock.h \
    FunctionDefinition.h \
    FunctionDefinitionDeserializer.h \
    FunctionDefinitionSerializer.h \
    FunctionBlockContainer.h \
    Icon.h \
    IconDefinition.h \
    IconDeserializer.h \
    IconSerializer.h \
    LabelDefinition.h \
    LabelDefinitionDeserializer.h \
    LabelDefinitionSerializer.h \
    Font.h \
    Label.h \
    Translation.h \
    Project.h \
    ProjectDeserializer.h \
    ProjectSerializer.h \
    ProjectValidator.h \
    SignalMappingDeserializer.h \
    SignalMapping.h \
    SignalMappingSerializer.h \
    IGCLSignalSource.h \
    IGCLSignalSink.h \
    Task.h \
    ConfigurationParameter.h \
    SerializerBase.h \
    ConfigFileWriter.h \
    DeserializerBase.h \
    ConfigFileReader.h \
    ToolSettings.h \
    UIWidgetsModel.h \
    UIWidgetBase.h \
    UIWidgetsRepository.h \
    Command.h \
    DisplayProperties.h \
    IndexToSignalMapping.h \
    InputToCommandMapping.h \
    MenuDefinition.h \
    MenuDefinitionDeserializer.h \
    MenuDefinitionSerializer.h \
    MenuFont.h \
    MenuItem.h \
    NavigationMapping.h \
    RootMenuItemContainer.h \
    SignalMappingMetaType.h \
    GCLSignalSinkMetaType.h \
    GCLSignalSourceMetaType.h \
    CompilerBase.h \
    GCCCompiler.h \
    CompilerFactory.h \
    TestCompiler.h \
    CWCompiler.h \
    ComboBoxMetaType.h \
    UIWidgetDefinitionBase.h \
    LabelWidget.h \
    LabelWidgetGraphicsItem.h \
    IconWidget.h \
    IconWidgetGraphicsItem.h \
    CalibrationParameter.h \
    UIWidgetContainerBase.h \
    BlockBarWidget.h \
    ICPlatform.h \
    BlockBarWidgetGraphicsItem.h \
    UIWidgetSerializer.h \
    UIWidgetDeserializer.h \
    SelectableListWidget.h \
    SelectableListWidgetGraphicsItem.h \
    LabelMappings.h \
    ProjectPostInitDeserializer.h \
    IconResource.h \
    DynamicIconWidget.h \
    SignalTriggerWidgetGraphicsItem.h \
    SignalTriggerWidget.h \
    ColumnLevelWidget.h \
    ColumnLevelWidgetGraphicsItem.h \
    DynamicValueLabelWidget.h \
    DynamicTextLabelWidget.h \
    FaultDefinition.h \
    DiagDefinition.h \
    DiagDefinitionDeserializer.h \
    DiagDefinitionSerializer.h \
    BSPSegmentDisplay.h \
    SegmentDisplay.h \
    SegmentDisplayDeserializer.h \
    SegmentDisplaySerializer.h \
    ListNavigationWidget.h \
    ListNavigationWidgetGraphicsItem.h \
    GCLSignalEndpoint.h \
    GCLSignalSink.h \
    GCLSignalSource.h \
    IRunnable.h \
    FontModel.h \
    IconModel.h \
    DTCLabelMappingModel.h \
    DIDToCALMappingModel.h \
    TargetSettings.h \
    LocalProjectSettingsDeserializer.h \
    LocalProjectSettingsSerializer.h \
    BSPDisplay.h \
    LayoutDefinition.h \
    LayoutDefinitionDeserializer.h \
    LayoutDefinitionSerializer.h \
    Layout.h \
    ImageWidget.h \
    ProjectObject.h \
    ProjectObjectVisitor.h \
    SignalSourceFinderVisitor.h \
    SinkFinderVisitor.h \
    ImageDefinition.h \
    Image.h \
    ImageModel.h \
    ImageDeserializer.h \
    ImageSerializer.h \
    ProjectSerializerVisitor.h \
    DirtyObjectsVisitor.h \
    ImageWidgetGraphicsItem.h \
    DynamicLabelWidget.h \
    ImageResource.h \
    DynamicImageWidget.h \
    Animation.h \
    AnimationFrameByFrame.h \
    AnimationOpacity.h \
    AnimationTranslation.h \
    AnimationDefinition.h \
    AnimationDeserializer.h \
    AnimationSerializer.h \
    LayoutTransitionTypes.h \
    TransitionClause.h \
    TransitionCondition.h \
    DynamicImageWidgetGraphicsItem.h \
    DynamicIconWidgetGraphicsItem.h \
    LabelObjectVisitor.h \
    ILabelHasFont.h \
    CheckMessageModel.h \
    BSPMemory.h \
    SelfValidationVisitor.h \
    ISelfValidation.h \
    IBSP_DCU_BPPModes.h \
    HardwareSpecific/MPC5606/MPC5606_DCU_BPPModes.h \
    HardwareSpecific/Mock/NoDCU.h \
    ColorPair.h \
    GenericObjectVisitor.h

SOURCES +=\
    GraphicsItemBase.cpp \
    BSP/BSPEnum.cpp \
    BSP/BSPFixedPointFloat.cpp \
    BSP/BSPFunction.cpp \
    BSP/BSPMapping.cpp \
    BSP/BSPSpecification.cpp \
    BSP/BSPSpecificationDeserializer.cpp \
    CAN/CANMessage.cpp \
    CAN/CANSignal.cpp \
    CAN/DBCParser.cpp \
    CAN/CANSpecificationDeserializer.cpp \
    CAN/CANSpecification.cpp \
    CheckMessages/CheckMessagesDeserializer.cpp \
    CheckMessages/CheckMessagesDefinition.cpp \
    CheckMessages/CheckMessagesSerializer.cpp \
    Display/DisplayArea.cpp \
    FunctionBlocks/FunctionDefinitionDeserializer.cpp \
    FunctionBlocks/FunctionBlock.cpp \
    FunctionBlocks/FunctionDefinitionSerializer.cpp \
    FunctionBlocks/FunctionDefinition.cpp \
    FunctionBlocks/FunctionBlockContainer.cpp \
    FunctionBlocks/ConfigurationParameter.cpp \
    Icons/Icon.cpp \
    Icons/IconDefinition.cpp \
    Icons/IconDeserializer.cpp \
    Icons/IconSerializer.cpp \
    Labels/LabelDefinitionDeserializer.cpp \
    Labels/LabelDefinition.cpp \
    Labels/LabelDefinitionSerializer.cpp \
    Labels/Label.cpp \
    Labels/Translation.cpp \
    Project/Project.cpp \
    Project/ProjectDeserializer.cpp \
    Project/ProjectSerializer.cpp \
    Project/ProjectValidator.cpp \
    SignalMappings/SignalMappingDeserializer.cpp \
    SignalMappings/SignalMappingSerializer.cpp \
    SignalMappings/SignalMapping.cpp \
    SerializerBase.cpp \
    ConfigFileWriter.cpp \
    DeserializerBase.cpp \
    ConfigFileReader.cpp \
    ToolSettings.cpp \
    Menu/MenuDefinition.cpp \
    Menu/MenuDefinitionDeserializer.cpp \
    Menu/MenuDefinitionSerializer.cpp \
    Menu/MenuItem.cpp \
    Project/RootMenuItemContainer.cpp \
    DTOs/SignalMappingMetaType.cpp \
    DTOs/GCLSignalSinkMetaType.cpp \
    DTOs/GCLSignalSourceMetaType.cpp \
    Compilers/CompilerBase.cpp \
    Compilers/GCCCompiler.cpp \
    Compilers/CompilerFactory.cpp \
    Compilers/TestCompiler.cpp \
    Compilers/CWCompiler.cpp \
    DTOs/ComboBoxMetaType.cpp \
    BSP/BSPRunnable.cpp \
    UIWidgets/UIWidgetsModel.cpp \
    UIWidgets/UIWidgetBase.cpp \
    UIWidgets/UIWidgetsRepository.cpp \
    UIWidgets/UIWidgetDefinitionBase.cpp \
    UIWidgets/LabelWidget.cpp \
    UIWidgets/LabelWidgetGraphicsItem.cpp \
    UIWidgets/IconWidget.cpp \
    UIWidgets/IconWidgetGraphicsItem.cpp \
    Calibration/CalibrationParameter.cpp \
    UIWidgetContainerBase.cpp \
    ICPlatform/ICPlatform.cpp \
    UIWidgets/BlockBarWidget.cpp \
    UIWidgets/BlockBarWidgetGraphicsItem.cpp \
    UIWidgets/UIWidgetSerializer.cpp \
    UIWidgets/UIWidgetDeserializer.cpp \
    UIWidgets/SelectableListWidget.cpp \
    UIWidgets/SelectableListWidgetGraphicsItem.cpp \
    FunctionBlocks/LabelMappings.cpp \
    Project/ProjectPostInitDeserializer.cpp \
    UIWidgets/DynamicIconWidget.cpp \
    UIWidgets/SignalTriggerWidgetGraphicsItem.cpp \
    UIWidgets/SignalTriggerWidget.cpp \
    UIWidgets/ColumnLevelWidget.cpp \
    UIWidgets/ColumnLevelWidgetGraphicsItem.cpp \
    UIWidgets/DynamicValueLabelWidget.cpp \
    UIWidgets/DynamicTextLabelWidget.cpp \
    Diag/DiagDefinition.cpp \
    Diag/DiagDefinitionDeserializer.cpp \
    Diag/DiagDefinitionSerializer.cpp \
    BSP/BSPSegmentDisplay.cpp \
    Display/SegmentDisplay.cpp \
    Display/SegmentDisplaySerializer.cpp \
    Display/SegmentDisplayDeserializer.cpp \
    UIWidgets/ListNavigationWidget.cpp \
    UIWidgets/ListNavigationWidgetGraphicsItem.cpp \
    Signals/GCLSignalEndpoint.cpp \
    Signals/GCLSignalSink.cpp \
    Signals/GCLSignalSource.cpp \
    Tasks/Task.cpp \
    BSP/BSPDisplay.cpp \
    Display/LayoutDefinition.cpp \
    Display/LayoutDefinitionDeserializer.cpp \
    Display/LayoutDefinitionSerializer.cpp \
    Labels/FontModel.cpp \
    Icons/IconModel.cpp \
    Diag/DTCLabelMappingModel.cpp \
    Diag/DIDToCALMappingModel.cpp \
    TargetSettings/TargetSettings.cpp \
    LocalProjectSettings/LocalProjectSettingsDeserializer.cpp \
    LocalProjectSettings/LocalProjectSettingsSerializer.cpp \
    Display/Layout.cpp \
    ProjectObject.cpp \
    SignalSourceFinderVisitor.cpp \
    ProjectObjectVisitor.cpp \
    Signals/SinkFinderVisitor.cpp \
    Project/ProjectSerializerVisitor.cpp \
    DirtyObjectsVisitor.cpp \
    Images/ImageDefinition.cpp \
    Images/Image.cpp \
    Images/ImageModel.cpp \
    Images/ImageDeserializer.cpp \
    Images/ImageSerializer.cpp \
    UIWidgets/ImageWidget.cpp \
    UIWidgets/ImageWidgetGraphicsItem.cpp \
    UIWidgets/DynamicLabelWidget.cpp \
    UIWidgets/DynamicImageWidget.cpp \
    Animation/Animation.cpp \
    Animation/AnimationFrameByFrame.cpp \
    Animation/AnimationOpacity.cpp \
    Animation/AnimationTranslation.cpp \
    Animation/AnimationDefinition.cpp \
    Animation/AnimationDeserializer.cpp \
    Animation/AnimationSerializer.cpp \
    LayoutTransition/TransitionClause.cpp \
    LayoutTransition/TransitionCondition.cpp \
    UIWidgets/DynamicImageWidgetGraphicsItem.cpp \
    UIWidgets/DynamicIconWidgetGraphicsItem.cpp \
    LabelObjectVisitor.cpp \
    CheckMessages/CheckMessageModel.cpp \
    CheckMessages/CheckMessage.cpp \
    BSP/BSPMemory.cpp \
    SelfValidationVisitor.cpp \
    HardwareSpecific/MPC5606/MPC5606_DCU_BPPModes.cpp \
    HardwareSpecific/Mock/NoDCU.cpp \
    ColorPair.cpp \
    GenericObjectVisitor.cpp

INCLUDEPATH += $$PWD/../Logger
DEPENDPATH += $$PWD/../Logger
    
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Logger/release/ -lLogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Logger/debug/ -lLogger
else:!win32: LIBS += -L$$OUT_PWD/../Logger/ -lLogger

win32:win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/release/libLogger.a
else:win32:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/debug/libLogger.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/release/Logger.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Logger/debug/Logger.lib
else:!win32: PRE_TARGETDEPS += $$OUT_PWD/../Logger/libLogger.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommonLibrary/release/ -lCommonLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommonLibrary/debug/ -lCommonLibrary
else:!win32: LIBS += -L$$OUT_PWD/../CommonLibrary/ -lCommonLibrary

INCLUDEPATH += $$PWD/../CommonLibrary
DEPENDPATH += $$PWD/../CommonLibrary
RESOURCES += \
    UIWidgets.qrc
