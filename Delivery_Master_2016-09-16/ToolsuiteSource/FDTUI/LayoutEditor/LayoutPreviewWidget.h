#ifndef LAYOUTPREVIEWWIDGET_H
#define LAYOUTPREVIEWWIDGET_H

class DisplayArea;
class Layout;
class Project;
class UIWidgetBase;

class QGraphicsScene;
class QFont;
class QModelIndex;
class QPen;
class QTransform;
class QWheelEvent;
class QWidget;

namespace Ui {
class LayoutPreviewWidget;
}

class LayoutPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LayoutPreviewWidget(QWidget* parent = 0);
    ~LayoutPreviewWidget();

    void SetProject(Project* project);
    void HighlightDisplayArea(QModelIndex index);
    void HighlightUIWidget(QModelIndex index);
    void SetLayout(int layoutID);

private slots:
    void slot_redraw(void);

private:
    QGraphicsScene* m_graphicsScene;
    Project* m_project;
    Layout* m_layout;
    int m_layoutID;
    DisplayArea* m_highlightedDisplayArea;
    UIWidgetBase* m_highlightedUIWidget;
    Ui::LayoutPreviewWidget* m_ui;
    bool m_drawBorders;
    bool m_drawOverlay;
    bool m_drawWidgets;
    bool m_showOpaqueBackgrounds;
    bool m_drawBackgroundColor;
    quint32 m_numberOfLayersAvailable;
    quint32 m_graphicsRamAvailable;
    double m_zoom;
    QTransform m_defTrans;

    QPen m_unselectedAreaLinePen;
    QFont m_normalText;
    QPen m_highlightedAreaLinePen;
    QPen m_highlightedWidgetLinePen;
    QFont m_highlightedText;

    void drawWidgets(DisplayArea* displayArea);
    void drawOverlay(DisplayArea* displayArea);
    void drawMenuPlaceHolder(DisplayArea* displayArea);
    void drawBorders(DisplayArea* displayArea);
    void drawDisplay(void);
    void drawAreaBackgroundColor(DisplayArea *displayArea);
    void drawBackground(void);
    void updateLayerAndGramUsageLabels(void);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // LAYOUTPREVIEWWIDGET_H
