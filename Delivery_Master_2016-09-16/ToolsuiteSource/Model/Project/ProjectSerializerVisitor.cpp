#include "ProjectSerializerVisitor.h"

#include "ConfigFileWriter.h"
#include "ProjectSerializer.h"
#include "LabelDefinitionSerializer.h"
#include "LocalProjectSettingsSerializer.h"
#include "CheckMessagesSerializer.h"
#include "SignalMappingSerializer.h"
#include "IconSerializer.h"
#include "imageserializer.h"
#include "DiagDefinitionSerializer.h"
#include "LayoutDefinitionSerializer.h"
#include "ConfigFileWriter.h"
#include "MenuDefinitionSerializer.h"
#include "SegmentDisplaySerializer.h"
#include "SerializerBase.h"
#include "AnimationSerializer.h"

const QString MenuDefinitionPath("Menu.xml");
const QString LayoutDefinitionPath("Layout.xml");
const QString SegmentDefinitionPath("SegmentDisplay.xml");
const QString LabelDefinitionPath("Labels.xml");
const QString SignalMappingPath("SignalMapping.xml");
const QString CheckMessagesDefinitionPath("CheckMessages.xml");
const QString IconDefinitionPath("Icons.xml");
const QString ImageDefinitionPath("Images.xml");
const QString FontsDirectoryName("Fonts");
const QString IconsDirectoryName("Icons");
const QString DiagDefinitionPath("Diagnostics.xml");
const QString AnimationDefinitionPath("Animations.xml");

ProjectSerializerVisitor::ProjectSerializerVisitor(const QDir& projectDirectory) :
    ProjectObjectVisitor(true),
    m_projectDirectory(projectDirectory),
    m_result(NotExecuted)
{
}

ProjectSerializerVisitor::ResultCode ProjectSerializerVisitor::Result()
{
    return m_result;
}

QStringList ProjectSerializerVisitor::ErrorStringList()
{
    return m_errors;
}

QString ProjectSerializerVisitor::ErrorString()
{
    return m_errors.join('\n');
}

void ProjectSerializerVisitor::writeFile(SerializerBase& serializer, const QString& fileName)
{
    QString error;
    if (!ConfigFileWriter::WriteToFile(serializer, fileName, error))
    {
        m_result = Failure;
        m_errors.append(fileName + ": " + error);
    }
}

void ProjectSerializerVisitor::VisitCheckMessages(CheckMessageDefinition* checkMessages)
{
    if (!checkMessages->IsDirtyRecursive()) return;

    QString checkMessagesDefinitionFileName = m_projectDirectory.absoluteFilePath(CheckMessagesDefinitionPath);
    CheckMessagesSerializer checkMessagesSerializer(*checkMessages);
    writeFile(checkMessagesSerializer, checkMessagesDefinitionFileName);
}

void ProjectSerializerVisitor::VisitDiagDefinition(DiagDefinition* diagDefinition)
{
    if (!diagDefinition->IsDirtyRecursive()) return;
    QString diagDefinitionFileName = m_projectDirectory.absoluteFilePath(DiagDefinitionPath);
    DiagDefinitionSerializer diagDefinitionSerializer(*diagDefinition);
    writeFile(diagDefinitionSerializer, diagDefinitionFileName);
}

void ProjectSerializerVisitor::VisitIconDefinition(IconDefinition* iconDefinition)
{
    if (!iconDefinition->IsDirtyRecursive()) return;
    QString iconDefinitionFileName = m_projectDirectory.absoluteFilePath(IconDefinitionPath);
    IconSerializer iconSerializer(*iconDefinition);
    writeFile(iconSerializer, iconDefinitionFileName);
}

void ProjectSerializerVisitor::VisitImageDefinition(ImageDefinition* imageDefinition)
{
    if (!imageDefinition->IsDirtyRecursive()) return;
    QString imageDefinitionFileName = m_projectDirectory.absoluteFilePath(ImageDefinitionPath);
    ImageSerializer imageSerializer(*imageDefinition);
    writeFile(imageSerializer, imageDefinitionFileName);
}

void ProjectSerializerVisitor::VisitLabelDefinition(LabelDefinition* labelDefinition)
{
    if (!labelDefinition->IsDirtyRecursive()) return;

    QString labelDefinitionFileName = m_projectDirectory.absoluteFilePath(LabelDefinitionPath);
    LabelDefinitionSerializer labelDefinitionSerializer(*labelDefinition);
    writeFile(labelDefinitionSerializer, labelDefinitionFileName);
}

void ProjectSerializerVisitor::VisitLayoutDefinition(LayoutDefinition* layoutDefinition)
{
    if (!layoutDefinition->IsDirtyRecursive()) return;

    QString layoutDefinitionFileName = m_projectDirectory.absoluteFilePath(LayoutDefinitionPath);
    LayoutDefinitionSerializer layoutDefinitionSerializer(*layoutDefinition);
    writeFile(layoutDefinitionSerializer, layoutDefinitionFileName);
}

void ProjectSerializerVisitor::VisitMenuDefinition(MenuDefinition* menuDefinition)
{
    if (!menuDefinition->IsDirtyRecursive()) return;

    QString menuDefinitionFileName = m_projectDirectory.absoluteFilePath(MenuDefinitionPath);
    MenuDefinitionSerializer menuDefinitionSerializer(*menuDefinition);
    writeFile(menuDefinitionSerializer, menuDefinitionFileName);
}

void ProjectSerializerVisitor::VisitAnimationDefinition(AnimationDefinition *animationDefinition)
{
    if (!animationDefinition->IsDirtyRecursive()) return;
    QString animationDefinitionFileName = m_projectDirectory.absoluteFilePath(AnimationDefinitionPath);
    AnimationSerializer animationSerializer(*animationDefinition);
    writeFile(animationSerializer, animationDefinitionFileName);
}

void ProjectSerializerVisitor::VisitProject(Project* project)
{
    m_result = Success;

    ProjectSerializer projectSerializer(*project);
    writeFile(projectSerializer, project->GetProjectFile().absoluteFilePath());

    QString localProjectSettingsFileName = QString(project->GetProjectFile().absoluteFilePath()).append(".local");
    LocalProjectSettingsSerializer localProjectSettingsSerializer(*project);
    writeFile(localProjectSettingsSerializer, localProjectSettingsFileName);

    SignalMappingSerializer signalMappingSerializer(*project);
    QString signalMappingFileName = m_projectDirectory.absoluteFilePath(SignalMappingPath);
    writeFile(signalMappingSerializer, signalMappingFileName);
}

void ProjectSerializerVisitor::VisitSegmentDisplay(SegmentDisplay* segmentDisplay)
{
    if (!segmentDisplay->IsDirtyRecursive()) return;

    QString segmentDisplayFileName = m_projectDirectory.absoluteFilePath(SegmentDefinitionPath);
    SegmentDisplaySerializer segmentDisplaySerializer(*segmentDisplay);
    writeFile(segmentDisplaySerializer, segmentDisplayFileName);
}
