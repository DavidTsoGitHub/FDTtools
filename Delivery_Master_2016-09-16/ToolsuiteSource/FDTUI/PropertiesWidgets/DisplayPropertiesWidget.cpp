#include "DisplayPropertiesWidget.h"

#include "BSPDisplay.h"
#include "Layout.h"
#include "LayoutDefinition.h"
#include "Project.h"
#include "ColorPair.h"
#include "RangePickerWidget.h"
#include "ColorRangePreviewWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>
#include <QTreeWidgetItem>

#include <climits>

DisplayPropertiesWidget::DisplayPropertiesWidget(Project* project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_display(project->GetBSPSpecification().GetDisplay()),
    m_layoutDefinition(NULL),
    m_renderingTypeLineEdit(NULL),
    m_heightLineEdit(NULL),
    m_widthLineEdit(NULL),
    m_bitsPerPixelLineEdit(NULL),
    m_useDCUMemoryLineEdit(NULL),
    m_dcuMemorySizeLineEdit(NULL),
    m_dcuNbrOfLayersLineEdit(NULL),
    m_cropLeftSpinBox(NULL),
    m_cropRightSpinBox(NULL),
    m_cropTopSpinBox(NULL),
    m_cropBottomSpinBox(NULL),
    m_MainLayoutComboBox(NULL),
    m_StartupEnabledCheckBox(NULL),
    m_StartupTimeoutSpinBox(NULL),
    m_chromaKeyRoot(NULL),
    m_chromaRedPicker(NULL),
    m_chromaGreenPicker(NULL),
    m_chromaBluePicker(NULL),
    m_chromaColorRangePreview(NULL),
    m_rotateScreenCounterClockwiseEnabledCheckBox(NULL)
{
}

DisplayPropertiesWidget::~DisplayPropertiesWidget()
{}

void DisplayPropertiesWidget::SetLayoutDefinition(LayoutDefinition* layoutDefinition)
{
    m_layoutDefinition = layoutDefinition;
    SetProjectObject(m_layoutDefinition);
    connect(m_layoutDefinition, &LayoutDefinition::PropertyUpdated, this, &DisplayPropertiesWidget::populateLayoutComboBox);
}

void DisplayPropertiesWidget::slot_StartupEnabledCheckBox_toggled(bool checked)
{
    m_layoutDefinition->SetStartupEnabled(checked);

    setStartupItemsState(checked);
}

void DisplayPropertiesWidget::slot_MainLayout_currentIndexChanged(int index)
{
    m_layoutDefinition->SetMainLayout(m_layoutDefinition->GetLayoutFromID(m_MainLayoutComboBox->itemData(index).toInt()));
}

void DisplayPropertiesWidget::beforePopulateSignalSinks()
{
    if (m_display != NULL)
    {
        m_renderingTypeLineEdit = new QLineEdit(this);
        m_renderingTypeLineEdit->setEnabled(false);
        m_renderingTypeLineEdit->setText((m_display->GetRenderingType() == BSPDisplay::HW) ? "HW" : "SW");
        AddEditableData("Rendering Type", m_renderingTypeLineEdit);

        m_heightLineEdit = new QLineEdit(this);
        m_heightLineEdit->setEnabled(false);
        m_heightLineEdit->setText(QString::number(m_display->GetHeight()));
        AddEditableData("Height", m_heightLineEdit);

        m_widthLineEdit = new QLineEdit(this);
        m_widthLineEdit->setEnabled(false);
        m_widthLineEdit->setText(QString::number(m_display->GetWidth()));
        AddEditableData("Width", m_widthLineEdit);

        m_bitsPerPixelLineEdit = new QLineEdit(this);
        m_bitsPerPixelLineEdit->setEnabled(false);
        m_bitsPerPixelLineEdit->setText(QString::number(m_display->GetBpp()));
        AddEditableData("Bits per Pixel", m_bitsPerPixelLineEdit);

        m_useDCUMemoryLineEdit = new QLineEdit(this);
        m_useDCUMemoryLineEdit->setEnabled(false);
        m_useDCUMemoryLineEdit->setText(m_display->GetUseGRAM() ? "true" : "false");
        AddEditableData("Use DCU Memory", m_useDCUMemoryLineEdit);

        if (m_display->GetUseGRAM())
        {
            m_dcuMemorySizeLineEdit = new QLineEdit(this);
            m_dcuMemorySizeLineEdit->setEnabled(false);
            m_dcuMemorySizeLineEdit->setText(QString::number(m_display->GetAvailableMemory()) + " bytes");
            AddEditableData("    - DCU Memory Size", m_dcuMemorySizeLineEdit);

            m_dcuNbrOfLayersLineEdit = new QLineEdit(this);
            m_dcuNbrOfLayersLineEdit->setEnabled(false);
            m_dcuNbrOfLayersLineEdit->setText(QString::number(m_display->GetNumberOfLayers()));
            AddEditableData("    - DCU Number of Layers", m_dcuNbrOfLayersLineEdit);
        }

        m_rotateScreenCounterClockwiseEnabledCheckBox = new QCheckBox(this);
        m_rotateScreenCounterClockwiseEnabledCheckBox->setEnabled(false);
        m_rotateScreenCounterClockwiseEnabledCheckBox->setChecked((m_display->GetRenderingOrientation() == BSPDisplay::CounterClockWise) ? true : false);
        AddEditableData("Rotate screen 90 degrees counter clockwise", m_rotateScreenCounterClockwiseEnabledCheckBox);

        m_cropLeftSpinBox = new QSpinBox(this);
        m_cropLeftSpinBox->setMaximum(USHRT_MAX);
        m_cropLeftSpinBox->setValue(m_layoutDefinition->CropLeft());
        AddEditableData("CropLeft", m_cropLeftSpinBox);
        connect(m_cropLeftSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_layoutDefinition->SetCropLeft(value); });

        m_cropRightSpinBox = new QSpinBox(this);
        m_cropRightSpinBox->setMaximum(USHRT_MAX);
        m_cropRightSpinBox->setValue(m_layoutDefinition->CropRight());
        AddEditableData("CropRight", m_cropRightSpinBox);
        connect(m_cropRightSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_layoutDefinition->SetCropRight(value); });

        m_cropTopSpinBox = new QSpinBox(this);
        m_cropTopSpinBox->setMaximum(USHRT_MAX);
        m_cropTopSpinBox->setValue(m_layoutDefinition->CropTop());
        AddEditableData("CropTop", m_cropTopSpinBox);
        connect(m_cropTopSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_layoutDefinition->SetCropTop(value); });

        m_cropBottomSpinBox = new QSpinBox(this);
        m_cropBottomSpinBox->setMaximum(USHRT_MAX);
        m_cropBottomSpinBox->setValue(m_layoutDefinition->CropBottom());
        AddEditableData("CropBottom", m_cropBottomSpinBox);
        connect(m_cropBottomSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_layoutDefinition->SetCropBottom(value); });

        m_MainLayoutComboBox = new QComboBox(this);
        AddEditableData("Main Layout", m_MainLayoutComboBox);
        connect(m_MainLayoutComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DisplayPropertiesWidget::slot_MainLayout_currentIndexChanged);

        m_StartupEnabledCheckBox = new QCheckBox(this);
        m_StartupEnabledCheckBox->setChecked(m_layoutDefinition->StartupEnabled());
        AddEditableData("Startup Screen Enabled", m_StartupEnabledCheckBox);
        connect(m_StartupEnabledCheckBox, &QCheckBox::toggled, this, &DisplayPropertiesWidget::slot_StartupEnabledCheckBox_toggled);

        m_StartupTimeoutSpinBox = new QSpinBox(this);
        m_StartupTimeoutSpinBox->setRange(1, 10);
        m_StartupTimeoutSpinBox->setValue(m_layoutDefinition->StartupScreenDisplayTime());
        m_StartupTimeoutSpinBox->setEnabled(m_layoutDefinition->StartupEnabled());
        AddEditableData("    - Startup Screen Timeout", m_StartupTimeoutSpinBox);
        connect(m_StartupTimeoutSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] (int value) { m_layoutDefinition->SetStartupScreenDisplayTime(value); });

        if (m_display->GetRenderingType() == BSPDisplay::HW)
        {
            m_chromaKeyRoot = AddContainer("ChromaKey");
            m_chromaKeyRoot->setExpanded(true);
            m_chromaRedPicker = new RangePickerWidget(this);
            AddEditableData("Red Range", m_chromaRedPicker, m_chromaKeyRoot);
            m_chromaRedPicker->slot_SetStartValue(m_layoutDefinition->GetStartChromaKeyColor().red());
            m_chromaRedPicker->slot_SetEndValue(m_layoutDefinition->GetEndChromaKeyColor().red());
            connect(m_chromaRedPicker, &RangePickerWidget::signal_SelectedRangeStart, [=] (int redStart) {
                m_layoutDefinition->SetStartChromaKeyColorComponent(ColorComponent::RED, redStart);
                updateChromaKeyPreview();
            });
            connect(m_chromaRedPicker, &RangePickerWidget::signal_SelectedRangeEnd, [=] (int redEnd) {
                m_layoutDefinition->SetEndChromaKeyColorComponent(ColorComponent::RED, redEnd);
                updateChromaKeyPreview();
            });

            m_chromaGreenPicker = new RangePickerWidget(this);
            AddEditableData("Green Range", m_chromaGreenPicker, m_chromaKeyRoot);
            m_chromaGreenPicker->slot_SetStartValue(m_layoutDefinition->GetStartChromaKeyColor().green());
            m_chromaGreenPicker->slot_SetEndValue(m_layoutDefinition->GetEndChromaKeyColor().green());
            connect(m_chromaGreenPicker, &RangePickerWidget::signal_SelectedRangeStart, [=] (int greenStart) {
                m_layoutDefinition->SetStartChromaKeyColorComponent(ColorComponent::GREEN, greenStart);
                updateChromaKeyPreview();
            });
            connect(m_chromaGreenPicker, &RangePickerWidget::signal_SelectedRangeEnd, [=] (int greenEnd) {
                m_layoutDefinition->SetEndChromaKeyColorComponent(ColorComponent::GREEN, greenEnd);
                updateChromaKeyPreview();
            });

            m_chromaBluePicker = new RangePickerWidget(this);
            AddEditableData("Blue Range", m_chromaBluePicker, m_chromaKeyRoot);
            m_chromaBluePicker->slot_SetStartValue(m_layoutDefinition->GetStartChromaKeyColor().blue());
            m_chromaBluePicker->slot_SetEndValue(m_layoutDefinition->GetEndChromaKeyColor().blue());
            connect(m_chromaBluePicker, &RangePickerWidget::signal_SelectedRangeStart, [=] (int blueStart) {
                m_layoutDefinition->SetStartChromaKeyColorComponent(ColorComponent::BLUE, blueStart);
                updateChromaKeyPreview();
            });
            connect(m_chromaBluePicker, &RangePickerWidget::signal_SelectedRangeEnd, [=] (int blueEnd) {
                m_layoutDefinition->SetEndChromaKeyColorComponent(ColorComponent::BLUE, blueEnd);
                updateChromaKeyPreview();
            });

            m_chromaColorRangePreview = new ColorRangePreviewWidget();
            AddEditableData("ChromaKey Range Preview", m_chromaColorRangePreview, m_chromaKeyRoot);
            updateChromaKeyPreview();
        }

        populateLayoutComboBox();
    }
}


