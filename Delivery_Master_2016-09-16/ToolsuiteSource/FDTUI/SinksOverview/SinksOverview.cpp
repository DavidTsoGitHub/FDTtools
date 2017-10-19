#include "SinksOverview.h"
#include "ui_SinksOverview.h"

#include "ISignalMappable.h"
#include "Project.h"
#include "SignalMapping.h"
#include "SignalMappingDelegate.h"
#include "SinksOverviewModel.h"
#include "SinksOverviewProxyModel.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QWidget>

SinksOverview::SinksOverview(QWidget *parent) : QWidget(parent),
    m_project(NULL),
    m_ui(new Ui::SinksOverview),
    m_tabShowing(true),
    m_populateTableNeeded(false),
    m_sinkModel(NULL),
    m_proxyModel(NULL)
{
    m_ui->setupUi(this);
}

SinksOverview::~SinksOverview()
{
    delete m_ui;
}

void SinksOverview::SetProject(Project* project)
{
    m_project = project;

    populate();
    setupSignals();
}

void SinksOverview::ClearProject()
{
    m_project = NULL;

    delete m_proxyModel;
    m_proxyModel = NULL;
    delete m_sinkModel;
    m_sinkModel = NULL;
}

void SinksOverview::JustSelected()
{
    m_tabShowing = true;
    if (m_populateTableNeeded)
    {
        m_sinkModel->PopulateModel();
        m_proxyModel->Invalidate();
    }
}

void SinksOverview::JustDeselected()
{
    m_tabShowing = false;
}

void SinksOverview::populate()
{

    m_ui->HALSelectionCheckBox->setChecked(true);
    m_ui->CANSelectionCheckBox->setChecked(true);
    m_ui->FBSelectionCheckBox->setChecked(true);
    m_ui->widgetsSelectionCheckBox->setChecked(true);
    m_ui->UMSinksCheckBox->setChecked(true);
    m_ui->MSinksCheckBox->setChecked(true);

    m_ui->tableView->setModel(NULL);
    if (m_proxyModel != NULL)
        delete m_proxyModel;

    if (m_sinkModel != NULL)
        delete m_sinkModel;

    m_sinkModel = new SinksOverviewModel(m_project);
    m_proxyModel = new SinksOverviewProxyModel(m_project, m_sinkModel);
    m_proxyModel->setSourceModel(m_sinkModel);
    m_proxyModel->setFilterRole(Qt::EditRole);
    m_proxyModel->SetFilter(m_ui->m_lineEditFilter->text());

    m_ui->tableView->setModel(m_proxyModel);
    m_ui->tableView->setSortingEnabled(true);
    m_ui->tableView->resizeColumnsToContents();
    m_ui->tableView->verticalHeader()->hide();

    m_ui->tableView->setItemDelegateForColumn(1, new SignalMappingDelegate(m_project, m_sinkModel));

}

void SinksOverview::setupSignals()
{
    connect(m_project, &Project::PropertyUpdated, [=] { if (!m_tabShowing) { m_populateTableNeeded = true; return; } m_sinkModel->PopulateModel(); m_proxyModel->Invalidate(); });

    connect(m_ui->CANSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowCAN);
    connect(m_ui->HALSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowHAL);
    connect(m_ui->FBSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowFunctionBlocks);
    connect(m_ui->widgetsSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowWidgets);
    connect(m_ui->UMSinksCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowUnmappedSinks);
    connect(m_ui->MSinksCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetShowMappedSinks);
    connect(m_ui->searchSourcesSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SinksOverviewProxyModel::SetSearchSources);
    connect(m_ui->m_lineEditFilter, &QLineEdit::textChanged, m_proxyModel, &SinksOverviewProxyModel::SetFilter);
}
