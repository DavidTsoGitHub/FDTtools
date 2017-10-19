#include "MenuEditor.h"
#include "ui_MenuEditor.h"

#include "BSPDisplay.h"
#include "IconWidget.h"
#include "Layout.h"
#include "LabelWidget.h"
#include "MenuDefinition.h"
#include "MenuItem.h"
#include "MenuItemPreviewWidget.h"
#include "MenuItemWidgetsModel.h"
#include "MenuTreeModel.h"
#include "Project.h"
#include "ProjectObject.h"
#include "SelectDisplayAreaDialog.h"
#include "UIToolboxDialog.h"

#include <QComboBox>
#include <QMessageBox>
#include <QString>
#include <QWidget>

MenuEditor::MenuEditor(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MenuEditor),
    m_menuTreeModel(NULL),
    m_menuItemWidgetsModel(NULL),
    m_menuDefinition(NULL),
    m_menuItemPreviewWidget(NULL),
    m_project(NULL),
    m_currentProjectObject(NULL)
{
    m_ui->setupUi(this);

    setupComboBoxes();

    connect(m_ui->addMenuItemButton, &QPushButton::clicked, [=]
    {
        bool areaInLayout = false;
        foreach(Layout* layout, m_project->GetLayoutDefinition().Layouts())
        {
            if(layout->DisplayAreas().count() > 0)
            {
                areaInLayout = true;
                break;
            }
        }

        if(!areaInLayout)
        {
            QMessageBox messageBox;
            messageBox.setText("Layout/Area missing");
            messageBox.setInformativeText("The project has no layouts or areas defined to add menu to.");
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.exec();
            return;
        }

        if (!m_project->GetLayoutDefinition().HasMenu())
        {
            SelectDisplayAreaDialog dialog(m_project);
            QDialog::DialogCode result = (QDialog::DialogCode)dialog.exec();

            if (result == QDialog::Accepted)
            {
                DisplayArea* area = dialog.GetSelectedDisplayArea();
                Layout* layout = static_cast<Layout*>(area->GetParent());
                layout->SetMenu(MenuDefinition::GetEmptyMenuDefinition(), area);
                SetProject(m_project);
                m_ui->MenuEditorTabWidget->setTabEnabled(m_ui->MenuEditorTabWidget->indexOf(m_ui->InputConfigurationTab), true);
            }
        }
        else
        {
            m_menuTreeModel->AddMenuItem(m_ui->menuBrowserTreeView->currentIndex());
        }
    });

    connect(m_ui->addWidgetButton, &QPushButton::clicked, [=]
    {
        QModelIndex currentMenuItemIndex = m_ui->menuBrowserTreeView->currentIndex();

        if (!currentMenuItemIndex.isValid()) return;
        MenuItem* menuItem = static_cast<MenuItem*>(currentMenuItemIndex.internalPointer());

        UIToolboxDialog* uiToolboxDialog = new UIToolboxDialog(m_project, menuItem);
        uiToolboxDialog->exec();
        delete uiToolboxDialog;
    });

    connect(m_ui->removeMenuItemButton, &QPushButton::clicked, [=]
    {
        m_currentProjectObject = static_cast<ProjectObject*>(m_ui->menuBrowserTreeView->currentIndex().internalPointer());

        emit signal_MenuEditorItemSelected(m_currentProjectObject);
        m_menuItemPreviewWidget->SetMenuItem(NULL);
        delete m_menuItemWidgetsModel; m_menuItemWidgetsModel = NULL;

        if (m_currentProjectObject == m_menuDefinition->MenuRootPointer())
        {
            QMessageBox removeMenuMessageBox;
            removeMenuMessageBox.setWindowTitle("Remove Menu");
            removeMenuMessageBox.setText("The Menu is about to be deleted. This will also remove the menu navigation item from the layout.\n" \
                                         "If the menu is very large this might take a while and the application might stop respongding during the operation.\n\nDo you want to continue?");
            removeMenuMessageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            removeMenuMessageBox.setDefaultButton(QMessageBox::Cancel);
            int res =  removeMenuMessageBox.exec();

            if (res != QMessageBox::Ok)
            {
                return;
            }

            RemoveCompleteMenu();
        }
        else
        {
            m_menuTreeModel->RemoveMenuItem(m_ui->menuBrowserTreeView->currentIndex());
        }

        if (m_ui->menuBrowserTreeView->currentIndex().isValid())
        {
            m_currentProjectObject = static_cast<ProjectObject*>(m_ui->menuBrowserTreeView->currentIndex().internalPointer());
            SetCurrentProjectObject(m_currentProjectObject);
            emit signal_MenuEditorItemSelected(m_currentProjectObject);
        }
    });
    connect(m_ui->moveMenuItemDownButton, &QPushButton::clicked, [=] { m_menuTreeModel->MoveMenuItemDown(m_ui->menuBrowserTreeView->currentIndex()); });
    connect(m_ui->moveMenuItemUpButton, &QPushButton::clicked, [=] { m_menuTreeModel->MoveMenuItemUp(m_ui->menuBrowserTreeView->currentIndex()); });
    connect(m_ui->removeWidgetButton, &QPushButton::clicked, [=] { m_menuItemWidgetsModel->RemoveWidget(m_ui->menuItemWidgetsListView->currentIndex()); });

}

