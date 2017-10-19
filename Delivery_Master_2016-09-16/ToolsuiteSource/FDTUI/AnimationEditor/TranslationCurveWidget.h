#ifndef TRANSLATIONCURVEWIDGET_H
#define TRANSLATIONCURVEWIDGET_H

#include "CurveWidget.h"

class AnimationTranslation;
class QTime;
class QTimer;

class TranslationCurveWidget : public CurveWidget
{
public:
    TranslationCurveWidget(AnimationTranslation *anim, QWidget *parent = 0);

    virtual void itemMoving(int id);
    virtual void itemMoved(int id);

    void Play();

private:
    static const int BEZIER = 1;
    static const int LINE_SEGMENT = 2;
    static const int C0_MARKER = 3;
    static const int C1_MARKER = 4;
    static const int P0_MARKER = 5;
    static const int PLAY_MARKER = 6;

    AnimationTranslation *m_animation;
    const qreal m_scale;
    QTimer *m_playTimer;
    QTime m_playStart;

    void setFromAnimation();

};

#endif // TRANSLATIONCURVEWIDGET_H
