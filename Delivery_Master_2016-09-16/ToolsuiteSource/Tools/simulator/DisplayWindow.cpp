#include "DisplayWindow.h"
#include "ui_DisplayWindow.h"

DisplayWindow::DisplayWindow(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::DisplayWindow)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint| Qt::WindowTitleHint);
}

DisplayWindow::~DisplayWindow()
{
    delete m_ui;
}

void DisplayWindow::SetRedrawText(QString string)
{
    m_ui->lastRedrawLabel->setText(string);
}

void DisplayWindow::SetScene(QGraphicsScene *scene)
{
    m_ui->displayGraphicsView->setScene(scene);
}

void DisplayWindow::SetSceneSize(quint16 width, quint16 height, DisplayWindow::Rotation_e rotation)
{
    m_ui->displayGraphicsView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_ui->displayGraphicsView->setSceneRect(0, 0 , width, height);

    if(rotation == Rotation_e::CCW)
    {
        m_ui->displayGraphicsView->rotate(90);
        m_size.setWidth(height + 28);
        m_size.setHeight(width + m_ui->lastRedrawLabel->size().height() + 33);
    }
    else
    {
        m_size.setWidth(width + 28);
        m_size.setHeight(height + m_ui->lastRedrawLabel->size().height() + 33);
    }

    this->setFixedSize(m_size);
}

void DisplayWindow::SetBackgroundBrush(QBrush brush)
{
    m_ui->displayGraphicsView->setBackgroundBrush(brush);
}

QGraphicsView *DisplayWindow::GetGraphicsView()
{
    return m_ui->displayGraphicsView;
}

QSize DisplayWindow::sizeHint() const
{
    return m_size;
}

QSize DisplayWindow::minimumSizeHint() const
{
    return m_size;
}
