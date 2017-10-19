#include "SignalMappingWidget.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "SignalMappingDialog.h"
#include "Project.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>

SignalMappingWidget::SignalMappingWidget(GCLSignalSink* sink, Project* project, QWidget *parent) :
    QWidget(parent),
    m_sink(sink),
    m_project(project),
    m_sourceLabel(new QLabel())
{
    setAutoFillBackground(true);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_sourceLabel, 1, Qt::AlignLeft);

    RefreshCurrentMapping();

    QPushButton* mappingButton = new QPushButton("...");
    mappingButton->setFixedWidth(30);
    layout->addWidget(mappingButton);
    setToolTip("Double-click to map the signal to a source");

    connect(mappingButton,  &QPushButton::clicked, this, &SignalMappingWidget::showSignalMappingDialog);
}

void SignalMappingWidget::RefreshCurrentMapping()
{
    if (m_sink != NULL)
    {
        if  (m_sink->GetSignalSource() != NULL)
        {
            m_sourceLabel->setText(m_sink->GetSignalSource()->LongName());
        }
        else
        {
            m_sourceLabel->setText("<unmapped>");
        }
    }
    else
    {
        m_sourceLabel->setText("<unmapped>");
        m_sink->NotifyPropertyUpdated();
    }

    emit signal_MappingUpdated(m_sink);
}

void SignalMappingWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    showSignalMappingDialog();
}

void SignalMappingWidget::showSignalMappingDialog()
{
    if (m_sink != NULL)
    {
        SignalMappingDialog dialog(m_project, m_sink, this);
        dialog.exec();
        RefreshCurrentMapping();
    }
}
