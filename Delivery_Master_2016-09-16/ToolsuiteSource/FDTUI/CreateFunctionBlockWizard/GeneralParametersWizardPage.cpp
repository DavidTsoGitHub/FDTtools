#include "GeneralParametersWizardPage.h"
#include "ui_GeneralParametersWizardPage.h"

#include "FunctionDefinition.h"
#include <QRegularExpression>
#include <QWizardPage>

GeneralParametersWizardPage::GeneralParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::GeneralParametersWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    m_validBlockNameRegexp = QRegularExpression("[^\\w_\\.]");

    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &GeneralParametersWizardPage::slot_nameLineEdit_textChanged);
}

GeneralParametersWizardPage::~GeneralParametersWizardPage()
{
    delete ui;
}

void GeneralParametersWizardPage::slot_nameLineEdit_textChanged(const QString& newText)
{
    ui->nameLineEdit->blockSignals(true);

    QString cleanText = newText;
    cleanText.remove(m_validBlockNameRegexp);
    ui->nameLineEdit->setText(cleanText);
    ui->nameLineEdit->blockSignals(false);
    completeChanged();
}

bool GeneralParametersWizardPage::isComplete() const
{
    return !ui->nameLineEdit->text().isEmpty();
}

bool GeneralParametersWizardPage::validatePage()
{
    m_functionDefinition.SetName(ui->nameLineEdit->text());

    m_functionDefinition.ClearTasks();
    Task* task = new Task();
    task->InitFunction = QString(ui->nameLineEdit->text()).append("_Init");
    task->Runnable = QString(ui->nameLineEdit->text()).append("_Run");
    task->Periodicity = ui->periodicitySpinBox->value();
    task->StartupDelay = ui->initialDelaySpinBox->value();
    m_functionDefinition.AddTask(task);

    return true;
}

