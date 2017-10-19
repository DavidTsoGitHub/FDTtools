#include "ProjectObjectPropertyVisitor.h"

#include "ProjectPropertiesWidget.h"
#include "FunctionBlockPropertiesWidget.h"
#include "BSPSpecificationPropertiesWidget.h"
#include "CANSpecificationPropertiesWidget.h"
#include "DisplayPropertiesWidget.h"
#include "DisplayAreaPropertiesWidget.h"
#include "ImagePreviewPropertiesWidget.h"
#include "LayoutPropertiesWidget.h"
#include "MenuDefinitionPropertiesWidget.h"
#include "MenuItemPropertiesWidget.h"
#include "RootMenuContainerProperties.h"
#include "UIWidgetPropertiesWidget.h"
#include "AnimationFBFPropertiesWidget.h"
#include "AnimationOpacityPropertiesWidget.h"
#include "AnimationTranslationPropertiesWidget.h"
#include "CheckMessagePropertiesWidget.h"

ProjectObjectPropertyVisitor::ProjectObjectPropertyVisitor(Project* project) : ProjectObjectVisitor(false),
    m_project(project),
    m_propertyWidget(NULL)
{
}

QWidget* ProjectObjectPropertyVisitor::GetPropertyWidget() const
{
    return m_propertyWidget;
}

void ProjectObjectPropertyVisitor::VisitProjectObject(ProjectObject* projectObject)
{
    ProjectObjectPropertiesWidget* widget = new ProjectObjectPropertiesWidget(m_project);
    widget->SetProjectObject(projectObject);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitBSPSpecification(BSPSpecification* bspSpecification)
{
    BSPSpecificationPropertiesWidget* widget = new BSPSpecificationPropertiesWidget(m_project);
    widget->SetBSPSpecification(bspSpecification);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitCANSpecification(CANSpecification* canSpecification)
{
    CANSpecificationPropertiesWidget* widget = new CANSpecificationPropertiesWidget(m_project, m_project->GetCANClusterECUName());
    widget->SetCANSpecification(canSpecification);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitCheckMessages(CheckMessageDefinition* checkMessages)
{


    VisitProjectObject(checkMessages);
}

void ProjectObjectPropertyVisitor::VisitCheckMessage(CheckMessage* checkMessage)
{
    CheckMessagePropertiesWidget* widget = new CheckMessagePropertiesWidget(m_project);
        widget->SetProjectObject(checkMessage);
        m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitDiagDefinition(DiagDefinition* diagDefinition)
{
    VisitProjectObject(diagDefinition);
}

void ProjectObjectPropertyVisitor::VisitDisplayArea(DisplayArea* displayArea)
{
    DisplayAreaPropertiesWidget* widget = new DisplayAreaPropertiesWidget(m_project);
    widget->SetDisplayArea(displayArea);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitFunctionBlock(FunctionBlock* functionBlock)
{
    FunctionBlockPropertiesWidget* widget = new FunctionBlockPropertiesWidget(m_project);
    widget->SetFunctionBlock(functionBlock);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitFunctionBlockContainer(FunctionBlockContainer* functionBlockContainer)
{
    VisitProjectObject(functionBlockContainer);
}

void ProjectObjectPropertyVisitor::VisitFunctionDefinition(FunctionDefinition* functionDefinition)
{
    VisitProjectObject(functionDefinition);
}

void ProjectObjectPropertyVisitor::VisitGCLSignalEndpoint(GCLSignalEndpoint* gclSignalEndpoint)
{
    VisitProjectObject(gclSignalEndpoint);
}

void ProjectObjectPropertyVisitor::VisitIcon(Icon* icon)
{
    ImagePreviewPropertiesWidget* widget = new ImagePreviewPropertiesWidget(m_project);
    widget->SetIcon(icon);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitIconDefinition(IconDefinition* iconDefinition)
{
    VisitProjectObject(iconDefinition);
}

void ProjectObjectPropertyVisitor::VisitImage(Image* image)
{
    ImagePreviewPropertiesWidget* widget = new ImagePreviewPropertiesWidget(m_project);
    widget->SetImage(image);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitImageDefinition(ImageDefinition* imageDefinition)
{
    VisitProjectObject(imageDefinition);
}

void ProjectObjectPropertyVisitor::VisitICPlatform(ICPlatform* icPlatform)
{
    VisitProjectObject(icPlatform);
}

void ProjectObjectPropertyVisitor::VisitLabelDefinition(LabelDefinition* labelDefinition)
{
    VisitProjectObject(labelDefinition);
}

void ProjectObjectPropertyVisitor::VisitLayoutDefinition(LayoutDefinition* layoutDefinition)
{
    if (m_project->GetBSPSpecification().HasDisplay())
    {
        DisplayPropertiesWidget* widget = new DisplayPropertiesWidget(m_project);
        widget->SetLayoutDefinition(layoutDefinition);
        m_propertyWidget = widget;
    }
    else
    {
        m_propertyWidget = NULL;
    }
}

void ProjectObjectPropertyVisitor::VisitLayout(Layout* layout)
{
    LayoutPropertiesWidget* widget = new LayoutPropertiesWidget(m_project);
    widget->SetLayout(layout);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitMenuDefinition(MenuDefinition* menuDefinition)
{
    MenuDefinitionPropertiesWidget* widget = new MenuDefinitionPropertiesWidget(m_project);
    widget->SetProjectObject(menuDefinition);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitMenuItem(MenuItem* menuItem)
{
    MenuItemPropertiesWidget* widget = new MenuItemPropertiesWidget(m_project);
    widget->SetMenuItem(menuItem);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitProject(Project* project)
{
    ProjectPropertiesWidget* widget = new ProjectPropertiesWidget(project);
    widget->SetProjectObject(project);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitRootMenuItemContainer(RootMenuItemContainer* rootMenuContainer)
{
    RootMenuContainerPropertiesWidget* widget = new RootMenuContainerPropertiesWidget(m_project);
    widget->SetRootMenuContainer(rootMenuContainer);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitSegmentDisplay(SegmentDisplay* segmentDisplay)
{
    VisitProjectObject(segmentDisplay);
}

void ProjectObjectPropertyVisitor::VisitUIWidgetBase(UIWidgetBase* uiWidget)
{
    UIWidgetPropertiesWidget* widget = new UIWidgetPropertiesWidget(m_project);
    widget->SetUIWidget(uiWidget);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitAnimationDefinition(AnimationDefinition *definition)
{
    VisitProjectObject(definition);
}

void ProjectObjectPropertyVisitor::VisitAnimationFrameByFrame(AnimationFrameByFrame *animation)
{
    AnimationFBFPropertiesWidget* widget = new AnimationFBFPropertiesWidget(m_project);
    widget->SetAnimationFrameByFrame(animation);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitAnimationOpacity(AnimationOpacity *animation)
{
    AnimationOpacityPropertiesWidget* widget = new AnimationOpacityPropertiesWidget(m_project);
    widget->SetAnimationOpacity(animation);
    m_propertyWidget = widget;
}

void ProjectObjectPropertyVisitor::VisitAnimationTranslation(AnimationTranslation *animation)
{
    AnimationTranslationPropertiesWidget* widget = new AnimationTranslationPropertiesWidget(m_project);
    widget->SetAnimationTranslation(animation);
    m_propertyWidget = widget;
}
