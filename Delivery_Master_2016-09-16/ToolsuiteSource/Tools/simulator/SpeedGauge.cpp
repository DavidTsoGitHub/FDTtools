#include "SpeedGauge.h"
#include "Exceptions/SimulatorGeneralException.h"

#include <QtDebug>
#include <QGridLayout>
#include <QLabel>
#include <QtMath>

#define GAUGE_RADIUS   (200)
#define START_ANGLE     (-135)
#define END_ANGLE       (135)

SpeedGauge::SpeedGauge(quint16 needleZero, quint16 needleMax, QWidget* parent) : QWidget(parent),
    m_needleZero(needleZero),
    m_needleMax(needleMax),
    m_kphLabelColor("white"),
    m_numericDisplayColor("white"),
    m_numericDisplayErrorColor("red"),
    m_tickColor("white"),
    m_tickLabelColor("white")
{
    if (needleZero >= needleMax)
    {
        throw SimulatorGeneralException("Needle Max is not greater then needle zero. The simulator will close.", true);
    }

    QGridLayout* layout = new QGridLayout(this);
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsView = new QGraphicsView(m_graphicsScene);
    setWindowFlags(Qt::FramelessWindowHint);
    m_graphicsView->setAlignment(Qt::AlignCenter);
    m_graphicsView->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    m_degreesPerKph = ((END_ANGLE - START_ANGLE) / (qreal)(m_needleMax - m_needleZero));
    m_tickInterval = 10;
    m_ticks = (m_needleMax - m_needleZero) / m_tickInterval;

    addBackground();
    addKphLabel();
    addNumericDisplay();
    addNeedleCenter();
    addTicks();
    addTickLabels();
    addNeedle();

    layout->addWidget(m_graphicsView, 0, 0);
    this->setLayout(layout);
}

void SpeedGauge::slot_setNeedleValue(int needleValue)
{
    if (needleValue < m_needleZero || needleValue > m_needleMax)
    {
        m_numericDisplay->setDefaultTextColor(m_numericDisplayErrorColor);
    }
    else
    {
        m_numericDisplay->setDefaultTextColor(m_numericDisplayColor);
    }
    QString displayValue = QString("%1").arg(QString::number(needleValue), 3, '0');
    m_numericDisplay->setPlainText(displayValue);
    qreal angle = START_ANGLE + (needleValue * m_degreesPerKph);
    m_needle->setRotation(angle);
}

void SpeedGauge::addBackground()
{
    m_graphicsScene->addEllipse(0, 0, GAUGE_RADIUS, GAUGE_RADIUS, QPen(QColor("black")), QBrush(QColor("black")));
}

void SpeedGauge::addNeedle()
{
    QPolygonF needlePolygon;
    qreal x = GAUGE_RADIUS / 2;
    qreal y = GAUGE_RADIUS / 2;

    x += 1;
    y -= 80;
    needlePolygon << QPointF(x, y);
    x -= 2;
    needlePolygon << QPointF(x, y);
    x -= 2;
    y += 100;
    needlePolygon << QPointF(x, y);
    x += 6;
    needlePolygon << QPointF(x, y);

    m_needle = m_graphicsScene->addPolygon(needlePolygon, QPen(QColor("darkred")), QBrush(QColor("red")));
    m_needle->setTransformOriginPoint(GAUGE_RADIUS/2, GAUGE_RADIUS/2);

    m_needle->setRotation(START_ANGLE);
}

void SpeedGauge::addNeedleCenter()
{
    m_graphicsScene->addEllipse(95, 95, 10, 10, QPen(QColor("darkgrey")), QBrush("darkgrey"));
}

void SpeedGauge::addTicks()
{
    qreal currentAngle = START_ANGLE;

    for (quint32 i=0; i<=(m_ticks*2); ++i)
    {
        if (i%2 == 0)
        {
            QGraphicsRectItem* tick = m_graphicsScene->addRect(99, 20, 2, 10, m_tickColor, m_tickColor);
            tick->setTransformOriginPoint(GAUGE_RADIUS/2, GAUGE_RADIUS/2);
            tick->setRotation(currentAngle);
        }
        else
        {
            QGraphicsRectItem* tick = m_graphicsScene->addRect(99, 20, 2, 6, m_tickColor, m_tickColor);
            tick->setTransformOriginPoint(GAUGE_RADIUS/2, GAUGE_RADIUS/2);
            tick->setRotation(currentAngle);
        }

        currentAngle += m_degreesPerKph*m_tickInterval/2;
    }
}

void SpeedGauge::addTickLabels()
{
    qreal currentAngle = START_ANGLE;
    qreal x,y;
    quint8 distanceToCenter = 55;
    QFont bigLabelFont("Arial", 7);

    for (quint32 i=0;i<=m_ticks; ++i)
    {
        if (i%2 == 0)
        {
            x = (GAUGE_RADIUS/2) - 12 + (qSin(qDegreesToRadians(currentAngle)) * distanceToCenter);
            y = (GAUGE_RADIUS/2) - 10 + (qCos(qDegreesToRadians(currentAngle)) * distanceToCenter * -1);

            QGraphicsTextItem* label = m_graphicsScene->addText(QString::number(i*10));
            label->setFont(bigLabelFont);
            label->setDefaultTextColor(m_tickLabelColor);
            label->setX(x);
            label->setY(y);
        }

        currentAngle += m_degreesPerKph*m_tickInterval;
    }
}

void SpeedGauge::addNumericDisplay()
{
    m_numericDisplay = m_graphicsScene->addText("000");
    m_numericDisplay->setDefaultTextColor(m_numericDisplayColor);
    m_numericDisplay->setX(GAUGE_RADIUS/2 - 14);
    m_numericDisplay->setY(GAUGE_RADIUS*0.7);
}


void SpeedGauge::addKphLabel()
{
    QFont kphLabelFont("Arial", 8);
    QString text("km/h");
    QFontMetrics fm(kphLabelFont);
    QGraphicsTextItem* kphLabel = m_graphicsScene->addText(text, kphLabelFont);
    kphLabel->setDefaultTextColor(m_kphLabelColor);
    kphLabel->setX((GAUGE_RADIUS/2) - 17);
    kphLabel->setY(GAUGE_RADIUS*0.8);
}
