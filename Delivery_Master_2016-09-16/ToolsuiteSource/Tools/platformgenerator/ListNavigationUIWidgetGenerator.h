#ifndef LISTNAVIGATIONUIWIDGETGENERATOR_H
#define LISTNAVIGATIONUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "ListNavigationWidget.h"

class LayoutDefinition;
class Project;

class ListNavigationUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit ListNavigationUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project);
    ~ListNavigationUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    Project& m_project;
    LayoutDefinition& m_layout;

    void generateListNavigationConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // LISTNAVIGATIONUIWIDGETGENERATOR_H
