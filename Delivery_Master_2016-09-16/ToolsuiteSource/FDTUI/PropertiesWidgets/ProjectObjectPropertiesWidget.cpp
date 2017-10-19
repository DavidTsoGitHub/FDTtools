#include "ProjectObjectPropertiesWidget.h"
#include "ui_ProjectObjectPropertiesWidget.h"

#include "DynamicImageWidget.h"
#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
//#include "Project.h"
#include "SignalNameWidget.h"
#include "SignalMappingWidget.h"

#include <QAction>
#include <QMenu>
#include <QString>
#include <QTreeWidgetItem>
#include <QWidget>

ProjectObjectPropertiesWidget::ProjectObjectPropertiesWidget(Project* project, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProjectObjectPropertiesWidget),
    m_project(project),
    m_projectObject(NULL),
    m_inputSignalsRoot(NULL),
    m_outputSignalsRoot(NULL),
    m_usesFrameByFrameAnimation(false)
{
    m_ui->setupUi(this);

    m_headerFont = font();
    m_headerFont.setBold(true);

    m_ui->propertiesTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->propertiesTree, &QTreeWidget::customContextMenuRequested, this, &ProjectObjectPropertiesWidget::slot_showContextMenu);
    connect(m_ui->collapseAllButton, &QPushButton::pressed, this, &ProjectObjectPropertiesWidget::slot_collapseAllButton_pressed);
    connect(m_ui->expandAllButton, &QPushButton::pressed, this, &ProjectObjectPropertiesWidget::slot_expandAllButton_pressed);
}

ProjectObjectPropertiesWidget::~ProjectObjectPropertiesWidget()
{
    delete m_ui;
}

void ProjectObjectPropertiesWidget::SetProjectObject(ProjectObject* projectObject)
{
    if (m_projectObject == projectObject) return; // This is to avoid repopulating objects heavy on input signals, these cause a noticeable delay in the UI

    if (m_projectObject != NULL)
    {
        disconnect(m_projectObject, &ProjectObject::PropertyUpdatedRedrawNeeded, this, &ProjectObjectPropertiesWidget::populate);
    }

    m_projectObject = projectObject;
    populate();
    connect(m_projectObject, &ProjectObject::PropertyUpdatedRedrawNeeded, this, &ProjectObjectPropertiesWidget::populate);
}

QTreeWidgetItem* ProjectObjectPropertiesWidget::AddContainer(QString text, QTreeWidgetItem* parent)
{
    QTreeWidgetItem* containerItem;
    if (parent == NULL)
    {
        containerItem = new QTreeWidgetItem(m_ui->propertiesTree);
    }
    else
    {
        containerItem = new QTreeWidgetItem(parent);
    }

    containerItem->setFont(0, m_headerFont);
    containerItem->setText(0, text);
    containerItem->setToolTip(0, text);

    return containerItem;
}

QTreeWidgetItem* ProjectObjectPropertiesWidget::AddDisplayData(QString header, QString data, QTreeWidgetItem* parent)
{
    QTreeWidgetItem* dataItem;
    if (parent == NULL)
    {
        dataItem = new QTreeWidgetItem(m_ui->propertiesTree);
    }
    else
    {
        dataItem = new QTreeWidgetItem(parent);
    }

    dataItem->setText(0, header);
    dataItem->setToolTip(0, header);
    dataItem->setText(1, data);
    dataItem->setToolTip(1, data);

    return dataItem;
}

QTreeWidgetItem* ProjectObjectPropertiesWidget::AddEditableData(QString header, QWidget* editorWidget, QTreeWidgetItem* parent)
{
    QTreeWidgetItem* dataItem;
    if (parent == NULL)
    {
        dataItem = new QTreeWidgetItem(m_ui->propertiesTree);
    }
    else
    {
        dataItem = new QTreeWidgetItem(parent);
    }

    dataItem->setText(0, header);
    dataItem->setToolTip(0, header);
    m_ui->propertiesTree->setItemWidget(dataItem, 1, editorWidget);

    return dataItem;
}

void ProjectObjectPropertiesWidget::AddTreeWidgetItem(QTreeWidgetItem* headerItem, QWidget* editorItem, int column)
{
    m_ui->propertiesTree->setItemWidget(headerItem, column, editorItem);
}

void ProjectObjectPropertiesWidget::populateSignalSinks()
{
    m_usesFrameByFrameAnimation = false;

    IGCLSignalSink* objectWithSinks = dynamic_cast<IGCLSignalSink*>(m_projectObject);
    if (objectWithSinks != NULL)
    {
        if  (objectWithSinks->GetSignalSinks().count() == 0) return;
        if ( m_projectObject->objectName().compare("CAN") == 0 )
        {
            m_inputSignalsRoot = AddContainer("Transmit Signals");
        }
        else
        {
            m_inputSignalsRoot = AddContainer("Input Signals");
        }

        DynamicImageWidget* widget = dynamic_cast<DynamicImageWidget*>(m_projectObject);
        if (widget != NULL)
        {
            if (widget->GetAnimationID(1) != 0)
            {
                const Animation* animation = m_project->GetAnimationDefinition().GetAnimationByID(widget->GetAnimationID(1));
                if (animation->GetAnimationType() == Animation::Type::FrameByFrame)
                {
                    m_usesFrameByFrameAnimation = true;
                }
            }

            if (widget->GetAnimationID(2) != 0)
            {
                const Animation* animation = m_project->GetAnimationDefinition().GetAnimationByID(widget->GetAnimationID(2));
                if (animation->GetAnimationType() == Animation::Type::FrameByFrame)
                {
                    m_usesFrameByFrameAnimation = true;
                }
            }
        }

        QList<GCLSignalSink*> lista = objectWithSinks->GetSignalSinks();

        foreach (GCLSignalSink* sink, lista)
        {
            addInputSignal(m_inputSignalsRoot, sink);
        }
        m_inputSignalsRoot->setExpanded(true);
    }
}

