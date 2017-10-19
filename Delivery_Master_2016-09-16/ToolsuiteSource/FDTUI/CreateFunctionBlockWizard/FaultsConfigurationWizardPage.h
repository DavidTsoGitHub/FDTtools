#ifndef FAULTSCONFIGURATIONWIZARDPAGE_H
#define FAULTSCONFIGURATIONWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class FaultsConfigurationWizardPage;
}

class FaultsConfigurationWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit FaultsConfigurationWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~FaultsConfigurationWizardPage();

private slots:
    void slot_cellChanged(int row, int column);

private:
    void addEmptyRow();
    int getLastRowIndex();
    bool tableContainsValue(QString value, int excludedRowIndex);
    int getRowIndexForValue(QString value);
    bool rowIsEmpty(int row);

private:
    Ui::FaultsConfigurationWizardPage *ui;
    FunctionDefinition& m_functionDefinition;

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // FAULTSCONFIGURATIONWIZARDPAGE_H
