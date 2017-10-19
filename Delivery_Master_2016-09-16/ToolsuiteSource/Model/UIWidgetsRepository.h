#ifndef UIWIDGETSREPOSITORY_H
#define UIWIDGETSREPOSITORY_H

#include "Project.h"
#include "UIWidgetBase.h"
#include <QList>

class UIWidgetsRepository
{
public:
    static QList<UIWidgetBase*>& GetUIWidgets(Project& project);
    static void ClearUIWidgets(void);
    static UIWidgetBase* CloneWidget(QString name, Project& project);

private:
    UIWidgetsRepository();
    static void initializeWidgets(Project& project);
    static bool initialized;

    static QList<UIWidgetBase*> uiWidgets;
};

#endif // UIWIDGETSREPOSITORY_H
