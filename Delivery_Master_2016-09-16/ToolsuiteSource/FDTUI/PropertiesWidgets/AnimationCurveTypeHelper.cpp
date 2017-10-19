#include "AnimationCurveTypeHelper.h"

#include <QColor>
#include <QEasingCurve>
#include <QIcon>
#include <QMap>
#include <QMetaEnum>
#include <QMetaObject>
#include <QPainter>
#include <QSize>
#include <QString>

QString AnimationCurveTypeHelper::curveTypeStr(QEasingCurve::Type type)
{
    const QMetaObject &mo = QEasingCurve::staticMetaObject;
    QMetaEnum metaEnum = mo.enumerator(mo.indexOfEnumerator("Type"));
    return metaEnum.key(type);
}

QMap<QEasingCurve::Type, QIcon> AnimationCurveTypeHelper::curveTypeIcons(QSize size, int margin, QColor bkg, QColor fg)
{
    QMap<QEasingCurve::Type, QIcon> icons;
    QPixmap pixmap(size);
    QPainter painter(&pixmap);

    for(int i = 0; i < QEasingCurve::NCurveTypes; ++i)
    {
        auto curveType = static_cast<QEasingCurve::Type>(i);
        // Can't render these
        if(curveType == QEasingCurve::TCBSpline  || curveType == QEasingCurve::Custom)
            continue;

        QEasingCurve curve(curveType);

        // Background fill
        painter.fillRect(QRect(QPoint(0, 0), size), bkg);

        const qreal xAxis = size.height()/1.5;
        const qreal yAxis = size.width()/3;

        // Draw axes
        painter.setPen(fg);
        painter.drawLine(margin, xAxis, size.width() - margin,  xAxis);
        painter.drawLine(yAxis, margin, yAxis, size.height() - margin);
        painter.setPen(Qt::NoPen);

        // Draw curve itself
        QPainterPath curvePath;
        qreal curveScale = size.height()/2;
        QPoint start(yAxis, xAxis - curveScale * curve.valueForProgress(0));
        curvePath.moveTo(start);
        for (qreal t=0; t<=1.0; t+=1.0/curveScale)
        {
            QPoint to;
            to.setX(yAxis + curveScale*t);
            to.setY(xAxis - curveScale*curve.valueForProgress(t));
            curvePath.lineTo(to);
        }
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.strokePath(curvePath, fg);
        painter.setRenderHint(QPainter::Antialiasing, false);

        QIcon icon(pixmap);
        icons[curveType] = icon;
    }

    return icons;
}

