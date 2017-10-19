#include "SourcesOverview.h"
#include "ui_SourcesOverview.h"

#include "Project.h"
#include "SignalMapping.h"
#include "SignalMappingDelegate.h"
#include "SignalMappingWidget.h"
#include "SourcesOverviewModel.h"
#include "SourcesOverviewProxyModel.h"

#include <QMessageBox>
#include <QWidget>

SourcesOverview::SourcesOverview(QWidget *parent) : QWidget(parent),
    m_ui(new Ui::SourcesOverview),
    m_project(NULL),
    m_tabShowing(true),
    m_populateTableNeeded(true),
    m_sourceModel(NULL),
    m_proxyModel(NULL)
{
   m_ui->setupUi(this);
}

SourcesOverview::~SourcesOverview()
{
    delete m_ui;
}

void SourcesOverview::SetProject(Project* project)
{
    m_project = project;

    populate();
    setupSignals();
}

void SourcesOverview::ClearProject()
{
    m_project = NULL;

    delete m_proxyModel;
    m_proxyModel = NULL;
    delete m_sourceModel;
    m_sourceModel = NULL;
}

void SourcesOverview::JustSelected()
{
    m_tabShowing = true;
    if (m_populateTableNeeded)
    {
        m_sourceModel->PopulateModel();
        m_proxyModel->Invalidate();
    }
}

void SourcesOverview::JustDeselected()
{
    m_tabShowing = false;
}

void SourcesOverview::populate()
{
    m_ui->HALSelectionCheckBox->setChecked(true);
    m_ui->CANSelectionCheckBox->setChecked(true);
    m_ui->FBSelectionCheckBox->setChecked(true);
    m_ui->displaySelectionCheckBox->setChecked(true);
    m_ui->widgetsSelectionCheckBox->setChecked(true);
    m_ui->animationSelectionCheckBox->setChecked(true);
    m_ui->UMSourcesCheckBox->setChecked(true);
    m_ui->MSourcesCheckBox->setChecked(true);

    m_ui->tableView->setModel(NULL);
    if (m_proxyModel != NULL)
        delete m_proxyModel;
    if (m_sourceModel != NULL)
        delete m_sourceModel;

    m_sourceModel = new SourcesOverviewModel(m_project);
    m_proxyModel = new SourcesOverviewProxyModel(m_project, m_sourceModel);
    m_proxyModel->setSourceModel(m_sourceModel);
    m_proxyModel->setFilterRole(Qt::DisplayRole);
    m_proxyModel->SetFilter(m_ui->m_lineEditFilter->text());

    m_ui->tableView->setModel(m_proxyModel);
    m_ui->tableView->setSortingEnabled(true);
    m_ui->tableView->resizeColumnsToContents();
    m_ui->tableView->verticalHeader()->hide();

    for (int i = SourcesOverviewModel::FirstSinkColumn; i <= m_sourceModel->GetMaxNbrOfSources(); i++)
    {
        m_ui->tableView->setItemDelegateForColumn(i, new SignalMappingDelegate(m_project, m_sourceModel));
    }
}

void SourcesOverview::setupSignals()
{
    connect(m_project, &Project::PropertyUpdated, [=] { if (!m_tabShowing) { m_populateTableNeeded = true; return; } m_sourceModel->PopulateModel(); m_proxyModel->Invalidate(); });

    connect(m_ui->CANSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowCAN);
    connect(m_ui->HALSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowHAL);
    connect(m_ui->FBSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowFunctionBlocks);
    connect(m_ui->displaySelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowDisplay);
    connect(m_ui->widgetsSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowWidgets);
    connect(m_ui->animationSelectionCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowAnimations);
    connect(m_ui->UMSourcesCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowUnmappedSources);
    connect(m_ui->MSourcesCheckBox, &QCheckBox::toggled, m_proxyModel, &SourcesOverviewProxyModel::SetShowMappedSources);

    connect(m_ui->m_lineEditFilter, &QLineEdit::textChanged, m_proxyModel, &SourcesOverviewProxyModel::SetFilter);
}
