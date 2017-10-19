#ifndef PROJECTOBJECTVISITOR_H
#define PROJECTOBJECTVISITOR_H

#include "AnimationDefinition.h"
#include "AnimationFrameByFrame.h"
#include "AnimationOpacity.h"
#include "AnimationTranslation.h"
#include "BSPSpecification.h"
#include "CANSpecification.h"
#include "CheckMessage.h"
#include "CheckMessagesDefinition.h"
#include "ProjectObject.h"
#include "DiagDefinition.h"
#include "DisplayArea.h"
#include "FunctionBlock.h"
#include "FunctionBlockContainer.h"
#include "FunctionDefinition.h"
#include "GCLSignalEndpoint.h"
#include "Icon.h"
#include "IconDefinition.h"
#include "Image.h"
#include "ImageDefinition.h"
#include "ICPlatform.h"
#include "LabelDefinition.h"
#include "Layout.h"
#include "LayoutDefinition.h"
#include "MenuDefinition.h"
#include "MenuItem.h"
#include "Project.h"
#include "RootMenuItemContainer.h"
#include "SegmentDisplay.h"
#include "UIWidgetBase.h"

#include <QDebug>

class ProjectObjectVisitor
{
public:

    const bool VisitRecursive;

    virtual void VisitProjectObject(ProjectObject*) { }
    virtual void VisitBSPSpecification(BSPSpecification* bspSpecification) { VisitProjectObject(bspSpecification); }
    virtual void VisitCANSpecification(CANSpecification* canSpecification) { VisitProjectObject(canSpecification); }
    virtual void VisitCheckMessages(CheckMessageDefinition* checkMessagesDefinition) { VisitProjectObject(checkMessagesDefinition); }
    virtual void VisitCheckMessage(CheckMessage* checkMessage) { VisitProjectObject(checkMessage); }
    virtual void VisitDiagDefinition(DiagDefinition* diagDefinition) { VisitProjectObject(diagDefinition); }
    virtual void VisitDisplayArea(DisplayArea* displayArea) { VisitProjectObject(displayArea); }
    virtual void VisitFunctionBlock(FunctionBlock* functionBlock) { VisitProjectObject(functionBlock); }
    virtual void VisitFunctionBlockContainer(FunctionBlockContainer* functionBlockContainer) { VisitProjectObject(functionBlockContainer); }
    virtual void VisitFunctionDefinition(FunctionDefinition* functionBlockDefinition) {  VisitProjectObject(functionBlockDefinition); }
    virtual void VisitGCLSignalEndpoint(GCLSignalEndpoint* gclSignalEndpoint) { VisitProjectObject(gclSignalEndpoint); }
    virtual void VisitIconDefinition(IconDefinition* iconDefinition) { VisitProjectObject(iconDefinition); }
    virtual void VisitIcon(Icon* icon) { VisitProjectObject(icon); }
    virtual void VisitImage(Image* image) { VisitProjectObject(image); }
    virtual void VisitImageDefinition(ImageDefinition* imageDefinition) { VisitProjectObject(imageDefinition); }
    virtual void VisitICPlatform(ICPlatform* icPlatform) { VisitProjectObject(icPlatform); }
    virtual void VisitLabelDefinition(LabelDefinition* labelDefinition) { VisitProjectObject(labelDefinition); }
    virtual void VisitLayoutDefinition(LayoutDefinition* layoutDefinition) { VisitProjectObject(layoutDefinition); }
    virtual void VisitLayout(Layout* layout) { VisitProjectObject(layout); }
    virtual void VisitMenuDefinition(MenuDefinition* menuDefinition) { VisitProjectObject(menuDefinition); }
    virtual void VisitMenuItem(MenuItem* menuItem) { VisitProjectObject(menuItem); }
    virtual void VisitProject(Project* project) { VisitProjectObject(project); }
    virtual void VisitRootMenuItemContainer(RootMenuItemContainer* rootMenuItem) { VisitProjectObject(rootMenuItem); }
    virtual void VisitSegmentDisplay(SegmentDisplay* segmentDisplay) { VisitProjectObject(segmentDisplay); }
    virtual void VisitUIWidgetBase(UIWidgetBase* uiWidget) { VisitProjectObject(uiWidget); }
    virtual void VisitAnimationDefinition(AnimationDefinition* animationDefinition) { VisitProjectObject(animationDefinition); }
    virtual void VisitAnimationFrameByFrame(AnimationFrameByFrame* animation) { VisitProjectObject(animation); }
    virtual void VisitAnimationOpacity(AnimationOpacity* animation) { VisitProjectObject(animation); }
    virtual void VisitAnimationTranslation(AnimationTranslation* animation) { VisitProjectObject(animation); }

protected:
    explicit ProjectObjectVisitor(bool visitRecursive);
};

#endif // PROJECTOBJECTVISITOR_H
