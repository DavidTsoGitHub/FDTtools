#include "LayoutPreviewWidget.h"
#include "ui_LayoutPreviewWidget.h"

#include "DisplayArea.h"
#include "BSPDisplay.h"
#include "Layout.h"
#include "Project.h"
#include "ProjectValidator.h"
#include "UIWidgetBase.h"
#include "ColorPair.h"

#include <QGraphicsScene>
#include <QFont>
#include <QModelIndex>
#include <QPen>
#include <QTransform>
#include <QWheelEvent>
#include <QWidget>

static QColor monochromeAreaBackgroundColor = QColor(0, 0, 0);

LayoutPreviewWidget::LayoutPreviewWidget(QWidget *parent) :
    QWidget(parent),
    m_project(NULL),
    m_layout(NULL),
    m_highlightedDisplayArea(NULL),
    m_highlightedUIWidget(NULL),
    m_ui(new Ui::LayoutPreviewWidget),
    m_drawBorders(true),
    m_drawOverlay(true),
    m_drawWidgets(true),
    m_showOpaqueBackgrounds(true),
    m_drawBackgroundColor(true),
    m_numberOfLayersAvailable(0),
    m_graphicsRamAvailable(0)
{
    m_ui->setupUi(this);
    m_zoom = 1.0;

    //mono outline color will be set as SetProject
    m_unselectedAreaLinePen = QPen(QColor("Black"));
    m_unselectedAreaLinePen.setStyle(Qt::DashLine);

    m_highlightedAreaLinePen = QPen(QColor("Blue"));
    m_highlightedAreaLinePen.setStyle(Qt::DashLine);

    m_highlightedWidgetLinePen = QPen(QColor("Red"));
    m_highlightedWidgetLinePen.setStyle(Qt::SolidLine);
    m_highlightedWidgetLinePen.setWidth(3);

    m_graphicsScene = new QGraphicsScene();
    m_ui->graphicsView->setScene(m_graphicsScene);
    m_defTrans = m_ui->graphicsView->transform();
    m_ui->graphicsView->scale(m_zoom, m_zoom);
    connect(m_ui->drawOverlayCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_drawOverlay = toggled; slot_redraw(); });
    connect(m_ui->drawBordersCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_drawBorders = toggled; slot_redraw(); });
    connect(m_ui->drawWidgetsCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_drawWidgets = toggled; slot_redraw(); });
    connect(m_ui->opaqueBackgroundsCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_showOpaqueBackgrounds = toggled; slot_redraw(); });
    connect(m_ui->drawBackgroundColorCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_drawBackgroundColor = toggled; slot_redraw(); });
    connect(m_ui->resetZoomButton, &QPushButton::clicked, [=]() {m_ui->graphicsView->setTransform(m_defTrans);});
}

LayoutPreviewWidget::~LayoutPreviewWidget()
{
    delete m_ui;
}

void LayoutPreviewWidget::SetProject(Project* project)
{
    m_project = project;

    m_numberOfLayersAvailable = m_project->GetBSPSpecification().GetDisplay()->GetNumberOfLayers();
    m_graphicsRamAvailable = m_project->GetBSPSpecification().GetDisplay()->GetAvailableMemory();

    const BSPDisplay* display = m_project->GetBSPSpecification().GetDisplay();

    if (display->GetRenderingType() == BSPDisplay::SW)
    {
        m_ui->drawBackgroundColorCheckBox->setVisible(false);
        m_ui->opaqueBackgroundsCheckBox->setVisible(false);
        m_ui->layerLabel->setVisible(false);
        m_ui->numberOfLayersUsedLabel->setVisible(false);

        m_unselectedAreaLinePen = QPen(QColor(0xff, 0xff, 0xff));
        m_unselectedAreaLinePen.setStyle(Qt::DashLine);
    }

    if (display->GetUseGRAM() == false)
    {
        m_ui->gramLabel->setVisible(false);
        m_ui->gramUsedLabel->setVisible(false);
    }

    if (m_project->GetLayoutDefinition().Layouts().count() > 0)
    {
        m_layout = m_project->GetLayoutDefinition().Layouts().first();
    }

    connect(&m_project->GetLayoutDefinition(), &LayoutDefinition::PropertyUpdated, this, &LayoutPreviewWidget::slot_redraw);

    slot_redraw();
}

void LayoutPreviewWidget::SetLayout(int layoutID)
{
    m_layoutID = layoutID;
    Layout* layout = m_project->GetLayoutDefinition().GetLayoutFromID(m_layoutID);
    if(layout != NULL)
    {
        m_layout = layout;
        connect(m_layout, &Layout::PropertyUpdated, this, &LayoutPreviewWidget::slot_redraw);
        slot_redraw();
    }
    else
    {
        m_layout = NULL;
    }
}

void LayoutPreviewWidget::HighlightDisplayArea(QModelIndex index)
{
    if (!index.isValid()) return;

    m_highlightedDisplayArea = static_cast<DisplayArea*>(index.internalPointer());
    slot_redraw();
}

