#include "ProjectPropertiesDialog.h"
#include "ui_ProjectPropertiesDialog.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "CANSpecificationDeserializer.h"
#include "ConfigFileReader.h"
#include "CompilerBase.h"
#include "Project.h"
#include "ToolSettings.h"
#include <QClipboard>
#include <QComboBox>
#include <QDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QListView>
#include <QMessageBox>
#include <QString>
#include <QWidget>

ProjectPropertiesDialog::ProjectPropertiesDialog(Project* project, ToolSettings& toolSettings, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProjectPropertiesDialog),
    m_project(project),
    m_targetSettings(project->GetTargetSettings())
{
    m_ui->setupUi(this);

    connect(m_ui->okButton, &QPushButton::clicked, this, &ProjectPropertiesDialog::okButtonClicked);
    connect(m_ui->cancelButton, &QPushButton::clicked, this, &ProjectPropertiesDialog::cancelButtonClicked);

    /* Build Configuration tab */
    populateCompilerComboBox(toolSettings.Compilers, m_ui->simulatorCompilerComboBox, m_targetSettings.GetSimulatorCompilerName());
    populateCompilerComboBox(toolSettings.Compilers, m_ui->targetCompilerComboBox, m_targetSettings.GetTargetCompilerName());
    populateEndianessComboBox(m_ui->targetEndianessComboBox, m_targetSettings.GetTargetEndianess());
    connect(m_ui->simulatorCompilerComboBox, &QComboBox::currentTextChanged, this, &ProjectPropertiesDialog::simulatorCompilerChanged);
    connect(m_ui->targetCompilerComboBox, &QComboBox::currentTextChanged, this, &ProjectPropertiesDialog::targetCompilerChanged);
    connect(m_ui->targetEndianessComboBox, &QComboBox::currentTextChanged, this, &ProjectPropertiesDialog::targetEndianessChanged);

    /* CAN specification tab */
    if(m_project->GetCANSpecificationPath().isEmpty())
    {
        m_ui->ProjectPropertiesTabWidget->setCurrentIndex(0);
        m_ui->ProjectPropertiesTabWidget->setTabEnabled(1,false);
    }
    else
    {
        m_ui->canPathLineEdit->setText(m_project->GetCANSpecificationPath());
        m_ui->ProjectPropertiesTabWidget->setTabEnabled(1,true);
        connect(m_ui->changeCanSpecButton, &QPushButton::clicked, this, &ProjectPropertiesDialog::slot_changeCanSpecificationButton_clicked);
        connect(m_ui->canCopyToClipboardButton, &QPushButton::clicked, this, &ProjectPropertiesDialog::slot_canCopyToClipboardButton_clicked);
    }
}

ProjectPropertiesDialog::~ProjectPropertiesDialog()
{
    delete m_ui;
}

void ProjectPropertiesDialog::populateCompilerComboBox(QList<CompilerBase*>& compilers, QComboBox* combobox, QString currentCompiler)
{
    combobox->addItem("<unset>");
    foreach (const CompilerBase* compiler, compilers)
    {
        combobox->addItem(compiler->GetName());
    }

    if (currentCompiler.isEmpty())
    {
        combobox->setCurrentText("<unset>");
    }
    else
    {
        combobox->setCurrentText(currentCompiler);
    }
}

void ProjectPropertiesDialog::populateEndianessComboBox(QComboBox* combobox, TargetSettings::TargetEndianess currentEndianess)
{
    combobox->addItem("LittleEndian");
    combobox->addItem("BigEndian");

    switch (currentEndianess)
    {
        case TargetSettings::BigEndian:
            combobox->setCurrentText("BigEndian");
            break;
        case TargetSettings::LittleEndian:
        default:
            combobox->setCurrentText("LittleEndian");
            break;

    }
}

