#include "SchedulerAnalysis.h"
#include "ui_SchedulerAnalysis.h"

#include "IncrementSpinBoxDelegate.h"
#include "Project.h"
#include "SchedulerAnalysisConstants.h"
#include "SchedulerTaskGraph.h"
#include "SchedulerTaskModel.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QWidget>

SchedulerAnalysis::SchedulerAnalysis(const Project& project, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SchedulerAnalysis),
    m_project(project),
    m_taskModel(NULL)
{
    m_ui->setupUi(this);

    m_ui->currentTimeSlotSpinBox->setMaximum(InitialSlotCount);
    m_ui->timeLimitSpinbox->setValue(InitialSlotCount*MillisecondsPerSlot);

    m_taskGraph = new SchedulerTaskGraph();
    layout()->addWidget(m_taskGraph);

    setupData();
    connect(m_ui->currentTimeSlotSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SchedulerAnalysis::slot_currentTimeChanged);
    connect(m_ui->timeLimitSpinbox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SchedulerAnalysis::slot_timeLimitChanged);
}

SchedulerAnalysis::~SchedulerAnalysis()
{
    delete m_ui;
    if (m_taskModel != NULL) delete m_taskModel;
}

void SchedulerAnalysis::setupData()
{
    m_taskModel = new SchedulerTaskModel(m_project);

    m_ui->taskTable->setModel(m_taskModel);
    m_ui->taskTable->resizeColumnsToContents();

    m_ui->taskTable->setItemDelegateForColumn(SchedulerTaskModel::PeriodicityColumn, new IncrementSpinBoxDelegate(5, 65535, 5, this));
    m_ui->taskTable->setItemDelegateForColumn(SchedulerTaskModel::StartupDelayColumn, new IncrementSpinBoxDelegate(0, 65535, 5, this));

    m_taskGraph->setModel(m_taskModel);
    updateCurrentTimeTable();
    connect(m_taskGraph, &SchedulerTaskGraph::slotClicked, m_ui->currentTimeSlotSpinBox, &QSpinBox::setValue);
    connect(m_ui->resetModelButton, &QPushButton::clicked, m_taskModel, &SchedulerTaskModel::Reset);
    connect(m_taskModel, &SchedulerTaskModel::modelReset, this, &SchedulerAnalysis::updateCurrentTimeTable);
    connect(m_taskModel, &SchedulerTaskModel::dataChanged, this, &SchedulerAnalysis::modelDataChanged);
}

void SchedulerAnalysis::timeClicked(int newTimeslotIndex)
{
    m_ui->currentTimeSlotSpinBox->setValue(newTimeslotIndex);
}

void SchedulerAnalysis::modelDataChanged(const QModelIndex&, const QModelIndex&)
{
    updateCurrentTimeTable();
}

void SchedulerAnalysis::updateCurrentTimeTable()
{
    m_taskGraph->HighlightTimeslot(m_ui->currentTimeSlotSpinBox->value());

    m_ui->currentRunnablesList->clear();

    foreach (QString runnable, m_taskGraph->GetRunnablesAtTimeSlot(m_ui->currentTimeSlotSpinBox->value()))
    {
        m_ui->currentRunnablesList->addItem(runnable);
    }

    m_ui->labelTotalRunnableCount->setText(QString::number(m_ui->currentRunnablesList->count()));
}

void SchedulerAnalysis::slot_timeLimitChanged(int newValue)
{
    int currentSlot = m_ui->currentTimeSlotSpinBox->value();
    int newSlotCount = newValue/MillisecondsPerSlot;

    m_taskGraph->SetTimeslotCount(newSlotCount);

    m_ui->currentTimeSlotSpinBox->setMaximum(newSlotCount);
    if (currentSlot > newSlotCount)
    {
        m_ui->currentTimeSlotSpinBox->setValue(newSlotCount);
    }
}

void SchedulerAnalysis::slot_currentTimeChanged(int newValue)
{
    m_ui->labelTimeInMs->setText(QString::number(newValue*MillisecondsPerSlot));
    updateCurrentTimeTable();
}
