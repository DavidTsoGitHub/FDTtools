#include "ProjectDefinitionWizardPage.h"
#include "ui_ProjectDefinitionWizardPage.h"

#include "CANSpecificationDeserializer.h"
#include "ProjectSettings.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QWizardPage>

ProjectDefinitionWizardPage::ProjectDefinitionWizardPage(ProjectSettings& projectSettings, QWidget* parent) :
    QWizardPage(parent),
    m_projectSettings(projectSettings),
    m_ui(new Ui::ProjectDefinitionWizardPage)
{
    m_ui->setupUi(this);
    setupSignals();
    m_ui->CANECUNameComboBox->setHidden(true);
    m_ui->CANECUNameLabel->setHidden(true);

}

ProjectDefinitionWizardPage::~ProjectDefinitionWizardPage()
{
    delete m_ui;
}

void ProjectDefinitionWizardPage::setupSignals()
{
    connect(m_ui->ProjectName, &QLineEdit::textChanged, this, &ProjectDefinitionWizardPage::slot_projectNameChanged);
    connect(m_ui->SelectProjectRootButton, &QAbstractButton::clicked, this, &ProjectDefinitionWizardPage::slot_selectProjectRootButtonClicked);
    connect(m_ui->ProjectRoot, &QLineEdit::textChanged, this, &ProjectDefinitionWizardPage::slot_projectRootChanged);
    connect(m_ui->SelectBSPSpecificationButton, &QAbstractButton::clicked, this, &ProjectDefinitionWizardPage::slot_selectBSPSpecificationButtonClicked);
    connect(m_ui->SelectCANSpecificationButton, &QAbstractButton::clicked, this, &ProjectDefinitionWizardPage::slot_selectCANSpecificationButtonClicked);
    connect(m_ui->BSPSpecification, &QLineEdit::textChanged, this, &ProjectDefinitionWizardPage::slot_bspSpecificationChanged);
    connect(m_ui->CANECUNameComboBox, &QComboBox::currentTextChanged, this, &ProjectDefinitionWizardPage::slot_canECUNameChanged);
    connect(m_ui->CANSpecification, &QLineEdit::editingFinished, this, &ProjectDefinitionWizardPage::slot_canSpecificationEditFinished);
}

// Slots

void ProjectDefinitionWizardPage::slot_projectNameChanged(const QString& name)
{
    if(!isValidProjectPath(name))
    {
        QMessageBox::information(this, "Invalid Name", "The selected name is not suitable for a project, please choose a name without special character och spaces.", QMessageBox::Ok);
        m_ui->ProjectName->setText(name.left(name.length()-1));
        return;
    }

    m_projectSettings.ProjectName = name;
    this->completeChanged();
}

void ProjectDefinitionWizardPage::slot_selectProjectRootButtonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Project Root", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!isValidProjectPath(dir))
    {
        QMessageBox::information(this, "Invalid Path", "The selected path is not suitable for a project, please choose a path without special character och spaces.", QMessageBox::Ok);
        return;
    }
    m_ui->ProjectRoot->setText(dir);
}

void ProjectDefinitionWizardPage::slot_projectRootChanged(const QString& dir)
{
    if(!isValidProjectPath(dir))
    {
        QMessageBox::information(this, "Invalid Path", "The selected path is not suitable for a project, please choose a path without special character och spaces.", QMessageBox::Ok);
        return;
    }
    m_projectSettings.ProjectRoot = QDir(dir);
    this->completeChanged();
}

void ProjectDefinitionWizardPage::slot_selectBSPSpecificationButtonClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select BSP Specification", QString(), "*.xml");
    m_ui->BSPSpecification->setText(file);
}

void ProjectDefinitionWizardPage::slot_bspSpecificationChanged(const QString& bspPath)
{
    m_projectSettings.BSPSpecificationPath = QFileInfo(bspPath);
    this->completeChanged();
}

void ProjectDefinitionWizardPage::slot_selectCANSpecificationButtonClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select CAN Specification", QString(), "*.xml");
    m_ui->CANSpecification->setText(file);
    slot_canSpecificationEdited(file);
}

void ProjectDefinitionWizardPage::slot_canSpecificationEditFinished()
{
    slot_canSpecificationEdited(m_ui->CANSpecification->text());
}

void ProjectDefinitionWizardPage::slot_canSpecificationEdited(const QString& canPath)
{
    if (canPath.isEmpty())
    {
        m_projectSettings.CANSpecificationPath = QFileInfo();
        m_projectSettings.HasCANSpecification = false;
        m_ui->CANECUNameComboBox->setHidden(true);
        m_ui->CANECUNameLabel->setHidden(true);
    }
    else
    {
        m_projectSettings.CANSpecificationPath = QFileInfo(canPath);
        m_projectSettings.HasCANSpecification = true;

        refreshCANECUNames();
    }

    this->completeChanged();
}

void ProjectDefinitionWizardPage::slot_canECUNameChanged(const QString& canECUName)
{
    m_projectSettings.CANECUName = canECUName;
    this->completeChanged();
}


// Private methods
void ProjectDefinitionWizardPage::refreshCANECUNames()
{
    m_ui->CANECUNameComboBox->clear();

    QFile file(m_projectSettings.CANSpecificationPath.absoluteFilePath());
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(this, "Invalid CAN Specification File", "The selected CAN specification could not be opened! " + file.errorString(), QMessageBox::Ok);
        m_ui->CANSpecification->setText("");
        return;
    }

    CANSpecification spec;
    CANSpecificationDeserializer deserializer(spec);

    QTextStream canXml(&file);
    if (deserializer.Deserialize(canXml.readAll()) == DeserializerBase::DeserializationOK)
    {
        m_ui->CANECUNameComboBox->addItems(spec.Modules);

        m_ui->CANECUNameComboBox->setHidden(false);
        m_ui->CANECUNameLabel->setHidden(false);
    }
    else
    {
        QMessageBox::information(this, "Invalid CAN Specification File", "The selected CAN specification could not be deserialized! " + deserializer.Error(), QMessageBox::Ok);
        m_ui->CANSpecification->setText("");
    }
}

// Overriden QWizardPage functions

bool ProjectDefinitionWizardPage::isComplete() const
{
    return !m_projectSettings.ProjectName.isEmpty() &&
           !m_projectSettings.ProjectRoot.absolutePath().isEmpty() &&
           m_projectSettings.BSPSpecificationPath.exists() &&
           (!m_projectSettings.HasCANSpecification || m_projectSettings.CANSpecificationPath.exists());
}

bool ProjectDefinitionWizardPage::isValidProjectPath(QString filename)
{
    bool isValid = true;
    for(int i = 0; i < filename.size(); ++i)
    {
        if(filename.at(i).unicode() > 127 ||     // Non Ascii
            filename.at(i).unicode() < 32 ||     // Unprintable
            filename.at(i).unicode() == 32 ||    // Space
            filename.at(i).unicode() == 34  ||   // "
            filename.at(i).unicode() == 42  ||   // *
            ((filename.at(i).unicode() == 58) && (i != 1))  ||   // : on other position then 2:nd
            filename.at(i).unicode() == 60  ||   // <
            filename.at(i).unicode() == 62  ||   // >
            filename.at(i).unicode() == 63  ||   // ?
            filename.at(i).unicode() == 124 ||   // |
            filename.at(i).unicode() == 127 )    // DEL
        {
            isValid = false;
        }
    }

    return isValid;
}
