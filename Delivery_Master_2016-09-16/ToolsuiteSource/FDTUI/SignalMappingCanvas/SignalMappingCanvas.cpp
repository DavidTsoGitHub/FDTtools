#include "SignalMappingCanvas.h"
#include "ui_SignalMappingCanvas.h"

#include "FunctionBlock.h"
#include "CenterBlockGraphicItem.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "InputBlockGraphicItem.h"
#include "OutputBlockGraphicItem.h"
#include "Project.h"
#include "ProjectObject.h"
#include "SignalMappingCanvasConstants.h"

#include <QFontMetrics>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHash>
#include <QList>
#include <QtMath>
#include <QWidget>

SignalMappingCanvas::SignalMappingCanvas(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::SignalMappingCanvas),
    m_selectedItem(NULL)
{
    m_ui->setupUi(this);

    m_fontMetrics = new QFontMetrics(QApplication::font());

    m_graphicsScene = new QGraphicsScene();
    m_graphicsView = new QGraphicsView(m_graphicsScene);
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    m_graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_ui->layout->addWidget(m_graphicsView);
}

SignalMappingCanvas::~SignalMappingCanvas()
{
    delete m_ui;
    delete m_graphicsScene;
    delete m_graphicsView;
    delete m_fontMetrics;
}

void SignalMappingCanvas::ProjectLoaded(Project* /*project*/)
{
    m_graphicsScene->clear();
    m_graphicsScene->addText("Select a component in the project browser to visualize signals.");
    m_selectedItem = NULL;
}

void SignalMappingCanvas::ProjectUnloaded()
{
    m_graphicsScene->clear();
    m_graphicsScene->addText("Select a component in the project browser to visualize signals.");
    m_selectedItem = NULL;
}

void SignalMappingCanvas::JustSelected()
{
    if (m_selectedItem != NULL)
    {
        emit signal_selectedProjectObject(static_cast<ProjectObject*>(m_selectedItem));
    }
}

void SignalMappingCanvas::SetCurrentProjectObject(ProjectObject* object)
{
    disconnect(m_selectedItem, &ProjectObject::PropertyUpdated, this, &SignalMappingCanvas::redraw);
    m_selectedItem = object;
    connect(m_selectedItem, &ProjectObject::PropertyUpdated, this, &SignalMappingCanvas::redraw);
    redraw();
}

void SignalMappingCanvas::redraw()
{
    if (m_selectedItem == NULL) return;

    m_graphicsScene->clear();
    m_inputBlocks.clear();
    m_outputBlocks.clear();
    m_centerBlock = NULL;

    if (dynamic_cast<IGCLSignalSink*>(m_selectedItem) == NULL &&
        dynamic_cast<IGCLSignalSource*>(m_selectedItem) == NULL) return;

    drawInputBlocks();
    drawOutputBlocks();
    drawCenterBlock();
    drawConnectors();
}

void SignalMappingCanvas::drawInputBlocks()
{
    IGCLSignalSink* sinks = dynamic_cast<IGCLSignalSink*>(m_selectedItem);
    if (sinks == NULL) return;

    // INPUTS
    QMultiHash<QString, const GCLSignalSource*> inputSourcesByComponentName;
    foreach (const GCLSignalSink* sink, sinks->GetSignalSinks())
    {
        const GCLSignalSource* source = sink->GetSignalSource();
        if (source == NULL) continue;

        inputSourcesByComponentName.insert(source->ComponentName(), source);
    }

    qreal inputBlocksY = CANVAS_PADDING;
    qreal inputBlocksX = CANVAS_PADDING;
    if (inputSourcesByComponentName.uniqueKeys().contains("HAL"))
    {
        QList<const GCLSignalSource*> sources = inputSourcesByComponentName.values("HAL");

        InputBlockGraphicItem* inputBlock = new InputBlockGraphicItem("HAL", sources, inputBlocksX, inputBlocksY);
        m_graphicsScene->addItem(inputBlock);
        m_inputBlocks.append(inputBlock);
        inputBlocksY += IOBLOCK_TO_IOBLOCK_VERTICAL_GAP + inputBlock->boundingRect().height();
    }
    if (inputSourcesByComponentName.uniqueKeys().contains("COM"))
    {
        QList<const GCLSignalSource*> sources = inputSourcesByComponentName.values("COM");
        InputBlockGraphicItem* inputBlock = new InputBlockGraphicItem("COM", sources, inputBlocksX, inputBlocksY);
        m_graphicsScene->addItem(inputBlock);
        m_inputBlocks.append(inputBlock);
        inputBlocksY += IOBLOCK_TO_IOBLOCK_VERTICAL_GAP + inputBlock->boundingRect().height();
    }

    foreach (QString key, inputSourcesByComponentName.uniqueKeys())
    {
        if (key == "HAL" || key == "COM") continue;
        QList<const GCLSignalSource*> sources = inputSourcesByComponentName.values(key);
        InputBlockGraphicItem* inputBlock = new InputBlockGraphicItem(key, sources, inputBlocksX, inputBlocksY);
        m_graphicsScene->addItem(inputBlock);
        m_inputBlocks.append(inputBlock);
        inputBlocksY += IOBLOCK_TO_IOBLOCK_VERTICAL_GAP + inputBlock->boundingRect().height();
    }
}

