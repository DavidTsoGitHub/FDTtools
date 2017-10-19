#include "CompilerSettings.h"
#include "ui_CompilerSettings.h"

#include "CheckBoxWidget.h"
#include "CompilerFactory.h"
#include "DirectorySelectorWidget.h"
#include "ExportCompilerDialog.h"
#include "FileSelectorWidget.h"
#include "ImportCompilerDialog.h"
#include "KeyValueTableWidget.h"
#include "ToolSettings.h"
#include "ToolTipLineEditWidget.h"

#include <QMap>
#include <QMessageBox>
#include <QWidget>

CompilerSettings::CompilerSettings(ToolSettings& toolSettings, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CompilerSettings),
    m_settings(toolSettings),
    m_currentCompiler(NULL),
    m_unsavedChanges(false)
{
    m_ui->setupUi(this);
    m_environmentVariablesTable = new KeyValueTableWidget();
    m_environmentVariablesTable->setEnabled(false);
    m_ui->environmentVariablesLayout->addWidget(m_environmentVariablesTable);

    connectUISignals();
    populateFields();
}

CompilerSettings::~CompilerSettings()
{
    delete m_ui;
}

void CompilerSettings::SetUnsavedChanges(bool unsavedChanges)
{
    m_unsavedChanges = unsavedChanges;
}

bool CompilerSettings::HasUnsavedChanges()
{
    return m_unsavedChanges;
}

void CompilerSettings::SaveChanges()
{
    m_ui->compilersList->blockSignals(true);
    slot_saveCompilerButton_clicked();
    m_ui->compilersList->blockSignals(false);
}

void CompilerSettings::connectUISignals()
{
    connect(m_ui->compilersList, &QListWidget::currentTextChanged, this, &CompilerSettings::slot_compilersList_currentTextChanged);
    connect(m_ui->compilerName, &QLineEdit::textChanged, this, &CompilerSettings::slot_enableSave);
    connect(m_ui->saveCompilerButton, &QAbstractButton::clicked, this, &CompilerSettings::slot_saveCompilerButton_clicked);
    connect(m_ui->removeCompilerButton, &QAbstractButton::clicked, this, &CompilerSettings::slot_removeCompilerButton_clicked);
    connect(m_ui->importButton, &QAbstractButton::clicked, this, &CompilerSettings::slot_importButton_clicked);
    connect(m_ui->exportButton, &QAbstractButton::clicked, this, &CompilerSettings::slot_exportButton_clicked);
    connect(m_ui->addCompilerComboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &CompilerSettings::slot_addCompilerComboBox_currentIndexChanged);
    connect(m_environmentVariablesTable, &KeyValueTableWidget::signal_DataChanged, this, &CompilerSettings::slot_enableSave);
}

void CompilerSettings::populateFields()
{
    m_ui->addCompilerComboBox->addItem("Add compiler");
    foreach (CompilerBase* compiler, CompilerFactory::GetAvailableCompilers())
    {
        m_ui->addCompilerComboBox->addItem(compiler->GetTypeString());
    }


    refreshCompilerList();
}

