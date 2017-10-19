#ifndef CALIBRATIONPARAMETERSWIZARDPAGE_H
#define CALIBRATIONPARAMETERSWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class CalibrationParametersWizardPage;
}

class CalibrationParametersWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit CalibrationParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~CalibrationParametersWizardPage();

private slots:
    void slot_addCalibrationParameterButton_clicked();
    void slot_removeCalibrationParameterButton_clicked();
    void slot_calibrationParametersTable_cellChanged(int row, int column);

private:
    Ui::CalibrationParametersWizardPage *ui;

    FunctionDefinition& m_functionDefinition;

    void showError(QString message);
    void hideError();

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // CALIBRATIONPARAMETERSWIZARDPAGE_H