MenuEditor::~MenuEditor()
{
    delete m_ui;
}

void MenuEditor::SetProject(Project* project)
{
    m_project = project;
    delete m_menuItemPreviewWidget;
    m_menuItemPreviewWidget = NULL;

    const BSPDisplay* display = m_project->GetBSPSpecification().GetDisplay();

    quint16 displayWidth = 0;
    quint16 displayHeight = 0;

    if (display->GetRenderingOrientation() == BSPDisplay::CounterClockWise)
    {
        displayWidth = display->GetHeight();
        displayHeight = display->GetWidth();
    }
    else
    {
        displayWidth = display->GetWidth();
        displayHeight = display->GetHeight();
    }

    m_menuItemPreviewWidget = new MenuItemPreviewWidget(displayHeight, displayWidth);
    m_ui->menuDesignPreviewLayout->addWidget(m_menuItemPreviewWidget);
    m_menuItemPreviewWidget->SetProject(project);

    if (!m_project->GetLayoutDefinition().HasMenu())
    {
        m_ui->MenuEditorTabWidget->setTabEnabled(m_ui->MenuEditorTabWidget->indexOf(m_ui->InputConfigurationTab), false);
        return;
    }
    else
    {
        m_ui->MenuEditorTabWidget->setTabEnabled(m_ui->MenuEditorTabWidget->indexOf(m_ui->InputConfigurationTab), true);
    }

    m_menuDefinition = project->GetLayoutDefinition().GetMenu();

    m_menuTreeModel = new MenuTreeModel(m_project);
    m_ui->menuBrowserTreeView->setModel(m_menuTreeModel);
    m_ui->menuBrowserTreeView->expandToDepth(1);
    connect(m_ui->menuBrowserTreeView, &QTreeView::clicked, [=] (const QModelIndex& clicked)
    {
        m_currentProjectObject = static_cast<ProjectObject*>(clicked.internalPointer());
        m_ui->removeMenuItemButton->setEnabled(clicked.isValid());
        m_ui->moveMenuItemDownButton->setEnabled(m_currentProjectObject != m_menuDefinition->MenuRootPointer());
        m_ui->moveMenuItemUpButton->setEnabled(m_currentProjectObject != m_menuDefinition->MenuRootPointer());
        m_ui->addWidgetButton->setEnabled(m_currentProjectObject != m_menuDefinition->MenuRootPointer());

        delete m_menuItemWidgetsModel;
        m_menuItemWidgetsModel = NULL;

        MenuItem* menuItem = dynamic_cast<MenuItem*>(m_currentProjectObject);
        m_menuItemPreviewWidget->SetMenuItem(menuItem);

        if (menuItem != NULL)
        {
            setupItemWidgetsList(menuItem);
        }

        emit signal_MenuEditorItemSelected(m_currentProjectObject);
    });

    connect(m_menuTreeModel, &MenuTreeModel::signal_modified, this, &MenuEditor::signal_modified);

    populateInputMapping();
}

