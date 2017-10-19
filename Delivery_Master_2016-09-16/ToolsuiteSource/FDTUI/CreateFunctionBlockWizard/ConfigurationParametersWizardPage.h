#ifndef CONFIGURATIONPARAMETERSWIZARDPAGE_H
#define CONFIGURATIONPARAMETERSWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class ConfigurationParametersWizardPage;
}

class ConfigurationParametersWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConfigurationParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~ConfigurationParametersWizardPage();

private slots:
    void slot_addConfigurationParameterButton_clicked();
    void slot_removeConfigurationParameterButton_clicked();

private:
    Ui::ConfigurationParametersWizardPage *ui;

    FunctionDefinition& m_functionDefinition;

    void showError(QString message);
    void hideError();

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // OUTPUTSIGNALSWIZARDPAGE_H