void CompilerSettings::setCompilerData(CompilerBase* compiler)
{
    m_currentCompiler = compiler;

    if (compiler == NULL)
    {
        setCompilerFieldsEnabled(false);
        return;
    }
    else
    {
        setCompilerFieldsEnabled(true);
    }

    m_ui->compilerTypeLabel->setText(compiler->GetTypeString());
    m_ui->compilerName->blockSignals(true);
    m_ui->compilerName->setText(compiler->GetName());
    m_ui->compilerName->blockSignals(false);

    const QMap<QString, QString>& envVars(compiler->GetEnvironmentVariables());
    m_environmentVariablesTable->SetData(envVars);

    m_ui->compilerSpecificSettingsTable->setRowCount(0);
    m_compilerAttributeMapping.clear();

    foreach (CompilerBase::CompilerAttribute attribute, compiler->GetCompilerAttributes())
    {
        QWidget* widget;

        switch (attribute.Type)
        {
            case CompilerBase::Text:
            {
                ToolTipLineEditWidget* lineEdit = new ToolTipLineEditWidget(compiler->GetCustomAttribute(attribute.Name), this);
                connect(lineEdit, &QLineEdit::textChanged, this, &CompilerSettings::slot_enableSave);
                widget = lineEdit;
                break;
            }
            case CompilerBase::Directory:
            {
                DirectorySelectorWidget* directorySelector = new DirectorySelectorWidget(compiler->GetCustomAttribute(attribute.Name), this);
                connect(directorySelector, &DirectorySelectorWidget::signal_DirectoryPathChanged, this, &CompilerSettings::slot_enableSave);
                widget = directorySelector;
                break;
            }
            case CompilerBase::File:
            {
                FileSelectorWidget* fileSelector = new FileSelectorWidget(compiler->GetCustomAttribute(attribute.Name), this);
                connect(fileSelector, &FileSelectorWidget::signal_FilePathChanged, this, &CompilerSettings::slot_enableSave);
                widget = fileSelector;
                break;
            }
            case CompilerBase::Checkbox:
            {
                CheckBoxWidget* checkBox = new CheckBoxWidget(compiler->GetCustomAttribute(attribute.Name).toUInt());
                connect(checkBox, &CheckBoxWidget::signal_ValueChanged, this, &CompilerSettings::slot_enableSave);
                widget = checkBox;
                break;
            }
            default: // Should never be default
                widget = new QLabel("invalid type", this);
                break;
        }

        int newRowIndex = m_ui->compilerSpecificSettingsTable->rowCount();
        m_compilerAttributeMapping[newRowIndex] = attribute;

        m_ui->compilerSpecificSettingsTable->insertRow(newRowIndex);

        m_ui->compilerSpecificSettingsTable->setItem(newRowIndex, 0, new QTableWidgetItem(attribute.ShortDescription));
        m_ui->compilerSpecificSettingsTable->setCellWidget(newRowIndex, 1, widget);

    }
    m_ui->compilerSpecificSettingsTable->resizeColumnToContents(0);
}

void CompilerSettings::refreshCompilerList()
{
    m_ui->compilersList->clear();
    foreach (CompilerBase* compiler, m_settings.Compilers)
    {
        m_ui->compilersList->addItem(compiler->GetName());
    }
}

void CompilerSettings::populateCurrentCompilerWithFieldValues()
{
    m_currentCompiler->SetName(m_ui->compilerName->text());

    QMapIterator<QString, QString> iter(m_environmentVariablesTable->GetData());
    while (iter.hasNext())
    {
        iter.next();
        m_currentCompiler->AddEnvironmentVariable(iter.key(), iter.value());
    }

    for (int i=0; i<m_ui->compilerSpecificSettingsTable->rowCount(); ++i)
    {
        CompilerBase::CompilerAttribute attribute = m_compilerAttributeMapping[i];
        QString attributeName = attribute.Name;
        QString attributeValue;
        QWidget* valueWidget = m_ui->compilerSpecificSettingsTable->cellWidget(i, 1);

        switch (attribute.Type)
        {
            case CompilerBase::Text:
            {
                ToolTipLineEditWidget* lineEdit = static_cast<ToolTipLineEditWidget*>(valueWidget);
                attributeValue = lineEdit->text();
                break;
            }
            case CompilerBase::File:
            {
                FileSelectorWidget* fileSelector = static_cast<FileSelectorWidget*>(valueWidget);
                attributeValue = fileSelector->GetPath();
                break;
            }
            case CompilerBase::Directory:
            {
                DirectorySelectorWidget* directorySelector = static_cast<DirectorySelectorWidget*>(valueWidget);
                attributeValue = directorySelector->GetPath();
                break;
            }
            case CompilerBase::Checkbox:
            {
                CheckBoxWidget* checkBox = static_cast<CheckBoxWidget*>(valueWidget);
                attributeValue = QString::number(checkBox->isChecked());
                break;
            }
            case CompilerBase::BSPPropertyExternalMemoryAddress:
                // Fall through
            default:
                break;
        }

        m_currentCompiler->SetCustomAttribute(attributeName, attributeValue);
    }
}

void CompilerSettings::setCompilerFieldsEnabled(bool enabled)
{
    m_ui->compilerName->setEnabled(enabled);
    m_ui->compilerSpecificSettingsTable->setEnabled(enabled);
    m_environmentVariablesTable->setEnabled(enabled);
}

bool CompilerSettings::compilerExistsInList(QString compilerName)
{
    for (int i=0; i<m_ui->compilersList->count(); ++i)
    {
        QListWidgetItem* item = m_ui->compilersList->item(i);
        if (item->text() == compilerName)
        {
            return true;
        }
    }

    return false;
}

