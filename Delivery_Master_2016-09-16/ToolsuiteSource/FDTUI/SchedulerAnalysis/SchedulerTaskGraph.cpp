#include "SchedulerTaskGraph.h"

#include "SchedulerAnalysisConstants.h"

#include "SchedulerTaskModel.h"

#include <QFont>
#include <QFontMetrics>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <QMap>
#include <QModelIndex>
#include <QMouseEvent>
#include <QMultiMap>
#include <QTimer>
#include <QScrollBar>
#include <QString>

static const int PixelsPerSlot = 4;
static const int SlotTickInterval = 20;

SchedulerTaskGraph::SchedulerTaskGraph() :
    m_timeslotMaxValue(InitialSlotCount),
    m_scale(1.0),
    m_panning(false)
{
    setScene(&m_graphicsScene);
    setAlignment(Qt::AlignLeft);
    scale(m_scale, m_scale);
    setTransformationAnchor(AnchorUnderMouse);
    m_redrawTimer = new QTimer(this);
    m_redrawTimer->setSingleShot(true);

    connect(m_redrawTimer, &QTimer::timeout, this, &SchedulerTaskGraph::redraw);

}

void SchedulerTaskGraph::setModel(SchedulerTaskModel* model)
{
    m_model = model;

    m_graphicsScene.clear();
    m_graphicItemCountByTimeslot.clear();
    m_runnablesByTimeslot.clear();

    drawAxes();
    drawTasks();

    m_highLight = m_graphicsScene.addRect(0,-190,3,200, QPen(QColor(0xff, 0x00, 0x00, 0x55)), QBrush(QColor(0xff, 0x00, 0x00, 0x55)));

    ensureVisible(0, 0, 10, 10);
    connect(m_model, &SchedulerTaskModel::dataChanged, this, &SchedulerTaskGraph::slot_modelDataChanged);
    connect(m_model, &SchedulerTaskModel::modelReset, this, &SchedulerTaskGraph::redraw);

    connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &SchedulerTaskGraph::requestRedraw);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &SchedulerTaskGraph::requestRedraw);
}

const QList<QString> SchedulerTaskGraph::GetRunnablesAtTimeSlot(int timeslot)
{
    QList<QString> reversedList;

    QListIterator<QString> reverse(m_runnablesByTimeslot.values(timeslot));
    reverse.toBack();

    while(reverse.hasPrevious())
    {
        reversedList.append(reverse.previous());
    }

    return reversedList;
}

void SchedulerTaskGraph::HighlightTimeslot(int timeslot)
{
    m_highLight->setX(timeslot*PixelsPerSlot);
}

void SchedulerTaskGraph::SetTimeslotCount(int timeslotCount)
{
    m_timeslotMaxValue = timeslotCount;
    requestRedraw();
}

void SchedulerTaskGraph::requestRedraw()
{
    m_redrawTimer->start(200);
}

void SchedulerTaskGraph::slot_modelDataChanged(const QModelIndex& /*topLeft*/, const QModelIndex& /*bottomRight*/)
{
    redraw();
}

void SchedulerTaskGraph::redraw()
{
    m_graphicsScene.removeItem(m_highLight);
    clearTasks();
    m_graphicsScene.clear();
    drawAxes();
    drawTasks();
    m_graphicsScene.addItem(m_highLight);
}

