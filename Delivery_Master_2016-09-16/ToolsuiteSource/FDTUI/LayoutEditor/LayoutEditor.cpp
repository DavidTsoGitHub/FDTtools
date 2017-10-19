#include "LayoutEditor.h"
#include "ui_LayoutEditor.h"
#include "SelectDisplayAreaDialog.h"
#include "AddNewLayoutDialog.h"
#include "Layout.h"
#include "LayoutPreviewWidget.h"
#include "LayoutsTreeModel.h"
#include "ProjectObject.h"
#include "UIToolboxDialog.h"
#include "UIWidgetsRepository.h"
#include "DynamicLabelWidget.h"
#include "DynamicImageWidget.h"
#include <QMessageBox>

LayoutEditor::LayoutEditor(QWidget *parent) :
    QWidget(parent),
    m_layoutPreviewWidget(NULL),
    m_layoutsTreeModel(NULL),
    m_project(NULL),
    m_ui(new Ui::LayoutEditor),
    m_layout(NULL),
    m_currentSelectedObject(NULL),
    m_layoutChoosenInProjectBrowser(false)
{
    m_ui->setupUi(this);

    m_layoutPreviewWidget = new LayoutPreviewWidget();
    m_ui->previewLayout->addWidget(m_layoutPreviewWidget);

    setupButtonSignals();
    connect(this, &LayoutEditor::signal_updateButtonStates, this, &LayoutEditor::setButtonStates);
    connect(this, &LayoutEditor::signal_LayoutEditorItemSelected, this, &LayoutEditor::signal_updateButtonStates);
}

LayoutEditor::~LayoutEditor()
{
    delete m_ui;
}

void LayoutEditor::SetProject(Project* project)
{
    blockSignals(true);
    m_project = project;
    m_layoutPreviewWidget->SetProject(project);

    setupLayoutModel();
    setupListSignals();

    m_ui->layoutsView->setFocus();
    m_ui->layoutsView->selectRow(0);
    blockSignals(false);
}

void LayoutEditor::ClearProject()
{
    m_project = NULL;
    delete m_layoutsTreeModel;
    m_layoutsTreeModel = NULL;
}

void LayoutEditor::setupLayoutModel()
{
    setLayoutButtonsDisabled();
    setAreaButtonsDisabled();
    setWidgetButtonsDisabled();

    delete m_layoutsTreeModel;
    m_layoutsTreeModel = new LayoutsTreeModel(m_project->GetLayoutDefinition());

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_modified, this, &LayoutEditor::signal_modified);

    setLayoutsModel(m_layoutsTreeModel);
    m_ui->layoutsView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_ui->layoutsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui->layoutsView->setSelectionMode(QAbstractItemView::SingleSelection);

    setAreasModel(m_layoutsTreeModel);
    setWidgetsModel(m_layoutsTreeModel);

    m_ui->areasView->setRootIndex(m_layoutsTreeModel->index(0, 0, QModelIndex()));
    m_ui->widgetsView->setRootIndex(m_layoutsTreeModel->index(0, 0, QModelIndex()));
}

void LayoutEditor::setupListSignals()
{
    // Listen to and emit layout changes
    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_layoutAdded, this, &LayoutEditor::signal_layoutAdded);

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_layoutAdded, this, [=] ()
    {
        m_ui->layoutsView->setFocus();
        m_ui->layoutsView->selectRow(0);
        setWidgetsModel(NULL);
    });

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_layoutRemoved, this, &LayoutEditor::signal_layoutRemoved);

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_layoutRemoved, this, [=] ()
    {
        m_ui->layoutsView->setFocus();
    });

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_areaAdded, this, [=] ()
    {
        slot_layoutView_CurrentChanged(m_ui->layoutsView->selectionModel()->currentIndex());
    });

    connect(m_layoutsTreeModel, &LayoutsTreeModel::signal_widgetRemoved, this, [=] ()
    {
        m_currentSelectedObject = NULL;
        signal_LayoutEditorItemSelected(m_currentSelectedObject);
    });
}

