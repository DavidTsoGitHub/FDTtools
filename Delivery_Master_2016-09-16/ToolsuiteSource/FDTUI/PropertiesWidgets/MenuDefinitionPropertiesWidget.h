#ifndef MENUDEFINITIONPROPERTIESWIDGET_H
#define MENUDEFINITIONPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class Project;

class QWidget;

class MenuDefinitionPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit MenuDefinitionPropertiesWidget(Project* project, QWidget *parent = 0);
    ~MenuDefinitionPropertiesWidget();

private:

};

#endif // MENUDEFINITIONPROPERTIESWIDGET_H
