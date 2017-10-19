#include "FuelGauge.h"

#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>

#define GAUGE_RADIUS   (130)
#define START_ANGLE     (-60)
#define END_ANGLE       (60)

FuelGauge::FuelGauge(QWidget* parent) : QWidget(parent),
    m_percentLabelColor("white"),
    m_numericDisplayColor("white"),
    m_numericDisplayErrorColor("red"),
    m_tickColor("white")
{
    QGridLayout* layout = new QGridLayout(this);
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsView = new QGraphicsView(m_graphicsScene);
    setWindowFlags(Qt::FramelessWindowHint);
    m_graphicsView->setAlignment(Qt::AlignCenter);
    m_graphicsView->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    m_degreesPerPercent = ((END_ANGLE - START_ANGLE) / (qreal)(100));

    addBackground();
    addNumericDisplay();
    addNeedleCenter();
    addTicks();
    addNeedle();

    setFixedHeight(GAUGE_RADIUS*1.2);
    setFixedWidth(GAUGE_RADIUS*1.2);
    layout->addWidget(m_graphicsView, 0, 0);
    this->setLayout(layout);
}

void FuelGauge::slot_setNeedleValue(int needleValue)
{
    QString displayValue = QString("%1%").arg(QString::number(needleValue));
    m_numericDisplay->setPlainText(displayValue);
    qreal angle = START_ANGLE + (needleValue * m_degreesPerPercent);
    m_needle->setRotation(angle);
}

void FuelGauge::addBackground()
{
    //QGraphicsEllipseItem* item = m_graphicsScene->addEllipse(0, 0, GAUGE_RADIUS, GAUGE_RADIUS, QPen(QColor("black")), QBrush(QColor("black")));
    //item->setStartAngle((180)*16);
    //item->setSpanAngle(-180*16);
    m_graphicsScene->addRect(0, GAUGE_RADIUS/2, GAUGE_RADIUS, 40, QPen(QColor("black")), QBrush(QColor("black")));


    QGraphicsTextItem* text = m_graphicsScene->addText("1/2");
    text->setDefaultTextColor(QColor("white"));
    text->setX((GAUGE_RADIUS/2)-14);
    text->setY(15);

    text = m_graphicsScene->addText("0");
    text->setDefaultTextColor(QColor("white"));
    text->setX(20);
    text->setY(30);

    text = m_graphicsScene->addText("1");
    text->setDefaultTextColor(QColor("white"));
    text->setX(GAUGE_RADIUS-40);
    text->setY(30);
}

void FuelGauge::addNeedle()
{
    QPolygonF needlePolygon;
    qreal x = GAUGE_RADIUS / 2;
    qreal y = GAUGE_RADIUS / 2;

    x += 1;
    y -= (GAUGE_RADIUS*0.4);
    needlePolygon << QPointF(x, y);
    x -= 2;
    needlePolygon << QPointF(x, y);
    x -= 2;
    y += (GAUGE_RADIUS/2);
    needlePolygon << QPointF(x, y);
    x += 6;
    needlePolygon << QPointF(x, y);

    m_needle = m_graphicsScene->addPolygon(needlePolygon, QPen(QColor("darkred")), QBrush(QColor("red")));
    m_needle->setTransformOriginPoint(GAUGE_RADIUS/2, GAUGE_RADIUS/2);

    m_needle->setRotation(START_ANGLE);
}

void FuelGauge::addNeedleCenter()
{
    m_graphicsScene->addEllipse((GAUGE_RADIUS/2)-5, (GAUGE_RADIUS/2)-5, 10, 10, QPen(QColor("black")), QBrush("black"));
}

void FuelGauge::addTicks()
{
    qreal currentAngle = START_ANGLE;

    for (quint32 i=0; i<=8; ++i)
    {
        QGraphicsRectItem* tick;
        if (i%4==0)
        {
            tick = m_graphicsScene->addRect(GAUGE_RADIUS/2-1, 10, 2, 10, m_tickColor, m_tickColor);
        }
        else
        {
            tick = m_graphicsScene->addRect(GAUGE_RADIUS/2, 10, 1, 6, m_tickColor, m_tickColor);
        }
        tick->setTransformOriginPoint(GAUGE_RADIUS/2, GAUGE_RADIUS/2);
        tick->setRotation(currentAngle);
        currentAngle += m_degreesPerPercent*12.5;
    }
}

void FuelGauge::addNumericDisplay()
{
    m_numericDisplay = m_graphicsScene->addText("0%");
    m_numericDisplay->setDefaultTextColor(m_numericDisplayColor);
    m_numericDisplay->setX(GAUGE_RADIUS/2 - 10);
    m_numericDisplay->setY(GAUGE_RADIUS*0.6);
}

