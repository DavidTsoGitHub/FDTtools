#ifndef DISPLAYMANAGERGENERATOR_H
#define DISPLAYMANAGERGENERATOR_H

#include "BSPDisplay.h"
#include "FileGeneratorBase.h"
#include "Project.h"
#include "LayoutDefinition.h"

class DisplayManagerGenerator : public FileGeneratorBase
{
public:
    explicit DisplayManagerGenerator(Project* project);
    ~DisplayManagerGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    const BSPDisplay* m_display;
    LayoutDefinition& m_layout;

    QString m_DisplayManagerHeaderFile;
    QString m_DisplayManagerConfigSourceFile;
    QString m_DisplayManagerConfigHeaderFile;

    void generateDisplayManagerHeaderFile(void);
    void generateDisplayManagerConfigurationSource(void);
    void generateDisplayManagerConfigurationHeader(void);
    void createStaticDisplayManagerFiles(void);

    QString createDisplayAreaDefinition(quint16 x, quint16 y, quint16 width, quint16 height, bool border);
};

#endif // DISPLAYMANAGERGENERATOR_H
