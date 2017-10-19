#include "LabelEditor.h"
#include "ui_LabelEditor.h"

#include "AddLanguageDialog.h"
#include "bspmemory.h"
#include "FontCheckBoxProxyModel.h"
#include "FontComboBoxDelegate.h"
#include "FontModel.h"
#include "LabelModel.h"
#include "LabelProxyModel.h"
#include "LabelObjectVisitor.h"
#include "Project.h"
#include "ProjectObject.h"
#include "LineSpacingSpinboxDelegate.h"

#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QItemSelection>
#include <QTableWidgetItem>
#include <QTimer>
#include <QItemSelectionRange>
#include <QSortFilterProxyModel>
#include <QString>
#include <QStringList>
#include <QWidget>

//typedef enum
//{
//    DescriptionColumn,
//    MasterTextColumn,
//    LabelEditorStandardColumnCount
//} LabelEditorStandardColumns;

LabelEditor::LabelEditor(QWidget* parent) : QWidget(parent),
    m_ui(new Ui::LabelEditor),
    m_labelModel(NULL),
    m_labelProxyModel(NULL),
    m_fontModel(NULL),
    m_updateMemoryUsageTimer(new QTimer(this)),
    m_tabShowing(true),
    m_refreshNeeded(true)
{
    m_ui->setupUi(this);
    setupSignals();

    m_updateMemoryUsageTimer->setSingleShot(true);
    connect(m_updateMemoryUsageTimer, &QTimer::timeout, [=] ()
    {
        slot_updateMemoryUsageButtonClicked();
    });
}

LabelEditor::~LabelEditor()
{
    delete m_ui;
}

void LabelEditor::SetProject(Project* project)
{
    m_project = project;
    m_projectRoot = project->GetProjectRoot();

    /* Labels tab */
    m_labelModel = new LabelModel(project->GetLabelDefinition());
    m_labelProxyModel = new LabelProxyModel(m_labelModel);

    m_labelProxyModel->setSourceModel(m_labelModel);
    m_labelProxyModel->setFilterRole(Qt::DisplayRole);
    m_labelProxyModel->SetFilter(m_ui->m_labelSearchLineEdit->text());

    m_ui->labelTable->setModel(m_labelProxyModel);
    m_ui->labelTable->setSortingEnabled(true);
    m_ui->labelTable->resizeColumnsToContents();
    m_ui->labelTable->verticalHeader()->hide();

    m_ui->labelTable->setItemDelegate(new FontComboBoxDelegate(project->GetLabelDefinition()));
    m_ui->labelTable->setColumnWidth(LabelModel::IDColumn,40);
    m_ui->labelTable->setColumnWidth(LabelModel::DescriptionColumn,300);
    m_ui->labelTable->setColumnWidth(LabelModel::MasterTextColumn,300);

    /* Font configuration tab */
    m_fontModel = new FontModel(project->GetLabelDefinition());

    m_ui->fontConfigurationTable->setModel(m_fontModel);
    m_ui->fontConfigurationTable->resizeColumnsToContents();
    m_ui->defaultFontComboBox->setModel(m_fontModel);
    m_ui->fontConfigurationTable->setItemDelegateForColumn(2, new LineSpacingSpinboxDelegate());

    if (project->GetLabelDefinition().GetDefaultFont() != NULL)
    {
        m_ui->defaultFontComboBox->setCurrentText(project->GetLabelDefinition().GetDefaultFont()->BDFPath);
    }
    m_ui->mandatoryGlyphsTextEdit->setPlainText(project->GetLabelDefinition().GetMandatoryCharacters());

    calculateMemoryUsage();

    /* Signals */
    connect(m_ui->labelTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LabelEditor::slot_labelSelectionChanged);
    connect(m_ui->fontConfigurationTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LabelEditor::slot_fontSelectionChanged);
    connect(m_ui->defaultFontComboBox, &QComboBox::currentTextChanged, this, &LabelEditor::slot_defaultFontComboBoxSelectionChanged);
    connect(m_ui->mandatoryGlyphsTextEdit, &QPlainTextEdit::textChanged, this, &LabelEditor::slot_mandatoryGlyphsTextEdit_changed);
    connect(m_ui->mandatoryGlyphsTextEdit, &QPlainTextEdit::textChanged, this, &LabelEditor::startTimerUpdateMemoryUsage);

    connect(m_labelModel, &LabelModel::dataChanged, [=] {
        QModelIndex currentIndex = m_ui->labelTable->currentIndex();
        m_ui->labelTable->scrollTo(currentIndex);
    });
    connect(m_labelModel, &LabelModel::signal_modified, this, &LabelEditor::signal_modified);
    connect(m_fontModel, &FontModel::signal_modified, this, &LabelEditor::signal_modified);

    connect(m_ui->m_labelSearchLineEdit, &QLineEdit::textChanged, m_labelProxyModel, &LabelProxyModel::SetFilter);

    connect(&m_project->GetLabelDefinition(), &LabelDefinition::signal_CalculateFontMemoryUsage, [=] { m_tabShowing ? calculateMemoryUsage() : setRefreshNeeded(); });
    connect(&m_project->GetImageDefinition(), &ImageDefinition::signal_CalculateImageMemoryUsage, [=] { m_tabShowing ? calculateMemoryUsage() : setRefreshNeeded(); });
    connect(&m_project->GetLabelDefinition(), &LabelDefinition::signal_FontRelocatedInMemory, [=] { m_tabShowing ? calculateMemoryUsage() : setRefreshNeeded(); });

    connect(&m_project->GetLabelDefinition(), &LabelDefinition::signal_FontRemoved, this, &LabelEditor::slot_removeFontFromLabels);
    connect(&m_project->GetLabelDefinition(), &LabelDefinition::signal_CalculationDone, [=] {
        calculateMemoryUsage();
    });
}

