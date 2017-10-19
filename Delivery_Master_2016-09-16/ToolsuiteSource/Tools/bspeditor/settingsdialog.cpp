#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // Default/custom schema selection
    connect(ui->radioButtonCustomXMLSchema, &QAbstractButton::toggled,
            this, &SettingsDialog::setCustomSchemaEnabled);
    ui->radioButtonDefaultXMLSchema->setChecked(useDefaultSchema());
    setCustomSchemaEnabled(!useDefaultSchema());

    // Custom schema file
    ui->lineEditCustomXMLSchema->setText(customSchema());
    connect(ui->pushButtonXMLSchema, &QAbstractButton::clicked,
            this, &SettingsDialog::selectCustomSchema);

    // Open recent file
    ui->checkBoxOpenRecent->setChecked(autoOpenRecent());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

bool SettingsDialog::useDefaultSchema() const
{
    QSettings settings;
    return settings.value("useDefaultSchema", true).toBool();
}

QString SettingsDialog::customSchema() const
{
    QSettings settings;
    return settings.value("customSchemaFile").toString();
}

bool SettingsDialog::autoOpenRecent() const
{
    QSettings settings;
    return settings.value("autoOpenRecent", true).toBool();
}

void SettingsDialog::setCustomSchemaEnabled(bool enabled)
{
    ui->radioButtonCustomXMLSchema->setChecked(enabled);
    ui->lineEditCustomXMLSchema->setEnabled(enabled);
    ui->pushButtonXMLSchema->setEnabled(enabled);
}

void SettingsDialog::selectCustomSchema()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select BSP spec XML Schema"), customSchema(), tr("XML Schema (*.xsd)"));

    if(fileName.isEmpty())
        return;

    ui->lineEditCustomXMLSchema->setText(fileName);
}

void SettingsDialog::accept()
{
    QSettings settings;
    settings.setValue("useDefaultSchema", ui->radioButtonDefaultXMLSchema->isChecked());
    settings.setValue("customSchemaFile", ui->lineEditCustomXMLSchema->text());
    settings.setValue("autoOpenRecent", ui->checkBoxOpenRecent->isChecked());

    QDialog::accept();
}
