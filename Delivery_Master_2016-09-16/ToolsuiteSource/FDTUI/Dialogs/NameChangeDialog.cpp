#include "NameChangeDialog.h"
#include "ui_NameChangeDialog.h"
#include "ProjectValidator.h"

#include <QDialog>
#include <QMessageBox>
#include <QRegExpValidator>

NameChangeDialog::NameChangeDialog(QString* name, QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::NameChangeDialog),
    m_name(name),
    m_cNameRegExpValidator(new QRegExpValidator(ProjectValidator::ValidCIdentifierRegExp()))
{
    m_ui->setupUi(this);
    m_ui->nameLineEdit->setText(*m_name);
    m_ui->nameLineEdit->setMaxLength(31);
    m_ui->nameLineEdit->setValidator(m_cNameRegExpValidator);
}

NameChangeDialog::~NameChangeDialog()
{
    delete m_ui;
    delete m_cNameRegExpValidator;
}

void NameChangeDialog::accept()
{
    QMessageBox* messageBox = new QMessageBox();
    messageBox->setWindowTitle("Illegal Name");
    messageBox->setIcon(QMessageBox::Warning);
    messageBox->setStandardButtons(QMessageBox::Ok);
    if (m_ui->nameLineEdit->text() == "")
    {
        messageBox->setText("The name cannot be empty!");
        messageBox->exec();
    }
    else if (m_ui->nameLineEdit->hasAcceptableInput() == false)
    {
        messageBox->setText("The inputted name is not valid (it probably contains special characters)");
        messageBox->exec();
    }
    else
    {
        *m_name = m_ui->nameLineEdit->text();
        QDialog::accept();
    }
}
