#ifndef PROJECT
#define PROJECT

#include "BSPRunnable.h"
#include "BSPSpecification.h"
#include "CANSpecification.h"
#include "FunctionBlockContainer.h"
#include "LabelDefinition.h"
#include "LayoutDefinition.h"
#include "MenuDefinition.h"
#include "CheckMessagesDefinition.h"
#include "TargetSettings.h"
#include "IconDefinition.h"
#include "ImageDefinition.h"
#include "DiagDefinition.h"
#include "ICPlatform.h"
#include "SegmentDisplay.h"
#include "Task.h"
#include "DeserializerBase.h"
#include "AnimationDefinition.h"

#include "ProjectObject.h"
#include <QDir>

class Project : public ProjectObject
{
    Q_OBJECT

public:
    Project();
    ~Project();

    static const QString MenuDefinitionPath;
    static const QString LayoutDefinitionPath;
    static const QString SegmentDefinitionPath;
    static const QString LabelDefinitionPath;
    static const QString SignalMappingPath;
    static const QString CheckMessagesDefinitionPath;
    static const QString IconDefinitionPath;
    static const QString FontsDirectoryName;
    static const QString ImageDefinitionPath;
    static const QString AnimationDefinitionPath;
    static const QString IconsDirectoryName;
    static const QString DiagDefinitionPath;
    static const QString ImagesDirectoryName;

    const QDir GetOutputFolder(void) const;

    void SetBSPSpecificationPath(QString bspSpecificationPath);
    void SetCANClusterECUName(QString canClusterECUName);
    void SetCANSpecificationPath(QString canSpecificationPath);
    void UpdateCanSpecification(QString path);

    void SetProjectName(QString projectName);
    const QString GetProjectName() const;
    const QString GetBSPSpecificationPath() const;
    const QString GetCANSpecificationPath() const;
    const QString GetCANClusterECUName() const;
    const QString InitializationError() const;

    void Initialize(QFileInfo projectRoot, QDir functionLibraryDir);
    bool IsInitialized() const;
    void AddFunctionBlock(FunctionBlock* functionBlock);
    void RemoveFunctionBlock(FunctionBlock* functionBlock);

    const QFileInfo GetProjectFile() const;
    const QDir GetProjectRoot() const;
    const QDir GetFunctionLibraryRoot() const;
    BSPSpecification& GetBSPSpecification();
    CANSpecification& GetCANSpecification();
    LayoutDefinition& GetLayoutDefinition();
    SegmentDisplay& GetSegmentDisplay();
    FunctionBlockContainer& GetFunctionBlocks();
    bool IsFunctionBlock(QString componentName);
    LabelDefinition& GetLabelDefinition();
    CheckMessageDefinition& GetCheckMessagesDefinition();
    IconDefinition& GetIconDefinition();
    ImageDefinition& GetImageDefinition();
    AnimationDefinition& GetAnimationDefinition();
    DiagDefinition& GetDiagDefinition();
    ICPlatform& GetICPlatform();
    TargetSettings& GetTargetSettings();
    void SetTargetSettings(const TargetSettings& newSettings);
    QStringList GetAllCALParameters();

    void GetAllTasks(QList<const Task*>& tasks) const;
    void GetAllGCLSinksByComponentName(QMultiMap<QString, GCLSignalSink*>& sinksByComponent);
    void GetAllGCLSourcesByComponentName(QMultiMap<QString, GCLSignalSource*>& sourcesByComponent);
    void GetAllGCLSinks(QList<GCLSignalSink*>& sinks) const;
    void GetAllGCLSources(QList<GCLSignalSource*>& source) const;

private:
    bool m_isInitialized;
    QString m_initializationError;
    QFileInfo m_projectFileInfo;
    QDir m_functionLibraryRoot;

    QString m_bspSpecificationPath;
    QString m_projectName;

    QString m_canSpecificationPath;
    QString m_canClusterECUName;

    BSPSpecification m_bspSpecification;
    CANSpecification m_canSpecification;
    CheckMessageDefinition m_checkMessagesDefinition;
    DiagDefinition m_diagDefinition;
    FunctionBlockContainer m_functionBlocks;
    ImageDefinition m_imageDefinition;
    AnimationDefinition m_animationDefinition;
    ICPlatform m_icPlatform;
    IconDefinition m_iconDefinition;
    LabelDefinition m_labelDefinition;
    LayoutDefinition m_layoutDefinition;
    TargetSettings m_targetSettings;
    SegmentDisplay m_segmentDisplay;

    bool initializeBSPSpecification(void);
    bool initializeCANSpecification(void);
    bool initializeDisplay(void);
    bool initializeSegmentDisplay(void);
    bool initializeFunctionBlockContainer(void);
    bool initializeSignalMappingContainer(void);
    bool initializeLabelDefinition(void);
    bool initializeCheckMessagesDefinition(void);
    bool initializeIconDefinition(void);
    bool initializeImageDefinition(void);
    bool initializeAnimationDefinition(void);
    bool initializeDiagDefinition(void);
    bool initializeLocalProjectSettings(void);
    bool postInitializeProject(void);
    DeserializerBase::DeserializerRespons initialize(DeserializerBase* deserializer, QString path);
    bool checkIfFileExists(QString filename);
    bool newFileNeededAndCreationApproved(QString file, QString description);
    bool createNewFileDialog(QString description);
    void createLabelDefintionFile(QString filePath);
    void createCheckMessageDefinitionFile(QString filePath);
    void createIconDefintionFile(QString file);
    void createImageDefintionFile(QString filePath);
    void createAnimationDefintionFile(QString filePath);
    void createDiagDefintionFile(QString filePath);
    void createLayoutDefintionFile(QString filePath);
    void createLocalSettingsFile(QString filePath);
    void setupSignals(void);
    void updateProjectFiles(void);

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // PROJECT