void LayoutPreviewWidget::HighlightUIWidget(QModelIndex index)
{
    if (!index.isValid()) return;

    m_highlightedUIWidget = static_cast<UIWidgetBase*>(index.internalPointer());
    slot_redraw();
}

void LayoutPreviewWidget::slot_redraw()
{
    m_graphicsScene->clear();

    Layout* layout = m_project->GetLayoutDefinition().GetLayoutFromID(m_layoutID);

    if (layout != NULL)
    {
        drawDisplay();
        if (layout->HasMenu())
        {
            drawMenuPlaceHolder(layout->GetMenuArea());
        }
        QListIterator<DisplayArea*> reverseIterator(layout->DisplayAreas());
        reverseIterator.toBack();
        while (reverseIterator.hasPrevious())
        {
            DisplayArea* displayArea = reverseIterator.previous();
            drawAreaBackgroundColor(displayArea);
            drawBorders(displayArea);
            drawOverlay(displayArea);
            drawWidgets(displayArea);
        }

        updateLayerAndGramUsageLabels();
    }

    m_ui->graphicsView->fitInView(0,0, 0, (this->sizeHint().height() > 500 ? 500 : this->sizeHint().height()), Qt::KeepAspectRatio);
    m_graphicsScene->setSceneRect(m_graphicsScene->itemsBoundingRect());
}

void LayoutPreviewWidget::drawDisplay()
{
    const LayoutDefinition& layout = m_project->GetLayoutDefinition();
    const BSPDisplay* display = m_project->GetBSPSpecification().GetDisplay();
    quint16 width, height, cropX, cropY, cropWidth, cropHeight;

    if (display->GetRenderingOrientation() == BSPDisplay::CounterClockWise)
    {
        width = display->GetHeight();
        height = display->GetWidth();
    }
    else
    {
        width = display->GetWidth();
        height = display->GetHeight();
    }

    cropX = layout.CropLeft();
    cropY = layout.CropTop();
    cropWidth = width - layout.CropLeft() - layout.CropRight();
    cropHeight = height - layout.CropTop() - layout.CropBottom();
    m_graphicsScene->addRect(0, 0, width, height, QPen(QColor("transparent")), QBrush(QColor("purple")));
    m_graphicsScene->addRect(-40, -40, width + 80, height + 80, QPen(QColor("transparent")), QBrush(QColor(0xbe, 0xbe, 0xbe)));

    if (m_drawBackgroundColor)
    {
        m_graphicsScene->addRect(cropX, cropY, cropWidth, cropHeight, QPen(QColor("transparent")), QBrush(m_layout->GetBackgroundColor()));
    }
    else
    {
        m_graphicsScene->addRect(cropX, cropY, cropWidth, cropHeight, QPen(QColor("transparent")), QBrush(QColor("white")));
    }
}

void LayoutPreviewWidget::drawAreaBackgroundColor(DisplayArea* displayArea)
{
    quint16 x, y, width, height;

    x = displayArea->GetX();
    y = displayArea->GetY();
    width = displayArea->GetWidth();
    height = displayArea->GetHeight();

    if (m_project->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::SW)
    {
        m_graphicsScene->addRect(x, y, width, height, QPen(QColor("transparent")), QBrush(monochromeAreaBackgroundColor));
    }
    else if (m_showOpaqueBackgrounds && displayArea->GetColorPair()->GetBGIsTransparent())
    {
        m_graphicsScene->addRect(x, y, width, height, QPen(QColor("transparent")), QBrush(m_project->GetLayoutDefinition().GetStartChromaKeyColor()));
    }
    else
    {
        if (!displayArea->GetColorPair()->GetBGIsTransparent())
        {
            m_graphicsScene->addRect(x, y, width, height, QPen(QColor("transparent")), QBrush(displayArea->GetColorPair()->GetBGColor()));
        }
    }
}

void LayoutPreviewWidget::drawWidgets(DisplayArea* displayArea)
{
    if (!m_drawWidgets) return;

    int borderOffset = 0;

    if (displayArea->GetBorder() > 0)
    {
        borderOffset = 2;
    }

    QListIterator<UIWidgetBase*> reverseIterator(displayArea->UIWidgets());
    reverseIterator.toBack();
    while (reverseIterator.hasPrevious())
    {
        UIWidgetBase* uiWidget = reverseIterator.previous();

        QGraphicsItem* widgetGraphics = uiWidget->GetGraphicsItem(m_showOpaqueBackgrounds);
        widgetGraphics->setX(displayArea->GetX() + uiWidget->GetX() + borderOffset);
        widgetGraphics->setY(displayArea->GetY() + uiWidget->GetY() + borderOffset);

        if (m_highlightedUIWidget == uiWidget && m_drawOverlay == true)
        {
            if (m_highlightedDisplayArea != NULL && m_highlightedDisplayArea->UIWidgets().contains(uiWidget))
            {
                m_graphicsScene->addRect(widgetGraphics->x(), widgetGraphics->y(), m_highlightedUIWidget->GetWidth()-1, m_highlightedUIWidget->GetHeight()-1, m_highlightedWidgetLinePen);
            }
        }
        m_graphicsScene->addItem(widgetGraphics);
    }
}

