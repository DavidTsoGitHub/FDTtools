#ifndef OUTPUTBLOCKGRAPHICITEM_H
#define OUTPUTBLOCKGRAPHICITEM_H

class GCLSignalSink;

class QGraphicsItem;
template <class Key, class T> class QHash;
template <typename T> class QList;
class QString;
class QPainter;
class QRectF;
class QStyleOptionGraphicsItem;
class QWidget;

class OutputBlockGraphicItem : public QGraphicsItem
{
public:
    OutputBlockGraphicItem(QString componentName, QList<const GCLSignalSink*> sinks, qreal x, qreal y);
    ~OutputBlockGraphicItem(void);
    void GetEndpointCoordinates(QHash<const GCLSignalSink*, QPointF>& endpointCoordinates);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QString m_componentName;
    QMap<QString, const GCLSignalSink*> m_sinksByName;
    QHash<const GCLSignalSink*, QPointF> m_sinkEndpointCoordinates;

    qreal m_height;
    void calculateHeight(void);
    void calculateEndpointCoordinates(void);

    void drawBorder(QPainter* painter);
    void drawComponentName(QPainter* painter);
    void drawSignals(QPainter* painter);
};

#endif // OUTPUTBLOCKGRAPHICITEM_H
