#ifndef ANIMATIONMANAGERGENERATOR_H
#define ANIMATIONMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

class AnimationManagerGenerator : public FileGeneratorBase
{
public:
    explicit AnimationManagerGenerator(Project* project);
    ~AnimationManagerGenerator();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    QString m_animationsManagerHeaderPath;
    QString m_animationsManagerSourcePath;
    QString m_animationsCfgHeaderPath;
    QString m_animationsCfgSourcePath;
    static const quint32 m_animationManagerCycleTimeMs;

    Project* m_project;
    quint32 m_numberOfAnimations;

    void generateAnimationManagerHeader(void);
    void generateAnimationManagerSource(void);
    void generateAnimationsCfgHeader(void);
    void generateAnimationsCfgSource(void);

};

#endif // ANIMATIONMANAGERGENERATOR_H