void CompilerSettings::slot_addCompilerComboBox_currentIndexChanged(QString compilerType)
{
    CompilerBase* compilerSkeleton = CompilerFactory::GetCompilerSkeleton(compilerType);

    if (compilerSkeleton != NULL)
    {
        setCompilerData(compilerSkeleton);
        m_ui->saveCompilerButton->setEnabled(true);
        m_ui->saveCompilerButton->setText("Save");
    }

    m_ui->addCompilerComboBox->setCurrentIndex(0);
}

void CompilerSettings::slot_compilersList_currentTextChanged(QString currentItemText)
{
    if (HasUnsavedChanges())
    {
        m_ui->compilersList->blockSignals(true);
        QMessageBox msgBox;
        msgBox.setText("There are unsaved changes!");
        msgBox.setInformativeText("Would you like to save the changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            slot_saveCompilerButton_clicked();
        }
        m_ui->compilersList->blockSignals(false);
    }
    m_ui->saveCompilerButton->setEnabled(false);

    if (currentItemText.isEmpty())
    {
        setCompilerData(NULL);
        m_ui->removeCompilerButton->setEnabled(false);
        return;
    }

    m_ui->removeCompilerButton->setEnabled(true);

    foreach (CompilerBase* compiler, m_settings.Compilers)
    {
        if (compiler->GetName() == currentItemText)
        {
            m_ui->saveCompilerButton->setText("Update");
            m_ui->exportButton->setEnabled(true);
            setCompilerData(compiler);
            return;
        }
    }
}

void CompilerSettings::slot_saveCompilerButton_clicked()
{
    if (m_ui->compilerName->text().isEmpty()) return;

    m_ui->compilersList->blockSignals(true);

    int currentListIndex = m_ui->compilersList->currentRow();

    if (m_ui->saveCompilerButton->text().compare("Update") == 0)
    {
        QString compilerToUpdate = m_currentCompiler->GetName();
        populateCurrentCompilerWithFieldValues();
        m_settings.UpdateCompiler(compilerToUpdate, m_currentCompiler->Serialize());
    }
    else if (m_ui->saveCompilerButton->text().compare("Save") == 0)
    {
        populateCurrentCompilerWithFieldValues();
        m_settings.AddCompiler(m_currentCompiler->Serialize());
        currentListIndex = m_settings.Compilers.count() - 1;
        m_ui->saveCompilerButton->setText("Update");
    }

    refreshCompilerList();
    m_ui->compilersList->setCurrentRow(currentListIndex);
    m_ui->saveCompilerButton->setEnabled(false);
    SetUnsavedChanges(false);
    m_ui->compilersList->blockSignals(false);
}

void CompilerSettings::slot_removeCompilerButton_clicked()
{
    QListWidgetItem* selectedItem = m_ui->compilersList->currentItem();

    if (selectedItem == NULL)
    {
        return;
    }

    m_settings.RemoveCompiler(selectedItem->text());
    delete selectedItem;

    if(m_ui->compilersList->count() == 0)
    {
        setCompilerFieldsEnabled(false);
        m_ui->compilerName->clear();
        m_ui->compilerTypeLabel->clear();
        m_ui->compilerSpecificSettingsTable->setRowCount(0);
        m_environmentVariablesTable->setRowCount(0);
    }
}

void CompilerSettings::slot_importButton_clicked()
{
    ImportCompilerDialog importDialog;
    QDialog::DialogCode result = static_cast<QDialog::DialogCode>(importDialog.exec());
    if (result == QDialog::Accepted)
    {
        QString compilerData = importDialog.GetCompilerData();

        CompilerBase* compiler = CompilerFactory::GetCompiler(compilerData);
        if (compiler != NULL)
        {
            setCompilerData(compiler);
            if (compilerExistsInList(compiler->GetName()))
            {
                m_ui->saveCompilerButton->setText("Update");
            }
            else
            {
                m_ui->saveCompilerButton->setText("Save");
            }

            m_ui->saveCompilerButton->setEnabled(true);
        }
        else
        {
            QMessageBox::warning(this, "Invalid Compiler XML", "The provided compiler XML was invalid!");
        }
    }
}

void CompilerSettings::slot_exportButton_clicked()
{
    ExportCompilerDialog exportDialog(m_currentCompiler->Serialize(), this);
    exportDialog.exec();
}

void CompilerSettings::slot_enableSave()
{
    m_ui->saveCompilerButton->setEnabled(true);
    SetUnsavedChanges(true);
}
