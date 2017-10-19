#ifndef DYNAMICVALUELABELUIWIDGETGENERATOR_H
#define DYNAMICVALUELABELUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "UIWidgetBase.h"
#include "DynamicValueLabelWidget.h"
#include "Project.h"

class DynamicValueLabelUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit DynamicValueLabelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgetList, Project& project);
    ~DynamicValueLabelUIWidgetGenerator();

private:
    QList<UIWidgetBase*> m_uiWidgetList;
    Project& m_project;

    QString m_dynamicValueLabelPath;
    void generateDynamicValueLabelConfigurationSourceFile();

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();
};

#endif // DYNAMICVALUELABELUIWIDGETGENERATOR_H
