#ifndef ICONRESOURCESWIZARDPAGE_H
#define ICONRESOURCESWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class IconResourcesWizardPage;
}

class IconResourcesWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit IconResourcesWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~IconResourcesWizardPage();

private slots:
    void slot_cellChanged(int row, int column);

private:
    void addEmptyRow();
    int getLastRowIndex();
    bool tableContainsValue(QString value, int excludedRowIndex);
    int getRowIndexForValue(QString value);

    Ui::IconResourcesWizardPage *ui;
    FunctionDefinition& m_functionDefinition;

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent* event);

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // ICONRESOURCESWIZARDPAGE_H
