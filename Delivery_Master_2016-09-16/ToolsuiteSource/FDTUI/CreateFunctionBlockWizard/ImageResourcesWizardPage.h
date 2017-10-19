#ifndef IMAGERESOURCESWIZARDPAGE_H
#define IMAGERESOURCESWIZARDPAGE_H

class FunctionDefinition;
class QWizardPage;

namespace Ui {
class ImageResourcesWizardPage;
}

class ImageResourcesWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ImageResourcesWizardPage(FunctionDefinition& functionDefinition, QWidget *parent = 0);
    ~ImageResourcesWizardPage();

private slots:
    void slot_cellChanged(int row, int column);

private:
    void addEmptyRow();
    int getLastRowIndex();
    bool tableContainsValue(QString value, int excludedRowIndex);
    int getRowIndexForValue(QString value);

    Ui::ImageResourcesWizardPage *ui;
    FunctionDefinition& m_functionDefinition;

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent* event);

    // QWizardPage interface
public:
    bool validatePage();
};

#endif // IMAGERESOURCESWIZARDPAGE_H
