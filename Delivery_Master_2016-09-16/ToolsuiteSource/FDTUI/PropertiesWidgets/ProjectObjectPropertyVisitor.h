#ifndef PROJECTOBJECTPROPERTYVISITOR_H
#define PROJECTOBJECTPROPERTYVISITOR_H

#include "ProjectObjectVisitor.h"

class ProjectObjectPropertyVisitor : public ProjectObjectVisitor
{
public:
    explicit ProjectObjectPropertyVisitor(Project* project);

    QWidget* GetPropertyWidget() const;

private:
    Project* m_project;
    QWidget* m_propertyWidget;

    // ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject*);
    void VisitBSPSpecification(BSPSpecification*);
    void VisitCANSpecification(CANSpecification*);
    void VisitCheckMessages(CheckMessageDefinition*);
    void VisitCheckMessage(CheckMessage *);
    void VisitDiagDefinition(DiagDefinition*);
    void VisitDisplayArea(DisplayArea*);
    void VisitFunctionBlock(FunctionBlock*);
    void VisitFunctionBlockContainer(FunctionBlockContainer*);
    void VisitFunctionDefinition(FunctionDefinition*);
    void VisitGCLSignalEndpoint(GCLSignalEndpoint*);
    void VisitIcon(Icon* icon);
    void VisitIconDefinition(IconDefinition*);
    void VisitImage(Image*);
    void VisitImageDefinition(ImageDefinition*);
    void VisitICPlatform(ICPlatform*);
    void VisitLabelDefinition(LabelDefinition*);
    void VisitLayoutDefinition(LayoutDefinition*);
    void VisitLayout(Layout*);
    void VisitMenuDefinition(MenuDefinition*);
    void VisitMenuItem(MenuItem*);
    void VisitProject(Project*);
    void VisitRootMenuItemContainer(RootMenuItemContainer*);
    void VisitSegmentDisplay(SegmentDisplay*);
    void VisitUIWidgetBase(UIWidgetBase*);
    void VisitAnimationDefinition(AnimationDefinition*);
    void VisitAnimationFrameByFrame(AnimationFrameByFrame*);
    void VisitAnimationOpacity(AnimationOpacity*);
    void VisitAnimationTranslation(AnimationTranslation*);
};

#endif // PROJECTOBJECTPROPERTYVISITOR_H