void LayoutEditor::slot_layoutView_CurrentChanged(const QModelIndex& current)
{
    m_ui->areasView->selectionModel()->clearCurrentIndex();
    m_ui->areasView->selectionModel()->clearSelection();

    m_layout = static_cast<Layout*>(current.internalPointer());
    if (m_layout != NULL && current.isValid())
    {
        if (m_ui->areasView->model() == NULL)
        {
            setAreasModel(m_layoutsTreeModel);
            m_ui->areasView->setRootIndex(current);
        }
        else
        {
            m_ui->areasView->setRootIndex(current);
        }

        setWidgetsModel(NULL);

        m_layoutPreviewWidget->SetLayout(m_layout->GetID());
        m_currentSelectedObject = static_cast<ProjectObject*>(current.internalPointer());
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    }
    else
    {
        setAreasModel(NULL);
        setWidgetsModel(NULL);

        m_layoutPreviewWidget->SetLayout(INT_MAX);
        m_currentSelectedObject = NULL;
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    }
}

void LayoutEditor::slot_areasView_CurrentChanged(const QModelIndex& current)
{
    m_ui->widgetsView->selectionModel()->clearCurrentIndex();
    m_ui->widgetsView->selectionModel()->clearSelection();

    if (current.isValid() && static_cast<DisplayArea*>(current.internalPointer()) != NULL)
    {
        if (m_ui->widgetsView->model() == NULL)
        {
            setWidgetsModel(m_layoutsTreeModel);
            m_ui->widgetsView->setRootIndex(current);
        }
        else
        {
            m_layoutPreviewWidget->HighlightDisplayArea(current);
            m_ui->widgetsView->setRootIndex(current);
        }

        m_currentSelectedObject = static_cast<ProjectObject*>(current.internalPointer());
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    }
    else
    {
        setWidgetsModel(NULL);

        m_currentSelectedObject = NULL;
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    }

    //set button state for addWidget
    if (m_ui->areasView->selectionModel()->selectedRows().count() > 0)
    {
        m_ui->addWidgetButton->setEnabled(true);
    }
}

void LayoutEditor::slot_layoutView_Clicked(const QModelIndex &clicked)
{
    m_currentSelectedObject = static_cast<ProjectObject*>(clicked.internalPointer());
    emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
}

void LayoutEditor::slot_areasView_Clicked(const QModelIndex &clicked)
{
    m_currentSelectedObject = static_cast<ProjectObject*>(clicked.internalPointer());;
    emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
}

void LayoutEditor::slot_widgetsView_Clicked(const QModelIndex &clicked)
{
    m_currentSelectedObject = static_cast<ProjectObject*>(clicked.internalPointer());
    emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
}

void LayoutEditor::slot_widgetsView_CurrentChanged(const QModelIndex &current)
{
    if (current.isValid())
    {
        m_currentSelectedObject = static_cast<ProjectObject*>(current.internalPointer());
    }
    else
    {
        m_currentSelectedObject = NULL;
    }

    emit signal_LayoutEditorItemSelected(m_currentSelectedObject);

    m_layoutPreviewWidget->HighlightUIWidget(current);
}

