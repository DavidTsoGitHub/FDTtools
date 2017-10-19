#include "Project.h"
#include "BSPSpecificationDeserializer.h"
#include "BSPDisplay.h"
#include "CANSpecificationDeserializer.h"
#include "SignalMapping.h"
#include "SignalMappingDeserializer.h"
#include "SignalMappingSerializer.h"
#include "LayoutDefinitionDeserializer.h"
#include "LayoutDefinitionSerializer.h"
#include "LabelDefinitionDeserializer.h"
#include "LabelDefinitionSerializer.h"
#include "HardwareSpecific/MPC5606/MPC5606_DCU_BPPModes.h"
#include "HardwareSpecific/Mock/NoDCU.h"
#include "CheckMessagesDeserializer.h"
#include "CheckMessagesSerializer.h"
#include "IconDeserializer.h"
#include "IconSerializer.h"
#include "ImageDeserializer.h"
#include "ImageSerializer.h"
#include "DiagDefinitionDeserializer.h"
#include "DiagDefinitionSerializer.h"
#include "Logger.h"
#include "ColorPair.h"
#include "ConfigFileReader.h"
#include "ConfigFileWriter.h"
#include "LocalProjectSettingsDeserializer.h"
#include "LocalProjectSettingsSerializer.h"
#include "Exceptions/ExceptionBase.h"
#include "Exceptions/CannotOpenFileException.h"
#include "Exceptions/DeserializationExeception.h"
#include "Exceptions/ProjectInitializationException.h"
#include "ProjectPostInitDeserializer.h"
#include "ToolSettings.h"
#include "TransitionCondition.h"
#include "Animation.h"
#include "AnimationDeserializer.h"
#include "AnimationSerializer.h"
#include "AnimationDefinition.h"
#include "AnimationFrameByFrame.h"

#include "ProjectObjectVisitor.h"

#include "LayoutDefinition.h"

#include <QTextStream>
#include <QMessageBox>
#include <QSettings>

const QString Project::MenuDefinitionPath("Menu.xml");
const QString Project::LayoutDefinitionPath("Layout.xml");
const QString Project::SegmentDefinitionPath("SegmentDisplay.xml");
const QString Project::LabelDefinitionPath("Labels.xml");
const QString Project::SignalMappingPath("SignalMapping.xml");
const QString Project::CheckMessagesDefinitionPath("CheckMessages.xml");
const QString Project::IconDefinitionPath("Icons.xml");
const QString Project::FontsDirectoryName("Fonts");
const QString Project::ImageDefinitionPath("Images.xml");
const QString Project::AnimationDefinitionPath("Animations.xml");
const QString Project::IconsDirectoryName("Icons");
const QString Project::DiagDefinitionPath("Diagnostics.xml");
const QString Project::ImagesDirectoryName("Images");

Project::Project() : ProjectObject(QString("Project")),
    m_isInitialized(false),
    m_bspSpecification(this),
    m_canSpecification(this),
    m_checkMessagesDefinition(this),
    m_diagDefinition(this),
    m_functionBlocks(this),
    m_imageDefinition(this),
    m_animationDefinition(this),
    m_icPlatform(this),
    m_iconDefinition(this),
    m_labelDefinition(this),
    m_layoutDefinition(this),
    m_segmentDisplay(this)
{
}

Project::~Project()
{
}

const QDir Project::GetOutputFolder() const
{
    return QDir(GetProjectRoot().absoluteFilePath("output"));
}

void Project::SetProjectName(QString projectName)
{
    setObjectName(projectName);
    m_projectName = projectName;
    NotifyPropertyUpdated();
}

void Project::SetBSPSpecificationPath(QString bspSpecificationPath)
{
    m_bspSpecificationPath = bspSpecificationPath;
    NotifyPropertyUpdated();
}

void Project::SetCANSpecificationPath(QString canSpecificationPath)
{
    m_canSpecificationPath = canSpecificationPath;
    NotifyPropertyUpdated();
}

