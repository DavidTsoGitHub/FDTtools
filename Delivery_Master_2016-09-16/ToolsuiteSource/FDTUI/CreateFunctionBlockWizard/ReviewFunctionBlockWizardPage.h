#ifndef REVIEWFUNCTIONBLOCKWIZARDPAGE_H
#define REVIEWFUNCTIONBLOCKWIZARDPAGE_H

class FunctionDefinition;
class QDir;
class QWizardPage;

namespace Ui {
class ReviewFunctionBlockWizardPage;
}

class ReviewFunctionBlockWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ReviewFunctionBlockWizardPage(FunctionDefinition& functionDefinition, QString functionLibraryRoot, QWidget *parent = 0);
    ~ReviewFunctionBlockWizardPage();

private:
    Ui::ReviewFunctionBlockWizardPage *ui;

    const FunctionDefinition& m_functionDefinition;
    QDir m_functionLibraryRoot;

    bool writeFunctionDefinitionFile(QDir& rootDir);
    bool writeExampleSourceFile(QDir& rootDir);
    bool writeExampleHeaderFile(QDir& rootDir);

    // QWizardPage interface
public:
    void initializePage();
    bool validatePage();
};

#endif // REVIEWFUNCTIONBLOCKWIZARDPAGE_H
