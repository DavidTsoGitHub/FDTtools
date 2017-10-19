#include "SelectDisplayAreaDialog.h"
#include "ui_SelectDisplayAreaDialog.h"

#include "DisplayArea.h"
#include "Layout.h"
#include "Project.h"
#include <QDialog>
#include <QWidget>

SelectDisplayAreaDialog::SelectDisplayAreaDialog(Project* project, quint8 currentLayoutIndex, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SelectDisplayAreaDialog),
    m_project(project),
    m_currentLayoutIndex(currentLayoutIndex),
    m_layout(NULL),
    m_selectedDisplayArea(NULL)
{
    this->setModal(true);
    m_ui->setupUi(this);

    populateLayoutsComboBox();
    connect(m_ui->layoutsComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SelectDisplayAreaDialog::slot_layoutsComboBox_currentIndexChanged);
    connect(m_ui->displayAreasComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SelectDisplayAreaDialog::slot_displayAreaComboBox_currentIndexChanged);

    if (m_ui->layoutsComboBox->count() > 0)
    {
        slot_layoutsComboBox_currentIndexChanged(m_currentLayoutIndex);
    }
}

SelectDisplayAreaDialog::~SelectDisplayAreaDialog()
{
    delete m_ui;
}

DisplayArea *SelectDisplayAreaDialog::GetSelectedDisplayArea()
{
    return m_selectedDisplayArea;
}

void SelectDisplayAreaDialog::populateDisplayAreasComboBox()
{
    m_ui->displayAreasComboBox->blockSignals(true);
    m_ui->displayAreasComboBox->clear();
    const QList<DisplayArea*>& displayAreas = m_layout->DisplayAreas();
    for (int i=0; i<displayAreas.count(); ++i)
    {
        m_ui->displayAreasComboBox->addItem(displayAreas.at(i)->GetName());
    }

    if (m_ui->displayAreasComboBox->count() > 0)
    {
        slot_displayAreaComboBox_currentIndexChanged(0);
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    m_ui->displayAreasComboBox->blockSignals(false);
}

void SelectDisplayAreaDialog::populateLayoutsComboBox()
{
    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        if (layout->IsStartupScreen() || layout->IsCheckMessageScreen())
        {
            continue;
        }

        m_ui->layoutsComboBox->addItem(layout->Name());
    }
    m_ui->layoutsComboBox->setCurrentIndex(m_currentLayoutIndex);
}

void SelectDisplayAreaDialog::slot_layoutsComboBox_currentIndexChanged(int index)
{
    const QList<Layout*>& layouts = m_project->GetLayoutDefinition().Layouts();
    foreach (Layout* layout, layouts)
    {
        if (m_ui->layoutsComboBox->currentText().compare(layout->Name()) == 0)
        {
            m_layout = layout;
        }
    }

    m_currentLayoutIndex = index;

    populateDisplayAreasComboBox();
}

void SelectDisplayAreaDialog::slot_displayAreaComboBox_currentIndexChanged(int index)
{
    const QList<DisplayArea*>& displayAreas = m_layout->DisplayAreas();
    m_selectedDisplayArea = displayAreas.at(index);
}
