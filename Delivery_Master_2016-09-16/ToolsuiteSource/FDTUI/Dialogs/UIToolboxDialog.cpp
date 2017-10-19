#include "UIToolboxDialog.h"
#include "ui_UIToolboxDialog.h"

#include "BSPDisplay.h"
#include "CheckMessage.h"
#include "DisplayArea.h"
#include "Layout.h"
#include "MenuItem.h"
#include "Project.h"
#include "UIWidgetContainerBase.h"
#include "UIWidgetsModel.h"
#include "UIWidgetsRepository.h"

#include <QItemSelectionModel>
#include <QDialogButtonBox>
#include <QPushButton>

UIToolboxDialog::UIToolboxDialog(Project *project, UIWidgetContainerBase* widgetContainer, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::UIToolboxDialog),
    m_project(project),
    m_selectedUIWidgetContainerBase(widgetContainer),
    m_activeWidget(NULL)
{
    m_ui->setupUi(this);
    m_uiWidgetsModel = new UIWidgetsModel(UIWidgetsRepository::GetUIWidgets(*m_project));
    m_ui->m_uiWidgetsListView->setModel(m_uiWidgetsModel);

    connect(m_ui->m_uiWidgetsListView->selectionModel(), &QItemSelectionModel::currentChanged, this, &UIToolboxDialog::trySelectedWidget);
    connect(m_ui->m_buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &UIToolboxDialog::createSelectedWidget);

    m_ui->m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

UIToolboxDialog::~UIToolboxDialog()
{
    delete m_ui;
    delete m_uiWidgetsModel;
}

UIWidgetBase* UIToolboxDialog::createSelectedWidget()
{
    m_activeWidget = m_activeWidget->Clone();
    m_selectedUIWidgetContainerBase->AddUIWidget(m_activeWidget);
    checkMessageHook();
    emit signal_widgetAdded(m_activeWidget);
    return m_activeWidget;
}

bool UIToolboxDialog::checkMessageHook()
{
    if (m_activeWidget->GetSignalSinks().count() > 0)
    {
        GCLSignalSink* signalSink = m_activeWidget->GetSignalSink("ResourceIDToDisplay");

        DisplayArea* possibleDisplayArea = dynamic_cast<DisplayArea*>(m_selectedUIWidgetContainerBase);

        if (possibleDisplayArea != NULL && possibleDisplayArea->IsCheckMessageArea() && signalSink != NULL)
        {
            m_project->GetCheckMessagesDefinition().AddWidgetSink(m_activeWidget->GetSignalSink("ResourceIDToDisplay"));
            foreach (CheckMessage* checkMessage, m_project->GetCheckMessagesDefinition().CheckMessages())
            {
                checkMessage->AddItem(new CheckMessageItem(m_project->GetCheckMessagesDefinition().GetIndexOfSource(signalSink->SignalName(), signalSink->ComponentName()), m_activeWidget->GetUIWidgetTypeName(), 1, m_activeWidget->GetUIWidgetID()));
            }

            return true;
        }
    }

    return false;
}

bool UIToolboxDialog::validation()
{
    if (m_selectedUIWidgetContainerBase == NULL) return false;

    if (m_uiWidgetsModel == NULL) return false;

    if (m_selectedUIWidgetContainerBase == NULL) return false;

    if (m_activeWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_ListNavigation)
    {
        if (!validateListNavigationWidget()) return false;
    }

    MenuItem* menuItem = dynamic_cast<MenuItem*>(m_selectedUIWidgetContainerBase);
    if (menuItem != NULL) return validateMenuItem(menuItem);

    QModelIndex currentIndex = m_ui->m_uiWidgetsListView->currentIndex();
    if (!currentIndex.isValid()) return false;

    return true;

}

bool UIToolboxDialog::validateListNavigationWidget()
{
    if (!m_project->GetLayoutDefinition().HasMenu())
    {
        //not allowed to add a listnavigation widget if there's no menu
        return false;
    }

    DisplayArea* currentDisplayArea = dynamic_cast<DisplayArea*>(m_selectedUIWidgetContainerBase);

    if (currentDisplayArea != NULL)
    {
        Layout* currentLayout = dynamic_cast<Layout*>(currentDisplayArea->GetParent());

        if (currentLayout != NULL)
        {
            if (currentLayout != m_project->GetLayoutDefinition().GetMenuLayout())
            {
                // not allowed to add a listnavigation widget if not on the menu layout
                return false;
            }
        }
    }
    else
    {
        // not allowed to add a listnavigation widget to a menu item
        return false;
    }

    //not allowed to add a listnavigation widget twice
    foreach (Layout* layout, m_project->GetLayoutDefinition().Layouts())
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);
        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_ListNavigation)
            {
                return false;
            }
        }
    }

    return true;
}

bool UIToolboxDialog::validateMenuItem(const MenuItem* /*menuItem*/)
{
    //no support for Image/DynamicImages within a menu layer
    if (m_activeWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
        return false;

    if (m_activeWidget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
        return false;

    return true;
}

bool UIToolboxDialog::trySelectedWidget(const QModelIndex& currentIndex, const QModelIndex& /*previous*/)
{
    if (!currentIndex.isValid()) return NULL;

    m_activeWidget = m_uiWidgetsModel->GetUIWidget(currentIndex.row());
    m_ui->m_uiWidgetsListView->setCurrentIndex(currentIndex);
    m_ui->textBrowser->setHtml(m_uiWidgetsModel->data(currentIndex, Qt::WhatsThisRole).toString());

    if (validation())
    {
        m_ui->m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        return true;
    }
    else
    {
        m_ui->m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return false;
    }

}
