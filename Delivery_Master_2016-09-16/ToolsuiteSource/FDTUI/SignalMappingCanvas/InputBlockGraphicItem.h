#ifndef INPUTBLOCKGRAPHICITEM_H
#define INPUTBLOCKGRAPHICITEM_H

class GCLSignalSink;
class GCLSignalSource;
class Project;

template <class Key, class T> class QHash;
class QGraphicsItem;
class QString;
class QPainter;
class QRectF;
class QStyleOptionGraphicsItem;
class QWidget;

class InputBlockGraphicItem : public QGraphicsItem
{
public:
    InputBlockGraphicItem(QString componentName, QList<const GCLSignalSource*> sources, qreal x, qreal y);
    ~InputBlockGraphicItem();
    void GetEndpointCoordinates(QHash<const GCLSignalSource*, QPointF>& endpointCoordinates);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QString m_componentName;
    QMap<QString, const GCLSignalSource*> m_sourcesByName;
    QHash<const GCLSignalSource*, QPointF> m_sourceEndpointCoordinates;

    qreal m_height;
    void calculateHeight(void);
    void calculateEndpointCoordinates(void);

    void drawBorder(QPainter* painter);
    void drawComponentName(QPainter* painter);
    void drawSignals(QPainter* painter);
};

#endif // INPUTBLOCKGRAPHICITEM_H
