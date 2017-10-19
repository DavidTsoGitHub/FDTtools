#ifndef GENERALPARAMETERSWIZARDPAGE_H
#define GENERALPARAMETERSWIZARDPAGE_H

class FunctionDefinition;
class QRegularExpression;
class QWizardPage;

namespace Ui {
class GeneralParametersWizardPage;
}

class GeneralParametersWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit GeneralParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~GeneralParametersWizardPage();

private slots:
    void slot_nameLineEdit_textChanged(const QString& newText);

private:
    Ui::GeneralParametersWizardPage *ui;
    FunctionDefinition& m_functionDefinition;

    QRegularExpression m_validBlockNameRegexp;

    // QWizardPage interface
public:
    bool isComplete() const;
    bool validatePage();
};

#endif // GENERALPARAMETERSWIZARDPAGE_H
