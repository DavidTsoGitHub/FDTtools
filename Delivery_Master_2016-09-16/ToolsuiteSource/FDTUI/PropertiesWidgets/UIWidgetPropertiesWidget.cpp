#include "UIWidgetPropertiesWidget.h"

#include "AnimationPropertiesWidget.h"
#include "AnimationTriggerWidget.h"
#include "BSPDisplay.h"
#include "Icon.h"
#include "Image.h"
#include "Label.h"
#include "NameChangeWidget.h"
#include "Project.h"
#include "ProjectObject.h"
#include "ColorPair.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QSpinBox>
#include <QSignalMapper>
#include <QTreeWidgetItem>
#include <QWidget>

UIWidgetPropertiesWidget::UIWidgetPropertiesWidget(Project* project, QWidget* parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_project(project),
    m_uiWidget(NULL),
    m_nameEdit(NULL),
    m_x(NULL),
    m_y(NULL),
    m_width(NULL),
    m_heigth(NULL),
    m_boxed(NULL),
    m_rgbPrimaryFGRed(NULL),
    m_rgbPrimaryFGGreen(NULL),
    m_rgbPrimaryFGBlue(NULL),
    m_primaryForegroundColorPreview(NULL),
    m_rgbPrimaryBGRed(NULL),
    m_rgbPrimaryBGGreen(NULL),
    m_rgbPrimaryBGBlue(NULL),
    m_primaryBackgroundColorPreview(NULL),
    m_primaryTransparentBackground(NULL),
    m_rgbSecondaryFGRed(NULL),
    m_rgbSecondaryFGGreen(NULL),
    m_rgbSecondaryFGBlue(NULL),
    m_secondaryForegroundColorPreview(NULL),
    m_rgbSecondaryBGRed(NULL),
    m_rgbSecondaryBGGreen(NULL),
    m_rgbSecondaryBGBlue(NULL),
    m_secondaryBackgroundColorPreview(NULL),
    m_secondaryTransparentBackground(NULL),
    m_animationContainer(NULL),
    m_nbrOfAnimation1Triggers(1),
    m_nbrOfAnimation2Triggers(1),
    m_description(NULL),
    m_layersNeeded(NULL)
{
}

UIWidgetPropertiesWidget::~UIWidgetPropertiesWidget()
{
}

void UIWidgetPropertiesWidget::SetUIWidget(UIWidgetBase* uiWidget)
{
    m_uiWidget = uiWidget;
    SetProjectObject(m_uiWidget);
}

void UIWidgetPropertiesWidget::updatePrimaryForegroundColorPreview()
{
    QPalette palette;
    palette.setColor(QPalette::Base, m_uiWidget->GetPrimaryColorPair()->GetFGColor());
    palette.setColor(QPalette::Text, Qt::black);
    m_primaryForegroundColorPreview->setPalette(palette);
}

void UIWidgetPropertiesWidget::updatePrimaryBackgroundColorPreview()
{
    QPalette palette;

    if (m_uiWidget->GetPrimaryColorPair()->GetBGIsTransparent())
    {
        palette.setColor(QPalette::Base, m_project->GetLayoutDefinition().GetStartChromaKeyColor());
    }
    else
    {
        palette.setColor(QPalette::Base, m_uiWidget->GetPrimaryColorPair()->GetBGColor());
    }

    palette.setColor(QPalette::Text, Qt::black);
    m_primaryBackgroundColorPreview->setPalette(palette);
}

void UIWidgetPropertiesWidget::updateSecondaryForegroundColorPreview()
{
    QPalette palette;
    palette.setColor(QPalette::Base, m_uiWidget->GetSecondaryColorPair()->GetFGColor());
    palette.setColor(QPalette::Text, Qt::black);
    m_secondaryForegroundColorPreview->setPalette(palette);
}

void UIWidgetPropertiesWidget::updateSecondaryBackgroundColorPreview()
{
    QPalette palette;

    if (m_uiWidget->GetSecondaryColorPair()->GetBGIsTransparent())
    {
        palette.setColor(QPalette::Base, m_project->GetLayoutDefinition().GetStartChromaKeyColor());
    }
    else
    {
        palette.setColor(QPalette::Base, m_uiWidget->GetSecondaryColorPair()->GetBGColor());
    }

    palette.setColor(QPalette::Text, Qt::black);
    m_secondaryBackgroundColorPreview->setPalette(palette);
}

void UIWidgetPropertiesWidget::beforePopulateSignals()
{
    m_nameEdit = new NameChangeWidget(static_cast<ProjectObject*>(m_uiWidget));
    AddEditableData("Name", m_nameEdit);

    AddDisplayData("GCL Name", m_uiWidget->GetUIWidgetUniqueName());

    m_x = new QSpinBox();
    m_x->setMaximum(USHRT_MAX);
    m_x->setValue(m_uiWidget->GetX());
    AddEditableData("X", m_x);
    connect(m_x, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_xChanged);

    m_y = new QSpinBox();
    m_y->setMaximum(USHRT_MAX);
    m_y->setValue(m_uiWidget->GetY());
    AddEditableData("Y", m_y);
    connect(m_y, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_yChanged);

    m_width = new QSpinBox();
    m_width->setMaximum(USHRT_MAX);
    m_width->setValue(m_uiWidget->GetWidth());
    AddEditableData("Width", m_width);
    if ((m_uiWidget->GetUIWidgetTypeName() == "Image") || (m_uiWidget->GetUIWidgetTypeName() == "Icon"))
    {
        m_width->setEnabled(false);
    }
    connect(m_width, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_widthChanged);

    m_heigth = new QSpinBox();
    m_heigth->setMaximum(USHRT_MAX);
    m_heigth->setValue(m_uiWidget->GetHeight());
    AddEditableData("Height", m_heigth);
    if ((m_uiWidget->GetUIWidgetTypeName() == "Image") || (m_uiWidget->GetUIWidgetTypeName() == "Icon"))
    {
        m_heigth->setEnabled(false);
    }
    connect(m_heigth, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_heightChanged);

    m_boxed = new QCheckBox();
    m_boxed->setChecked(m_uiWidget->GetBoxed());
    AddEditableData("Boxed", m_boxed);
    connect(m_boxed, &QCheckBox::toggled, this, &UIWidgetPropertiesWidget::slot_boxedChanged);

    bool hideColors  = (m_uiWidget->GetUIWidgetTypeName() == "Image") || (m_uiWidget->GetUIWidgetTypeName() == "DynamicImage") || (m_uiWidget->GetUIWidgetTypeName() == "SignalTrigger");

    if((getProject()->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW) && !hideColors)
    {
        //Foreground color
        QTreeWidgetItem* foregroundContainer = AddContainer("Foreground Color");
        foregroundContainer->setExpanded(true);
        m_rgbPrimaryFGRed = new QSpinBox();
        m_rgbPrimaryFGRed->setPrefix("0x");
        m_rgbPrimaryFGRed->setDisplayIntegerBase(16);
        m_rgbPrimaryFGRed->setMaximum(0xFF);
        m_rgbPrimaryFGRed->setValue(m_uiWidget->GetPrimaryColorPair()->GetFGColor().red());
        AddEditableData("Red", m_rgbPrimaryFGRed, foregroundContainer);
        connect(m_rgbPrimaryFGRed, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryFGRedChanged);

        m_rgbPrimaryFGGreen = new QSpinBox();
        m_rgbPrimaryFGGreen->setPrefix("0x");
        m_rgbPrimaryFGGreen->setDisplayIntegerBase(16);
        m_rgbPrimaryFGGreen->setMaximum(0xFF);
        m_rgbPrimaryFGGreen->setValue(m_uiWidget->GetPrimaryColorPair()->GetFGColor().green());
        AddEditableData("Green", m_rgbPrimaryFGGreen, foregroundContainer);
        connect(m_rgbPrimaryFGGreen, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryFGGreenChanged);

        m_rgbPrimaryFGBlue = new QSpinBox();
        m_rgbPrimaryFGBlue->setPrefix("0x");
        m_rgbPrimaryFGBlue->setDisplayIntegerBase(16);
        m_rgbPrimaryFGBlue->setMaximum(0xFF);
        m_rgbPrimaryFGBlue->setValue(m_uiWidget->GetPrimaryColorPair()->GetFGColor().blue());
        AddEditableData("Blue", m_rgbPrimaryFGBlue, foregroundContainer);
        connect(m_rgbPrimaryFGBlue, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryFGBlueChanged);

        m_primaryForegroundColorPreview = new QLineEdit(this);
        m_primaryForegroundColorPreview->setEnabled(false);
        m_primaryForegroundColorPreview->setText("");
        AddEditableData("Preview", m_primaryForegroundColorPreview, foregroundContainer);
        updatePrimaryForegroundColorPreview();

        //Background color
        QTreeWidgetItem* backgroundContainer = AddContainer("Background Color");
        backgroundContainer->setExpanded(true);

        m_primaryTransparentBackground = new QCheckBox(this);
        m_primaryTransparentBackground->setChecked(m_uiWidget->GetPrimaryColorPair()->GetBGIsTransparent());
        AddEditableData("Transparent", m_primaryTransparentBackground, backgroundContainer);
        connect(m_primaryTransparentBackground, &QCheckBox::toggled, this, &UIWidgetPropertiesWidget::slot_primaryBackgroundTransparent_toggled);

        m_rgbPrimaryBGRed = new QSpinBox();
        m_rgbPrimaryBGRed->setPrefix("0x");
        m_rgbPrimaryBGRed->setDisplayIntegerBase(16);
        m_rgbPrimaryBGRed->setMaximum(0xFF);
        m_rgbPrimaryBGRed->setValue(m_uiWidget->GetPrimaryColorPair()->GetBGColor().red());
        m_rgbPrimaryBGRed->setEnabled(!m_uiWidget->GetPrimaryColorPair()->GetBGIsTransparent());
        AddEditableData("Red", m_rgbPrimaryBGRed, backgroundContainer);
        connect(m_rgbPrimaryBGRed, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryBGRedChanged);

        m_rgbPrimaryBGGreen = new QSpinBox();
        m_rgbPrimaryBGGreen->setPrefix("0x");
        m_rgbPrimaryBGGreen->setDisplayIntegerBase(16);
        m_rgbPrimaryBGGreen->setMaximum(0xFF);
        m_rgbPrimaryBGGreen->setValue(m_uiWidget->GetPrimaryColorPair()->GetBGColor().green());
        m_rgbPrimaryBGGreen->setEnabled(!m_uiWidget->GetPrimaryColorPair()->GetBGIsTransparent());
        AddEditableData("Green", m_rgbPrimaryBGGreen, backgroundContainer);
        connect(m_rgbPrimaryBGGreen, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryBGGreenChanged);

        m_rgbPrimaryBGBlue = new QSpinBox();
        m_rgbPrimaryBGBlue->setPrefix("0x");
        m_rgbPrimaryBGBlue->setDisplayIntegerBase(16);
        m_rgbPrimaryBGBlue->setMaximum(0xFF);
        m_rgbPrimaryBGBlue->setValue(m_uiWidget->GetPrimaryColorPair()->GetBGColor().blue());
        m_rgbPrimaryBGBlue->setEnabled(!m_uiWidget->GetPrimaryColorPair()->GetBGIsTransparent());
        AddEditableData("Blue", m_rgbPrimaryBGBlue, backgroundContainer);
        connect(m_rgbPrimaryBGBlue, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbPrimaryBGBlueChanged);

        m_primaryBackgroundColorPreview = new QLineEdit(this);
        m_primaryBackgroundColorPreview->setEnabled(false);
        m_primaryBackgroundColorPreview->setText("");
        AddEditableData("Preview", m_primaryBackgroundColorPreview, backgroundContainer);
        updatePrimaryBackgroundColorPreview();
    }

    bool useSecondaryColorSet = m_uiWidget->GetUseSecondaryColorPair();

    if((getProject()->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW) && !hideColors && useSecondaryColorSet)
    {
        //Secondary Foreground color
        QTreeWidgetItem* foregroundContainer = AddContainer(m_uiWidget->GetSecondaryColorPairForegroundHeader());
        foregroundContainer->setExpanded(true);
        m_rgbSecondaryFGRed = new QSpinBox();
        m_rgbSecondaryFGRed->setPrefix("0x");
        m_rgbSecondaryFGRed->setDisplayIntegerBase(16);
        m_rgbSecondaryFGRed->setMaximum(0xFF);
        m_rgbSecondaryFGRed->setValue(m_uiWidget->GetSecondaryColorPair()->GetFGColor().red());
        AddEditableData("Red", m_rgbSecondaryFGRed, foregroundContainer);
        connect(m_rgbSecondaryFGRed, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryFGRedChanged);

        m_rgbSecondaryFGGreen = new QSpinBox();
        m_rgbSecondaryFGGreen->setPrefix("0x");
        m_rgbSecondaryFGGreen->setDisplayIntegerBase(16);
        m_rgbSecondaryFGGreen->setMaximum(0xFF);
        m_rgbSecondaryFGGreen->setValue(m_uiWidget->GetSecondaryColorPair()->GetFGColor().green());
        AddEditableData("Green", m_rgbSecondaryFGGreen, foregroundContainer);
        connect(m_rgbSecondaryFGGreen, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryFGGreenChanged);

        m_rgbSecondaryFGBlue = new QSpinBox();
        m_rgbSecondaryFGBlue->setPrefix("0x");
        m_rgbSecondaryFGBlue->setDisplayIntegerBase(16);
        m_rgbSecondaryFGBlue->setMaximum(0xFF);
        m_rgbSecondaryFGBlue->setValue(m_uiWidget->GetSecondaryColorPair()->GetFGColor().blue());
        AddEditableData("Blue", m_rgbSecondaryFGBlue, foregroundContainer);
        connect(m_rgbSecondaryFGBlue, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryFGBlueChanged);

        m_secondaryForegroundColorPreview = new QLineEdit(this);
        m_secondaryForegroundColorPreview->setEnabled(false);
        m_secondaryForegroundColorPreview->setText("");
        AddEditableData("Preview", m_secondaryForegroundColorPreview, foregroundContainer);
        updateSecondaryForegroundColorPreview();

        //Secondary Background color
        QTreeWidgetItem* backgroundContainer = AddContainer(m_uiWidget->GetSecondaryColorPairBackgroundHeader());
        backgroundContainer->setExpanded(true);

        m_secondaryTransparentBackground = new QCheckBox(this);
        m_secondaryTransparentBackground->setChecked(m_uiWidget->GetSecondaryColorPair()->GetBGIsTransparent());
        AddEditableData("Transparent", m_secondaryTransparentBackground, backgroundContainer);
        connect(m_secondaryTransparentBackground, &QCheckBox::toggled, this, &UIWidgetPropertiesWidget::slot_secondaryBackgroundTransparent_toggled);

        m_rgbSecondaryBGRed = new QSpinBox();
        m_rgbSecondaryBGRed->setPrefix("0x");
        m_rgbSecondaryBGRed->setDisplayIntegerBase(16);
        m_rgbSecondaryBGRed->setMaximum(0xFF);
        m_rgbSecondaryBGRed->setValue(m_uiWidget->GetSecondaryColorPair()->GetBGColor().red());
        m_rgbSecondaryBGRed->setEnabled(!m_uiWidget->GetSecondaryColorPair()->GetBGIsTransparent());
        AddEditableData("Red", m_rgbSecondaryBGRed, backgroundContainer);
        connect(m_rgbSecondaryBGRed, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryBGRedChanged);

        m_rgbSecondaryBGGreen = new QSpinBox();
        m_rgbSecondaryBGGreen->setPrefix("0x");
        m_rgbSecondaryBGGreen->setDisplayIntegerBase(16);
        m_rgbSecondaryBGGreen->setMaximum(0xFF);
        m_rgbSecondaryBGGreen->setValue(m_uiWidget->GetSecondaryColorPair()->GetBGColor().green());
        m_rgbSecondaryBGGreen->setEnabled(!m_uiWidget->GetSecondaryColorPair()->GetBGIsTransparent());
        AddEditableData("Green", m_rgbSecondaryBGGreen, backgroundContainer);
        connect(m_rgbSecondaryBGGreen, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryBGGreenChanged);

        m_rgbSecondaryBGBlue = new QSpinBox();
        m_rgbSecondaryBGBlue->setPrefix("0x");
        m_rgbSecondaryBGBlue->setDisplayIntegerBase(16);
        m_rgbSecondaryBGBlue->setMaximum(0xFF);
        m_rgbSecondaryBGBlue->setValue(m_uiWidget->GetSecondaryColorPair()->GetBGColor().blue());
        m_rgbSecondaryBGBlue->setEnabled(!m_uiWidget->GetSecondaryColorPair()->GetBGIsTransparent());
        AddEditableData("Blue", m_rgbSecondaryBGBlue, backgroundContainer);
        connect(m_rgbSecondaryBGBlue, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &UIWidgetPropertiesWidget::slot_rgbSecondaryBGBlueChanged);

        m_secondaryBackgroundColorPreview = new QLineEdit(this);
        m_secondaryBackgroundColorPreview->setEnabled(false);
        m_secondaryBackgroundColorPreview->setText("");
        AddEditableData("Preview", m_secondaryBackgroundColorPreview, backgroundContainer);
        updateSecondaryBackgroundColorPreview();
    }

    m_customPropertyWidgets.clear();
    QSignalMapper* mapper = new QSignalMapper(this);
    connect(mapper, static_cast<void(QSignalMapper::*)(int)>(&QSignalMapper::mapped), this, &UIWidgetPropertiesWidget::slot_customPropertyChanged);

    quint8 animationIndex = 0;
    for (int i=0; i<m_uiWidget->GetWidgetPropertyCount(); ++i)
    {
        QTreeWidgetItem* parent = NULL;
        UIWidgetBase::DataType_t dataType = m_uiWidget->GetWidgetPropertyDataType(i);
        QString header = m_uiWidget->GetWidgetPropertyHeader(i);
        QWidget* widgetToAdd = NULL;
        switch (dataType)
        {
            case UIWidgetBase::LineEdit_Editable:
            case UIWidgetBase::LineEdit_NonEditable:
            {
                QLineEdit* lineEdit = new QLineEdit();
                lineEdit->setText(m_uiWidget->GetWidgetPropertyData(i).toString());
                if (dataType == UIWidgetBase::LineEdit_NonEditable)
                {
                    lineEdit->setReadOnly(true);
                }
                connect(lineEdit, &QLineEdit::textChanged, mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = lineEdit;
                break;
            }
            case UIWidgetBase::SpinBox_Uint8:
            case UIWidgetBase::SpinBox_Uint8_Min1:
            case UIWidgetBase::SpinBox_Uint16:
            case UIWidgetBase::SpinBox_Uint32:
            case UIWidgetBase::SpinBox_Int8:
            case UIWidgetBase::SpinBox_Int16:
            case UIWidgetBase::SpinBox_Int32:
            case UIWidgetBase::SpinBox_DynamicValueType:
            {
                QSpinBox* spinBox = getNewSpinBox(dataType);
                spinBox->setValue(m_uiWidget->GetWidgetPropertyData(i).toInt());
                connect(spinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = spinBox;
                break;
            }
            case UIWidgetBase::CheckBox:
            {
                QCheckBox* checkBox = new QCheckBox();
                checkBox->setChecked(m_uiWidget->GetWidgetPropertyData(i).toBool());
                connect(checkBox, &QCheckBox::toggled, mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = checkBox;
                break;
            }
            case UIWidgetBase::AnimationCheckBox:
            {
                QCheckBox* checkBox = new QCheckBox();
                checkBox->setChecked(m_uiWidget->GetWidgetPropertyData(i).toBool());
                connect(checkBox, &QCheckBox::toggled, mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = checkBox;
                parent = m_animationContainer;

                if (checkBox->isChecked())
                {
                    mapper->setMapping(widgetToAdd, i);
                    m_customPropertyWidgets.insert(i, widgetToAdd);
                    AddEditableData(header, widgetToAdd, parent);
                    populateTriggers(m_uiWidget, animationIndex);
                    widgetToAdd = NULL;
                }

                break;
            }
            case UIWidgetBase::AnimationTriggers:
            {

                break;
            }
            case UIWidgetBase::DynamicImageAnimationComboBox:
            case UIWidgetBase::AnimationComboBox:
            {
                m_animationContainer = AddContainer("Animation " + QString::number(++animationIndex));
                m_animationContainer->setExpanded(true);
                quint32 currentId = m_uiWidget->GetWidgetPropertyData(i).toUInt();
                QComboBox* comboBox = getNewComboBox(dataType, currentId);
                connect(comboBox, &QComboBox::currentTextChanged, mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = comboBox;

                mapper->setMapping(widgetToAdd, i);
                m_customPropertyWidgets.insert(i, widgetToAdd);
                AddEditableData(header, widgetToAdd, m_animationContainer);
                widgetToAdd = NULL;

                break;
            }
            case UIWidgetBase::IconsComboBox:
            case UIWidgetBase::LabelsComboBox:
            case UIWidgetBase::ImagesComboBox:
            case UIWidgetBase::TextAlignmentComboBox:
            case UIWidgetBase::FontComboBox:
            {
                quint32 currentId = m_uiWidget->GetWidgetPropertyData(i).toInt();
                QComboBox* comboBox = getNewComboBox(dataType, currentId);
                connect(comboBox, &QComboBox::currentTextChanged, mapper, static_cast<void(QSignalMapper::*)(void)>(&QSignalMapper::map));
                widgetToAdd = comboBox;
            }

            case UIWidgetBase::Not_Available: // Unsupported properties
            case UIWidgetBase::CompilerComboBox:
            case UIWidgetBase::SignalSink:
            case UIWidgetBase::SignalSinkDataTypeCombobox:
            case UIWidgetBase::FileSelector:
            case UIWidgetBase::FileSelector_ProjectJailed:
            case UIWidgetBase::FolderSelector:
            case UIWidgetBase::FolderSelector_ProjectJailed:
            default:
                break;
        }
        if (widgetToAdd != NULL)
        {
            mapper->setMapping(widgetToAdd, i);
            m_customPropertyWidgets.insert(i, widgetToAdd);
            AddEditableData(header, widgetToAdd, parent);
        }
    }
}

void UIWidgetPropertiesWidget::afterPopulateSignals()
{
    QTreeWidgetItem* root = AddContainer("Description");
    root->setExpanded(true);

    AddDisplayData("Widget Type", m_uiWidget->GetUIWidgetTypeName(), root);

    if (m_uiWidget->GetNbrOfLayersNeeded() > 0)
    {
        AddDisplayData("Rendering Type", "HW Rendered", root);

        AddDisplayData("Layers", QString::number(m_uiWidget->GetNbrOfLayersNeeded()), root);
    }
    else
    {
        AddDisplayData("Rendering Type", "SW Rendered", root);
    }

    m_description = new QTextEdit(m_uiWidget->LongDescription());
    m_description->setWordWrapMode(QTextOption::WordWrap);
    m_description->setEnabled(false);
    AddEditableData("Description", m_description, root);
}

QSpinBox* UIWidgetPropertiesWidget::getNewSpinBox(UIWidgetBase::DataType_t dataType)
{
    QSpinBox* spinBox = new QSpinBox();

    switch (dataType)
    {
        case UIWidgetBase::SpinBox_Uint8:
            spinBox->setMinimum(0);
            spinBox->setMaximum(UCHAR_MAX);
            break;
        case UIWidgetBase::SpinBox_Uint8_Min1:
            spinBox->setMinimum(1);
            spinBox->setMaximum(UCHAR_MAX);
            break;
        case UIWidgetBase::SpinBox_Uint16:
            spinBox->setMinimum(0);
            spinBox->setMaximum(USHRT_MAX);
            break;
        case UIWidgetBase::SpinBox_Uint32:
            spinBox->setMinimum(0);
            // Qt prevents a spinbox from representing a value greater than INT_MAX
            spinBox->setMaximum(INT_MAX);
            break;
        case UIWidgetBase::SpinBox_Int8:
            spinBox->setMinimum(CHAR_MIN);
            spinBox->setMaximum(CHAR_MAX);
            break;
        case UIWidgetBase::SpinBox_Int16:
            spinBox->setMinimum(SHRT_MIN);
            spinBox->setMaximum(SHRT_MAX);
            break;
        case UIWidgetBase::SpinBox_DynamicValueType:
            spinBox->setMinimum(0);
            spinBox->setMaximum(10);
            break;
        case UIWidgetBase::SpinBox_Int32:
        default:
            spinBox->setMinimum(INT_MIN);
            spinBox->setMaximum(INT_MAX);
            break;
    }

    return spinBox;
}

QComboBox* UIWidgetPropertiesWidget::getNewComboBox(UIWidgetBase::DataType_t dataType, quint32 currentId)
{
    QComboBox* comboBox = new QComboBox();
    comboBox->addItem("<unset>", QVariant::fromValue(0));

    switch (dataType)
    {
        case UIWidgetBase::IconsComboBox:
        {
            foreach (Icon* icon, getProject()->GetIconDefinition().Icons())
            {
                comboBox->addItem(icon->GetName(), QVariant::fromValue(icon->GetID()));
                if (icon->GetID() == currentId)
                {
                    comboBox->setCurrentText(icon->GetName());
                }
            }
            break;
        }
        case UIWidgetBase::ImagesComboBox:
        {
            foreach (Image* image, getProject()->GetImageDefinition().Images())
            {
                if(!image->GetUseForNonAnimations())
                    continue;

                comboBox->addItem(image->GetName(), QVariant::fromValue(image->GetID()));
                if (image->GetID() == currentId)
                {
                    comboBox->setCurrentText(image->GetName());
                }
            }
            break;
        }
        case UIWidgetBase::TextAlignmentComboBox:
        {
            comboBox->addItem("Left", 1);
            comboBox->addItem("Center", 2);
            comboBox->addItem("Right", 3);
            comboBox->setCurrentIndex(currentId);
            break;
        }
        case UIWidgetBase::LabelsComboBox:
        {
            foreach (Label* label, getProject()->GetLabelDefinition().Labels())
            {
                comboBox->addItem(label->GetMasterText(), QVariant::fromValue(label->GetID()));
                if (label->GetID() == currentId)
                {
                    comboBox->setCurrentText(label->GetMasterText());
                }
            }
            break;
        }
        case UIWidgetBase::FontComboBox:
        {
            foreach (const Font* font, getProject()->GetLabelDefinition().Fonts())
            {
                comboBox->addItem(font->Description, QVariant::fromValue(font->ID));
                if (font->ID == currentId)
                {
                    comboBox->setCurrentText(font->Description);
                }
            }
            break;
        }
        case UIWidgetBase::DynamicImageAnimationComboBox:
        {
            foreach (const Animation* anim, getProject()->GetAnimationDefinition().FrameByFrameAnimations())
            {
                comboBox->addItem(anim->GetName(), QVariant::fromValue(anim->GetID()));
                if (anim->GetID() == currentId)
                {
                    comboBox->setCurrentText(anim->GetName());
                }
            }
        } // OBS! no break
        case UIWidgetBase::AnimationComboBox:
        {
            foreach (const Animation* anim, getProject()->GetAnimationDefinition().OpacityAnimations())
            {
                comboBox->addItem(anim->GetName(), QVariant::fromValue(anim->GetID()));
                if (anim->GetID() == currentId)
                {
                    comboBox->setCurrentText(anim->GetName());
                }
            }

            foreach (const Animation* anim, getProject()->GetAnimationDefinition().TranslationAnimations())
            {
                comboBox->addItem(anim->GetName(), QVariant::fromValue(anim->GetID()));
                if (anim->GetID() == currentId)
                {
                    comboBox->setCurrentText(anim->GetName());
                }
            }
            break;
        }
        default:
            comboBox->clear();
            comboBox->addItem("<Unknown data type>");
            break;
    }

    return comboBox;
}

void UIWidgetPropertiesWidget::slot_nameChanged(const QString& newName)
{
    if (m_uiWidget->GetUserDefinedName() != newName)
    {
        m_uiWidget->SetUserDefinedName(newName);
        m_uiWidget->NotifyPropertyUpdatedRedrawNeeded();
    }
}

void UIWidgetPropertiesWidget::slot_xChanged(int newValue)
{
    if (m_uiWidget->GetX() != newValue)
    {
        m_uiWidget->SetX(newValue);
    }
}

void UIWidgetPropertiesWidget::slot_yChanged(int newValue)
{
    if (m_uiWidget->GetY() != newValue)
    {
        m_uiWidget->SetY(newValue);
    }
}

void UIWidgetPropertiesWidget::slot_widthChanged(int newValue)
{
    if (m_uiWidget->GetWidth() != newValue)
    {
        m_uiWidget->SetWidth(newValue);
    }
}

void UIWidgetPropertiesWidget::slot_heightChanged(int newValue)
{
    if (m_uiWidget->GetHeight() != newValue)
    {
        m_uiWidget->SetHeight(newValue);
    }
}

void UIWidgetPropertiesWidget::slot_boxedChanged(bool checked)
{
    if (m_uiWidget->GetBoxed() != checked)
    {
        m_uiWidget->SetBoxed(checked);
    }
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryFGRedChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetFGColor().red() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetFGColorComponent(ColorComponent::RED, newValue);
        updatePrimaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryFGGreenChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetFGColor().green() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetFGColorComponent(ColorComponent::GREEN, newValue);
        updatePrimaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryFGBlueChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetFGColor().blue() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetFGColorComponent(ColorComponent::BLUE, newValue);
        updatePrimaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_primaryBackgroundTransparent_toggled(bool checked)
{
    m_uiWidget->GetPrimaryColorPair()->SetBGIsTransparent(checked);

    m_rgbPrimaryBGRed->setEnabled(!checked);
    m_rgbPrimaryBGGreen->setEnabled(!checked);
    m_rgbPrimaryBGBlue->setEnabled(!checked);
    updatePrimaryBackgroundColorPreview();
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryBGRedChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetBGColor().red() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetBGColorComponent(ColorComponent::RED, newValue);
        updatePrimaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryBGGreenChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetBGColor().green() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetBGColorComponent(ColorComponent::GREEN, newValue);
        updatePrimaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbPrimaryBGBlueChanged(int newValue)
{
    if (m_uiWidget->GetPrimaryColorPair()->GetBGColor().blue() != newValue)
    {
        m_uiWidget->GetPrimaryColorPair()->SetBGColorComponent(ColorComponent::BLUE, newValue);
        updatePrimaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryFGRedChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetFGColor().red() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetFGColorComponent(ColorComponent::RED, newValue);
        updateSecondaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryFGGreenChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetFGColor().green() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetFGColorComponent(ColorComponent::GREEN, newValue);
        updateSecondaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryFGBlueChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetFGColor().blue() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetFGColorComponent(ColorComponent::BLUE, newValue);
        updateSecondaryForegroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_secondaryBackgroundTransparent_toggled(bool checked)
{
    m_uiWidget->GetSecondaryColorPair()->SetBGIsTransparent(checked);

    m_rgbSecondaryBGRed->setEnabled(!checked);
    m_rgbSecondaryBGGreen->setEnabled(!checked);
    m_rgbSecondaryBGBlue->setEnabled(!checked);
    updateSecondaryBackgroundColorPreview();
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryBGRedChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetBGColor().red() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetBGColorComponent(ColorComponent::RED, newValue);
        updateSecondaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryBGGreenChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetBGColor().green() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetBGColorComponent(ColorComponent::GREEN, newValue);
        updateSecondaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_rgbSecondaryBGBlueChanged(int newValue)
{
    if (m_uiWidget->GetSecondaryColorPair()->GetBGColor().blue() != newValue)
    {
        m_uiWidget->GetSecondaryColorPair()->SetBGColorComponent(ColorComponent::BLUE, newValue);
        updateSecondaryBackgroundColorPreview();
    }
}

void UIWidgetPropertiesWidget::slot_customPropertyChanged(int propertyIndex)
{
    UIWidgetBase::DataType_t dataType = m_uiWidget->GetWidgetPropertyDataType(propertyIndex);

    if (!m_customPropertyWidgets.contains(propertyIndex))
    {
        qDebug("Custom property index %d does not exist on UIWidget %s", propertyIndex, qUtf8Printable(m_uiWidget->GetUIWidgetTypeName()));
        return;
    }

    QWidget* editorWidget = m_customPropertyWidgets.value(propertyIndex);
    switch (dataType)
    {
        case UIWidgetBase::LineEdit_Editable:
        case UIWidgetBase::LineEdit_NonEditable:
        {
            QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, QVariant::fromValue(lineEdit->text()));
            break;
        }
        case UIWidgetBase::SpinBox_Uint8:
        case UIWidgetBase::SpinBox_Uint8_Min1:
        case UIWidgetBase::SpinBox_Uint16:
        case UIWidgetBase::SpinBox_Uint32:
        case UIWidgetBase::SpinBox_Int8:
        case UIWidgetBase::SpinBox_Int16:
        case UIWidgetBase::SpinBox_Int32:
        case UIWidgetBase::SpinBox_DynamicValueType:
        {
            QSpinBox* spinBox = qobject_cast<QSpinBox*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, QVariant::fromValue(spinBox->value()));
            break;
        }
        case UIWidgetBase::CheckBox:
        {
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, QVariant::fromValue(checkBox->isChecked()));
            break;
        }
        case UIWidgetBase::AnimationCheckBox:
        {
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, QVariant::fromValue(checkBox->isChecked()));
            m_uiWidget->PropertyUpdatedRedrawNeeded();
            break;
        }
        case UIWidgetBase::IconsComboBox:
        {
            QComboBox* comboBox = qobject_cast<QComboBox*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, comboBox->currentData());

            QImage icon(getProject()->GetIconDefinition().GetResource(comboBox->currentData().toUInt()));
            m_width->setValue(icon.width());
            m_heigth->setValue(icon.height());
            break;
        }
        case UIWidgetBase::LabelsComboBox:
        case UIWidgetBase::TextAlignmentComboBox:
        case UIWidgetBase::FontComboBox:
        {
             QComboBox* comboBox = qobject_cast<QComboBox*>(editorWidget);
             m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, comboBox->currentData());
             break;
        }
        case UIWidgetBase::ImagesComboBox:
        {
             QComboBox* comboBox = qobject_cast<QComboBox*>(editorWidget);
             m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, comboBox->currentData());

             QImage image(getProject()->GetImageDefinition().GetResource(comboBox->currentData().toUInt()));
             m_width->setValue(image.width());
             m_heigth->setValue(image.height());
             break;
        }
        case UIWidgetBase::DynamicImageAnimationComboBox:
        case UIWidgetBase::AnimationComboBox:
        {
            QComboBox* comboBox = qobject_cast<QComboBox*>(editorWidget);
            m_uiWidget->SetProperty(propertyIndex + UIWidgetBase::WidgetBasePropertyCount, comboBox->currentData());
            break;
        }



        // Handled by ProjectObjectProperties base class
        case UIWidgetBase::SignalSink:

        // Unsupported properties on UIWidgets
        case UIWidgetBase::CompilerComboBox:
        case UIWidgetBase::SignalSinkDataTypeCombobox:
        case UIWidgetBase::FileSelector:
        case UIWidgetBase::FileSelector_ProjectJailed:
        case UIWidgetBase::FolderSelector:
        case UIWidgetBase::FolderSelector_ProjectJailed:
        case UIWidgetBase::Not_Available:
        default:
            break;
    }
}

void UIWidgetPropertiesWidget::populateTriggers(UIWidgetBase* widget, quint8 animationIndex)
{
    new AnimationPropertiesWidget(m_animationContainer, widget, this, m_project, animationIndex);

    QList<TransitionClause*> triggers = widget->GetAnimationTriggers(animationIndex);

    foreach (TransitionClause* trigger, triggers)
    {
        new AnimationTriggerWidget(trigger, m_animationContainer, widget, m_project, this, animationIndex);
    }

    QPushButton* newTriggerButton = new QPushButton("New Trigger");
    newTriggerButton->setFixedWidth(120);
    if (animationIndex == 1)
    {
        connect(newTriggerButton, &QPushButton::clicked, this, &UIWidgetPropertiesWidget::addNewAnimation1Trigger);
    }
    else if (animationIndex == 2)
    {
        connect(newTriggerButton, &QPushButton::clicked, this, &UIWidgetPropertiesWidget::addNewAnimation2Trigger);
    }
    else
    {
        // Do nothing
    }

    AddEditableData("", NULL, m_animationContainer);
    AddEditableData("", newTriggerButton, m_animationContainer);
}

void UIWidgetPropertiesWidget::addNewAnimation1Trigger()
{
    QString componentName = m_uiWidget->GetUIWidgetTypeName() + "_" + QString::number(m_uiWidget->GetUIWidgetID()) + "_Animation1";
    TransitionClause* trigger = new TransitionClause(m_uiWidget->GetAnimationTriggers(1).count(), m_uiWidget, componentName);
    m_uiWidget->AddAnimationTrigger(1, trigger);
    m_uiWidget->NotifyPropertyUpdated();
    m_uiWidget->PropertyUpdatedRedrawNeeded();
}

void UIWidgetPropertiesWidget::addNewAnimation2Trigger()
{
    QString componentName = m_uiWidget->GetUIWidgetTypeName() + "_" + QString::number(m_uiWidget->GetUIWidgetID()) + "_Animation2";
    TransitionClause* trigger = new TransitionClause(m_uiWidget->GetAnimationTriggers(2).count(), m_uiWidget, componentName);
    m_uiWidget->AddAnimationTrigger(2, trigger);
    m_uiWidget->NotifyPropertyUpdated();
    m_uiWidget->PropertyUpdatedRedrawNeeded();
}
