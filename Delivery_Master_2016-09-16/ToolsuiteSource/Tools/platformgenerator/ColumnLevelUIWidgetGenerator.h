#ifndef COLUMNLEVELUIWIDGETGENERATOR_H
#define COLUMNLEVELUIWIDGETGENERATOR_H

#include "FileGeneratorBase.h"
#include "ColumnLevelWidget.h"

class Project;

class ColumnLevelUIWidgetGenerator : public FileGeneratorBase
{
public:
    explicit ColumnLevelUIWidgetGenerator(QList<UIWidgetBase*> uiWidgets, Project &project);

    // FileGeneratorBase interface
protected:
    void OnGenerateFiles(void);

private:
    QList<UIWidgetBase*> m_uiWidgets;
    Project& m_project;
    QString m_Path;

    void generateSource(void);
};

#endif // COLUMNLEVELUIWIDGETGENERATOR_H
