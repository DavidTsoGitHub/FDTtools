#include "TranslationCurveWidget.h"

#include "AnimationTranslation.h"
#include "Logger.h"

#include <QTime>
#include <QTimer>

TranslationCurveWidget::TranslationCurveWidget(AnimationTranslation *anim, QWidget *parent)
    : CurveWidget(parent),
      m_animation(anim),
      m_scale(2.5)
{
    // Grid
    AddGrid(QRectF(-500 * m_scale, -500 * m_scale, 1000 * m_scale, 1000 * m_scale), 10 * m_scale);

    AddBezier(BEZIER);
    AddLine(LINE_SEGMENT);
    AddControlMarker(QPointF() , C0_MARKER);
    AddControlMarker(QPointF() , C1_MARKER);
    AddControlMarker(QPointF() , P0_MARKER);

    // Fix end point at center
    SetBezierP1({0,0}, BEZIER);
    SetLineP1({0, 0}, LINE_SEGMENT);
    AddCross({0,0}, 10);

    // Play animation preview
    m_playTimer = new QTimer(this);
    m_playTimer->setTimerType(Qt::PreciseTimer);
    connect(m_playTimer, &QTimer::timeout, [=]() {
        const quint32 showAfterStopMs = 100;
        if(static_cast<quint32>(m_playStart.elapsed()) >= m_animation->GetLengthMs() + showAfterStopMs)
        {
            SetVisible(false, PLAY_MARKER);
            m_playTimer->stop();
            return;
        }
        SetControlMarkerPos(m_animation->GetRelativePosAtTime(m_playStart.elapsed())* m_scale, PLAY_MARKER);
        SetVisible(true, PLAY_MARKER);
    });

    AddControlMarker(QPointF(), Qt::darkRed, Qt::red, PLAY_MARKER);
    SetVisible(false, PLAY_MARKER);

    // Update widget
    connect(m_animation, &ProjectObject::PropertyUpdated, this, &TranslationCurveWidget::setFromAnimation);
    setFromAnimation();
}

void TranslationCurveWidget::itemMoving(int id)
{
    if(id == C0_MARKER)
    {
        SetBezierC0(GetControlMarkerPos(C0_MARKER), BEZIER);
    }
    else if(id == C1_MARKER)
    {
        SetBezierC1(GetControlMarkerPos(C1_MARKER), BEZIER);
    }
    else if(id == P0_MARKER)
    {
        SetBezierP0(GetControlMarkerPos(P0_MARKER), BEZIER);
        SetLineP0(GetControlMarkerPos(P0_MARKER), LINE_SEGMENT);
    }
}

void TranslationCurveWidget::itemMoved(int id)
{
    if(id == P0_MARKER)
        m_animation->SetStartPoint(QPointF(GetControlMarkerPos(P0_MARKER) / m_scale).toPoint());
    else if(id == C0_MARKER)
        m_animation->SetBezierC0(QPointF(GetControlMarkerPos(C0_MARKER) / m_scale).toPoint());
    else if(id == C1_MARKER)
        m_animation->SetBezierC1(QPointF(GetControlMarkerPos(C1_MARKER) / m_scale).toPoint());
}

void TranslationCurveWidget::Play()
{
    m_playStart.start();
    m_playTimer->setInterval(20);
    m_playTimer->setSingleShot(false);
    m_playTimer->start();
}

void TranslationCurveWidget::setFromAnimation()
{
    bool showBezier = (m_animation->GetPathType() == AnimationTranslation::Path::Bezier);
    SetVisible(showBezier, BEZIER);
    SetVisible(showBezier, C0_MARKER);
    SetVisible(showBezier, C1_MARKER);
    SetVisible(!showBezier, LINE_SEGMENT);
    SetControlMarkerPos(m_animation->GetStartPoint() * m_scale, P0_MARKER);
    SetControlMarkerPos(m_animation->GetBezierC0() * m_scale, C0_MARKER);
    SetControlMarkerPos(m_animation->GetBezierC1() * m_scale, C1_MARKER);
}
