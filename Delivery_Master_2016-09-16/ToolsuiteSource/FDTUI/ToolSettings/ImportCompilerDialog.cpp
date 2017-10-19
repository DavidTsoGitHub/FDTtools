#include "ImportCompilerDialog.h"
#include "ui_ImportCompilerDialog.h"

#include <QDialog>
#include <QPushButton>
#include <QWidget>

ImportCompilerDialog::ImportCompilerDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ImportCompilerDialog)
{
    m_ui->setupUi(this);
    m_okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    m_okButton->setEnabled(false);

    connect(m_ui->compilerDataText, &QTextEdit::textChanged, [=] { m_okButton->setEnabled(!m_ui->compilerDataText->toPlainText().isEmpty()); });
}

ImportCompilerDialog::~ImportCompilerDialog()
{
    delete m_ui;
}

QString ImportCompilerDialog::GetCompilerData()
{
    return m_ui->compilerDataText->toPlainText();
}
