#ifndef DYNAMICTEXTLABELUIWIDGETGENERATOR_H
#define DYNAMICTEXTLABELUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "DynamicTextLabelWidget.h"
#include "Project.h"

class DynamicTextLabelUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit DynamicTextLabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project);
    ~DynamicTextLabelUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    Project& m_project;

    QString m_dynamicTextLabelPath;
    void generateDynamicTextLabelConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // DYNAMICTEXTLABELUIWIDGETGENERATOR_H
