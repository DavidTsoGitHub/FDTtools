#ifndef SPEEDGAUGE_H
#define SPEEDGAUGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>

class SpeedGauge : public QWidget
{
    Q_OBJECT

public:
    SpeedGauge(quint16 needleZero, quint16 needleMax, QWidget* parent = 0);

public slots:
    void slot_setNeedleValue(int needleValue);

private:
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_graphicsScene;

    QGraphicsPolygonItem* m_needle;
    QGraphicsTextItem* m_numericDisplay;

    quint16 m_needleZero;
    quint16 m_needleMax;
    qreal m_degreesPerKph;
    quint8 m_ticks;
    quint8 m_tickInterval;

    QColor m_kphLabelColor;
    QColor m_numericDisplayColor;
    QColor m_numericDisplayErrorColor;
    QColor m_tickColor;
    QColor m_tickLabelColor;

    void addBackground(void);
    void addNeedleCenter(void);
    void addNeedle(void);
    void addTicks(void);
    void addTickLabels(void);
    void addNumericDisplay(void);
    void addKphLabel(void);
};

#endif // SPEEDGAUGE_H
