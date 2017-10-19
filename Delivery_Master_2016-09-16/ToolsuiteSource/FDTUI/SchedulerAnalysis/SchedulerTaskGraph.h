#ifndef SCHEDULERTASKGRAPH_H
#define SCHEDULERTASKGRAPH_H

class SchedulerTaskModel;

class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsScene;
class QGraphicsView;
template <typename T> class QList;
template <class Key, class T> class QMap;
class QModelIndex;
class QMouseEvent;
template <class Key, class T> class QMultiMap;
class QTimer;
class QString;

class SchedulerTaskGraph :public QGraphicsView
{
    Q_OBJECT
public:
    SchedulerTaskGraph();

    void setModel(SchedulerTaskModel* model);
    const QList<QString> GetRunnablesAtTimeSlot(int timeslot);
    void HighlightTimeslot(int timeslot);
    void SetTimeslotCount(int timeslotCount);

signals:
    void slotClicked(int timeSlot);

private slots:
    void slot_modelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    SchedulerTaskModel* m_model;
    QMap<int, int> m_graphicItemCountByTimeslot;
    QMultiMap<int, QString> m_runnablesByTimeslot;
    QList<QGraphicsItem*> m_taskGraphicsItems;
    QGraphicsScene m_graphicsScene;
    QGraphicsRectItem* m_highLight;
    QTimer* m_redrawTimer;

    int m_timeslotMaxValue;

    qreal m_scale;
    bool m_panning;
    int m_panningStartX;
    int m_panningStartY;

    void reset();

    void drawTasks();
    void drawTask(int taskIndex);
    void drawAxes();
    void clearTasks();
    void redraw();
    void requestRedraw();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
};

#endif // SCHEDULERTASKGRAPH_H
