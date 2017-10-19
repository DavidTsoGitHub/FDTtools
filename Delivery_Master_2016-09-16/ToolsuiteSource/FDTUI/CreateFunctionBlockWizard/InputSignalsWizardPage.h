#ifndef INPUTSIGNALSWIZARDPAGE_H
#define INPUTSIGNALSWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class InputSignalsWizardPage;
}

class InputSignalsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit InputSignalsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~InputSignalsWizardPage();

private slots:
    void slot_addInputSignalButton_clicked();
    void slot_removeInputSignalButton_clicked();

private:
    Ui::InputSignalsWizardPage *ui;

    FunctionDefinition& m_functionDefinition;

    void showError(QString message);
    void hideError();

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // INPUTSIGNALSWIZARDPAGE_H
