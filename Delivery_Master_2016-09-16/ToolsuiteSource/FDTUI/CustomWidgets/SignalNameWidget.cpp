#include "SignalNameWidget.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

SignalNameWidget::SignalNameWidget(const GCLSignalSink* sink, QWidget *parent) : QWidget(parent)
{
    setupWidget();

    QLabel* nameLabel = new QLabel(sink->SignalName());
    nameLabel->setFont(m_signalNameFont);
    thisLayout()->addWidget(nameLabel, 0, Qt::AlignLeft);

    QLabel* dataTypeLabel;
    if (sink->IsMandatory())
    {
        dataTypeLabel = new QLabel("(" + sink->DataType() + ") (Mandatory)");
    }
    else
    {
        dataTypeLabel = new QLabel("(" + sink->DataType() + ")");
    }

    dataTypeLabel->setFont(m_signalTypeFont);
    thisLayout()->addWidget(dataTypeLabel, 0, Qt::AlignLeft);
}

SignalNameWidget::SignalNameWidget(const GCLSignalSource* source, QWidget *parent) : QWidget(parent)
{
    setupWidget();

    QLabel* nameLabel = new QLabel(source->SignalName());
    nameLabel->setFont(m_signalNameFont);
    thisLayout()->addWidget(nameLabel, 0, Qt::AlignLeft);

    QLabel* dataTypeLabel = new QLabel("(" + source->DataType() + ")");
    dataTypeLabel->setFont(m_signalTypeFont);
    thisLayout()->addWidget(dataTypeLabel, 0, Qt::AlignLeft);
}

QHBoxLayout* SignalNameWidget::thisLayout()
{
    return static_cast<QHBoxLayout*>(layout());
}

void SignalNameWidget::setupWidget()
{
    m_signalNameFont = font();
    m_signalNameFont.setPixelSize(14);
    m_signalTypeFont = m_signalNameFont;
    m_signalTypeFont.setItalic(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 0, 0);
}
