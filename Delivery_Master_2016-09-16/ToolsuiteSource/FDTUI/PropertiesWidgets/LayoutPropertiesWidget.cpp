#include "LayoutPropertiesWidget.h"

#include "BSPDisplay.h"
#include "Layout.h"
#include "NameChangeWidget.h"
#include "Project.h"
#include "TransitionWidget.h"
#include "ColorPair.h"

#include <QComboBox>
#include <QLabel>
#include <QList>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QSpinBox>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QWidgetList>

LayoutPropertiesWidget::LayoutPropertiesWidget(Project* project, QWidget* parent) :
  ProjectObjectPropertiesWidget(project, parent),
  m_project(project),
  m_layout(NULL),
  m_nameLineEdit(NULL),
  m_componentNameLabel(NULL),
  m_upComboBox(NULL),
  m_downComboBox(NULL),
  m_leftComboBox(NULL),
  m_rightComboBox(NULL),
  m_setComboBox(NULL),
  m_clearComboBox(NULL),
  m_backgroundColorRedSpinBox(NULL),
  m_backgroundColorGreenSpinBox(NULL),
  m_backgroundColorBlueSpinBox(NULL),
  m_transitionCounter(0),
  m_backgroundColorRoot(NULL),
  m_signalTransitionsRoot(NULL),
  m_buttonTransitionsRoot(NULL),
  m_nameEditedTimer(new QTimer(this))
{
    m_allPropertyWidgets.clear();
}

LayoutPropertiesWidget::~LayoutPropertiesWidget()
{
}

void LayoutPropertiesWidget::SetLayout(Layout* layout)
{
    m_layout = layout;
    SetProjectObject(layout);
}

void LayoutPropertiesWidget::beforePopulateSignalSources()
{
    m_nameLineEdit = new NameChangeWidget(static_cast<ProjectObject*>(m_layout));
    m_allPropertyWidgets.append(m_nameLineEdit);
    AddEditableData("Name", m_nameLineEdit);

    // Component Name
    m_componentNameLabel = new QLabel(this);
    m_componentNameLabel->setText(m_layout->GetComponentName());
    AddEditableData("GCL Name", m_componentNameLabel);

    if (m_project->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        QTreeWidgetItem* m_backgroundColorRoot = AddContainer("Background Color");
        m_backgroundColorRoot->setExpanded(true);
        m_backgroundColorRedSpinBox = new QSpinBox();
        m_allPropertyWidgets.append(m_backgroundColorRedSpinBox);
        m_backgroundColorRedSpinBox->setPrefix("0x");
        m_backgroundColorRedSpinBox->setDisplayIntegerBase(16);
        m_backgroundColorRedSpinBox->setMaximum(0xFF);
        m_backgroundColorRedSpinBox->setValue(m_layout->GetBackgroundColor().red());
        AddEditableData("Red", m_backgroundColorRedSpinBox, m_backgroundColorRoot);
        connect(m_backgroundColorRedSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
        {
            m_layout->SetBackgroundColorComponent(ColorComponent::RED, value);
        });

        m_backgroundColorGreenSpinBox = new QSpinBox();
        m_allPropertyWidgets.append(m_backgroundColorGreenSpinBox);
        m_backgroundColorGreenSpinBox->setPrefix("0x");
        m_backgroundColorGreenSpinBox->setDisplayIntegerBase(16);
        m_backgroundColorGreenSpinBox->setMaximum(0xFF);
        m_backgroundColorGreenSpinBox->setValue(m_layout->GetBackgroundColor().green());
        AddEditableData("Green", m_backgroundColorGreenSpinBox, m_backgroundColorRoot);
        connect(m_backgroundColorGreenSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
        {
            m_layout->SetBackgroundColorComponent(ColorComponent::GREEN, value);
        });

        m_backgroundColorBlueSpinBox = new QSpinBox();
        m_allPropertyWidgets.append(m_backgroundColorBlueSpinBox);
        m_backgroundColorBlueSpinBox->setPrefix("0x");
        m_backgroundColorBlueSpinBox->setDisplayIntegerBase(16);
        m_backgroundColorBlueSpinBox->setMaximum(0xFF);
        m_backgroundColorBlueSpinBox->setValue(m_layout->GetBackgroundColor().blue());
        AddEditableData("Blue", m_backgroundColorBlueSpinBox, m_backgroundColorRoot);
        connect(m_backgroundColorBlueSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int value)
        {
            m_layout->SetBackgroundColorComponent(ColorComponent::BLUE, value);
        });
    }

    populateTransitionsTree();
    if (m_layout->IsCheckMessageScreen())
    {
        foreach (QWidget* widget, m_allPropertyWidgets)
        {
            widget->setEnabled(false);
        }
        m_nameLineEdit->setEnabled(true);
    }
}

