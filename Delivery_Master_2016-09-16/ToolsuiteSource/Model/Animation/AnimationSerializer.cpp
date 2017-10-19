#include "AnimationSerializer.h"
#include "Animation.h"
#include "Logger.h"

AnimationSerializer::AnimationSerializer(AnimationDefinition& animationDefinition) : SerializerBase(NULL),
    m_animationDefinition(animationDefinition)
{}

AnimationSerializer::~AnimationSerializer()
{}

QString AnimationSerializer::Serialize()
{
    LOG_DEBUG("Entering");
    QString xmlString;

    QXmlStreamWriter xmlWriter(&xmlString);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("AnimationDefinition");
    xmlWriter.writeStartElement("Animations");
    xmlWriter.writeAttribute("HighestID", QString::number(Animation::GetAnimationCount()));

    foreach (const AnimationFrameByFrame* animation, m_animationDefinition.FrameByFrameAnimations())
    {
        xmlWriter.writeStartElement("FrameByFrameAnimation");
        writeCommonAttributes(xmlWriter, *animation);

        xmlWriter.writeAttribute("FrameDurationMs", QString::number(animation->GetFrameDurationMs()));

        xmlWriter.writeStartElement("Frames");
        foreach(quint32 id, animation->ImageIds())
        {
            xmlWriter.writeStartElement("Frame");
            xmlWriter.writeAttribute("ImageId", QString::number(id));
            xmlWriter.writeEndElement(); // Frame
        }
        xmlWriter.writeEndElement(); // Frames
        xmlWriter.writeEndElement(); // FrameByFrameAnimation
    }
    foreach (const AnimationOpacity* animation, m_animationDefinition.OpacityAnimations())
    {
        xmlWriter.writeStartElement("OpacityAnimation");
        writeCommonAttributes(xmlWriter, *animation);
        xmlWriter.writeAttribute("LengthMs", QString::number(animation->GetLengthMs()));
        xmlWriter.writeAttribute("StartOpacityPercent", QString::number(animation->GetStartOpacityPercent()));
        xmlWriter.writeAttribute("StopOpacityPercent", QString::number(animation->GetStopOpacityPercent()));
        xmlWriter.writeAttribute("CurveType", QString::number(static_cast<int>(animation->GetCurveType())));
        xmlWriter.writeEndElement(); // OpacityAnimation
    }
    foreach (const AnimationTranslation* animation, m_animationDefinition.TranslationAnimations())
    {
        xmlWriter.writeStartElement("TranslationAnimation");
        writeCommonAttributes(xmlWriter, *animation);
        xmlWriter.writeAttribute("LengthMs", QString::number(animation->GetLengthMs()));
        xmlWriter.writeAttribute("PathType", QString::number(static_cast<int>(animation->GetPathType())));
        xmlWriter.writeAttribute("StartPointX", QString::number(animation->GetStartPoint().x()));
        xmlWriter.writeAttribute("StartPointY", QString::number(animation->GetStartPoint().y()));
        xmlWriter.writeAttribute("StopPointX", QString::number(animation->GetStopPoint().x()));
        xmlWriter.writeAttribute("StopPointY", QString::number(animation->GetStopPoint().y()));
        xmlWriter.writeAttribute("BezierC0X", QString::number(animation->GetBezierC0().x()));
        xmlWriter.writeAttribute("BezierC0Y", QString::number(animation->GetBezierC0().y()));
        xmlWriter.writeAttribute("BezierC1X", QString::number(animation->GetBezierC1().x()));
        xmlWriter.writeAttribute("BezierC1Y", QString::number(animation->GetBezierC1().y()));
        xmlWriter.writeAttribute("EasingCurveType", QString::number(static_cast<int>(animation->GetEasingCurveType())));
        xmlWriter.writeEndElement(); // TranslationAnimation
    }

    xmlWriter.writeEndElement(); // Animations
    xmlWriter.writeEndElement(); //AnimationDefinition

    xmlString.append("\n");
    return xmlString;
}

void AnimationSerializer::writeCommonAttributes(QXmlStreamWriter &xmlWriter, const Animation& animation)
{
    xmlWriter.writeAttribute("ID", QString::number(animation.GetID()));
    xmlWriter.writeAttribute("Name", animation.GetName());
    xmlWriter.writeAttribute("RepeatMode", QString::number(static_cast<uint>(animation.GetRepeatMode())));
    xmlWriter.writeAttribute("DefaultStep", QString::number(animation.GetDefaultStep()));
}