void LayoutEditor::setButtonStates()
{
    LayoutDefinition& layoutDefinition = m_project->GetLayoutDefinition();
    int layoutCount = layoutDefinition.Layouts().count();

    if (layoutCount == 0)
    {
        m_ui->removeLayoutButton->setEnabled(false);
    }
    else
    {
        m_ui->removeLayoutButton->setEnabled(true);
    }

    QModelIndexList selectedLayoutIndexList = m_ui->layoutsView->selectionModel()->selectedIndexes();
    Layout* layout = NULL;
    if (selectedLayoutIndexList.count() > 0)
    {
        QModelIndex layoutModelIndex = selectedLayoutIndexList.first();
        layout = static_cast<Layout*>(layoutModelIndex.internalPointer());
        m_ui->addLayoutButton->setEnabled(true);
        int layoutIndex = layoutDefinition.Layouts().indexOf(layout);

        Layout* startupLayout = layoutDefinition.GetStartupLayout();
        int startupLayoutIndex = -1;
        if (startupLayout != NULL)
        {
            startupLayoutIndex = layoutDefinition.Layouts().indexOf(startupLayout);
        }

        Layout* checkMessageLayout = layoutDefinition.GetCheckMessageLayout();
        int checkMessageLayoutIndex = -1;
        if (checkMessageLayout != NULL)
        {
            checkMessageLayoutIndex = layoutDefinition.Layouts().indexOf(checkMessageLayout);
        }

        //allow moving layout up in priority, except for the topmost layout
        if (layoutIndex < 1)
        {
            m_ui->moveUpButton->setEnabled(false);
        }
        else
        {
            m_ui->moveUpButton->setEnabled(true);
        }

        //allow moving layouts down in priority, except for the last layout
        if (layoutIndex >= (layoutCount - 1))
        {
            m_ui->moveDownButton->setEnabled(false);
        }
        else
        {
            m_ui->moveDownButton->setEnabled(true);
        }

        //blocks the ability to move regular layouts above the startup layout
        if (startupLayoutIndex != -1)
        {
            if ( layoutIndex == (startupLayoutIndex + 1) )
            {
                m_ui->moveUpButton->setEnabled(false);
            }
            else
            {
                m_ui->moveUpButton->setEnabled(true);
            }
        }

        //blocks the ability to move regular layouts below the check message layout
        if (checkMessageLayoutIndex != -1)
        {
            if ( layoutIndex == (checkMessageLayoutIndex - 1) )
            {
                m_ui->moveDownButton->setEnabled(false);
            }
            else
            {
                m_ui->moveDownButton->setEnabled(true);
            }
        }

        //blocks the ability to move checkmessagelayouts and startup layouts altogether
        if (layout->IsStartupScreen() || layout->IsCheckMessageScreen())
        {
            m_ui->moveUpButton->setEnabled(false);
            m_ui->moveDownButton->setEnabled(false);
        }

        if (layout->IsCheckMessageScreen())
        {
            m_ui->addAreaButton->setEnabled(false);
            m_ui->removeAreaButton->setEnabled(false);
        }
        else
        {
            m_ui->addAreaButton->setEnabled(true);
        }

        if (layout->DisplayAreas().count() == 0)
        {
            m_ui->removeAreaButton->setEnabled(false);
        }
    }
    else
    {
        setLayoutButtonsDisabled();
        setAreaButtonsDisabled();
        setWidgetButtonsDisabled();
        return;
    }

    QModelIndex currentAreaIndex = m_ui->areasView->currentIndex();

    if (currentAreaIndex.isValid())
    {
        //QModelIndex areaModelIndex = currentAreaIndex.first();
        DisplayArea* displayArea = static_cast<DisplayArea*>(currentAreaIndex.internalPointer());
        QModelIndex layoutIndex = m_ui->layoutsView->currentIndex();
        if (!layoutIndex.isValid()) return;

        Layout* layout = static_cast<Layout*>(layoutIndex.internalPointer());
        int displayAreaCount = layout->DisplayAreas().count();
        int areaIndex = layout->DisplayAreas().indexOf(displayArea);

        if (areaIndex < 0)
        {
            //No area is selected in the current layout
            m_ui->areasView->selectionModel()->clearSelection();
            setAreaButtonsDisabled();
            m_ui->addAreaButton->setEnabled(true);
            setWidgetButtonsDisabled();
            return;
        }
        else
        {
            m_ui->addWidgetButton->setEnabled(true);
        }

        //allow moving area up in priority, except for the topmost area
        if (areaIndex < 1)
        {
            m_ui->moveAreaUpButton->setEnabled(false);
        }
        else
        {
            m_ui->moveAreaUpButton->setEnabled(true);
        }

        //allow moving area down in priority, except for the last area
        if (areaIndex >= (displayAreaCount - 1))
        {
            m_ui->moveAreaDownButton->setEnabled(false);
        }
        else
        {
            m_ui->moveAreaDownButton->setEnabled(true);
        }

        if (displayArea->IsCheckMessageArea())
        {
            m_ui->addAreaButton->setEnabled(false);
            m_ui->removeAreaButton->setEnabled(false);
        }
        else
        {
            m_ui->addAreaButton->setEnabled(true);
            m_ui->removeAreaButton->setEnabled(true);
        }

    }
    else
    {
        setAreaButtonsDisabled();
        setWidgetButtonsDisabled();

        if (!layout->IsCheckMessageScreen())
        {
            m_ui->addAreaButton->setEnabled(true);
        }
        return;
    }

    QModelIndexList selectedWidgetIndexList = m_ui->widgetsView->selectionModel()->selectedIndexes();

    if (selectedWidgetIndexList.count() > 0)
    {
        UIWidgetBase* widget = static_cast<UIWidgetBase*>(selectedWidgetIndexList.first().internalPointer());

        QModelIndex areaIndex = m_ui->areasView->currentIndex();
        if (!areaIndex.isValid()) return;

        DisplayArea* displayArea = static_cast<DisplayArea*>(areaIndex.internalPointer());

        int widgetCount = displayArea->UIWidgets().count();

        int widgetIndex = displayArea->UIWidgets().indexOf(widget);

        if (widgetIndex < 0)
        {
            //No widget in this area is selected
            m_ui->widgetsView->selectionModel()->clearSelection();
            setWidgetButtonsDisabled();
            m_ui->addWidgetButton->setEnabled(true);
            return;
        }

        m_ui->removeWidgetButton->setEnabled(true);

        //allow moving widget up in priority, except for the topmost widget
        if (widgetIndex < 1)
        {
            m_ui->moveWidgetUpButton->setEnabled(false);
        }
        else
        {
            m_ui->moveWidgetUpButton->setEnabled(true);
        }

        //allow moving widget down in priority, except for the last widget
        if (widgetIndex >= (widgetCount - 1))
        {
            m_ui->moveWidgetDownButton->setEnabled(false);
        }
        else
        {
            m_ui->moveWidgetDownButton->setEnabled(true);
        }

        if (displayArea->IsCheckMessageArea())
        {
            m_ui->moveWidgetButton->setEnabled(false);
        }
        else
        {
            m_ui->moveWidgetButton->setEnabled(true);
        }
    }
    else
    {
        setWidgetButtonsDisabled();
        m_ui->addWidgetButton->setEnabled(true);
    }
}