void SignalMappingCanvas::drawOutputBlocks()
{
    IGCLSignalSource* sources = dynamic_cast<IGCLSignalSource*>(m_selectedItem);

    if (sources == NULL) return;

    // Get all mapped output components
    QMultiHash<QString, const GCLSignalSink*> outputSinksByComponentName;
    foreach (const GCLSignalSource* source, sources->GetSignalSources())
    {
        if (source->GetConnectedSinks().count() == 0) continue;
        foreach (const GCLSignalSink* sink, source->GetConnectedSinks())
        {
            outputSinksByComponentName.insert(sink->ComponentName(), sink);
        }
    }

    // Draw output components
    qreal outputBlocksY = CANVAS_PADDING;
    qreal outputBlocksX = CANVAS_PADDING + IOBLOCK_WIDTH + IOBLOCK_TO_CENTERBLOCK_GAP + CENTERBLOCK_WIDTH + IOBLOCK_TO_CENTERBLOCK_GAP;
    if (outputSinksByComponentName.uniqueKeys().contains("HAL"))
    {
        QList<const GCLSignalSink*> sinks = outputSinksByComponentName.values("HAL");
        OutputBlockGraphicItem* outputBlock = new OutputBlockGraphicItem("HAL", sinks, outputBlocksX, outputBlocksY);
        m_graphicsScene->addItem(outputBlock);
        m_outputBlocks.append(outputBlock);
        outputBlocksY += 50 + outputBlock->boundingRect().height();
    }
    if (outputSinksByComponentName.uniqueKeys().contains("COM"))
    {
        QList<const GCLSignalSink*> sinks = outputSinksByComponentName.values("COM");
        OutputBlockGraphicItem* outputBlock = new OutputBlockGraphicItem("COM", sinks, outputBlocksX, outputBlocksY);
        m_graphicsScene->addItem(outputBlock);
        m_outputBlocks.append(outputBlock);
        outputBlocksY += 50 + outputBlock->boundingRect().height();
    }

    foreach (QString key, outputSinksByComponentName.uniqueKeys())
    {
        if (key == "HAL" || key == "COM") continue;
        QList<const GCLSignalSink*> sinks = outputSinksByComponentName.values(key);
        OutputBlockGraphicItem* outputBlock = new OutputBlockGraphicItem(key, sinks, outputBlocksX, outputBlocksY);
        m_graphicsScene->addItem(outputBlock);
        m_outputBlocks.append(outputBlock);
        outputBlocksY += 50 + outputBlock->boundingRect().height();
    }
}

void SignalMappingCanvas::drawCenterBlock()
{
    FunctionBlock* block = static_cast<FunctionBlock*>(m_selectedItem);
    qreal x, y;

    x = CANVAS_PADDING + IOBLOCK_WIDTH + IOBLOCK_TO_CENTERBLOCK_GAP;
    y = CANVAS_PADDING;

    CenterBlockGraphicItem* functionBlockGraphic = new CenterBlockGraphicItem(block, x, y);
    m_graphicsScene->addItem(functionBlockGraphic);
    m_centerBlock = functionBlockGraphic;
}

void SignalMappingCanvas::drawConnectors()
{
    // Get all endpoints
    m_sinkEndpoints.clear();
    m_sourceEndpoints.clear();

    foreach (InputBlockGraphicItem* inputBlock, m_inputBlocks)
    {
        inputBlock->GetEndpointCoordinates(m_sourceEndpoints);
    }

    foreach (OutputBlockGraphicItem* outputBlock, m_outputBlocks)
    {
        outputBlock->GetEndpointCoordinates(m_sinkEndpoints);
    }

    m_centerBlock->GetSinkEndpointCoordinates(m_sinkEndpoints);
    m_centerBlock->GetSourceEndpointCoordinates(m_sourceEndpoints);

    foreach (const GCLSignalSink* sink, m_centerBlock->GetSignalSinks())
    {
        if (sink->GetSignalSource() == NULL) continue;
        drawArrow(sink->GetSignalSource(), sink);
    }

    foreach (const GCLSignalSource* source, m_centerBlock->GetSignalSources())
    {
        if (source->GetConnectedSinks().count() == 0) continue;
        foreach (const GCLSignalSink* sink, source->GetConnectedSinks())
        {
            drawArrow(source, sink);
        }
    }
}

void SignalMappingCanvas::drawArrow(const GCLSignalSource* from, const GCLSignalSink* to)
{
    QPointF sourceCoords = m_sourceEndpoints.value(from);
    QPointF sinkCoords = m_sinkEndpoints.value(to);

    qreal x1 = sourceCoords.x();
    qreal y1 = sourceCoords.y();
    qreal x2 = sinkCoords.x();
    qreal y2 = sinkCoords.y();
    qreal deltaX = x2-x1;
    qreal deltaY = y2-y1;
    qreal angle = qRadiansToDegrees(qAtan(deltaY/deltaX));

    m_graphicsScene->addLine(x1, y1, x2, y2); // Arrow shaft
    m_graphicsScene->addRect(x1-2, y1-2, 4, 4, QPen(), QBrush(Qt::SolidPattern)); // Source connector

    // Arrow
    QPolygonF arrowPolygon;
    arrowPolygon.append(QPointF(x2, y2));
    arrowPolygon.append(QPointF(x2-6, y2-5));
    arrowPolygon.append(QPointF(x2-6, y2+5));
    arrowPolygon.append(QPointF(x2, y2));
    QGraphicsPolygonItem* arrow = m_graphicsScene->addPolygon(arrowPolygon, QPen(), QBrush(Qt::SolidPattern));
    arrow->setTransformOriginPoint(sinkCoords);
    arrow->setRotation(angle);
}