void LayoutPropertiesWidget::populateTransitionsTree()
{
    if (!m_layout->IsStartupScreen())
    {
        m_buttonTransitionsRoot = AddContainer("Button Transitions");
        m_buttonTransitionsRoot->setExpanded(true);

        m_upComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_upComboBox);
        AddEditableData("Up Button", m_upComboBox, m_buttonTransitionsRoot);

        m_downComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_downComboBox);
        AddEditableData("Down Button", m_downComboBox, m_buttonTransitionsRoot);

        m_leftComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_leftComboBox);
        AddEditableData("Left Button", m_leftComboBox, m_buttonTransitionsRoot);

        m_rightComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_rightComboBox);
        AddEditableData("Right Button", m_rightComboBox, m_buttonTransitionsRoot);

        m_setComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_setComboBox);
        AddEditableData("Set Button", m_setComboBox, m_buttonTransitionsRoot);

        m_clearComboBox = new QComboBox(this);
        m_allPropertyWidgets.append(m_clearComboBox);
        AddEditableData("Clear Button", m_clearComboBox, m_buttonTransitionsRoot);

        populateLayoutComboboxes();

        connectLayoutComboBoxesSignals();

        AddEditableData("", NULL);

        m_signalTransitionsRoot = AddContainer("Signal Transitions");
        m_signalTransitionsRoot->setExpanded(true);

        m_transitionCounter = 0;
        foreach (TransitionClause* transition, m_layout->GetSignalTransitions())
        {
            addTransitionToTreeView(transition);
            ++m_transitionCounter;
        }

        QPushButton* newTransitionButton = new QPushButton("New Transition");
        m_allPropertyWidgets.append(newTransitionButton);
        newTransitionButton->setFixedWidth(120);
        connect(newTransitionButton, &QPushButton::clicked, this, &LayoutPropertiesWidget::addNewTransition);

        AddEditableData("", NULL);
        AddEditableData("", newTransitionButton);
    }
}

void LayoutPropertiesWidget::populateLayoutComboboxes()
{
    m_upComboBox->addItem("<unset>");
    m_downComboBox->addItem("<unset>");
    m_leftComboBox->addItem("<unset>");
    m_rightComboBox->addItem("<unset>");
    m_setComboBox->addItem("<unset>");
    m_clearComboBox->addItem("<unset>");

    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        if (layout == m_layout || layout->IsCheckMessageScreen() || layout->IsStartupScreen()) continue;

        qint8 layoutIndex;

        m_upComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_UP);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_upComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

        m_downComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_DOWN);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_downComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

        m_leftComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_LEFT);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_leftComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

        m_rightComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_RIGHT);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_rightComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

        m_setComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_SET);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_setComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

        m_clearComboBox->addItem(layout->Name(), m_project->GetLayoutDefinition().Layouts().indexOf(layout));
        layoutIndex = m_layout->GetButtonTransition(Layout::LAYOUT_CLEAR);
        if ((layoutIndex != -1) && (layoutIndex < m_project->GetLayoutDefinition().Layouts().count()))
            m_clearComboBox->setCurrentText(m_project->GetLayoutDefinition().GetLayoutFromIndex(layoutIndex)->Name());

    }
}

void LayoutPropertiesWidget::connectLayoutComboBoxesSignals()
{
    connect(m_upComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_UP);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_UP, m_upComboBox->itemData(index).toUInt());
        }
    });

    connect(m_downComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_DOWN);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_DOWN, m_downComboBox->itemData(index).toUInt());
        }
    });

    connect(m_leftComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_LEFT);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_LEFT, m_leftComboBox->itemData(index).toUInt());
        }
    });

    connect(m_rightComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_RIGHT);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_RIGHT, m_rightComboBox->itemData(index).toUInt());
        }
    });

    connect(m_setComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_SET);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_SET, m_setComboBox->itemData(index).toUInt());
        }
    });

    connect(m_clearComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
    {
        if (index == 0)
        {
            m_layout->RemoveButtonTransition(Layout::LAYOUT_CLEAR);
        }
        else
        {
            m_layout->AddButtonTransition(Layout::LAYOUT_CLEAR, m_clearComboBox->itemData(index).toUInt());
        }
    });
}

void LayoutPropertiesWidget::addTransitionToTreeView(TransitionClause* transition)
{
    new TransitionWidget(transition, m_signalTransitionsRoot, m_layout, m_project, this);
}

void LayoutPropertiesWidget::addNewTransition()
{
    TransitionClause* transition = new TransitionClause(m_transitionCounter++, m_layout);
    m_layout->AddSignalTransition(transition);
}