void SchedulerTaskGraph::drawAxes()
{
    // Y-axis
    m_graphicsScene.addLine(0, 0, 0, -200);
    m_graphicsScene.addLine(0, -200, -5, -195);
    m_graphicsScene.addLine(0, -201, 5, -196);
    QGraphicsTextItem* yAxisUnitLabel = m_graphicsScene.addText("(Task count)");
    yAxisUnitLabel->setX(-25);
    yAxisUnitLabel->setY(-225);

    QPen dottedPen(QBrush(QColor(0x11, 0x11, 0x11)), 1, Qt::DotLine);
    // Y-axis labels
    for (int i=0; i<=15; ++i)
    {
        QGraphicsTextItem* text = m_graphicsScene.addText(QString("%1").arg(i+1));
        text->setX(-25);
        text->setY(-23-(i*11));
        m_graphicsScene.addLine(-5,-11-(i*11), m_timeslotMaxValue*PixelsPerSlot, -11-(i*11), dottedPen);
    }

    // X-axis
    int xAxisEndpoint = (m_timeslotMaxValue*PixelsPerSlot)+20;
    m_graphicsScene.addLine(0, 0, xAxisEndpoint, 0);
    m_graphicsScene.addLine(xAxisEndpoint, 0, xAxisEndpoint-5, -5);
    m_graphicsScene.addLine(xAxisEndpoint, 1, xAxisEndpoint-5, 6);
    QGraphicsTextItem* xAxisUnitLabel = m_graphicsScene.addText("(Time slot)");
    xAxisUnitLabel->setX(xAxisEndpoint+5);
    xAxisUnitLabel->setY(0);

    // X-axis labels
    for (int i=0; i<=(m_timeslotMaxValue/SlotTickInterval); ++i)
    {
        int xCoordinate = (i*SlotTickInterval*PixelsPerSlot) + (PixelsPerSlot/2);
        m_graphicsScene.addLine(xCoordinate, 0, xCoordinate, 5);
        QString label = QString::number(i*SlotTickInterval);
        int labelWidth = fontMetrics().boundingRect(label).width();
        QGraphicsTextItem* text = m_graphicsScene.addText(label);
        text->setY(5);
        text->setX(xCoordinate-(labelWidth/2));
    }
}

void SchedulerTaskGraph::drawTasks()
{
    for (int i=0; i<m_model->rowCount(QModelIndex()); ++i)
    {
        drawTask(i);
    }
}

void SchedulerTaskGraph::drawTask(int taskIndex)
{
    QString runnable = m_model->data(m_model->index(taskIndex, SchedulerTaskModel::RunnableFunctionColumn), Qt::DisplayRole).toString();
    int initialDelay = m_model->data(m_model->index(taskIndex, SchedulerTaskModel::StartupDelayColumn), Qt::DisplayRole).toInt();
    int periodicity = m_model->data(m_model->index(taskIndex, SchedulerTaskModel::PeriodicityColumn), Qt::DisplayRole).toInt();

    for (int ms=initialDelay; ms<=(m_timeslotMaxValue*MillisecondsPerSlot); ms+=periodicity)
    {
        int slotIndex = ms/MillisecondsPerSlot;
        int graphicsX = slotIndex*PixelsPerSlot;
        int existingItems = m_graphicItemCountByTimeslot.value(slotIndex, 0);
        int graphicsY = -11-(existingItems*11);
        QGraphicsRectItem* rect = m_graphicsScene.addRect(graphicsX, graphicsY, PixelsPerSlot-1, 10);
        rect->setToolTip(QString("%1 @ %2ms").arg(runnable).arg(ms));

        m_taskGraphicsItems.append(rect);
        m_graphicItemCountByTimeslot[slotIndex] = existingItems + 1;
        m_runnablesByTimeslot.insert(slotIndex, runnable);
    }
}

void SchedulerTaskGraph::clearTasks()
{
    m_runnablesByTimeslot.clear();
    m_graphicItemCountByTimeslot.clear();
}


void SchedulerTaskGraph::mousePressEvent(QMouseEvent* event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
        {
            event->accept();
            QPointF position = mapToScene(event->pos());
            if (position.x() < 0 || position.x() > m_timeslotMaxValue*PixelsPerSlot) return;
            emit slotClicked(position.toPoint().x()/PixelsPerSlot);
            m_redrawTimer->start(100);
            break;
        }
        case Qt::RightButton:
        {
            m_panning = true;
            m_panningStartX = event->x();
            m_panningStartY = event->y();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
            break;
        }
        default:
            event->ignore();
            break;
    }
}

void SchedulerTaskGraph::mouseMoveEvent(QMouseEvent* event)
{
    if (m_panning)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_panningStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_panningStartY));
        m_panningStartX = event->x();
        m_panningStartY = event->y();
        m_redrawTimer->start(100);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SchedulerTaskGraph::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void SchedulerTaskGraph::wheelEvent(QWheelEvent* event)
{
    event->accept();

    static const qreal maxScale = 1.5;
    static const qreal minScale = 0.5;
    if (event->delta() > 0)
    {
        if (m_scale < maxScale)
        {
            m_scale+=0.1;
            scale(1.25, 1.25);
        }
    }
    else
    {
        if (m_scale > minScale)
        {
            m_scale-=0.1;
            scale(0.8, 0.8);
        }
    }

}