void Project::UpdateCanSpecification(QString path)
{
    CANSpecification newCanSpecification;
    ConfigFileReader configFileReader;
    CANSpecificationDeserializer* deserializer = new CANSpecificationDeserializer(newCanSpecification);

    if (configFileReader.Deserialize(deserializer, path) != DeserializerBase::DeserializationOK)
    {
        QFileInfo fileInfo(path);
        throw DeserializationExeception(fileInfo, configFileReader.Error());
    }
    newCanSpecification.ParseGCLSignals(GetCANClusterECUName());

    QFileInfo newCanSpec(path);

    SetCANSpecificationPath(newCanSpec.fileName());
    m_canSpecification.UpdateSpecification(newCanSpecification);
}

void Project::SetCANClusterECUName(QString canClusterECUName)
{
    m_canClusterECUName = canClusterECUName;
    NotifyPropertyUpdated();
}

void Project::Initialize(QFileInfo projectFile, QDir functionLibraryRoot)
{
    qDebug("%s ProjectFile:%s FunctionLibraryRoot: %s", __FUNCTION__, qUtf8Printable(projectFile.absoluteFilePath()), qUtf8Printable(functionLibraryRoot.absolutePath()));
    m_isInitialized = false;

    try
    {
        bool initializationSuccessful = true;

        m_projectFileInfo = projectFile;
        m_functionLibraryRoot = functionLibraryRoot;

        initializationSuccessful &= initializeLocalProjectSettings();
        initializationSuccessful &= initializeBSPSpecification();
        initializationSuccessful &= initializeCANSpecification();
        initializationSuccessful &= initializeIconDefinition();
        initializationSuccessful &= initializeImageDefinition();
        initializationSuccessful &= initializeAnimationDefinition();
        initializationSuccessful &= initializeDiagDefinition();
        initializationSuccessful &= initializeLabelDefinition();
        initializationSuccessful &= initializeDisplay();
        initializationSuccessful &= initializeSegmentDisplay();
        initializationSuccessful &= initializeFunctionBlockContainer();
        initializationSuccessful &= initializeCheckMessagesDefinition();

        //Must be last component to initialize. Can not be initialized if not all other initializiations with signals succeded.
        initializationSuccessful &= initializeSignalMappingContainer();
        initializationSuccessful &= postInitializeProject();

        if (initializationSuccessful)
        {
            m_isInitialized = true;
        }
        else
        {
            m_isInitialized = false;
            m_projectFileInfo = QFileInfo();
            m_functionLibraryRoot = "";

            QMessageBox messageBox;
            messageBox.setText("Project initialization unsuccessful.");
            messageBox.setInformativeText(m_initializationError);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.exec();
        }
    }
    catch (...) //(ExceptionBase& e)
    {
        m_isInitialized = false;
        m_projectFileInfo = QFileInfo();
        m_functionLibraryRoot = "";

        QMessageBox messageBox;
        messageBox.setText("Project initialization unsuccessful.");
        messageBox.setInformativeText(m_initializationError);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();
        throw ProjectInitializationException("Project initialization unsuccessfully.");
    }

    ClearDirtyRecursive();
}

bool Project::IsInitialized() const
{
    return m_isInitialized;
}

void Project::AddFunctionBlock(FunctionBlock *functionBlock)
{
    m_functionBlocks.AddFunctionBlock(functionBlock);
}

void Project::RemoveFunctionBlock(FunctionBlock *functionBlock)
{
    foreach (QString calParameter, functionBlock->GetAllCalibrationParameters().keys())
    {
        m_diagDefinition.ClearMappingForCALParameter(calParameter);
    }

    m_functionBlocks.RemoveFunctionBlock(functionBlock);
}

const QFileInfo Project::GetProjectFile() const
{
    return m_projectFileInfo;
}

const QDir Project::GetProjectRoot() const
{
    return m_projectFileInfo.absoluteDir();
}

const QDir Project::GetFunctionLibraryRoot() const
{
    return m_functionLibraryRoot;
}

const QString Project::GetProjectName() const
{
    return m_projectName;
}

const QString Project::GetBSPSpecificationPath() const
{
    return m_bspSpecificationPath;
}

const QString Project::GetCANSpecificationPath() const
{
    return m_canSpecificationPath;
}