void LayoutEditor::slot_addNewCheckMessageLayout()
{
    Layout* checkMessageLayout = m_project->GetLayoutDefinition().GetCheckMessageLayout();
    DisplayArea* checkMessageArea = m_layoutsTreeModel->AddAreaToLayout(checkMessageLayout, "Check Message Area");

    checkMessageArea->SetCheckMessageArea(true);
}

void LayoutEditor::JustSelected()
{
    if (m_currentSelectedObject != NULL && m_layoutChoosenInProjectBrowser == false)
    {
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    }

    m_layoutChoosenInProjectBrowser = false;
}

void LayoutEditor::LayoutChoosenInProjectBrowser(void)
{
    m_layoutChoosenInProjectBrowser = true;
}

void LayoutEditor::LayoutNotChoosenInProjectBrowser(void)
{
    m_layoutChoosenInProjectBrowser = false;
}

void LayoutEditor::setupButtonSignals()
{
    // Layout buttons
    connect(m_ui->addLayoutButton, &QPushButton::clicked, [=]
    {
        AddNewLayoutDialog addNewLayoutDialog(m_project->GetLayoutDefinition());
        QDialog::DialogCode result = static_cast<QDialog::DialogCode>(addNewLayoutDialog.exec());
        if (result != QDialog::Accepted) return;

        QString layoutName = addNewLayoutDialog.GetLayoutName();
        LayoutDefinition::LayoutType type = addNewLayoutDialog.GetLayoutType();

        Layout* layout = m_layoutsTreeModel->AddLayout(layoutName, type);
        if (type == LayoutDefinition::CheckMessage)
        {
            slot_addNewCheckMessageLayout();
        }

        if (m_project->GetLayoutDefinition().Layouts().count() == 1)
        {
            setAreasModel(NULL);
            setWidgetsModel(NULL);
        }

        QModelIndex indexOfLayout = m_layoutsTreeModel->index(m_project->GetLayoutDefinition().Layouts().indexOf(layout), 0, QModelIndex());
        m_ui->layoutsView->setCurrentIndex(indexOfLayout);
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    });

    connect(m_ui->removeLayoutButton, &QPushButton::clicked, [=]
    {
        QModelIndex layoutIndex = m_ui->layoutsView->currentIndex();
        if (!layoutIndex.isValid()) return;

        Layout* layoutToRemove = static_cast<Layout*>(layoutIndex.internalPointer());

        QMessageBox messageBox;
        messageBox.setText("The layout '" + layoutToRemove->Name() + "' will be removed permanently.");
        messageBox.setInformativeText("Are you sure that you want to continue?");
        messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Ok);
        int retValue = messageBox.exec();

        if (layoutToRemove->IsCheckMessageScreen())
        {
            DisplayArea* checkMessageArea = static_cast<DisplayArea*>(layoutToRemove->GetAreaFromIndex(0));
            foreach (UIWidgetBase* widget, checkMessageArea->UIWidgets())
            {
                foreach (GCLSignalSink* sink, widget->GetSignalSinks())
                {
                    emit signal_WidgetToBeRemoved(sink, widget->GetUIWidgetID());
                }
            }
        }

        if (retValue == QMessageBox::Ok)
        {
            m_layoutsTreeModel->RemoveLayout(layoutIndex, m_project);
            m_currentSelectedObject = NULL;
        }

        QModelIndexList selectedIndexList = m_ui->layoutsView->selectionModel()->selectedIndexes();

        if (selectedIndexList.count() > 0)
        {
            QModelIndex currentSelectedIndex = selectedIndexList.first();
            if (currentSelectedIndex.isValid())
            {
                m_currentSelectedObject = static_cast<ProjectObject*>(currentSelectedIndex.internalPointer());
            }
            else
            {
                m_currentSelectedObject = NULL;
            }
        }
        else
        {
            m_currentSelectedObject = NULL;
            m_ui->layoutsView->selectionModel()->clearCurrentIndex();
            m_ui->layoutsView->selectionModel()->clearSelection();
            m_ui->areasView->selectionModel()->clearCurrentIndex();
            m_ui->areasView->selectionModel()->clearSelection();
            m_ui->widgetsView->selectionModel()->clearCurrentIndex();
            m_ui->widgetsView->selectionModel()->clearSelection();
        }

        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    });

    connect(m_ui->moveUpButton, &QPushButton::clicked, [=]
    {
        QModelIndex layoutIndex = m_ui->layoutsView->currentIndex();
        if (!layoutIndex.isValid()) return;

        m_layoutsTreeModel->MoveLayoutUp(layoutIndex);
        emit signal_updateButtonStates();
        //setButtonStates(layout);
    });

    connect(m_ui->moveDownButton, &QPushButton::clicked, [=]
    {
        QModelIndex layoutIndex = m_ui->layoutsView->currentIndex();
        if (!layoutIndex.isValid()) return;

        m_layoutsTreeModel->MoveLayoutDown(layoutIndex);
        emit signal_updateButtonStates();
    });

    // Area buttons
    connect(m_ui->addAreaButton, &QPushButton::clicked, [=]
    {
        DisplayArea* newArea = m_layoutsTreeModel->AddAreaToLayout(m_ui->layoutsView->currentIndex());
        Layout* layout = static_cast<Layout*>(newArea->GetParent());

        QModelIndex indexOfLayout = m_layoutsTreeModel->index(m_project->GetLayoutDefinition().Layouts().indexOf(layout), 0, QModelIndex());
        QModelIndex indexOfArea = m_layoutsTreeModel->index(m_layout->DisplayAreas().indexOf(newArea), 0, indexOfLayout);
        m_ui->areasView->selectionModel()->setCurrentIndex(indexOfArea, QItemSelectionModel::Select);
        m_ui->areasView->selectionModel()->select(indexOfArea, QItemSelectionModel::Select);

        m_currentSelectedObject = static_cast<ProjectObject*>(newArea);
        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    });

    connect(m_ui->removeAreaButton, &QPushButton::clicked, [=]
    {
        m_layoutsTreeModel->RemoveAreaFromLayout(m_ui->areasView->currentIndex());

        QModelIndexList selectedIndexList = m_ui->areasView->selectionModel()->selectedIndexes();

        if (selectedIndexList.count() > 0)
        {
            QModelIndex currentSelectedIndex = selectedIndexList.first();
            if (currentSelectedIndex.isValid())
            {
                m_currentSelectedObject = static_cast<ProjectObject*>(currentSelectedIndex.internalPointer());
            }
            else
            {
                m_currentSelectedObject = NULL;
            }
        }
        else
        {
            m_currentSelectedObject = NULL;
            m_ui->areasView->selectionModel()->clearCurrentIndex();
            m_ui->areasView->selectionModel()->clearSelection();
            m_ui->widgetsView->selectionModel()->clearCurrentIndex();
            m_ui->widgetsView->selectionModel()->clearSelection();
        }

        emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
    });

    connect(m_ui->moveAreaUpButton, &QPushButton::clicked, [=]
    {
        QModelIndex areaIndex = m_ui->areasView->currentIndex();
        if (!areaIndex.isValid()) return;

        m_layoutsTreeModel->MoveDisplayAreaUp(areaIndex);
        emit signal_updateButtonStates();
    });

    connect(m_ui->moveAreaDownButton, &QPushButton::clicked, [=]
    {
        QModelIndex areaIndex = m_ui->areasView->currentIndex();
        if (!areaIndex.isValid()) return;

        m_layoutsTreeModel->MoveDisplayAreaDown(areaIndex);
        emit signal_updateButtonStates();
    });

    // Widget buttons
    connect(m_ui->addWidgetButton, &QPushButton::clicked, [=]
    {
        QModelIndex areaIndex = m_ui->areasView->currentIndex();
        if (!areaIndex.isValid()) return;

        DisplayArea* parentArea = static_cast<DisplayArea*>(areaIndex.internalPointer());

        UIToolboxDialog* toolboxDialog = new UIToolboxDialog(m_project, parentArea);
        int result = toolboxDialog->exec();

        if (result == QDialog::Accepted)
        {
            if (m_ui->widgetsView->model() == NULL)
            {
                setWidgetsModel(m_layoutsTreeModel);
                m_ui->widgetsView->setRootIndex(m_layoutsTreeModel->index(0, 0, m_ui->areasView->rootIndex()));
            }
            UIWidgetBase* newWidget = parentArea->UIWidgets().first();

            m_currentSelectedObject = static_cast<ProjectObject*>(newWidget);
            Layout* layout = static_cast<Layout*>(parentArea->GetParent());
            QModelIndex indexOfLayout = m_layoutsTreeModel->index(m_project->GetLayoutDefinition().Layouts().indexOf(layout), 0, QModelIndex());
            QModelIndex indexOfArea = m_layoutsTreeModel->index(m_layout->DisplayAreas().indexOf(parentArea), 0, indexOfLayout);
            QModelIndex indexOfNewWidget = m_layoutsTreeModel->index(parentArea->UIWidgets().indexOf(newWidget), 0, indexOfArea);
            m_ui->widgetsView->setCurrentIndex(indexOfNewWidget);
            emit signal_LayoutEditorItemSelected(m_currentSelectedObject);

            UIWidgetBase* widget = static_cast<UIWidgetBase*>(m_ui->widgetsView->currentIndex().internalPointer());

            if(widget == NULL) return;

            DisplayArea* parentArea = static_cast<DisplayArea*>(widget->GetParent());

            if (parentArea->IsCheckMessageArea())
            {
                QList<quint32> widgetIDs;
                foreach (UIWidgetBase* widget, parentArea->UIWidgets())
                {
                    if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicIcon ||
                        widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage ||
                        widget->GetUIWidgetType() == UIWidgetBase::UIWIdget_Type_DynamicLabel)
                    {
                        widgetIDs.append(widget->GetUIWidgetID());
                    }
                }
                emit signal_updateWidgetOrder(widgetIDs);
            }
        }

        delete toolboxDialog;
    });

    connect(m_ui->removeWidgetButton, &QPushButton::clicked, [=]
    {
        UIWidgetBase* widget = static_cast<UIWidgetBase*>(m_ui->widgetsView->currentIndex().internalPointer());

        if(widget == NULL) return;

        DisplayArea* parentArea = static_cast<DisplayArea*>(widget->GetParent());

        if (parentArea->IsCheckMessageArea())
        {
            foreach (GCLSignalSink* sink, widget->GetSignalSinks())
            {
                emit signal_WidgetToBeRemoved(sink, widget->GetUIWidgetID());
            }
        }
        m_layoutsTreeModel->RemoveWidget(m_ui->widgetsView->currentIndex());


        QModelIndexList selectedIndexList = m_ui->widgetsView->selectionModel()->selectedIndexes();

        if (selectedIndexList.count() > 0)
        {
            QModelIndex currentSelectedIndex = selectedIndexList.first();
            if (currentSelectedIndex.isValid())
            {
                m_currentSelectedObject = static_cast<ProjectObject*>(currentSelectedIndex.internalPointer());
                emit signal_LayoutEditorItemSelected(m_currentSelectedObject);
            }
            else
            {
                m_currentSelectedObject = NULL;
            }
        }
        else
        {
            m_ui->widgetsView->selectionModel()->clearCurrentIndex();
            m_ui->widgetsView->selectionModel()->clearSelection();
            m_currentSelectedObject = NULL;
        }

        if (parentArea->UIWidgets().count() == 0)
        {
            setWidgetButtonsDisabled();
        }
    });

    connect(m_ui->moveWidgetButton, &QPushButton::clicked, this, &LayoutEditor::slot_moveWidgetButton_clicked);

    connect(m_ui->moveWidgetUpButton, &QPushButton::clicked, [=]
    {
        QModelIndex widgetIndex = m_ui->widgetsView->currentIndex();
        if (!widgetIndex.isValid()) return;

        m_layoutsTreeModel->MoveWidgetUp(widgetIndex);
        emit signal_updateButtonStates();

        UIWidgetBase* widget = static_cast<UIWidgetBase*>(m_ui->widgetsView->currentIndex().internalPointer());

        if(widget == NULL) return;

        DisplayArea* parentArea = static_cast<DisplayArea*>(widget->GetParent());

        if (parentArea->IsCheckMessageArea())
        {
            QList<quint32> widgetIDs;
            foreach (UIWidgetBase* widget, parentArea->UIWidgets())
            {
                if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicIcon ||
                    widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage ||
                    widget->GetUIWidgetType() == UIWidgetBase::UIWIdget_Type_DynamicLabel)
                {
                    widgetIDs.append(widget->GetUIWidgetID());
                }
            }
            emit signal_updateWidgetOrder(widgetIDs);
        }
    });

    connect(m_ui->moveWidgetDownButton, &QPushButton::clicked, [=]
    {
        QModelIndex widgetIndex = m_ui->widgetsView->currentIndex();
        if (!widgetIndex.isValid()) return;

        m_layoutsTreeModel->MoveWidgetDown(widgetIndex);
        emit signal_updateButtonStates();

        UIWidgetBase* widget = static_cast<UIWidgetBase*>(m_ui->widgetsView->currentIndex().internalPointer());

        if(widget == NULL) return;

        DisplayArea* parentArea = static_cast<DisplayArea*>(widget->GetParent());

        if (parentArea->IsCheckMessageArea())
        {
            QList<quint32> widgetIDs;
            foreach (UIWidgetBase* widget, parentArea->UIWidgets())
            {
                if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicIcon ||
                    widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage ||
                    widget->GetUIWidgetType() == UIWidgetBase::UIWIdget_Type_DynamicLabel)
                {
                    widgetIDs.append(widget->GetUIWidgetID());
                }
            }
            emit signal_updateWidgetOrder(widgetIDs);
        }
    });
}

