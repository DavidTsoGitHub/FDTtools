#include "DisplayAreaPropertiesWidget.h"

#include "BSPDisplay.h"
#include "DisplayArea.h"
#include "NameChangeWidget.h"
#include "Project.h"
#include "ColorPair.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

#include <climits>

DisplayAreaPropertiesWidget::DisplayAreaPropertiesWidget(Project* project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_displayArea(NULL),
    m_project(project),
    m_nameLineEdit(NULL),
    m_xSpinBox(NULL),
    m_ySpinBox(NULL),
    m_widthSpinBox(NULL),
    m_heightSpinBox(NULL),
    m_borderCheckBox(NULL),
    m_rgbFGRed(NULL),
    m_rgbFGGreen(NULL),
    m_rgbFGBlue(NULL),
    m_foregroundColorPreview(NULL),
    m_transparentBackground(NULL),
    m_rgbBGRed(NULL),
    m_rgbBGGreen(NULL),
    m_rgbBGBlue(NULL),
    m_backgroundColorPreview(NULL)
{
}

DisplayAreaPropertiesWidget::~DisplayAreaPropertiesWidget()
{}

void DisplayAreaPropertiesWidget::SetDisplayArea(DisplayArea* displayArea)
{
    m_displayArea = displayArea;
    SetProjectObject(m_displayArea);
}

void DisplayAreaPropertiesWidget::updateForegroundColorPreview()
{
    QPalette palette;
    palette.setColor(QPalette::Base, m_displayArea->GetColorPair()->GetFGColor());
    palette.setColor(QPalette::Text, Qt::black);
    m_foregroundColorPreview->setPalette(palette);
}

void DisplayAreaPropertiesWidget::updateBackgroundColorPreview()
{
    QPalette palette;

    if (m_displayArea->GetColorPair()->GetBGIsTransparent())
    {
        palette.setColor(QPalette::Base, m_project->GetLayoutDefinition().GetStartChromaKeyColor());
    }
    else
    {
        palette.setColor(QPalette::Base, m_displayArea->GetColorPair()->GetBGColor());
    }

    palette.setColor(QPalette::Text, Qt::black);
    m_backgroundColorPreview->setPalette(palette);
}

void DisplayAreaPropertiesWidget::beforePopulateSignals()
{
    m_nameLineEdit = new NameChangeWidget(static_cast<ProjectObject*>(m_displayArea));
    AddEditableData("Name", m_nameLineEdit);

    m_xSpinBox = new QSpinBox(this);
    m_xSpinBox->setMaximum(USHRT_MAX);
    m_xSpinBox->setValue(m_displayArea->GetX());
    AddEditableData("X", m_xSpinBox);
    connect(m_xSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value) { m_displayArea->SetX(value); });

    m_ySpinBox = new QSpinBox(this);
    m_ySpinBox->setMaximum(USHRT_MAX);
    m_ySpinBox->setValue(m_displayArea->GetY());
    AddEditableData("Y", m_ySpinBox);
    connect(m_ySpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value) { m_displayArea->SetY(value); });

    m_widthSpinBox = new QSpinBox(this);
    m_widthSpinBox->setMaximum(USHRT_MAX);
    m_widthSpinBox->setValue(m_displayArea->GetWidth());
    AddEditableData("Width", m_widthSpinBox);
    connect(m_widthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value) { m_displayArea->SetWidth(value); });

    m_heightSpinBox = new QSpinBox(this);
    m_heightSpinBox->setMaximum(USHRT_MAX);
    m_heightSpinBox->setValue(m_displayArea->GetHeight());
    AddEditableData("Height", m_heightSpinBox);
    connect(m_heightSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value) { m_displayArea->SetHeight(value); });

    m_borderCheckBox = new QCheckBox(this);
    m_borderCheckBox->setChecked(m_displayArea->GetBorder() > 0);
    AddEditableData("Border", m_borderCheckBox);
    connect(m_borderCheckBox, &QCheckBox::toggled, [=](bool toggled) { m_displayArea->SetBorder(toggled ? 1 : 0); });

    if(getProject()->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        //Foreground color
        QTreeWidgetItem* foregroundContainer = AddContainer("Foreground Color");
        foregroundContainer->setExpanded(true);

        m_rgbFGRed = new QSpinBox();
        m_rgbFGRed->setPrefix("0x");
        m_rgbFGRed->setDisplayIntegerBase(16);
        m_rgbFGRed->setMaximum(0xFF);
        m_rgbFGRed->setValue(m_displayArea->GetColorPair()->GetFGColor().red());
        AddEditableData("Red", m_rgbFGRed, foregroundContainer);
        connect(m_rgbFGRed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetFGColor().red() != value)
            {
                m_displayArea->GetColorPair()->SetFGColorComponent(ColorComponent::RED, value);
                updateForegroundColorPreview();
            }
        });

        m_rgbFGGreen = new QSpinBox();
        m_rgbFGGreen->setPrefix("0x");
        m_rgbFGGreen->setDisplayIntegerBase(16);
        m_rgbFGGreen->setMaximum(0xFF);
        m_rgbFGGreen->setValue(m_displayArea->GetColorPair()->GetFGColor().green());
        AddEditableData("Green", m_rgbFGGreen, foregroundContainer);
        connect(m_rgbFGGreen, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetFGColor().green() != value)
            {
                m_displayArea->GetColorPair()->SetFGColorComponent(ColorComponent::GREEN, value);
                updateForegroundColorPreview();
            }
        });

        m_rgbFGBlue = new QSpinBox();
        m_rgbFGBlue->setPrefix("0x");
        m_rgbFGBlue->setDisplayIntegerBase(16);
        m_rgbFGBlue->setMaximum(0xFF);
        m_rgbFGBlue->setValue(m_displayArea->GetColorPair()->GetFGColor().blue());
        AddEditableData("Blue", m_rgbFGBlue, foregroundContainer);
        connect(m_rgbFGBlue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetFGColor().blue() != value)
            {
                m_displayArea->GetColorPair()->SetFGColorComponent(ColorComponent::BLUE, value);
                updateForegroundColorPreview();
            }
        });

        m_foregroundColorPreview = new QLineEdit(this);
        m_foregroundColorPreview->setEnabled(false);
        m_foregroundColorPreview->setText("");
        AddEditableData("Preview", m_foregroundColorPreview, foregroundContainer);
        updateForegroundColorPreview();

        //Background color
        QTreeWidgetItem* backgroundContainer = AddContainer("Background Color");
        backgroundContainer->setExpanded(true);

        m_transparentBackground = new QCheckBox(this);
        m_transparentBackground->setChecked(m_displayArea->GetColorPair()->GetBGIsTransparent());
        AddEditableData("Transparent", m_transparentBackground, backgroundContainer);
        connect(m_transparentBackground, &QCheckBox::toggled, this, &DisplayAreaPropertiesWidget::slot_backgroundTransparent_toggled);

        m_rgbBGRed = new QSpinBox();
        m_rgbBGRed->setPrefix("0x");
        m_rgbBGRed->setDisplayIntegerBase(16);
        m_rgbBGRed->setMaximum(0xFF);
        m_rgbBGRed->setValue(m_displayArea->GetColorPair()->GetBGColor().red());
        AddEditableData("Red", m_rgbBGRed, backgroundContainer);
        connect(m_rgbBGRed, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetBGColor().red() != value)
            {
                m_displayArea->GetColorPair()->SetBGColorComponent(ColorComponent::RED, value);
                updateBackgroundColorPreview();
            }
        });

        m_rgbBGGreen = new QSpinBox();
        m_rgbBGGreen->setPrefix("0x");
        m_rgbBGGreen->setDisplayIntegerBase(16);
        m_rgbBGGreen->setMaximum(0xFF);
        m_rgbBGGreen->setValue(m_displayArea->GetColorPair()->GetBGColor().green());
        AddEditableData("Green", m_rgbBGGreen, backgroundContainer);
        connect(m_rgbBGGreen, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetBGColor().green() != value)
            {
                m_displayArea->GetColorPair()->SetBGColorComponent(ColorComponent::GREEN, value);
                updateBackgroundColorPreview();
            }
        });

        m_rgbBGBlue = new QSpinBox();
        m_rgbBGBlue->setPrefix("0x");
        m_rgbBGBlue->setDisplayIntegerBase(16);
        m_rgbBGBlue->setMaximum(0xFF);
        m_rgbBGBlue->setValue(m_displayArea->GetColorPair()->GetBGColor().blue());
        AddEditableData("Blue", m_rgbBGBlue, backgroundContainer);
        connect(m_rgbBGBlue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  [=](int value)
        {
            if (m_displayArea->GetColorPair()->GetBGColor().blue() != value)
            {
                m_displayArea->GetColorPair()->SetBGColorComponent(ColorComponent::BLUE, value);
                updateBackgroundColorPreview();
            }
        });

        m_backgroundColorPreview = new QLineEdit(this);
        m_backgroundColorPreview->setEnabled(false);
        m_backgroundColorPreview->setText("");
        AddEditableData("Preview", m_backgroundColorPreview, backgroundContainer);
        updateBackgroundColorPreview();

        slot_backgroundTransparent_toggled(m_displayArea->GetColorPair()->GetBGIsTransparent());
    }
}

void DisplayAreaPropertiesWidget::slot_backgroundTransparent_toggled(bool checked)
{
    m_displayArea->GetColorPair()->SetBGIsTransparent(checked);

    m_rgbBGRed->setEnabled(!checked);
    m_rgbBGGreen->setEnabled(!checked);
    m_rgbBGBlue->setEnabled(!checked);
    updateBackgroundColorPreview();
}
