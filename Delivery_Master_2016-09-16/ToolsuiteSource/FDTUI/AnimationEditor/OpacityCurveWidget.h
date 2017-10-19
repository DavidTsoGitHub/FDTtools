#ifndef OPACITYCURVEWIDGET_H
#define OPACITYCURVEWIDGET_H

#include "CurveWidget.h"

class AnimationOpacity;

class OpacityCurveWidget : public CurveWidget
{
    Q_OBJECT
public:
    OpacityCurveWidget(AnimationOpacity *anim, QWidget *parent = 0);

    virtual void itemMoving(int id);
    virtual void itemMoved(int id);

private:
    static const int START_OPACITY_MARKER = 1;
    static const int STOP_OPACITY_MARKER = 2;
    static const int CURVE_SEGMENT = 3;
    static const int XAXIS = 4;
    static const int YAXIS = 5;
    static const int XAXIS_LABEL = 6;
    static const int YAXIS_LABEL = 7;

    AnimationOpacity *m_animation;

    const qreal m_xScale;
    const qreal m_yScale;

    void setCurveFromAnimation();

};

#endif // OPACITYCURVEWIDGET_H
