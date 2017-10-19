#ifndef ANIMATIONCURVETYPEHELPER_H
#define ANIMATIONCURVETYPEHELPER_H

class QColor;
class QEasingCurve;
class QIcon;
template <class Key, class T> class QMap;
class QSize;
class QString;

class AnimationCurveTypeHelper
{
public:
    static QString curveTypeStr(QEasingCurve::Type type);
    static QMap<QEasingCurve::Type, QIcon> curveTypeIcons(QSize size, int margin, QColor bkg, QColor fg);
};

#endif // ANIMATIONCURVETYPEHELPER_H