void DisplayPropertiesWidget::updateChromaKeyPreview()
{
    m_chromaColorRangePreview->slot_SetStartRangeColor(m_layoutDefinition->GetStartChromaKeyColor());
    m_chromaColorRangePreview->slot_SetEndRangeColor(m_layoutDefinition->GetEndChromaKeyColor());
}

void DisplayPropertiesWidget::populateLayoutComboBox()
{
    m_MainLayoutComboBox->blockSignals(true);

    m_MainLayoutComboBox->clear();

    QString startupLayout = "";
    bool hasStartupScreen = false;
    m_MainLayoutComboBox->addItem("<unset>", 0);
    foreach (Layout* layout, m_layoutDefinition->Layouts())
    {
        if (layout->IsStartupScreen())
        {
            startupLayout = layout->Name();
            hasStartupScreen = true;
        }
        else
        {
            m_MainLayoutComboBox->addItem(layout->Name(), layout->GetID());
        }
    }
    if (!hasStartupScreen)
    {
        m_StartupEnabledCheckBox->setChecked(false);
        m_StartupTimeoutSpinBox->setEnabled(false);
    }

    if (m_layoutDefinition->GetMainLayout() != NULL)
    {
        m_MainLayoutComboBox->setCurrentText(m_layoutDefinition->GetMainLayout()->Name());
    }

    m_MainLayoutComboBox->blockSignals(false);
}

void DisplayPropertiesWidget::setStartupItemsState(bool checked)
{
    m_StartupTimeoutSpinBox->setEnabled(checked);
}

