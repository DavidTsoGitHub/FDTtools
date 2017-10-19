#ifndef UIWIDGETDEFINITIONBASE_H
#define UIWIDGETDEFINITIONBASE_H

#include "ProjectObject.h"

class UIWidgetDefinitionBase : public ProjectObject
{
public:
    explicit UIWidgetDefinitionBase(const QString& objectName, ProjectObject* parent = 0);
    virtual QString GetResource(quint32 id) = 0;
    virtual QString GetResourceName(quint32 id) = 0;
};

#endif // UIWIDGETDEFINITIONBASE_H
