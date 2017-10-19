#include "ImagesInUseDialog.h"
#include "ui_ImagesInUseDialog.h"

#include <QItemSelection>

ImagesInUseDialog::ImagesInUseDialog(QString content, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImagesInUseDialog)
{
    ui->setupUi(this);

    ui->fileInfoTextBrowser->setText(content);
}

ImagesInUseDialog::~ImagesInUseDialog()
{
    delete ui;
}
