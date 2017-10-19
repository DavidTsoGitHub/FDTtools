#ifndef PROJECTSERIALIZERVISITOR_H
#define PROJECTSERIALIZERVISITOR_H

#include "ProjectObjectVisitor.h"

class SerializerBase;

class ProjectSerializerVisitor : public ProjectObjectVisitor
{
public:
    enum ResultCode
    {
        NotExecuted,
        Failure,
        Success
    };

    explicit ProjectSerializerVisitor(const QDir& projectDirectory);

    ResultCode Result();
    QStringList ErrorStringList();
    QString ErrorString();

private:
    QDir m_projectDirectory;
    ResultCode m_result;
    QStringList m_errors;

    void writeFile(SerializerBase& serializer, const QString& fileName);

    // ProjectObjectVisitor interface
public:
    void VisitCheckMessages(CheckMessageDefinition* checkMessages);
    void VisitDiagDefinition(DiagDefinition* diagDefinition);
    void VisitIconDefinition(IconDefinition* iconDefinition);
    void VisitImageDefinition(ImageDefinition* imageDefinition);
    void VisitLabelDefinition(LabelDefinition* labelDefinition);
    void VisitLayoutDefinition(LayoutDefinition* layoutDefinition);
    void VisitMenuDefinition(MenuDefinition* menuDefinition);
    void VisitAnimationDefinition(AnimationDefinition* animationDefinition);
    void VisitProject(Project* project);
    void VisitSegmentDisplay(SegmentDisplay* segmentDisplay);
};

#endif // PROJECTSERIALIZERVISITOR_H
