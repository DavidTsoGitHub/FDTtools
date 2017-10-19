#ifndef SIGNALMAPPINGCANVAS_H
#define SIGNALMAPPINGCANVAS_H

class CenterBlockGraphicItem;
class GCLSignalSink;
class GCLSignalSource;
class InputBlockGraphicItem;
class OutputBlockGraphicItem;
class Project;
class ProjectObject;

class QFontMetrics;
class QGraphicsScene;
class QGraphicsView;
template <class Key, class T> class QHash;
template <typename T> class QList;
class QWidget;

namespace Ui
{
class SignalMappingCanvas;
}

class SignalMappingCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit SignalMappingCanvas(QWidget* parent = 0);
    ~SignalMappingCanvas();

public slots:
    void SetCurrentProjectObject(ProjectObject* object);
    void ProjectLoaded(Project*);
    void ProjectUnloaded();
    void JustSelected();

signals:
    void signal_selectedProjectObject(ProjectObject* selectedProjectObject);

private:
    Ui::SignalMappingCanvas* m_ui;
    ProjectObject* m_selectedItem;
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_graphicsScene;
    QFontMetrics* m_fontMetrics;

    QList<InputBlockGraphicItem*> m_inputBlocks;
    QList<OutputBlockGraphicItem*> m_outputBlocks;
    CenterBlockGraphicItem* m_centerBlock;

    QHash<const GCLSignalSink*, QPointF> m_sinkEndpoints;
    QHash<const GCLSignalSource*, QPointF> m_sourceEndpoints;

    void redraw(void);
    void drawInputBlocks(void);
    void drawOutputBlocks(void);
    void drawCenterBlock(void);
    void drawConnectors(void);
    void drawArrow(const GCLSignalSource* from, const GCLSignalSink* to);
};

#endif // SIGNALMAPPINGCANVAS_H
