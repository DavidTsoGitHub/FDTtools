#include "PropertiesWidgetContainer.h"

#include "Project.h"
#include "ProjectObject.h"
#include "ProjectObjectPropertyVisitor.h"

#include <QStackedWidget>

PropertiesWidgetContainer::PropertiesWidgetContainer(QWidget *parent) : QStackedWidget(parent),
    m_project(NULL)
{
    setContentsMargins(0, 0, 0, 0);
}

void PropertiesWidgetContainer::SetProject(Project* project)
{
    m_project = project;
}

void PropertiesWidgetContainer::ClearProject()
{
    ClearCurrentProjectObject();
    m_project = NULL;
}

void PropertiesWidgetContainer::SetCurrentProjectObject(ProjectObject* selectedItem)
{
    if (m_project == NULL)
    {
        return;
    }

    ClearCurrentProjectObject();

    if (selectedItem == NULL)
    {
        emit LayoutNotChoosenAnymore();
        return;
    }

    ProjectObjectPropertyVisitor v(m_project);
    selectedItem->Accept(v);

    QWidget* propertyWidget = v.GetPropertyWidget();
    if (propertyWidget == NULL)
    {
        emit LayoutNotChoosenAnymore();
        return;
    }
    addWidget(propertyWidget);
    propertyWidget->setVisible(true);

    if (dynamic_cast<LayoutDefinition*>(selectedItem) == NULL)
    {
        emit LayoutNotChoosenAnymore();
    }
}

void PropertiesWidgetContainer::ClearCurrentProjectObject()
{
    QWidget* current = currentWidget();
    removeWidget(current);
    delete current;
}

