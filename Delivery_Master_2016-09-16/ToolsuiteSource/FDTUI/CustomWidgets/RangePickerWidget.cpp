#include "RangePickerWidget.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

RangePickerWidget::RangePickerWidget(QWidget* parent) :
    QWidget(parent),
    m_rangeStart(new QSpinBox()),
    m_rangeEnd(new QSpinBox()),
    m_arrowLabel(NULL)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    initializeSpinBox(m_rangeStart);
    initializeSpinBox(m_rangeEnd);

    m_arrowLabel = new QLabel("->");
    m_arrowLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addWidget(m_rangeStart, 1);
    layout->addWidget(m_arrowLabel, 1);
    layout->addWidget(m_rangeEnd, 1);

    layout->setContentsMargins(0, 0, 0, 0);

    connect(m_rangeStart, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RangePickerWidget::signal_SelectedRangeStart);
    connect(m_rangeEnd, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RangePickerWidget::signal_SelectedRangeEnd);
}

RangePickerWidget::~RangePickerWidget()
{
    delete m_rangeStart;
    delete m_rangeEnd;
    delete m_arrowLabel;
}

void RangePickerWidget::slot_SetStartValue(int startValue)
{
    m_rangeStart->setValue(startValue);
}

void RangePickerWidget::slot_SetEndValue(int endValue)
{
    m_rangeEnd->setValue(endValue);
}

void RangePickerWidget::initializeSpinBox(QSpinBox* spinBox)
{
    spinBox->setDisplayIntegerBase(16);
    spinBox->setMinimum(0x0);
    spinBox->setMaximum(0xFF);
    spinBox->setValue(0x0);
    spinBox->setPrefix("0x");
    spinBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