void LabelEditor::startTimerUpdateMemoryUsage()
{
    m_updateMemoryUsageTimer->start(1000);
}

void LabelEditor::calculateMemoryUsage()
{
    quint32 fontSizeInInternalMemory = m_project->GetLabelDefinition().GetFontSizeInInternalFlash();
    quint32 fontSizeInExternalMemory = m_project->GetLabelDefinition().GetFontSizeInExternalFlash();

    m_ui->fontsInInternalMemoryLabel->setText("~" + QString::number(fontSizeInInternalMemory) + " bytes");
    m_ui->fontsInExternalMemoryLabel->setText("~" + QString::number(fontSizeInExternalMemory) + " bytes");

    quint32 totalSizeInInternalFlash = fontSizeInInternalMemory + m_project->GetImageDefinition().GetImageSizeInInternalFlash();
    quint32 totalSizeInExternalFlash = fontSizeInExternalMemory + m_project->GetImageDefinition().GetImageSizeInExternalFlash();

    quint32 internalMemorySize = m_project->GetBSPSpecification().GetMemory()->GetInternalMemorySize();
    quint32 externalMemorySize = m_project->GetBSPSpecification().GetMemory()->GetExternalMemorySize();

    m_ui->internalMemoryUsageLabel->setText("~" + QString::number(totalSizeInInternalFlash) + " / " + QString::number(internalMemorySize));
    m_ui->externalMemoryUsageLabel->setText("~" + QString::number(totalSizeInExternalFlash) + " / " + QString::number(externalMemorySize));
}

void LabelEditor::ClearProject()
{
    m_project = NULL;
    delete m_labelModel;
    m_labelModel = NULL;
    delete m_fontModel;
    m_fontModel = NULL;

    disconnect(m_ui->labelTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LabelEditor::slot_labelSelectionChanged);
    disconnect(m_ui->fontConfigurationTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LabelEditor::slot_fontSelectionChanged);
    disconnect(m_ui->defaultFontComboBox, &QComboBox::currentTextChanged, this, &LabelEditor::slot_defaultFontComboBoxSelectionChanged);
    disconnect(m_ui->mandatoryGlyphsTextEdit, &QPlainTextEdit::textChanged, this, &LabelEditor::slot_mandatoryGlyphsTextEdit_changed);
}

void LabelEditor::JustSelected()
{
    m_tabShowing = true;
    if (m_refreshNeeded)
    {
        calculateMemoryUsage();
    }
}

void LabelEditor::JustDeselected()
{
    m_tabShowing = false;
}


void LabelEditor::setRefreshNeeded()
{
    m_refreshNeeded = true;
}