void ProjectObjectPropertiesWidget::populateSignalSources()
{
    IGCLSignalSource* objectWithSources = dynamic_cast<IGCLSignalSource*>(m_projectObject);
    if (objectWithSources != NULL)
    {
        if  (objectWithSources->GetSignalSources().count() == 0) return;
        if ( m_projectObject->objectName().compare("CAN") == 0 )
        {
            m_outputSignalsRoot = AddContainer("Receive Signals");
        }
        else
        {
            m_outputSignalsRoot = AddContainer("Output Signals");
        }

        foreach (const GCLSignalSource* source, objectWithSources->GetSignalSources())
        {
            addOutputSignal(m_outputSignalsRoot, source);
        }
        m_outputSignalsRoot->setExpanded(true);
    }
}

void ProjectObjectPropertiesWidget::clearPropertiesTree()
{
    m_ui->propertiesTree->clear();
}

void ProjectObjectPropertiesWidget::populate()
{
    if (m_projectObject == NULL) return;

    clearPropertiesTree();

    beforePopulateSignals();

    // Add sinks
    beforePopulateSignalSinks();
    populateSignalSinks();
    afterPopulateSignalSinks();

    // Add sources
    beforePopulateSignalSources();
    populateSignalSources();
    afterPopulateSignalSources();

    afterPopulateSignals();
    afterPopulateUIHook();
}

void ProjectObjectPropertiesWidget::addInputSignal(QTreeWidgetItem* parent, GCLSignalSink* sink)
{
    QTreeWidgetItem* inputSignal = new QTreeWidgetItem(parent);
    inputSignal->setFirstColumnSpanned(true);
    if (m_usesFrameByFrameAnimation) inputSignal->setExpanded(true);
    m_ui->propertiesTree->setItemWidget(inputSignal, 0, new SignalNameWidget(sink));

    QTreeWidgetItem* mappingItem = new QTreeWidgetItem(inputSignal);
    mappingItem->setFirstColumnSpanned(true);
    SignalMappingWidget* signalMappingWidget = new SignalMappingWidget(sink, m_project);
    if (m_usesFrameByFrameAnimation) signalMappingWidget->setDisabled(true);
    m_ui->propertiesTree->setItemWidget(mappingItem, 0, signalMappingWidget);
}

void ProjectObjectPropertiesWidget::addOutputSignal(QTreeWidgetItem* parent, const GCLSignalSource* source)
{
    QTreeWidgetItem* inputSignal = new QTreeWidgetItem(parent);
    inputSignal->setFirstColumnSpanned(true);
    m_ui->propertiesTree->setItemWidget(inputSignal, 0, new SignalNameWidget(source));
}

void ProjectObjectPropertiesWidget::beforePopulateUIHook()
{}

void ProjectObjectPropertiesWidget::beforePopulateSignals()
{}

void ProjectObjectPropertiesWidget::beforePopulateSignalSinks()
{}

void ProjectObjectPropertiesWidget::afterPopulateSignalSinks()
{}

void ProjectObjectPropertiesWidget::beforePopulateSignalSources()
{}

void ProjectObjectPropertiesWidget::afterPopulateSignalSources()
{}

void ProjectObjectPropertiesWidget::afterPopulateSignals()
{}

void ProjectObjectPropertiesWidget::afterPopulateUIHook()
{
    m_ui->propertiesTree->resizeColumnToContents(0);
    m_ui->propertiesTree->resizeColumnToContents(1);
}

void ProjectObjectPropertiesWidget::slot_collapseAllButton_pressed()
{
    m_ui->propertiesTree->collapseAll();
}

void ProjectObjectPropertiesWidget::slot_expandAllButton_pressed()
{
    m_ui->propertiesTree->expandAll();
}

void ProjectObjectPropertiesWidget::slot_showContextMenu(QPoint pos)
{
    QPoint globalpos = mapToGlobal(pos);

    QMenu menu;
    QAction* expandAll = new QAction(this);
    expandAll->setText("Expand all");
    menu.addAction(expandAll);
    QAction* collapseAll = new QAction(this);
    collapseAll->setText("Collapse all");
    menu.addAction(collapseAll);
    menu.addSeparator();
    QAction* expandSelected = new QAction(this);
    expandSelected->setText("Expand selected");
    menu.addAction(expandSelected);
    QAction* collapseSelected = new QAction(this);
    collapseSelected->setText("Collapse selected");
    menu.addAction(collapseSelected);

    QAction* selectedItem = menu.exec(globalpos);
    if (selectedItem == expandAll)
    {
        m_ui->propertiesTree->expandAll();
    }
    else if (selectedItem == collapseAll)
    {
        m_ui->propertiesTree->collapseAll();
    }
    else if (selectedItem == expandSelected)
    {
        QTreeWidgetItem* currentItem = m_ui->propertiesTree->currentItem();
        setExpandedRecursive(currentItem, true);
    }
    else if (selectedItem == collapseSelected)
    {
        QTreeWidgetItem* currentItem = m_ui->propertiesTree->currentItem();
        setExpandedRecursive(currentItem, false);
    }
}

Project* ProjectObjectPropertiesWidget::getProject() const
{
    return m_project;
}

void ProjectObjectPropertiesWidget::setExpandedRecursive(QTreeWidgetItem* item, bool expanded)
{
    if (item == NULL) return;
    item->setExpanded(expanded);
    for (int i=0; i<item->childCount(); ++i)
    {
        setExpandedRecursive(item->child(i), expanded);
    }
}
