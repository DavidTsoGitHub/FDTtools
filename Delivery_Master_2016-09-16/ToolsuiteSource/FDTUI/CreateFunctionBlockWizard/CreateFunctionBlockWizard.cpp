#include "CreateFunctionBlockWizard.h"
#include "ui_CreateFunctionBlockWizard.h"

#include "GeneralParametersWizardPage.h"
#include "InputSignalsWizardPage.h"
#include "OutputSignalsWizardPage.h"
#include "ConfigurationParametersWizardPage.h"
#include "CalibrationParametersWizardPage.h"
#include "LabelMappingsWizardPage.h"
#include "IconResourcesWizardPage.h"
#include "ImageResourcesWizardPage.h"
#include "FaultsConfigurationWizardPage.h"
#include "ReviewFunctionBlockWizardPage.h"

#include <QWizard>

CreateFunctionBlockWizard::CreateFunctionBlockWizard(QString functionLibraryRoot, QWidget* parent) : QWizard(parent), m_ui(new Ui::CreateFunctionBlockWizard)
{
    m_ui->setupUi(this);

    m_generalParametersPage = new GeneralParametersWizardPage(m_functionDefinition);
    m_inputSignalsPage = new InputSignalsWizardPage(m_functionDefinition);
    m_outputSignalsPage = new OutputSignalsWizardPage(m_functionDefinition);
    m_configurationParametersPage = new ConfigurationParametersWizardPage(m_functionDefinition);
    m_calibrationParametersPage = new CalibrationParametersWizardPage(m_functionDefinition);
    m_labelMappingsPage = new LabelMappingsWizardPage(m_functionDefinition);
    m_iconResourcesPage = new IconResourcesWizardPage(m_functionDefinition);
    m_imageResourcesPage = new ImageResourcesWizardPage(m_functionDefinition);
    m_faultsConfigurationPage = new FaultsConfigurationWizardPage(m_functionDefinition);
    m_reviewPage = new ReviewFunctionBlockWizardPage(m_functionDefinition, functionLibraryRoot);

    this->addPage(m_generalParametersPage);
    this->addPage(m_inputSignalsPage);
    this->addPage(m_outputSignalsPage);
    this->addPage(m_configurationParametersPage);
    this->addPage(m_calibrationParametersPage);
    this->addPage(m_labelMappingsPage);
    this->addPage(m_iconResourcesPage);
    this->addPage(m_imageResourcesPage);
    this->addPage(m_faultsConfigurationPage);
    this->addPage(m_reviewPage);
}

CreateFunctionBlockWizard::~CreateFunctionBlockWizard()
{
    delete m_generalParametersPage;
    delete m_inputSignalsPage;
    delete m_outputSignalsPage;
    delete m_configurationParametersPage;
    delete m_calibrationParametersPage;
    delete m_labelMappingsPage;
    delete m_iconResourcesPage;
    delete m_imageResourcesPage;
    delete m_faultsConfigurationPage;
    delete m_reviewPage;
    delete m_ui;
}
