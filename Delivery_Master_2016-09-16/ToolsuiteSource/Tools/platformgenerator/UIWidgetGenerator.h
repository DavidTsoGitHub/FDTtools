#ifndef UIWIDGETGENERATOR_H
#define UIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"
#include <QMap>

class UIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit UIWidgetGenerator(Project& project);
    ~UIWidgetGenerator(void);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles();

private:
    Project& m_project;
    QMap<QString, UIWidgetBase*> m_allWidgetsByType;
    QStringList m_widgetTypes;
    QString m_UIWidgetSourcePath;


    void readAllWidgets();
    void addAllStaticUIWidgetHeaderFiles();
    //void addStaticUIWidgetFile(QString uiWidgetTypeName);
    void addDynamicUIWidgetFiles();
    void parseMenuWidgetsForMenuItem(MenuItem *currentMenuItem);
};

#endif // UIWIDGETGENERATOR_H
