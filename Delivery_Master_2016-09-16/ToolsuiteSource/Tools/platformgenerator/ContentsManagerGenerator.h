#ifndef CONTENTSMANAGERGENERATOR_H
#define CONTENTSMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "LayoutTransitionTypes.h"

class AnimationDefinition;
class BSPDisplay;
class Project;
class Layout;
class LayoutDefinition;

class ContentsManagerGenerator : public FileGeneratorBase
{
public:
    explicit ContentsManagerGenerator(Project* project);
    ~ContentsManagerGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    const BSPDisplay* m_display;
    LayoutDefinition& m_layoutDefinition;
    AnimationDefinition& m_animationDefinition;

    QString m_ContentsManagerConfigHeaderFile;
    QString m_ContentsManagerConfigSourceFile;

    void generateContentsManagerConfigurationHeader(void);
    void generateContentsManagerConfigurationSource(void);
    QString getConnectiveString(LayoutTransitionTypes::TransitionConnective connective);
    QString getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition);
    QString getButtonMappingsString(Layout *layout);
    QString getGCLString(Layout *layout);
};

#endif // CONTENTSMANAGERGENERATOR_H