void ProjectPropertiesDialog::targetEndianessChanged(const QString& endianessString)
{
    TargetSettings::TargetEndianess endianess = (endianessString.compare("LittleEndian") == 0) ? TargetSettings::LittleEndian : TargetSettings::BigEndian;

    if (endianess == m_targetSettings.GetTargetEndianess()) return;

    m_targetSettings.SetTargetEndianess(endianess);
    m_ui->okButton->setEnabled(true);
}

void ProjectPropertiesDialog::simulatorCompilerChanged(const QString& compiler)
{
    if (compiler.compare(m_targetSettings.GetSimulatorCompilerName()) == 0) return;

    m_targetSettings.SetSimulatorCompilerName(compiler);
    m_ui->okButton->setEnabled(true);
}

void ProjectPropertiesDialog::targetCompilerChanged(const QString& compiler)
{
    if (compiler.compare(m_targetSettings.GetTargetCompilerName()) == 0) return;

    m_targetSettings.SetTargetCompilerName(compiler);
    m_ui->okButton->setEnabled(true);
}

void ProjectPropertiesDialog::slot_canCopyToClipboardButton_clicked(bool)
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(m_ui->canChangeSummaryTextBrowser->toPlainText());
}

void ProjectPropertiesDialog::slot_changeCanSpecificationButton_clicked()
{
    QDir projectPath(m_project->GetProjectRoot());
    QString newCANSourceFileName = QFileDialog::getOpenFileName(this, "Select new CAN specification file", projectPath.absolutePath(), "CAN XML files (*.xml)");

    if (newCANSourceFileName.isEmpty()) return;

    QFileInfo newCanSourceFile(newCANSourceFileName);
    if (m_ui->canPathLineEdit->text().compare(newCanSourceFile.fileName()) == 0) return;


    m_newCANFile = QFileInfo(projectPath.absoluteFilePath(newCanSourceFile.fileName()));

    QFile::copy(newCANSourceFileName, m_newCANFile.absoluteFilePath());

    m_ui->canPathLineEdit->setText(m_newCANFile.fileName());
    m_ui->canPathLineEdit->setToolTip(m_newCANFile.absoluteFilePath());
    parseNewCanSpecification();
}

