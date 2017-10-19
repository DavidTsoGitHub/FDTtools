#include "ExportCompilerDialog.h"
#include "ui_ExportCompilerDialog.h"

#include <QClipboard>
#include <QDialog>

ExportCompilerDialog::ExportCompilerDialog(QString compilerData, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ExportCompilerDialog)
{
    m_ui->setupUi(this);
    m_ui->compilerExportText->setText(compilerData);
    connect(m_ui->buttonCopyToClipboard, &QPushButton::clicked, [=]
    {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(m_ui->compilerExportText->toPlainText());
    });

}

ExportCompilerDialog::~ExportCompilerDialog()
{
    delete m_ui;
}