const QString Project::GetCANClusterECUName() const
{
    return m_canClusterECUName;
}

const QString Project::InitializationError() const
{
    return m_initializationError;
}

BSPSpecification& Project::GetBSPSpecification()
{
    return m_bspSpecification;
}

CANSpecification& Project::GetCANSpecification()
{
    return m_canSpecification;
}

LayoutDefinition& Project::GetLayoutDefinition()
{
    return m_layoutDefinition;
}

SegmentDisplay& Project::GetSegmentDisplay()
{
    return m_segmentDisplay;
}

FunctionBlockContainer& Project::GetFunctionBlocks()
{
    return m_functionBlocks;
}

bool Project::IsFunctionBlock(QString componentName)
{
    foreach (FunctionBlock* functionBlock, GetFunctionBlocks().List())
    {
        if (functionBlock->GetFunctionDefinition().GetName().compare(componentName) == 0)
        {
            return true;
        }
    }

    return false;
}

LabelDefinition& Project::GetLabelDefinition()
{
    return m_labelDefinition;
}

CheckMessageDefinition& Project::GetCheckMessagesDefinition()
{
    return m_checkMessagesDefinition;
}

IconDefinition& Project::GetIconDefinition()
{
    return m_iconDefinition;
}

ImageDefinition& Project::GetImageDefinition()
{
    return m_imageDefinition;
}

AnimationDefinition &Project::GetAnimationDefinition()
{
    return m_animationDefinition;
}

DiagDefinition& Project::GetDiagDefinition()
{
    return m_diagDefinition;
}

ICPlatform& Project::GetICPlatform()
{
    return m_icPlatform;
}

TargetSettings& Project::GetTargetSettings()
{
    return m_targetSettings;
}

void Project::SetTargetSettings(const TargetSettings& newSettings)
{
    m_targetSettings = TargetSettings(newSettings);
}

bool Project::initializeBSPSpecification()
{
    QString file = GetProjectRoot().absoluteFilePath(m_bspSpecificationPath);
    if(!checkIfFileExists(file))
    {
        m_initializationError.append("Missing file: " + file);
        throw CannotOpenFileException(QFileInfo(file));
    }

    BSPSpecificationDeserializer deserializer(m_bspSpecification);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);
    switch(response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
        {
            QMessageBox msgBox;
            msgBox.setText("BSP Specification is corrupt. Can't continue loading the project.");
            msgBox.setModal(true);
            msgBox.exec();
            return false;
        }
    }

    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        m_bspSpecification.SetBSPDCUBPPModes(new MPC5606_DCU_BPPModes(this));
    }
    else
    {
        m_bspSpecification.SetBSPDCUBPPModes(new NoDCU());
    }

    return true;
}

bool Project::initializeCANSpecification()
{
    if (m_canSpecificationPath.isEmpty())
    {
        LOG_INFO("No CAN XML file specified in project file, assuming no CAN!");
        return true;
    }

    CANSpecificationDeserializer* deserializer = new CANSpecificationDeserializer(m_canSpecification);

    DeserializerBase::DeserializerRespons response = initialize(deserializer, GetProjectRoot().absoluteFilePath(m_canSpecificationPath));
    delete deserializer;

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            m_canSpecification.ParseGCLSignals(m_canClusterECUName);
            break;
        case DeserializerBase::ErrorNoFile:
            LOG_INFO("No CAN XML file specified in project file, assuming no CAN!");
            break;
        default:
            return false;
    }

    return true;
}

bool Project::initializeLabelDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::LabelDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Label definition"))
    {
        createLabelDefintionFile(file);
    }

    LabelDefinitionDeserializer deserializer(m_labelDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
            if (createNewFileDialog("Label Definition"))
            {
                createLabelDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }
    return true;
}

bool Project::initializeCheckMessagesDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::CheckMessagesDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Check message definition"))
    {
        createCheckMessageDefinitionFile(file);
    }

    CheckMessagesDeserializer deserializer(m_layoutDefinition, m_checkMessagesDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
            if (createNewFileDialog("Check message definition"))
            {
                createCheckMessageDefinitionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }
    return true;
}