void LabelEditor::setupSignals()
{
    connect(m_ui->removeLabelsButton, &QPushButton::clicked, this, &LabelEditor::slot_removeSelectedLabels);
    connect(m_ui->addLanguageButton, &QPushButton::pressed, this, &LabelEditor::slot_addLanguageButton_pressed);
    connect(m_ui->addFontButton, &QPushButton::clicked, this, &LabelEditor::slot_addFontButton_clicked);
    connect(m_ui->removeFontButton, &QPushButton::clicked, this, &LabelEditor::slot_removeSelectedFonts);
    connect(m_ui->replaceFontButton, &QPushButton::clicked, this, &LabelEditor::slot_replaceFontButton_clicked);
    connect(m_ui->addLabelButton, &QPushButton::clicked, this, &LabelEditor::slot_addLabelButton_clicked);
}

/* Labels */
void LabelEditor::slot_labelSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
    m_ui->removeLabelsButton->setEnabled(m_ui->labelTable->selectionModel()->hasSelection());
}

void LabelEditor::slot_removeSelectedLabels()
{
    const QItemSelection selection = m_ui->labelTable->selectionModel()->selection();

    foreach (QItemSelectionRange range, selection)
    {
        m_labelProxyModel->removeRows(range.top(), range.height(), range.parent());
    }
}

void LabelEditor::slot_removeFontFromLabels(quint32 removedFontID)
{
    LabelObjectVisitor labelObjectVisitor;
    m_project->Accept(labelObjectVisitor);

    QPair<ProjectObject*, ILabelHasFont*> pair;
    foreach(pair, labelObjectVisitor.GetFoundObjectsList())
    {
        if (pair.second->GetFontID() == removedFontID)
        {
            pair.second->SetFontID(0);
            pair.first->NotifyPropertyUpdated();
        }
    }
}

void LabelEditor::addNewLanguages(QStringList& selectedLanguages)
{
    foreach(QString language, selectedLanguages)
    {
        if (m_project->GetLabelDefinition().Languages().contains(language))
        {
            continue;
        }

        m_labelModel->addLanguage(language);
    }
}

void LabelEditor::deleteRemovedLanguages(QStringList& selectedLanguages)
{
    QStringList languagesToDelete;

    foreach(QString language, m_project->GetLabelDefinition().Languages())
    {
        if (selectedLanguages.contains(language))
        {
            continue;
        }

        languagesToDelete.append(language);
    }

    if(languagesToDelete.count() < 1)
    {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete languages", "You are about to delete translations for following languages\n" + languagesToDelete.join(", ") + "\n\nDo you want to proceed?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        return;
    }

    foreach(QString language, languagesToDelete)
    {
        m_labelModel->removeLanguage(language);
    }
}

void LabelEditor::slot_addLanguageButton_pressed()
{
    if (m_project->GetLabelDefinition().Fonts().count() == 0)
    {
        QMessageBox::warning(this, "No fonts available", "Please add at least one font before adding languages!");
        return;
    }

    m_addLanguageDialog = new AddLanguageDialog(m_project->GetLabelDefinition().Languages(), this);
    m_addLanguageDialog->setModal(true);

    connect(m_addLanguageDialog, &AddLanguageDialog::signal_buttonOK_clicked, this, &LabelEditor::slot_addLanguageDialog_accepted);
    connect(m_addLanguageDialog, &AddLanguageDialog::signal_buttonCancel_clicked, this, &LabelEditor::slot_addLanguageDialog_rejected);

    m_addLanguageDialog->show();
}

void LabelEditor::slot_addLanguageDialog_accepted(QStringList* selectedLanguages)
{
    delete m_addLanguageDialog;

    addNewLanguages(*selectedLanguages);
    deleteRemovedLanguages(*selectedLanguages);
}

void LabelEditor::slot_addLanguageDialog_rejected()
{
    delete m_addLanguageDialog;
}

/* Fonts */
void LabelEditor::slot_fontSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
    m_ui->removeFontButton->setEnabled(m_ui->fontConfigurationTable->selectionModel()->hasSelection());
    m_ui->replaceFontButton->setEnabled(m_ui->fontConfigurationTable->selectionModel()->hasSelection());
}

void LabelEditor::slot_defaultFontComboBoxSelectionChanged(const QString& text)
{
    if (m_project->GetLabelDefinition().Fonts().count() == 0)
    {
        m_project->GetLabelDefinition().SetDefaultFont(NULL);
    }
    else
    {
        Font* font = m_project->GetLabelDefinition().GetFontByBDFPath(text);
        m_project->GetLabelDefinition().SetDefaultFont(font);
    }
}