void MenuEditor::ClearProject()
{
    m_project = NULL;

    if (m_menuItemPreviewWidget != NULL)
    {
        delete m_menuItemPreviewWidget;
        m_menuItemPreviewWidget = NULL;
    }

    if (m_menuItemWidgetsModel != NULL)
    {
        delete m_menuItemWidgetsModel;
        m_menuItemWidgetsModel = NULL;
    }

    if (m_menuTreeModel != NULL)
    {
        delete m_menuTreeModel;
        m_menuTreeModel = NULL;
    }
}

void MenuEditor::SetCurrentProjectObject(ProjectObject* projectObject)
{
    MenuItem* menuItem = dynamic_cast<MenuItem*>(projectObject);
    if (menuItem != NULL)
    {
        selectMenuItem(menuItem);
    }
    else
    {
        UIWidgetBase* uiWidget = dynamic_cast<UIWidgetBase*>(projectObject);
        if (uiWidget != NULL)
        {
            MenuItem* parent = dynamic_cast<MenuItem*>(uiWidget->GetParent());
            if (parent == NULL) return;

            if (m_ui->menuBrowserTreeView->currentIndex().isValid() &&
                m_ui->menuBrowserTreeView->currentIndex().internalPointer() != parent)
            {
                selectMenuItem(parent);
            }

            m_ui->menuItemWidgetsListView->setCurrentIndex(m_menuItemWidgetsModel->FindIndexForWidget(uiWidget));
            m_menuItemPreviewWidget->HighLightUIWidget(uiWidget);
        }
    }
}

void MenuEditor::JustSelected()
{
    if (m_project->GetLayoutDefinition().HasMenu())
    {
        m_menuItemPreviewWidget->Redraw();
    }

    if (m_currentProjectObject != NULL)
    {
        emit signal_MenuEditorItemSelected(m_currentProjectObject);
    }
}

void MenuEditor::RemoveCompleteMenu()
{
    m_ui->menuBrowserTreeView->setModel(NULL);
    m_menuItemPreviewWidget->SetProject(NULL);
    m_menuTreeModel->RemoveMenu();

    m_project->GetLayoutDefinition().GetMenuLayout()->RemoveMenu(true);

    m_ui->removeMenuItemButton->setEnabled(false);
    m_ui->MenuEditorTabWidget->setTabEnabled(m_ui->MenuEditorTabWidget->indexOf(m_ui->InputConfigurationTab), false);
}

void MenuEditor::setupItemWidgetsList(MenuItem* menuItem)
{
    m_ui->removeWidgetButton->setEnabled(false);

    if (menuItem == NULL)
    {
        QItemSelectionModel* m = m_ui->menuItemWidgetsListView->selectionModel();
        delete m;
    }
    else
    {
        m_menuItemWidgetsModel = new MenuItemWidgetsModel(menuItem);
        QItemSelectionModel* m = m_ui->menuItemWidgetsListView->selectionModel();
        m_ui->menuItemWidgetsListView->setModel(m_menuItemWidgetsModel);
        delete m;
        connect(m_ui->menuItemWidgetsListView->selectionModel(), &QItemSelectionModel::currentChanged, [=] (const QModelIndex& current, const QModelIndex&)
        {
            m_ui->removeWidgetButton->setEnabled(current.isValid());

            m_currentProjectObject = static_cast<ProjectObject*>(current.internalPointer());
            UIWidgetBase* widget = static_cast<UIWidgetBase*>(m_currentProjectObject);
            m_menuItemPreviewWidget->HighLightUIWidget(widget);
            emit signal_MenuEditorItemSelected(m_currentProjectObject);
        });

        connect(m_menuItemWidgetsModel, &MenuItemWidgetsModel::signal_modified, this, &MenuEditor::signal_modified);
    }
}