bool Project::postInitializeProject()
{
    ProjectPostInitDeserializer deserializer(this);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, m_projectFileInfo.absoluteFilePath());

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
            return false;
            break;
    }
    return true;
}

bool Project::initializeIconDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::IconDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Icon definition"))
    {
        createIconDefintionFile(file);
    }

    m_iconDefinition.SetIconFolderPath(GetProjectRoot().filePath(Project::IconsDirectoryName));
    IconDeserializer deserializer(m_iconDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        case DeserializerBase::ErrorPointsToMissingFile:
        {
            LOG_INFO("Ignored definition of deleted icon files. Lingering definition will be removed on next serialization.");
            break;
        }
        default:
            if (createNewFileDialog("Icon Definition"))
            {
                createIconDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }
    return true;
}

bool Project::initializeImageDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::ImageDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Image definition"))
    {
        createImageDefintionFile(file);
    }

    m_imageDefinition.SetImageFolderPath(GetProjectRoot().filePath(Project::ImagesDirectoryName));
    ImageDeserializer deserializer(m_imageDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        case DeserializerBase::ErrorPointsToMissingFile:
        {
            LOG_WARNING("Ignored definition of deleted image files. Lingering definition will be removed on next serialization.");
            break;
        }
        default:
            if (createNewFileDialog("Image Definition"))
            {
                createImageDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }

    return true;
}

bool Project::initializeAnimationDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::AnimationDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Animation definition"))
    {
        createAnimationDefintionFile(file);
    }

    AnimationDeserializer deserializer(m_animationDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
            if (createNewFileDialog("Animation Definition"))
            {
                createAnimationDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }
    return true;
}

bool Project::initializeDiagDefinition()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::DiagDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Diagnostics definition"))
    {
        createDiagDefintionFile(file);
    }

    DiagDefinitionDeserializer deserializer(m_diagDefinition);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        default:
            if (createNewFileDialog("Diagnostics Definition"))
            {
                createDiagDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }
    return true;
}

bool Project::initializeLocalProjectSettings()
{
    QString localSettingsFilePath = QString("%1.local").arg(m_projectFileInfo.absoluteFilePath());
    QFile file(localSettingsFilePath);
    if(!file.exists())
    {
        createLocalSettingsFile(localSettingsFilePath);
    }
    file.close();

    LocalProjectSettingsDeserializer deserializer(*this);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, localSettingsFilePath);

    if (response != DeserializerBase::DeserializationOK)
    {
        createLocalSettingsFile(localSettingsFilePath);
    }

    if(!file.exists())
    {
        return false;
    }

    return true;
}

bool Project::initializeDisplay()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::LayoutDefinitionPath);
    if(newFileNeededAndCreationApproved(file, "Layout definition"))
    {
        createLayoutDefintionFile(file);
    }

    LayoutDefinitionDeserializer deserializer(m_layoutDefinition, *this);
    DeserializerBase::DeserializerRespons response = initialize(&deserializer, file);

    switch (response)
    {
        case DeserializerBase::DeserializationOK:
            break;
        case DeserializerBase::ErrorGeneral:
            m_initializationError.append("The menu defintion file is missing or currupt. Try to repair it manually or create a new menu by adding it in the Menu Editor.");
            return false;
            break;
        case DeserializerBase::ErrorWrongAIV:

        default:
            if (createNewFileDialog("Layout Definition"))
            {
                createLayoutDefintionFile(file);
            }
            else
            {
                return false;
            }
            break;
    }

    if (m_bspSpecification.GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        //override chroma key (shouldn't occur anyways)

        m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::RED, 0x00);
        m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::GREEN, 0x00);
        m_layoutDefinition.SetStartChromaKeyColorComponent(ColorComponent::BLUE, 0x00);

        m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::RED, 0x00);
        m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::GREEN, 0x00);
        m_layoutDefinition.SetEndChromaKeyColorComponent(ColorComponent::BLUE, 0x00);
    }

    return true;
}