void ProjectPropertiesDialog::parseNewCanSpecification()
{
    qDebug("%s %s", __FUNCTION__, qUtf8Printable(m_newCANFile.absoluteFilePath()));
    CANSpecification newCanSpecification;
    ConfigFileReader configFileReader;
    CANSpecificationDeserializer* deserializer = new CANSpecificationDeserializer(newCanSpecification);

    if(configFileReader.Deserialize(deserializer, m_newCANFile.absoluteFilePath())!= DeserializerBase::DeserializationOK)
    {
        QMessageBox::critical(this, "Deserialization failed", "Failed to deserialize CAN specification");
        return;
    }

    newCanSpecification.ParseGCLSignals(m_project->GetCANClusterECUName());

    // Sinks
    QList<GCLSignalSink*> newSinks = newCanSpecification.GetSignalSinks();
    QList<GCLSignalSink*> allSinks;
    m_project->GetAllGCLSinks(allSinks);

    // Find all differences
    foreach (GCLSignalSink* oldSink, allSinks)
    {
        if (oldSink->ComponentName() != "CAN")
        {
            allSinks.removeOne(oldSink);
            continue;
        }

        foreach (GCLSignalSink* newSink, newSinks)
        {
            if (newSink->SignalName() == oldSink->SignalName())
            {
                allSinks.removeOne(oldSink);
                newSinks.removeOne(newSink);
            }
        }
    }

    QStringList removedSinks;
    // Unmap all any removed sinks (i.e. not present in new can spec.)
    foreach (const GCLSignalSink* oldSink, allSinks)
    {
        removedSinks.append("- " + oldSink->ComponentName() + "." + oldSink->SignalName());
        GCLSignalSource* source = oldSink->GetSignalSource();
        if (source != NULL)
        {
            removedSinks.append("   - Is mapped to:");
            removedSinks.append("      -" + source->ComponentName() + "." + source->SignalName() + ")" );
        }
    }

    // Sources
    QList<GCLSignalSource*> allSources;
    QList<GCLSignalSource*> newSources = newCanSpecification.GetSignalSources();
    m_project->GetAllGCLSources(allSources);

    foreach (GCLSignalSource* oldSource, allSources)
    {
        if (oldSource->ComponentName() != "CAN")
        {
            allSources.removeOne(oldSource);
            continue;
        }

        foreach (GCLSignalSource* newSource, newSources)
        {
            if (newSource->SignalName() == oldSource->SignalName())
            {
                allSources.removeOne(oldSource);
                newSources.removeOne(newSource);
            }
        }
    }

    QStringList removedSources;
    // Unmap all any removed sources (i.e. not present in new can spec.)
    foreach (const GCLSignalSource* oldSource, allSources)
    {
        removedSources.append("- " + oldSource->ComponentName() + "." + oldSource->SignalName());
        if (oldSource->GetConnectedSinks().count() > 0)
        {
            removedSources.append( + "   - Is mapped to:");

            foreach(GCLSignalSink* sink, oldSource->GetConnectedSinks())
            {
                removedSources.append("      - " + sink->ComponentName() + "." + sink->SignalName() );
            }
        }
    }

    QString oldSpecificationPath = m_project->GetProjectRoot().absoluteFilePath(m_project->GetCANSpecificationPath());
    QStringList output;
    output.append("Old specification: " + oldSpecificationPath);
    output.append("New specification: " + m_newCANFile.absoluteFilePath());
    output.append("");
    output.append("Added sinks:");
    foreach (const GCLSignalSink* newSink, newSinks)
    {
        output.append("- " + newSink->ComponentName() + "." + newSink->SignalName());
    }
    output.append("");
    output.append("Removed sinks:");
    output.append(removedSinks);
    output.append("");
    output.append("Added sources:");
    foreach (const GCLSignalSource* newSource, newSources)
    {
        output.append("- " + newSource->ComponentName() + "." + newSource->SignalName());
    }
    output.append("");
    output.append("Removed sources:");
    output.append(removedSources);

    m_ui->canChangeSummaryTextBrowser->setText(output.join("\n"));
    m_ui->canCopyToClipboardButton->setEnabled(true);
    m_ui->okButton->setEnabled(true);
}

void ProjectPropertiesDialog::okButtonClicked()
{
    updateModel();
    QDialog::accept();
}

void ProjectPropertiesDialog::cancelButtonClicked()
{
    if (!m_ui->okButton->isEnabled())
    {
        QDialog::reject();
        return;
    }

    int result = QMessageBox::warning(this, "Discard changes?", "You have done changes in the project properties. Do you wish to keep, discard or go back to editing?", "Keep", "Discard", "Edit", 0, 2);
    switch (result)
    {
        case 0:
            updateModel();
            QDialog::accept();
            break;
        case 1:
            cleanupCANChanges();
            QDialog::reject();
            break;
        case 2:
        default:
            break;
    }
}

void ProjectPropertiesDialog::updateModel()
{
    if (m_newCANFile.exists() && m_newCANFile.fileName().compare(m_project->GetCANSpecificationPath(), Qt::CaseInsensitive) != 0)
    {
        m_project->UpdateCanSpecification(m_newCANFile.absoluteFilePath());
    }

    m_project->SetTargetSettings(m_targetSettings);
    m_targetSettings.SaveSettings(m_project);
}

void ProjectPropertiesDialog::cleanupCANChanges()
{
    if (m_newCANFile.fileName().compare(m_project->GetCANSpecificationPath(), Qt::CaseInsensitive) != 0)
    {
        QFile file(m_newCANFile.absoluteFilePath());
        if (file.exists())
        {
            file.remove();
        }
    }
}

void ProjectPropertiesDialog::reject()
{
    cancelButtonClicked();
}
