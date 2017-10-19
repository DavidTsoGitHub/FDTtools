#include "Exceptions/WidgetInitializationException.h"
#include "UIWidgetsRepository.h"
#include "BlockBarWidget.h"
#include "DynamicValueLabelWidget.h"
#include "DynamicTextLabelWidget.h"
#include "IconWidget.h"
#include "ImageWidget.h"
#include "LabelWidget.h"
#include "SelectableListWidget.h"
#include "SignalTriggerWidget.h"
#include "DynamicIconWidget.h"
#include "DynamicImageWidget.h"
#include "ColumnLevelWidget.h"
#include "ListNavigationWidget.h"
#include "DynamicLabelWidget.h"
#include "BSPDisplay.h"

QList<UIWidgetBase*> UIWidgetsRepository::uiWidgets;
bool UIWidgetsRepository::initialized = false;

QList<UIWidgetBase*>& UIWidgetsRepository::GetUIWidgets(Project& project)
{
    if (!initialized)
    {
        initializeWidgets(project);
    }

    return uiWidgets;
}

void UIWidgetsRepository::ClearUIWidgets()
{
    uiWidgets.clear();
    initialized = false;
}

void UIWidgetsRepository::initializeWidgets(Project& project)
{
    uiWidgets.append(new BlockBarWidget(project.GetLayoutDefinition()));
    uiWidgets.append(new ColumnLevelWidget(project.GetLayoutDefinition()));
    uiWidgets.append(new DynamicIconWidget());
    if (project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        uiWidgets.append(new DynamicImageWidget(project.GetAnimationDefinition()));
    }
    uiWidgets.append(new DynamicLabelWidget(project.GetLayoutDefinition()));
    uiWidgets.append(new DynamicTextLabelWidget(project.GetLabelDefinition(), project.GetLayoutDefinition()));
    uiWidgets.append(new DynamicValueLabelWidget(project.GetLabelDefinition(), project.GetLayoutDefinition()));
    uiWidgets.append(new IconWidget(project.GetIconDefinition()));
    if (project.GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        uiWidgets.append(new ImageWidget(project.GetLayoutDefinition(), project.GetAnimationDefinition(), project.GetImageDefinition()));
    }
    uiWidgets.append(new LabelWidget(project.GetLabelDefinition(), project.GetLayoutDefinition()));
    uiWidgets.append(new ListNavigationWidget(project.GetLayoutDefinition()));
    uiWidgets.append(new SelectableListWidget(project.GetLayoutDefinition()));
    uiWidgets.append(new SignalTriggerWidget());

    initialized = true;
}

UIWidgetBase* UIWidgetsRepository::CloneWidget(QString name, Project& project)
{
    if (!initialized)
    {
        initializeWidgets(project);
    }

    foreach (UIWidgetBase* widget, uiWidgets)
    {
        if (widget->GetUIWidgetTypeName().compare(name, Qt::CaseInsensitive) == 0)
        {
            return widget->Clone();
        }
    }

    return NULL;
}