bool Project::initializeSegmentDisplay()
{
    if (m_bspSpecification.SegmentDisplay() != NULL)
    {
        m_segmentDisplay.SetBSPSegmentDefinition(*m_bspSpecification.SegmentDisplay());
        m_segmentDisplay.SetEnabled(true);
    }
    else
    {
        m_segmentDisplay.SetEnabled(false);
    }

    return true;
}

bool Project::initializeFunctionBlockContainer()
{
    return m_functionBlocks.Initialize(m_functionLibraryRoot);
}

bool Project::initializeSignalMappingContainer()
{
    QString file = GetProjectRoot().absoluteFilePath(Project::SignalMappingPath);
    if(newFileNeededAndCreationApproved(file, "Signal mapping"))
    {
        QString error;
        SignalMappingSerializer serializer(*this);
        ConfigFileWriter::WriteToFile(serializer, file, error);
    }

    QFile mappingFile(GetProjectRoot().absoluteFilePath(Project::SignalMappingPath));
    if (!mappingFile.open(QFile::ReadOnly))
    {
        QString errorMessage("Error opening SignalMapping XML file! (" + mappingFile.fileName() + ") " + mappingFile.errorString());
        LOG_ERROR(errorMessage);
        m_initializationError.append(errorMessage + "\n");
        throw CannotOpenFileException(QFileInfo(mappingFile));
    }

    QTextStream signalMappingXml(&mappingFile);
    SignalMappingDeserializer deserializer(signalMappingXml.readAll());
    mappingFile.close();

    QList<SignalMapping*> signalMappings;

    if (!deserializer.Deserialize(signalMappings))
    {
        QString errorMessage("Error deserializing SignalMapping XML file! (" + mappingFile.fileName() + ") " + deserializer.Error());
        LOG_ERROR(errorMessage);
        m_initializationError.append(errorMessage + "\n");
        throw DeserializationExeception(QFileInfo(mappingFile), deserializer.Error());
    }

    QMultiMap<QString, GCLSignalSink*> allSinksByComponent;
    QMultiMap<QString, GCLSignalSource*> allSourcesByComponent;

    GetAllGCLSinksByComponentName(allSinksByComponent);
    GetAllGCLSourcesByComponentName(allSourcesByComponent);

    foreach (SignalMapping* signalMapping, signalMappings)
    {
        SimpleGCLEndpointDescription sourceDescription = signalMapping->GetSource();
        GCLSignalSource* sourceToMap = NULL;
        foreach (GCLSignalSource* source, allSourcesByComponent.values(sourceDescription.Name))
        {
            if (source->SignalName() == sourceDescription.Signal)
            {
                sourceToMap = source;
                break;
            }
        }

        if (sourceToMap == NULL)
        {
            QString warningMessage("Source: " + sourceDescription.Name + "." + sourceDescription.Signal + " in SignalMapping.xml was not found in project!");
            LOG_WARNING(warningMessage);
            continue;
        }

        if (signalMapping->GetSinks().count() == 0)
        {
            QString warningMessage("Source: " + sourceDescription.Name + "." + sourceDescription.Signal + " in SignalMapping.xml does not appear to have any sinks!");
            LOG_WARNING(warningMessage);
            continue;
        }

        foreach (SimpleGCLEndpointDescription sinkDescription, signalMapping->GetSinks())
        {
            GCLSignalSink* sinkToMap = NULL;
            foreach (GCLSignalSink* sink, allSinksByComponent.values(sinkDescription.Name))
            {
                if (sink->SignalName() == sinkDescription.Signal)
                {
                    sinkToMap = sink;
                    break;
                }
            }

            if (sinkToMap == NULL)
            {
                QString warningMessage("Sink: " + sinkDescription.Name + "." + sinkDescription.Signal + " in SignalMapping.xml was not found in project!");
                LOG_WARNING(warningMessage);
                continue;
            }

            sinkToMap->ConnectTo(sourceToMap);
        }
    }

    return true;
}

DeserializerBase::DeserializerRespons Project::initialize(DeserializerBase* deserializer, QString path)
{
    ConfigFileReader configFileReader;
    DeserializerBase::DeserializerRespons response = configFileReader.Deserialize(deserializer, path);

    if(response != DeserializerBase::DeserializationOK)
    {
        m_initializationError.append(configFileReader.Error());
    }

    return response;
}

