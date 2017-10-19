#include "Splash.h"
#include "ui_Splash.h"
#include "CreateProjectWizard.h"

#include <QFileDialog>
#include <QDialog>
#include <QWidget>

Splash::Splash(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::Splash)
{
    m_ui->setupUi(this);

    connect(m_ui->btnOpen, &QAbstractButton::clicked, this, &Splash::slot_btnOpenClicked);
    connect(m_ui->btnCreate, &QAbstractButton::clicked, this, &Splash::slot_btnCreateClicked);
}

Splash::~Splash()
{
    delete m_ui;
}

void Splash::slot_btnOpenClicked()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open FDT Project"), "c:\\", tr("FDT Project File (*.fdt)"));
    if(!m_fileName.isEmpty())
    {
        this->accept();
    }
}

void Splash::slot_btnCreateClicked()
{
    this->setVisible(false);
    CreateProjectWizard wizard;

    DialogCode result = (DialogCode)wizard.exec();

    if (result == Rejected)
    {
        this->setVisible(true);
    }
    else
    {
        m_fileName = wizard.GetProjectFilePath();
        this->accept();
    }
}

QString Splash::GetFilePath()
{
    return m_fileName;
}
