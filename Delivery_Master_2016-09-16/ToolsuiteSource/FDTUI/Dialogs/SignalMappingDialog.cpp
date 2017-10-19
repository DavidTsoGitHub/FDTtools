#include "SignalMappingDialog.h"
#include "ui_SignalMappingDialog.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Project.h"
#include "SignalSourceFinderVisitor.h"
#include <QDialog>
#include <QList>
#include <QMultiMap>
#include <QPushButton>
#include <QString>
#include <QWidget>

SignalMappingDialog::SignalMappingDialog(Project* project, GCLSignalSink* signalToMap, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SignalMappingDialog),
    m_preselectCurrentlyMappedSignal(true),
    m_project(project),
    m_selectedSource(NULL),
    m_signalToMap(signalToMap)
{
    m_ui->setupUi(this);
    setWindowTitle("SignalMapping for " + signalToMap->LongName() + " (" + signalToMap->DataType() + ")");

    this->setModal(true);

    m_ui->removeMappingButton->setEnabled((m_signalToMap->GetSignalSource() != NULL) ? true : false);

    SignalSourceFinderVisitor v(signalToMap);
    project->Accept(v);
    m_validSources = v.GetFoundSources();

    populateComponentNameList();
    connectUISignals();
    m_ui->componentNameList->setFocus();

    if ((m_signalToMap->GetSignalSource()) != NULL && (m_signalToMap->GetSignalSource()->DataType() != m_signalToMap->DataType()))
    {
        //when a function block designer changes datatype of a previous signal,
        //the matchingItems.first() will crash FDT
        m_preselectCurrentlyMappedSignal = false;
    }
}

SignalMappingDialog::~SignalMappingDialog()
{
    delete m_ui;
}

void SignalMappingDialog::connectUISignals()
{
    connect(m_ui->okButton, &QPushButton::clicked, this, &SignalMappingDialog::slot_okButton_clicked);
    connect(m_ui->cancelButton, &QPushButton::clicked, this, &SignalMappingDialog::slot_cancelButton_clicked);
    connect(m_ui->removeMappingButton, &QPushButton::clicked, this, &SignalMappingDialog::slot_removeMappingButton_clicked);
    connect(m_ui->componentNameList, &QListWidget::currentTextChanged, this, &SignalMappingDialog::slot_componentNameList_currentTextChanged);
    connect(m_ui->signalNameList, &QListWidget::currentTextChanged, this, &SignalMappingDialog::slot_signalNameList_currentTextChanged);
}

void SignalMappingDialog::slot_componentNameList_currentTextChanged(QString componentName)
{
    if (m_preselectCurrentlyMappedSignal)
    {
        m_preselectCurrentlyMappedSignal = false;

        GCLSignalSource* source = m_signalToMap->GetSignalSource();
        if (source != NULL)
        {
            QList<QListWidgetItem *> matchingItems = m_ui->componentNameList->findItems(source->ComponentName(), Qt::MatchExactly);
            if (m_ui->componentNameList->currentItem() == matchingItems.first())
            {
                slot_componentNameList_currentTextChanged(matchingItems.first()->text());
            }
            else
            {
                m_ui->componentNameList->setCurrentItem(matchingItems.first());
            }

            matchingItems = m_ui->signalNameList->findItems(source->SignalName(), Qt::MatchExactly);
            m_ui->signalNameList->setCurrentItem(matchingItems.first());
            return;
        }
    }


    QList<QListWidgetItem *> matchingItems = m_ui->componentNameList->findItems(componentName, Qt::MatchExactly);

    m_ui->signalNameList->clear();

    if (matchingItems.count() > 0)
    {
        foreach (const GCLSignalSource* source, m_validSources)
        {
            if (source->ComponentName() == componentName)
            {
                m_ui->signalNameList->addItem(source->SignalName());
            }
        }

        m_ui->signalNameList->setCurrentRow(0);
    }
}

void SignalMappingDialog::slot_signalNameList_currentTextChanged(QString signalName)
{
    QString componentName(m_ui->componentNameList->currentItem()->text());

    if (componentName.isEmpty()) return;

    foreach (GCLSignalSource* source, m_sourcesByComponentName.values(componentName))
    {
        if (source->SignalName().compare(signalName) == 0)
        {
            setSelectedSource(source);
            break;
        }
    }
}

void SignalMappingDialog::slot_okButton_clicked(bool /*checked*/)
{
    if (m_selectedSource != NULL)
    {
        m_signalToMap->ConnectTo(m_selectedSource);
    }
    this->accept();
}

void SignalMappingDialog::slot_cancelButton_clicked(bool /*checked*/)
{
    this->reject();
}

void SignalMappingDialog::slot_removeMappingButton_clicked(bool /*checked*/)
{
    m_signalToMap->DisconnectFromSource();
    this->accept();
}

void SignalMappingDialog::populateComponentNameList()
{
    foreach (GCLSignalSource *source, m_validSources)
    {
        if (source->ComponentName() != m_signalToMap->ComponentName())
        {
            m_sourcesByComponentName.insert(source->ComponentName(), source);
        }
    }

    foreach (QString componentName, m_sourcesByComponentName.uniqueKeys())
    {
        m_ui->componentNameList->addItem(componentName);
    }
}

void SignalMappingDialog::setSelectedSource(GCLSignalSource* source)
{
    if (source == NULL)
    {
        m_ui->okButton->setEnabled(false);
    }
    else if (m_signalToMap->GetSignalSource() != NULL && m_signalToMap->GetSignalSource() == source)
    {
        m_ui->okButton->setEnabled(false);
    }
    else
    {
        m_selectedSource = source;
        m_ui->okButton->setEnabled(true);
    }
}

