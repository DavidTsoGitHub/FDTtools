#ifndef OUTPUTSIGNALSWIZARDPAGE_H
#define OUTPUTSIGNALSWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class OutputSignalsWizardPage;
}

class OutputSignalsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit OutputSignalsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~OutputSignalsWizardPage();

private slots:
    void slot_addOutputSignalButton_clicked();
    void slot_removeOutputSignalButton_clicked();

private:
    Ui::OutputSignalsWizardPage *ui;

    FunctionDefinition& m_functionDefinition;

    void showError(QString message);
    void hideError();

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // OUTPUTSIGNALSWIZARDPAGE_H