void LayoutEditor::setLayoutButtonsDisabled()
{
    m_ui->removeLayoutButton->setEnabled(false);
    m_ui->moveUpButton->setEnabled(false);
    m_ui->moveDownButton->setEnabled(false);
}

void LayoutEditor::setAreaButtonsDisabled()
{
    m_ui->addAreaButton->setEnabled(false);
    m_ui->removeAreaButton->setEnabled(false);
    m_ui->moveAreaUpButton->setEnabled(false);
    m_ui->moveAreaDownButton->setEnabled(false);
}

void LayoutEditor::setWidgetButtonsDisabled()
{
    m_ui->addWidgetButton->setEnabled(false);
    m_ui->removeWidgetButton->setEnabled(false);
    m_ui->moveWidgetButton->setEnabled(false);
    m_ui->moveWidgetUpButton->setEnabled(false);
    m_ui->moveWidgetDownButton->setEnabled(false);
}

void LayoutEditor::setLayoutsModel(LayoutsTreeModel* model)
{
    if (m_ui->layoutsView->model() != NULL)
    {
        disconnect(m_ui->layoutsView, &QTableView::clicked, this, &LayoutEditor::slot_layoutView_Clicked);
        disconnect(m_ui->layoutsView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_layoutView_CurrentChanged);
    }

    m_ui->layoutsView->setModel(model);

    if (model != NULL)
    {
        connect(m_ui->layoutsView, &QTableView::clicked, this, &LayoutEditor::slot_layoutView_Clicked);
        connect(m_ui->layoutsView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_layoutView_CurrentChanged);
    }
}

