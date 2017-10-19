#ifndef SELECTABLELISTUIWIDGETGENERATOR_H
#define SELECTABLELISTUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "SelectableListWidget.h"

class LayoutDefinition;
class Project;

class SelectableListUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit SelectableListUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project);
    ~SelectableListUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    Project& m_project;
    LayoutDefinition& m_layoutDefinition;

    void generateConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

};

#endif // SELECTABLELISTUIWIDGETGENERATOR_H