void LayoutPreviewWidget::drawOverlay(DisplayArea* displayArea)
{
    if (!m_drawOverlay) return;

    // Read the Coordinates section of http://doc.qt.io/qt-4.8/qrect.html for an explanation to the -1's in width and height.
    m_graphicsScene->addRect(displayArea->GetX(), displayArea->GetY(), displayArea->GetWidth()-1, displayArea->GetHeight()-1, (m_highlightedDisplayArea == displayArea ? m_highlightedAreaLinePen : m_unselectedAreaLinePen));

    QGraphicsTextItem* name = m_graphicsScene->addText(displayArea->GetName());
    name->setDefaultTextColor((m_highlightedDisplayArea == displayArea ? QColor("Blue") : QColor("Black")));
    name->setX(displayArea->GetX());
    name->setY(displayArea->GetY());
}

void LayoutPreviewWidget::drawMenuPlaceHolder(DisplayArea* displayArea)
{
    Q_UNUSED(displayArea);
    //TODO: make it look ok
    //m_graphicsScene->addRect(displayArea->GetX() + 1, displayArea->GetY() + 1, displayArea->GetWidth() - 2, displayArea->GetHeight() - 2, normalLinePen, QBrush(Qt::black, Qt::BDiagPattern));
    //m_graphicsScene->addRect(displayArea->GetX() + 1, displayArea->GetY() + 1, displayArea->GetWidth() - 2, displayArea->GetHeight() - 2, normalLinePen, QBrush(Qt::white, Qt::FDiagPattern));
}

void LayoutPreviewWidget::drawBorders(DisplayArea* displayArea)
{
    if (!m_drawBorders || displayArea->GetBorder() == 0) return;
    int x1 = displayArea->GetX();
    int y1 = displayArea->GetY();
    int x2 = x1 + displayArea->GetWidth() - 1;
    int y2 = y1 + displayArea->GetHeight() - 1;

    m_graphicsScene->addLine((x1+2), y1, (x2-2), y1);
    m_graphicsScene->addLine((x1+2), y2, (x2-2), y2);
    m_graphicsScene->addLine(x1, (y1+2), x1, (y2-2));
    m_graphicsScene->addLine(x2, (y1+2), x2, (y2-2));

    m_graphicsScene->addLine((x1+1), (y1+1), (x1+1), (y1+1));
    m_graphicsScene->addLine((x2-1), (y1+1), (x2-1), (y1+1));
    m_graphicsScene->addLine((x1+1), (y2-1), (x1+1), (y2-1));
    m_graphicsScene->addLine((x2-1), (y2-1), (x2-1), (y2-1));
}

void LayoutPreviewWidget::updateLayerAndGramUsageLabels(void)
{
    if (m_project->GetBSPSpecification().GetDisplay()->GetRenderingType() != BSPDisplay::HW) return;

    ProjectValidator::layerAndGramUsage_t layoutUsage = ProjectValidator::CalculateLayerAndGramUsage(m_project, m_layout);

    QString usedLayouts = tr("<font color='%1'>%2 / %3</font>");
    if (layoutUsage.layerCount > m_numberOfLayersAvailable)
    {
        m_ui->numberOfLayersUsedLabel->setText(usedLayouts.arg("red",  QString::number(layoutUsage.layerCount),  QString::number(m_numberOfLayersAvailable)));
    }
    else
    {
        m_ui->numberOfLayersUsedLabel->setText(usedLayouts.arg("black",  QString::number(layoutUsage.layerCount),  QString::number(m_numberOfLayersAvailable)));
    }

    QString gramLabel = tr("<font color='%1'>%2 / %3 bytes");
    if (layoutUsage.gramUsage > m_graphicsRamAvailable)
    {
        m_ui->gramUsedLabel->setText(gramLabel.arg("red", QString::number(layoutUsage.gramUsage), QString::number(m_graphicsRamAvailable)));
    }
    else
    {
        m_ui->gramUsedLabel->setText(gramLabel.arg("black", QString::number(layoutUsage.gramUsage), QString::number(m_graphicsRamAvailable)));
    }
}

void LayoutPreviewWidget::wheelEvent(QWheelEvent* event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal)
    {
        ;
    }
    else
    {
        double zoom = 1.0;
        if((numSteps < 0))
        {
            zoom = 1.0 + (0.015 * numSteps);
        }
        else if((numSteps > 0))
        {
            zoom = 1.0 + (0.015 * numSteps);
        }
        m_ui->graphicsView->scale(zoom, zoom);
    }
    event->accept();
}