void LabelEditor::slot_removeSelectedFonts()
{
    if (m_project->GetLabelDefinition().Fonts().count() == 1 && m_project->GetLabelDefinition().Languages().count() > 0)
    {
        QMessageBox::information(this, "Can not delete last font", "You must have at least one font as long as you have one translation language.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete font", "You are about to delete a font\n\nDo you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    const QItemSelection selection = m_ui->fontConfigurationTable->selectionModel()->selection();

    foreach (QItemSelectionRange range, selection)
    {
        m_fontModel->removeRows(range.top(), range.height(), range.parent());
    }
}

void LabelEditor::slot_addFontButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select font file"), m_projectRoot.absolutePath(), tr("BDF Font File (*.bdf)"));
    if(!fileName.isEmpty())
    {
        QFileInfo fontFileInfo(fileName);
        QFileInfo expectedProjectFontFile(m_projectRoot.absoluteFilePath(Project::FontsDirectoryName +  "/" + fontFileInfo.fileName()));

        if (!expectedProjectFontFile.absoluteDir().exists())
        {
           expectedProjectFontFile.absoluteDir().mkpath(".");
        }

        if (!expectedProjectFontFile.exists())
        {
           QFile fontFile(fileName);

           if (!fontFile.open(QFile::ReadOnly))
           {
               QMessageBox::warning(this, "Error opening font file!", "Couldn't open font file for copying:\n" + fontFile.errorString());
               return;
           }

           if (!fontFile.copy(expectedProjectFontFile.absoluteFilePath()))
           {
               QMessageBox::warning(this, "Error copying font file!", "Couldn't copy font file!\n" + fontFile.fileName());
           }
        }

        QString fontPath = expectedProjectFontFile.absoluteFilePath().replace(m_projectRoot.absolutePath() + "/","");
        m_fontModel->AddFont(fontPath);
    }
}

void LabelEditor::slot_addLabelButton_clicked()
{
    int newRow = m_ui->labelTable->model()->rowCount();
    m_ui->labelTable->model()->insertRow(newRow);
    QModelIndex index = m_labelProxyModel->index(newRow,0);

    m_ui->labelTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_ui->labelTable->setCurrentIndex(index);
    m_ui->labelTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void LabelEditor::slot_updateMemoryUsageButtonClicked()
{
    LabelDefinition& labelDefinition = m_project->GetLabelDefinition();
    for (int i = 0; i < m_fontModel->rowCount(); ++i)
    {
        QModelIndex index = m_fontModel->index(i, FontModel::MemoryUsageColumn);
        m_fontModel->setData(index, labelDefinition.Fonts().at(i)->MemoryUsage, Qt::EditRole);
    }

    emit m_project->GetLabelDefinition().signal_CalculateFontMemoryUsage();
}

void LabelEditor::slot_replaceFontButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select new font file"), m_projectRoot.absolutePath(), tr("BDF Font File (*.bdf)"));
    if(!fileName.isEmpty())
    {
        QFileInfo fontFileInfo(fileName);
        QFileInfo expectedProjectFontFile(m_projectRoot.absoluteFilePath(Project::FontsDirectoryName +  "/" + fontFileInfo.fileName()));

        if (!expectedProjectFontFile.absoluteDir().exists())
        {
           expectedProjectFontFile.absoluteDir().mkpath(".");
        }

        if (!expectedProjectFontFile.exists())
        {
           QFile fontFile(fileName);

           if (!fontFile.open(QFile::ReadOnly))
           {
               QMessageBox::warning(this, "Error opening font file!", "Couldn't open font file for copying:\n" + fontFile.errorString());
               return;
           }

           if (!fontFile.copy(expectedProjectFontFile.absoluteFilePath()))
           {
               QMessageBox::warning(this, "Error copying font file!", "Couldn't copy font file!\n" + fontFile.fileName());
           }
        }

        QString fontPath = expectedProjectFontFile.absoluteFilePath().replace(m_projectRoot.absolutePath() + "/","");

        QModelIndex selectedFontIndex = m_ui->fontConfigurationTable->selectionModel()->currentIndex();

        if (selectedFontIndex.isValid())
        {
            QModelIndex fontPathIndex = m_fontModel->index(selectedFontIndex.row(), FontModel::BDFPathColumn);
            m_fontModel->setData(fontPathIndex, fontPath, Qt::EditRole);
        }
    }
}

void LabelEditor::slot_mandatoryGlyphsTextEdit_changed()
{
    m_project->GetLabelDefinition().SetMandatoryCharacters(m_ui->mandatoryGlyphsTextEdit->toPlainText());
}
