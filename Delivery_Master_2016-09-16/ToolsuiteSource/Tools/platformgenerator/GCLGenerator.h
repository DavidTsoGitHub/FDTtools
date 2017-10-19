#ifndef GCLGENERATOR_H
#define GCLGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"
#include "TransitionCondition.h"

#include <QStringList>

class GCLGenerator : public FileGeneratorBase
{
public:
    explicit GCLGenerator(Project* project);
    ~GCLGenerator(void);

    //FileGeneratorBase
public:
    void OnGenerateFiles(void);

private:
    QList<GCLSignalSink*> m_allSinks;
    QList<GCLSignalSource*> m_allSources;
    QMultiHash<QString, GCLSignalSource*> m_sourcesByComponentName;
    QMultiHash<QString, GCLSignalSink*> m_sinksByComponentName;

    const BSPSpecification& m_bspSpecification;
    const CANSpecification& m_canSpecification;
    const FunctionBlockContainer& m_functionBlocks;
    const LayoutDefinition& m_layout;
    const SegmentDisplay& m_segmentDisplay;
    const ICPlatform& m_icPlatform;
    const AnimationDefinition* m_animationDefinition;
    MenuDefinition* m_menuDefinition;
    const CheckMessageDefinition& m_checkMessagesDefinition;
    QList<GCLSignalSink*> m_startupLayoutSinks;

    QList<UIWidgetBase*> m_uiWidgets;
    QList<TransitionCondition*> m_transitions;
    QString m_GCLMenuControllerHeaderPath;
    QString m_GCLCheckMessageManagerHeaderPath;
    QString m_GCLContentsManagerHeaderPath;
    QString m_GCLTransitionsManagerHeaderPath;
    QString m_GCLSegmentDisplayManagerHeaderPath;
    QString m_GCLWidgetsHeaderPath;
    QString m_GCLPlatformHeaderPath;
    QString m_GCLCOMHeaderPath;
    QString m_GCLAnimationManagerHeaderPath;
    QString m_GCLSourcePath;
    QString m_GCLHeaderPath;


    /* Preparation */
    void readAllSignals(void);
    void parseMenuWidgetsSinks(const MenuItem* menuItem);

    /* Validation */
    bool signalMappingsAreValid(void);

    /* Generators */
    void generateGCLCOMHeader(void);
    void generateGCLFunctionBlockHeaders(void);
    void generateGCLMenuControllerHeader(void);
    void generateGCLCheckMessageManagerHeader(void);
    void generateGCLLayoutManagerHeader(void);
    void generateGCLTransitionManagerHeader(void);
    void generateGCLSegmentDisplayManagerHeader(void);
    void generateGCLWidgetsHeader(void);
    void generateAnimationsHeader(void);
    void generateGCLPlatformHeader(void);
    void generateGCLHeader(void);
    void generateGCLSource(void);

    /* Helpers */
    void addFunctionBlocksSignalMappingMembers(QStringList& sourceFileLines);
    void getAllStartupLayoutConditionSinks();
};

#endif // GCLGENERATOR_H
