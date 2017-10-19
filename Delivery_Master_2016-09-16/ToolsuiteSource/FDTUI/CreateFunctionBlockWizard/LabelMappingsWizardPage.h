#ifndef LABELMAPPINGSWIZARDPAGE_H
#define LABELMAPPINGSWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class LabelMappingsWizardPage;
}

class LabelMappingsWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LabelMappingsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~LabelMappingsWizardPage();

private slots:
    void slot_addLabelMappingsButton_clicked();
    void slot_removeLabelMappingsButton_clicked();


private:
    Ui::LabelMappingsWizardPage *ui;
    FunctionDefinition& m_functionDefinition;

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // LABELMAPPINGSWIZARDPAGE_H
