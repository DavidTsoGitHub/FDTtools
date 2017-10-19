#include "AnimationDeserializer.h"
#include "Animation.h"
#include "Logger.h"
#include "Project.h"

AnimationDeserializer::AnimationDeserializer(AnimationDefinition &animationDefinition) : DeserializerBase(NULL),
    m_animationDefinition(animationDefinition)
{}

AnimationDeserializer::~AnimationDeserializer()
{}

DeserializerBase::DeserializerRespons AnimationDeserializer::Deserialize(const QString& xml)
{
    LOG_DEBUG("Entering");

    if (xml.isNull() || xml.isEmpty())
    {
        m_error = "XML string is empty!";
        return ErrorFileEmpty;
    }

    QXmlStreamReader xmlReader(xml);

    if (!xmlReader.readNextStartElement() || xmlReader.name() != "AnimationDefinition")
    {
        m_error = "No AnimationDefinition root element found in XML!";
        return ErrorTagNotFound;
    }

    //Project* project = static_cast<Project*>(m_animationDefinition.GetParent());

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "Animations")
            {
                Animation::SetAnimationCount(xmlReader.attributes().value("HighestID").toUInt());
                while (!isEndOfElement(xmlReader, "Animations"))
                {
                    if (xmlReader.name() == "FrameByFrameAnimation" && !xmlReader.isEndElement())
                    {
                        auto& animation = *m_animationDefinition.AddAnimationFrameByFrame(xmlReader.attributes().value("ID").toInt());
                        animation.SetName(xmlReader.attributes().value("Name").toString());
                        animation.SetRepeatMode(static_cast<Animation::RepeatMode>(xmlReader.attributes().value("RepeatMode").toUInt()));
                        animation.SetDefaultStep(xmlReader.attributes().value("DefaultStep").toInt());
                        animation.SetFrameDurationMs(xmlReader.attributes().value("FrameDurationMs").toUInt());

                        while (!isEndOfElement(xmlReader, "FrameByFrameAnimation"))
                        {
                            if(xmlReader.name() == "Frames")
                            {
                                while(!isEndOfElement(xmlReader, "Frames"))
                                {
                                    if (xmlReader.name() == "Frame" && !xmlReader.isEndElement())
                                    {
                                        animation.AddImageId(xmlReader.attributes().value("ImageId").toUInt());
                                    }
                                    xmlReader.readNext();
                                }
                            }
                            xmlReader.readNext();
                        }
                    }
                    else if (xmlReader.name() == "OpacityAnimation" && !xmlReader.isEndElement())
                    {
                        auto& animation = *m_animationDefinition.AddAnimationOpacity(xmlReader.attributes().value("ID").toInt());
                        animation.SetName(xmlReader.attributes().value("Name").toString());
                        animation.SetRepeatMode(static_cast<Animation::RepeatMode>(xmlReader.attributes().value("RepeatMode").toUInt()));
                        animation.SetDefaultStep(xmlReader.attributes().value("DefaultStep").toInt());
                        animation.SetLengthMs(xmlReader.attributes().value("LengthMs").toUInt());
                        animation.SetStartOpacityPercent(xmlReader.attributes().value("StartOpacityPercent").toUInt());
                        animation.SetStopOpacityPercent(xmlReader.attributes().value("StopOpacityPercent").toUInt());
                        animation.SetCurveType(static_cast<QEasingCurve::Type>(xmlReader.attributes().value("CurveType").toInt()));
                    }
                    else if (xmlReader.name() == "TranslationAnimation" && !xmlReader.isEndElement())
                    {
                        auto& animation = *m_animationDefinition.AddAnimationTranslation(xmlReader.attributes().value("ID").toInt());
                        animation.SetName(xmlReader.attributes().value("Name").toString());
                        animation.SetRepeatMode(static_cast<Animation::RepeatMode>(xmlReader.attributes().value("RepeatMode").toUInt()));
                        animation.SetDefaultStep(xmlReader.attributes().value("DefaultStep").toInt());
                        animation.SetLengthMs(xmlReader.attributes().value("LengthMs").toUInt());
                        animation.SetPathType(static_cast<AnimationTranslation::Path>(xmlReader.attributes().value("PathType").toInt()));

                        QPoint startPoint;
                        startPoint.setX(xmlReader.attributes().value("StartPointX").toInt());
                        startPoint.setY(xmlReader.attributes().value("StartPointY").toInt());
                        animation.SetStartPoint(startPoint);

                        QPoint stopPoint;
                        stopPoint.setX(xmlReader.attributes().value("StopPointX").toInt());
                        stopPoint.setY(xmlReader.attributes().value("StopPointY").toInt());
                        animation.SetStopPoint(stopPoint);

                        QPoint bezierC0;
                        bezierC0.setX(xmlReader.attributes().value("BezierC0X").toInt());
                        bezierC0.setY(xmlReader.attributes().value("BezierC0Y").toInt());
                        animation.SetBezierC0(bezierC0);

                        QPoint bezierC1;
                        bezierC1.setX(xmlReader.attributes().value("BezierC1X").toInt());
                        bezierC1.setY(xmlReader.attributes().value("BezierC1Y").toInt());
                        animation.SetBezierC1(bezierC1);

                        animation.SetEasingCurveType(static_cast<QEasingCurve::Type>(xmlReader.attributes().value("EasingCurveType").toInt()));
                    }

                    xmlReader.readNext();
                }
            }
        }

        xmlReader.readNext();
    }

    if (xmlReader.hasError())
    {
        switch (xmlReader.error())
        {
            case QXmlStreamReader::NotWellFormedError:
                m_error = "Animation XML not well formed! Line:" +  QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::PrematureEndOfDocumentError:
                m_error = "Premature end of Animation XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::UnexpectedElementError:
                m_error = "Unexpected element in Animation XML! Line:" + QString::number(xmlReader.lineNumber()) + " Col:" + QString::number(xmlReader.columnNumber());
            break;
            case QXmlStreamReader::CustomError:
                m_error = xmlReader.errorString();
            break;
            case QXmlStreamReader::NoError:
            break;
        }
        return ErrorIncorrectStructure;
    }

    return DeserializationOK;
}
