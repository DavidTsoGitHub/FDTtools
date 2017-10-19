#include "MenuItemPreviewWidget.h"
#include "ui_MenuItemPreviewWidget.h"

#include "BSPDisplay.h"
#include "IconWidget.h"
#include "IconWidgetGraphicsItem.h"
#include "ImageWidgetGraphicsItem.h"
#include "Layout.h"
#include "LabelWidget.h"
#include "LabelWidgetGraphicsItem.h"
#include "MenuItem.h"
#include "Project.h"
#include "UIWidgetBase.h"
#include "ColorPair.h"

#include <QGraphicsScene>
#include <QWidget>

MenuItemPreviewWidget::MenuItemPreviewWidget(quint16 displayHeight, quint16 displayWidth, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuItemPreviewWidget),
    m_menuAreaHeight(0),
    m_menuAreaWidth(0),
    m_menuAreaPosX(0),
    m_menuAreaPosY(0),
    m_displayHeight(displayHeight),
    m_displayWidth(displayWidth),
    m_menuItem(NULL),
    m_project(NULL),
    m_highLightedUIWidget(NULL),
    m_showChromaKey(true)
{
    ui->setupUi(this);
    ui->chromaKeyCheckBox->setEnabled(false);
    m_graphicsScene = new QGraphicsScene();
    ui->previewGraphicsView->setScene(m_graphicsScene);
    ui->previewGraphicsView->setBackgroundBrush(QBrush(QColor(0xbe, 0xbe, 0xbe)));
    ui->previewGraphicsView->resize(m_displayWidth, m_displayHeight);

    connect(ui->chromaKeyCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_showChromaKey = toggled; Redraw(); });
}

MenuItemPreviewWidget::~MenuItemPreviewWidget()
{
    delete ui;
    delete m_graphicsScene;
}

void MenuItemPreviewWidget::SetProject(Project* project)
{
    m_project = project;
    m_menuItem = NULL;

    if (m_project == NULL)
    {
        m_graphicsScene->clear();
        ui->chromaKeyCheckBox->setEnabled(false);
    }
    else
    {
        if (m_project->GetLayoutDefinition().HasMenu())
        {
            ui->chromaKeyCheckBox->setEnabled(true);
        }
        else
        {
            ui->chromaKeyCheckBox->setEnabled(false);
        }

        const BSPDisplay* display = m_project->GetBSPSpecification().GetDisplay();

        if (display->GetRenderingType() == BSPDisplay::SW)
        {
            ui->chromaKeyCheckBox->setVisible(false);
        }

        Redraw();
    }
}

void MenuItemPreviewWidget::SetMenuItem(MenuItem* menuItem)
{
    if (m_menuItem != NULL) disconnect(m_menuItem, &MenuItem::PropertyUpdated, this, &MenuItemPreviewWidget::Redraw);
    m_menuItem = menuItem;
    if (m_menuItem != NULL) connect(m_menuItem, &MenuItem::PropertyUpdated, this, &MenuItemPreviewWidget::Redraw);
    m_highLightedUIWidget = NULL;
    Redraw();
}

void MenuItemPreviewWidget::HighLightUIWidget(UIWidgetBase* uiWidget)
{
    m_highLightedUIWidget = uiWidget;
    Redraw();
}

void MenuItemPreviewWidget::Redraw()
{
    m_graphicsScene->clear();

    Layout* menuLayout = m_project->GetLayoutDefinition().GetMenuLayout();

    if (menuLayout != NULL)
    {
        const BSPDisplay* display = m_project->GetBSPSpecification().GetDisplay();
        quint16 width, height;

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

        m_graphicsScene->addRect(0, 0, width, height, QPen(QColor("transparent")), QBrush(QColor(menuLayout->GetBackgroundColor())));

        DisplayArea* menuArea = menuLayout->GetMenuArea();

        if (m_showChromaKey && menuArea->GetColorPair()->GetBGIsTransparent())
        {
            m_graphicsScene->addRect(menuArea->GetX(), menuArea->GetY(), menuArea->GetWidth(), menuArea->GetHeight(), QPen(QColor("White")), QBrush(m_project->GetLayoutDefinition().GetStartChromaKeyColor()));
        }
        else
        {
            if (!menuArea->GetColorPair()->GetBGIsTransparent())
            {
                m_graphicsScene->addRect(menuArea->GetX(), menuArea->GetY(), menuArea->GetWidth(), menuArea->GetHeight(), QPen(QColor("White")), QBrush(menuArea->GetColorPair()->GetBGColor()));
            }
            else
            {
                m_graphicsScene->addRect(menuArea->GetX(), menuArea->GetY(), menuArea->GetWidth(), menuArea->GetHeight(), QPen(QColor("White")), QBrush(menuLayout->GetBackgroundColor()));
            }
        }

        if (m_menuItem == NULL)
        {
            return;
        }

        int borderOffset = 0;

        if (menuArea->GetBorder() > 0)
        {
            borderOffset = 2;
        }

        foreach (UIWidgetBase* uiWidget, m_menuItem->UIWidgets())
        {
            QGraphicsItem* graphicsItem = NULL;

            graphicsItem = uiWidget->GetGraphicsItem(m_showChromaKey);
            static_cast<GraphicsItemBase*>(graphicsItem)->SetHighLight(m_highLightedUIWidget == uiWidget);
            graphicsItem->setX(menuArea->GetX() + uiWidget->GetX() + borderOffset);
            graphicsItem->setY(menuArea->GetY() + uiWidget->GetY() + borderOffset);

            m_graphicsScene->addItem(graphicsItem);
        }
    }
}