bool Project::checkIfFileExists(QString filename)
{
    QFileInfo file(filename);
    if(!file.exists())
    {
        return false;
    }
    return true;
}

bool Project::newFileNeededAndCreationApproved(QString file, QString description)
{
    if(!checkIfFileExists(file))
    {
        if (createNewFileDialog(description) != true)
        {
            m_initializationError.append("Missing file: " + file);
            throw CannotOpenFileException();
        }
        else
        {
            return true;
        }
    }

    return false;
}

bool Project::createNewFileDialog(QString description)
{
    QMessageBox messageBox;
    messageBox.setText(m_initializationError);
    messageBox.setInformativeText("The above file (" + description + ") is missing or corrupt, should a new, empty file be created?");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    int res =  messageBox.exec();

    if (res != QMessageBox::Yes)
    {
        return false;
    }

    return true;
}

void Project::createLabelDefintionFile(QString filePath)
{
    QString error;
    LabelDefinitionSerializer serializer(m_labelDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createCheckMessageDefinitionFile(QString filePath)
{
    QString error;
    CheckMessagesSerializer serializer(m_checkMessagesDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createIconDefintionFile(QString file)
{
    QString error;
    IconSerializer serializer(m_iconDefinition);
    ConfigFileWriter::WriteToFile(serializer, file, error);
}

void Project::createImageDefintionFile(QString filePath)
{
    QString error;
    ImageSerializer serializer(m_imageDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createAnimationDefintionFile(QString filePath)
{
    QString error;
    AnimationSerializer serializer(m_animationDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createDiagDefintionFile(QString filePath)
{
    QString error;
    DiagDefinitionSerializer serializer(m_diagDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createLayoutDefintionFile(QString filePath)
{
    QString error;
    LayoutDefinitionSerializer serializer(m_layoutDefinition);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::createLocalSettingsFile(QString filePath)
{
    QString error;
    LocalProjectSettingsSerializer serializer(*this);
    ConfigFileWriter::WriteToFile(serializer, filePath, error);
}

void Project::GetAllGCLSources(QList<GCLSignalSource*>& sources) const
{
    sources.append(m_icPlatform.GetSignalSources());
    sources.append(m_bspSpecification.GetSignalSources());
    sources.append(m_canSpecification.GetSignalSources());

    if (m_bspSpecification.HasDisplay())
    {
        foreach (Layout* layout, m_layoutDefinition.Layouts())
        {
            sources.append(layout->GetSignalSources());

            if (layout->HasMenu())
            {
                sources.append(layout->GetMenu()->GetSignalSources());
            }

            QList<UIWidgetBase*> widgets;
            layout->GetUIWidgets(widgets);
            foreach (UIWidgetBase* widget, widgets)
            {
                sources.append(widget->GetSignalSources());
            }
        }

        sources.append(m_checkMessagesDefinition.GetSignalSources());

        foreach(AnimationFrameByFrame* anim, m_animationDefinition.FrameByFrameAnimations())
        {
            sources.append((anim->GetSignalSources()));
        }

        foreach(AnimationOpacity* anim, m_animationDefinition.OpacityAnimations())
        {
            sources.append((anim->GetSignalSources()));
        }

        foreach(AnimationTranslation* anim, m_animationDefinition.TranslationAnimations())
        {
            sources.append((anim->GetSignalSources()));
        }
    }

    foreach (FunctionBlock* functionBlock, m_functionBlocks.List())
    {
        sources.append(functionBlock->GetSignalSources());
    }

}

void Project::GetAllGCLSinksByComponentName(QMultiMap<QString, GCLSignalSink*>& sinksByComponent)
{
    QList<GCLSignalSink*> sinks;
    GetAllGCLSinks(sinks);
    foreach (GCLSignalSink* sink, sinks)
    {
        sinksByComponent.insert(sink->ComponentName(), sink);
    }
}

void Project::GetAllGCLSourcesByComponentName(QMultiMap<QString, GCLSignalSource*>& sourcesByComponent)
{
    QList<GCLSignalSource*> sources;
    GetAllGCLSources(sources);
    foreach (GCLSignalSource* source, sources)
    {
        sourcesByComponent.insert(source->ComponentName(), source);
    }
}

void Project::GetAllGCLSinks(QList<GCLSignalSink*>& sinks) const
{
    sinks.append(m_bspSpecification.GetSignalSinks());
    sinks.append(m_canSpecification.GetSignalSinks());
    sinks.append(m_checkMessagesDefinition.GetSignalSinks());

    if (m_bspSpecification.HasDisplay())
    {
        sinks.append(m_layoutDefinition.GetSignalSinks());
        foreach (Layout* layout, m_layoutDefinition.Layouts())
        {
            foreach (TransitionClause* transition, layout->GetSignalTransitions())
            {
                sinks.append(transition->GetFirstCondition()->GetSignalSinks());

                if (transition->GetSecondCondition() != NULL)
                {
                    sinks.append(transition->GetSecondCondition()->GetSignalSinks());
                }
            }

            if (layout->HasMenu())
            {
                sinks.append(layout->GetMenu()->GetSignalSinks());
            }
            QList<UIWidgetBase*> widgets;
            layout->GetUIWidgets(widgets);
            foreach (UIWidgetBase* widget, widgets)
            {
                sinks.append(widget->GetSignalSinks());

                if (widget->GetAnimationTriggers(1).count() > 0)
                {
                    foreach (TransitionClause* trigger, widget->GetAnimationTriggers(1))
                    {
                        sinks.append(trigger->GetFirstCondition()->GetSignalSinks());

                        if (trigger->GetSecondCondition() != NULL)
                        {
                            sinks.append(trigger->GetSecondCondition()->GetSignalSinks());
                        }
                    }
                }

                if (widget->GetAnimationTriggers(2).count() > 0)
                {
                    foreach (TransitionClause* trigger, widget->GetAnimationTriggers(2))
                    {
                        sinks.append(trigger->GetFirstCondition()->GetSignalSinks());

                        if (trigger->GetSecondCondition() != NULL)
                        {
                            sinks.append(trigger->GetSecondCondition()->GetSignalSinks());
                        }
                    }
                }
            }
        }
    }

    if (m_segmentDisplay.GetEnabled())
    {
        sinks.append(m_segmentDisplay.GetSignalSinks());
    }

    foreach (FunctionBlock* functionBlock, m_functionBlocks.List())
    {
        sinks.append(functionBlock->GetSignalSinks());
    }
}

QStringList Project::GetAllCALParameters()
{
    QStringList parameters;
    foreach (const CalibrationParameter *param, m_bspSpecification.CalibrationParameters())
    {
        parameters.append(param->GetParameterName());
    }

    foreach (FunctionBlock* fb, m_functionBlocks.List())
    {
        QMapIterator<QString, QString> it(fb->GetAllCalibrationParameters());
        while(it.hasNext())
        {
            it.next();
            parameters.append(it.key());
        }
    }
    return parameters;
}

void Project::GetAllTasks(QList<const Task*>& tasks) const
{
    foreach (const BSPRunnable* runnable, m_bspSpecification.Runnables())
    {
        tasks.append(new Task(runnable->GetInitFunction(), runnable->GetRunnableFunction(), runnable->GetInitialDelay(), runnable->GetCycleTime()));
    }

    foreach (FunctionBlock* function, m_functionBlocks.List())
    {
        tasks.append(function->GetFunctionDefinition().Tasks());
    }

    if (m_bspSpecification.HasDisplay())
    {
        tasks.append(m_layoutDefinition.Tasks());
        tasks.append(m_checkMessagesDefinition.Tasks());
        foreach (Layout* layout, m_layoutDefinition.Layouts())
        {

            if (layout->HasMenu())
            {
                tasks.append(layout->GetMenu()->Tasks());
            }
        }
    }

    if (m_segmentDisplay.GetEnabled())
    {
        tasks.append(m_segmentDisplay.Tasks());
    }

    tasks.append(m_icPlatform.Tasks());
}


void Project::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitProject(this);
}