void LayoutEditor::setAreasModel(LayoutsTreeModel* model)
{
    if (m_ui->areasView->model() != NULL)
    {
        disconnect(m_ui->areasView, &QTableView::clicked, this, &LayoutEditor::slot_areasView_Clicked);
        disconnect(m_ui->areasView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_areasView_CurrentChanged);
    }

    m_ui->areasView->setModel(model);

    if (model != NULL)
    {
        connect(m_ui->areasView, &QTableView::clicked, this, &LayoutEditor::slot_areasView_Clicked);
        connect(m_ui->areasView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_areasView_CurrentChanged);
    }
}

void LayoutEditor::setWidgetsModel(LayoutsTreeModel* model)
{
    if (m_ui->widgetsView->model() != NULL)
    {
        disconnect(m_ui->widgetsView, &QTableView::clicked, this, &LayoutEditor::slot_widgetsView_Clicked);
        disconnect(m_ui->widgetsView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_widgetsView_CurrentChanged);
    }

    m_ui->widgetsView->setModel(model);

    if (model != NULL)
    {
        connect(m_ui->widgetsView, &QTableView::clicked, this, &LayoutEditor::slot_widgetsView_Clicked);
        connect(m_ui->widgetsView->selectionModel(), &QItemSelectionModel::currentChanged, this, &LayoutEditor::slot_widgetsView_CurrentChanged);
    }
}

void LayoutEditor::slot_moveWidgetButton_clicked()
{
    QModelIndex selectedItemIndex = m_ui->widgetsView->currentIndex();
    if (!selectedItemIndex.isValid()) return;

    SelectDisplayAreaDialog selectDisplayAreaDialog(m_project);
    QDialog::DialogCode result = static_cast<QDialog::DialogCode>(selectDisplayAreaDialog.exec());

    if (result != QDialog::Accepted) return;

    DisplayArea* destinationArea = selectDisplayAreaDialog.GetSelectedDisplayArea();
    if (destinationArea == NULL ) return;

    m_layoutsTreeModel->MoveToArea(selectedItemIndex, destinationArea);
}

