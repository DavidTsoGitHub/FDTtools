#include "ColorRangePreviewWidget.h"

#include <QWidget>
#include <QPalette>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

ColorRangePreviewWidget::ColorRangePreviewWidget(QWidget* parent) :
    QWidget(parent),
    m_arrowLabel(NULL),
    m_startColor(new QLineEdit()),
    m_endColor(new QLineEdit())
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    initializeColorWidgets(m_startColor);
    initializeColorWidgets(m_endColor);

    m_arrowLabel = new QLabel("->");
    m_arrowLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addWidget(m_startColor, 1);
    layout->addWidget(m_arrowLabel, 1);
    layout->addWidget(m_endColor, 1);

    layout->setContentsMargins(0, 0, 0, 0);
}

ColorRangePreviewWidget::~ColorRangePreviewWidget()
{
    delete m_startColor;
    delete m_arrowLabel;
    delete m_endColor;
}

void ColorRangePreviewWidget::slot_SetStartRangeColor(QColor startColor)
{
    QPalette palette;
    palette.setColor(QPalette::Base, startColor);
    palette.setColor(QPalette::Text, Qt::black);
    m_startColor->setPalette(palette);
}

void ColorRangePreviewWidget::slot_SetEndRangeColor(QColor endColor)
{
    QPalette palette;
    palette.setColor(QPalette::Base, endColor);
    palette.setColor(QPalette::Text, Qt::black);
    m_endColor->setPalette(palette);
}

void ColorRangePreviewWidget::initializeColorWidgets(QLineEdit* widget)
{
    widget->setEnabled(false);
    widget->setText("");
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

