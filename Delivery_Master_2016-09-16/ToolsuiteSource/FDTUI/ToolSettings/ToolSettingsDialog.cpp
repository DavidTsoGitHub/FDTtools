#include "ToolSettingsDialog.h"
#include "ui_ToolSettingsDialog.h"

#include "CompilerSettings.h"
#include "ToolSettings.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

ToolSettingsDialog::ToolSettingsDialog(ToolSettings& toolSettings, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ToolSettingsDialog),
    m_settings(toolSettings),
    m_compilerSettings(new CompilerSettings(m_settings))
{
    m_ui->setupUi(this);

    m_ui->compilerSettingsTab->layout()->addWidget(m_compilerSettings);

    m_okButton = m_ui->buttonBox->button(QDialogButtonBox::Ok);

    connectUISignals();
    populateFields();
}

ToolSettingsDialog::~ToolSettingsDialog()
{
    delete m_ui;
}

void ToolSettingsDialog::connectUISignals()
{
    connect(m_ui->functionLibraryBrowseButton, &QAbstractButton::clicked,  this, &ToolSettingsDialog::slot_functionLibraryBrowseButton_clicked);
    connect(m_ui->platformGeneratorBrowseButton, &QAbstractButton::clicked, this, &ToolSettingsDialog::slot_platformGeneratorBrowseButton_clicked);
    connect(m_ui->simulatorBrowseButton, &QAbstractButton::clicked, this, &ToolSettingsDialog::slot_simulatorBrowseButton_clicked);
    connect(m_ui->builderBrowseButton, &QAbstractButton::clicked, this, &ToolSettingsDialog::slot_builderBrowseButton_clicked);

    connect(m_ui->functionLibraryLineEdit, &QLineEdit::textChanged, [=] (const QString& text) { m_settings.FunctionLibraryPath = text; });
    connect(m_ui->platformGeneratorLineEdit, &QLineEdit::textChanged, [=] (const QString& text) { m_settings.PlatformGeneratorFilePath = text; });
    connect(m_ui->simulatorLineEdit, &QLineEdit::textChanged, [=] (const QString& text) { m_settings.SimulatorFilePath = text; });
    connect(m_ui->builderLineEdit, &QLineEdit::textChanged, [=] (const QString& text) { m_settings.BuilderFilePath = text; });

    connect(m_okButton, &QAbstractButton::clicked, this, &ToolSettingsDialog::slot_buttonBoxOk_clicked);

}

void ToolSettingsDialog::populateFields()
{
    // FunctionLibraryPath
    if (m_settings.FunctionLibraryPath.isEmpty() || !QDir(m_settings.FunctionLibraryPath).exists())
    {
        m_ui->functionLibraryLineEdit->setText("");
    }
    else
    {
        m_ui->functionLibraryLineEdit->setText(m_settings.FunctionLibraryPath);
    }

    // PlatformGeneratorFilePath
    if (m_settings.PlatformGeneratorFilePath.isEmpty() || !QFile(m_settings.PlatformGeneratorFilePath).exists())
    {
        m_ui->platformGeneratorLineEdit->setText("");
    }
    else
    {
        m_ui->platformGeneratorLineEdit->setText(m_settings.PlatformGeneratorFilePath);
    }

    // SimulatorFilePath
    if (m_settings.SimulatorFilePath.isEmpty() || !QFile(m_settings.SimulatorFilePath).exists())
    {
        m_ui->simulatorLineEdit->setText("");
    }
    else
    {
        m_ui->simulatorLineEdit->setText(m_settings.SimulatorFilePath);
    }


    // BuilderFilePath
    if (m_settings.BuilderFilePath.isEmpty() || !QFile(m_settings.BuilderFilePath).exists())
    {
        m_ui->builderLineEdit->setText("");
    }
    else
    {
        m_ui->builderLineEdit->setText(m_settings.BuilderFilePath);
    }

}

void ToolSettingsDialog::slot_functionLibraryBrowseButton_clicked(bool /*checked*/)
{
    QString functionLibraryPath = QFileDialog::getExistingDirectory(this, "Set Function Library Path", m_settings.FunctionLibraryPath);
    if (functionLibraryPath.isEmpty())
    {
        return;
    }

    m_ui->functionLibraryLineEdit->setText(functionLibraryPath);
    m_settings.FunctionLibraryPath = functionLibraryPath;
}

void ToolSettingsDialog::slot_platformGeneratorBrowseButton_clicked(bool /*checked*/)
{
    QDir oldPlatformGeneratorPath(m_settings.PlatformGeneratorFilePath);
#ifdef _WIN32
    QString platformFileGeneratorPath = QFileDialog::getOpenFileName(this, "Path to platform generator", oldPlatformGeneratorPath.absolutePath(), "platformgenerator.exe");
#else
    QString platformFileGeneratorPath = QFileDialog::getOpenFileName(this, "Path to platform generator", oldPlatformGeneratorPath.absolutePath(), "platformgenerator");
#endif
    if (platformFileGeneratorPath.isEmpty())
    {
        return;
    }

    m_ui->platformGeneratorLineEdit->setText(platformFileGeneratorPath);
    m_settings.PlatformGeneratorFilePath = platformFileGeneratorPath;
}

void ToolSettingsDialog::slot_simulatorBrowseButton_clicked(bool /*checked*/)
{
    QDir oldsimulatorPath(m_settings.SimulatorFilePath);
#ifdef _WIN32
    QString simulatorFilePath = QFileDialog::getOpenFileName(this, "Path to simulator file", oldsimulatorPath.absolutePath(), "simulator.exe");
#else
    QString simulatorFilePath = QFileDialog::getOpenFileName(this, "Path to simulator file", oldsimulatorPath.absolutePath(), "simulator");
#endif
    if(simulatorFilePath.isEmpty())
    {
        return;
    }
    m_ui->simulatorLineEdit->setText(simulatorFilePath);
    m_settings.SimulatorFilePath = simulatorFilePath;
}

void ToolSettingsDialog::slot_builderBrowseButton_clicked(bool /*checked*/)
{
    QDir oldBuilderPath(m_settings.BuilderFilePath);

#ifdef _WIN32
    QString builderPath = QFileDialog::getOpenFileName(this, "Path to builder", oldBuilderPath.absolutePath(), "builder.exe");
#else
    QString builderPath = QFileDialog::getOpenFileName(this, "Path to builder", oldBuilderPath.absolutePath(), "builder");
#endif
    if(builderPath.isEmpty())
    {
        return;
    }
    m_ui->builderLineEdit->setText(builderPath);
    m_settings.BuilderFilePath = builderPath;
}

void ToolSettingsDialog::slot_buttonBoxOk_clicked(bool /*checked*/)
{
    if (m_compilerSettings->HasUnsavedChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("There are unsaved compiler settings!");
        msgBox.setInformativeText("Do you want to save the changes before exiting?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            m_compilerSettings->SaveChanges();
        }
    }
}
