#ifndef CREATEFUNCTIONBLOCKWIZARD_H
#define CREATEFUNCTIONBLOCKWIZARD_H

#include "FunctionDefinition.h"

class GeneralParametersWizardPage;
class InputSignalsWizardPage;
class OutputSignalsWizardPage;
class ConfigurationParametersWizardPage;
class CalibrationParametersWizardPage;
class LabelMappingsWizardPage;
class IconResourcesWizardPage;
class ImageResourcesWizardPage;
class FaultsConfigurationWizardPage;
class ReviewFunctionBlockWizardPage;
class QWizard;

namespace Ui
{
class CreateFunctionBlockWizard;
}

class CreateFunctionBlockWizard : public QWizard
{
    Q_OBJECT

public:
    explicit CreateFunctionBlockWizard(QString functionLibraryRoot, QWidget* parent = 0);
    ~CreateFunctionBlockWizard(void);

private:
    FunctionDefinition m_functionDefinition;

    GeneralParametersWizardPage* m_generalParametersPage;
    InputSignalsWizardPage* m_inputSignalsPage;
    OutputSignalsWizardPage* m_outputSignalsPage;
    ConfigurationParametersWizardPage* m_configurationParametersPage;
    CalibrationParametersWizardPage* m_calibrationParametersPage;
    LabelMappingsWizardPage* m_labelMappingsPage;
    IconResourcesWizardPage* m_iconResourcesPage;
    ImageResourcesWizardPage* m_imageResourcesPage;
    FaultsConfigurationWizardPage* m_faultsConfigurationPage;
    ReviewFunctionBlockWizardPage* m_reviewPage;

    Ui::CreateFunctionBlockWizard* m_ui;
};

#endif // CREATEFUNCTIONBLOCKWIZARD_H
