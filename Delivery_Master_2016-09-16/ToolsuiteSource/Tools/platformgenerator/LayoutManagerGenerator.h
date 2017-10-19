#ifndef LAYOUTMANAGERGENERATOR_H
#define LAYOUTMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"
#include "LayoutDefinition.h"
#include "AnimationDefinition.h"

class LayoutManagerGenerator : public FileGeneratorBase
{
public:
    explicit LayoutManagerGenerator(Project* project);
    ~LayoutManagerGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    LayoutDefinition& m_layout;

    QString m_LayoutManagerHeaderFile;

    void generateLayoutManagerHeader(void);
};

#endif // LAYOUTMANAGERGENERATOR_H
