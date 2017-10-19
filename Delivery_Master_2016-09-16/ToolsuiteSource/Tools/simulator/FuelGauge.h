#ifndef FUELGAUGE_H
#define FUELGAUGE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsPolygonItem>

class FuelGauge : public QWidget
{
    Q_OBJECT

public:
    explicit FuelGauge(QWidget* parent = 0);

public slots:
    void slot_setNeedleValue(int needleValue);

private:
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_graphicsScene;

    QGraphicsPolygonItem* m_needle;
    QGraphicsTextItem* m_numericDisplay;

    qreal m_degreesPerPercent;

    QColor m_percentLabelColor;
    QColor m_numericDisplayColor;
    QColor m_numericDisplayErrorColor;
    QColor m_tickColor;

    void addBackground(void);
    void addNeedleCenter(void);
    void addNeedle(void);
    void addTicks(void);
    void addNumericDisplay(void);
};

#endif // SPEEDGAUGE_H