void MenuEditor::selectMenuItem(MenuItem* itemToSelect)
{
    m_menuItemPreviewWidget->SetMenuItem(itemToSelect);
    m_ui->menuBrowserTreeView->setCurrentIndex(m_menuTreeModel->FindIndexForMenuItem(itemToSelect));

    delete m_menuItemWidgetsModel;

    setupItemWidgetsList(itemToSelect);
}

void MenuEditor::setupComboBoxes()
{
    setupComboBox(m_ui->inputSignalUpActionComboBox);
    setupComboBox(m_ui->inputSignalDownActionComboBox);
    setupComboBox(m_ui->inputSignalLeftActionComboBox);
    setupComboBox(m_ui->inputSignalRightActionComboBox);
    setupComboBox(m_ui->inputSignalSetActionComboBox);
    setupComboBox(m_ui->inputSignalClearActionComboBox);
}

void MenuEditor::setupComboBox(QComboBox* comboBox)
{
    addComboBoxItems(comboBox);
    connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &MenuEditor::slot_inputMappingComboBox_activated);
}

void MenuEditor::addComboBoxItems(QComboBox* comboBox)
{
    comboBox->addItem(MenuEditor::stringForCommand(CommandNone), CommandNone);
    comboBox->addItem(MenuEditor::stringForCommand(CommandNext), CommandNext);
    comboBox->addItem(MenuEditor::stringForCommand(CommandPrevious), CommandPrevious);
    comboBox->addItem(MenuEditor::stringForCommand(CommandEnter), CommandEnter);
    comboBox->addItem(MenuEditor::stringForCommand(CommandLeave), CommandLeave);
}

void MenuEditor::populateInputMapping()
{
    selectMappingComboBoxItem(m_ui->inputSignalUpActionComboBox, m_menuDefinition->GetCommandForInput(InputUp));
    selectMappingComboBoxItem(m_ui->inputSignalDownActionComboBox, m_menuDefinition->GetCommandForInput(InputDown));
    selectMappingComboBoxItem(m_ui->inputSignalLeftActionComboBox, m_menuDefinition->GetCommandForInput(InputLeft));
    selectMappingComboBoxItem(m_ui->inputSignalRightActionComboBox, m_menuDefinition->GetCommandForInput(InputRight));
    selectMappingComboBoxItem(m_ui->inputSignalSetActionComboBox, m_menuDefinition->GetCommandForInput(InputSet));
    selectMappingComboBoxItem(m_ui->inputSignalClearActionComboBox, m_menuDefinition->GetCommandForInput(InputClear));
}

void MenuEditor::selectMappingComboBoxItem(QComboBox* comboBox, MenuCommandID command)
{
    comboBox->setCurrentText(MenuEditor::stringForCommand(command));
}

QString MenuEditor::stringForCommand(MenuCommandID command)
{
    switch (command)
    {
        case CommandNext:
            return QString("Next Menu Item");
        case CommandPrevious:
            return QString("Previous Menu Item");
        case CommandEnter:
            return QString("Enter Submenu");
        case CommandLeave:
            return QString("Leave Submenu");
        case CommandNone:
        default:
            return QString("Unmapped");
    }
}

void MenuEditor::slot_inputMappingComboBox_activated(int currentIndex)
{
    QComboBox* changedComboBox = static_cast<QComboBox*>(sender());
    MenuCommandID command = static_cast<MenuCommandID>(changedComboBox->itemData(currentIndex).toInt());

    if (changedComboBox == m_ui->inputSignalUpActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputUp, command);
    }
    else if (changedComboBox == m_ui->inputSignalDownActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputDown, command);
    }
    else if (changedComboBox == m_ui->inputSignalLeftActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputLeft, command);
    }
    else if (changedComboBox == m_ui->inputSignalRightActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputRight, command);
    }
    else if (changedComboBox == m_ui->inputSignalSetActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputSet, command);
    }
    else if (changedComboBox == m_ui->inputSignalClearActionComboBox)
    {
        m_menuDefinition->ModifyInputToCommandMapping(InputClear, command);
    }
    else
    {
        // Do Nothing
    }
}
