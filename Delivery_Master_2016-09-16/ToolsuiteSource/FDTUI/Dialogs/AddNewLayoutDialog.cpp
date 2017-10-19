#include "AddNewLayoutDialog.h"
#include "ui_AddNewLayoutDialog.h"

#include <QDialog>

AddNewLayoutDialog::AddNewLayoutDialog(LayoutDefinition& layoutDefinition, QWidget* parent) : QDialog(parent),
    m_layoutDefinition(layoutDefinition),
    m_name(""),
    m_ui(new Ui::AddNewLayoutDialog)
{
    this->setModal(true);
    m_ui->setupUi(this);

    if(m_layoutDefinition.HasStartupLayout())
    {
        m_ui->startupRadioButton->setEnabled(false);
    }
    else
    {
        m_ui->startupRadioButton->setEnabled(true);
    }

    if(m_layoutDefinition.HasCheckMessageLayout())
    {
        m_ui->checkMessageRadioButton->setEnabled(false);
    }
    else
    {
        m_ui->checkMessageRadioButton->setEnabled(true);
    }

    connect(m_ui->nameLineEdit, &QLineEdit::textChanged, [=] (const QString text) { m_name = text; });
    m_ui->nameLineEdit->setFocus();
}

AddNewLayoutDialog::~AddNewLayoutDialog()
{
    delete m_ui;
}

QString AddNewLayoutDialog::GetLayoutName()
{
    return m_name;
}

LayoutDefinition::LayoutType AddNewLayoutDialog::GetLayoutType()
{
    if(m_ui->startupRadioButton->isChecked())
    {
        return LayoutDefinition::Startup;
    }
    else if(m_ui->checkMessageRadioButton->isChecked())
    {
        return LayoutDefinition::CheckMessage;
    }
    else
    {
        return LayoutDefinition::Regular;
    }
}

void AddNewLayoutDialog::accept()
{
    if (m_ui->nameLineEdit->text() == "")
    {
        QMessageBox* messageBox = new QMessageBox();
        messageBox->setWindowTitle("Illegal Name");
        messageBox->setText("The name cannot be empty!");
        messageBox->setIcon(QMessageBox::Warning);
        messageBox->setStandardButtons(QMessageBox::Ok);
        messageBox->exec();
    }
    else
    {
        m_name = m_ui->nameLineEdit->text();
        QDialog::accept();
    }
}
