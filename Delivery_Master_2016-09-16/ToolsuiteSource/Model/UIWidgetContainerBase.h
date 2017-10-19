#ifndef UIWIDGETCONTAINERBASE_H
#define UIWIDGETCONTAINERBASE_H

#include "ProjectObject.h"
#include "UIWidgetBase.h"

class UIWidgetContainerBase : public ProjectObject
{
public:
    UIWidgetContainerBase(const QString& containerName,
                          ProjectObject* parent = 0);
    virtual ~UIWidgetContainerBase() { }
    virtual void AddUIWidget(UIWidgetBase* uiWidget) = 0;
};

#endif // UIWIDGETCONTAINERBASE_H
