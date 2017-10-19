#include "AnimationFrameByFrame.h"
#include "ProjectObjectVisitor.h"

AnimationFrameByFrame::AnimationFrameByFrame() : Animation(),
    m_frameDurationMs(50)
{
    SetName("FrameAnimation");
    SetID(Animation::GetNewID());
    AddSignalSource("CurrentImageID", "uint32");
}

AnimationFrameByFrame::AnimationFrameByFrame(quint32 id) : Animation(),
    m_frameDurationMs(50)
{
    SetName("FrameAnimation");
    SetID(id);
    AddSignalSource("CurrentImageID", "uint32");
}

AnimationFrameByFrame::~AnimationFrameByFrame()
{
    RemoveSignalSource("CurrentImageID");
}

void AnimationFrameByFrame::AddImageId(quint32 imageId)
{
   m_imageIds.append(imageId);
   NotifyPropertyUpdated();
   emit PropertyUpdatedRedrawNeeded();
}

void AnimationFrameByFrame::RemoveImageId(quint32 imageId)
{
    m_imageIds.removeAll(imageId);
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

QList<quint32> AnimationFrameByFrame::ImageIds() const
{
    return m_imageIds;
}

void AnimationFrameByFrame::SetFrameDurationMs(quint32 duration)
{
    m_frameDurationMs = duration;
    NotifyPropertyUpdated();
}

quint32 AnimationFrameByFrame::GetFrameDurationMs() const
{
    return m_frameDurationMs;
}

void AnimationFrameByFrame::Clear()
{
    m_imageIds.clear();
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

void AnimationFrameByFrame::AcceptVisitor(ProjectObjectVisitor &visitor)
{
    visitor.VisitAnimationFrameByFrame(this);
}

Animation::Type AnimationFrameByFrame::GetAnimationType() const
{
    return Animation::Type::FrameByFrame;
}
