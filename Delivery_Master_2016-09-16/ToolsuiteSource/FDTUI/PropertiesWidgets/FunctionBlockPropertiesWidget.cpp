#include "FunctionBlockPropertiesWidget.h"

#include "ConfigurationParameter.h"
#include "ConfigurationParameterSpinBox.h"
#include "CalibrationParameterSpinBox.h"
#include "DataArrayLineEdit.h"
#include "DTCSelectorComboBox.h"
#include "FunctionBlock.h"
#include "IconSelectorComboBox.h"
#include "ImageSelectorComboBox.h"
#include "LabelMappingsPushButton.h"
#include "MappingsDialog.h"
#include "Project.h"

#include <QSpinBox>
#include <QPushButton>
#include <QWidget>

FunctionBlockPropertiesWidget::FunctionBlockPropertiesWidget(Project* project, QWidget* parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_functionBlock(NULL)
{
}

FunctionBlockPropertiesWidget::~FunctionBlockPropertiesWidget()
{
}

void FunctionBlockPropertiesWidget::SetFunctionBlock(FunctionBlock* functionBlock)
{
    m_functionBlock = functionBlock;
    SetProjectObject(m_functionBlock);
}

void FunctionBlockPropertiesWidget::createTasksTreeView()
{
    const FunctionDefinition& functionDef = m_functionBlock->GetFunctionDefinition();
    QTreeWidgetItem* tasksRootItem = AddContainer("Tasks");

    int taskNr = 1;
    foreach(const Task* task, functionDef.Tasks())
    {
        QTreeWidgetItem* taskSubItem = AddContainer("Task" + QString::number(taskNr++), tasksRootItem);
        AddDisplayData("Runnable", task->Runnable, taskSubItem);
        AddDisplayData("Init Function", task->InitFunction, taskSubItem);
        AddDisplayData("Initial Delay", QString::number(task->StartupDelay), taskSubItem);
        AddDisplayData("Periodicity", QString::number(task->Periodicity), taskSubItem);
        taskSubItem->setExpanded(true);
    }

    tasksRootItem->setExpanded(true);
}

void FunctionBlockPropertiesWidget::createConfigurationParametersTreeView()
{
    const FunctionDefinition& functionDefinition = m_functionBlock->GetFunctionDefinition();
    if (functionDefinition.ConfigurationParameters().count() == 0) return;

    QTreeWidgetItem* configurationParametersRootItem = AddContainer("Configuration Parameters");
    foreach (const ConfigurationParameter* cfgParam , functionDefinition.ConfigurationParameters())
    {
        QTreeWidgetItem* configurationParameterSubItem = AddContainer(cfgParam->GetDescriptiveName(), configurationParametersRootItem);
        AddDisplayData("Parameter Name", cfgParam->GetParameterName(), configurationParameterSubItem);
        AddDisplayData("Description", cfgParam->GetDescription(), configurationParameterSubItem);
        AddDisplayData("DataType", cfgParam->GetDataType(), configurationParameterSubItem);

        if (cfgParam->GetArrayLength() > 1)
        {
            AddDisplayData("ArrayLength", QString::number(cfgParam->GetArrayLength()), configurationParameterSubItem);
            DataArrayLineEdit* arrayEditor = new DataArrayLineEdit(m_functionBlock, cfgParam);
            AddEditableData("Values", arrayEditor, configurationParameterSubItem);
        }
        else
        {
            ConfigurationParameterSpinBox* spinBox = new ConfigurationParameterSpinBox(m_functionBlock, cfgParam);
            AddEditableData("Value", spinBox, configurationParameterSubItem);
        }
    }
    configurationParametersRootItem->setExpanded(true);
}

void FunctionBlockPropertiesWidget::createCalibrationParametersTreeView()
{
    const FunctionDefinition& functionDefinition = m_functionBlock->GetFunctionDefinition();
    if (functionDefinition.CalibrationParameters().count() == 0) return;

    QTreeWidgetItem* calibrationParametersRootItem = AddContainer("Calibration Parameters");
    foreach (const CalibrationParameter* calParam , functionDefinition.CalibrationParameters())
    {
        QTreeWidgetItem* calibrationParameterSubItem = AddContainer(calParam->GetDescriptiveName(), calibrationParametersRootItem);
        AddDisplayData("Parameter Name", calParam->GetParameterName(), calibrationParameterSubItem);
        AddDisplayData("Description", calParam->GetDescription(), calibrationParameterSubItem);
        AddDisplayData("DataType", calParam->GetDataType(), calibrationParameterSubItem);

        if (calParam->GetArrayLength() > 1)
        {
            AddDisplayData("ArrayLength", QString::number(calParam->GetArrayLength()), calibrationParameterSubItem);
            DataArrayLineEdit* arrayEditor = new DataArrayLineEdit(m_functionBlock, calParam);
            AddEditableData("Values", arrayEditor, calibrationParameterSubItem);
        }
        else
        {
            CalibrationParameterSpinBox* spinBox = new CalibrationParameterSpinBox(m_functionBlock, calParam);
            AddEditableData("Value", spinBox, calibrationParameterSubItem);
        }

    }

    calibrationParametersRootItem->setExpanded(true);
}

void FunctionBlockPropertiesWidget::createLabelMappingsTreeView()
{
    const FunctionDefinition& functionDefinition = m_functionBlock->GetFunctionDefinition();
    if (functionDefinition.LabelMappingsParameters().count() == 0) return;

    QTreeWidgetItem* labelMappingsRootItem = AddContainer("Label Mappings");
    foreach (LabelMappings* labelMappingsParameter, functionDefinition.LabelMappingsParameters())
    {
        QTreeWidgetItem* labelMappingsSubItem = AddContainer(labelMappingsParameter->GetDescriptiveName(), labelMappingsRootItem);
        AddDisplayData("Label Mappings Name", labelMappingsParameter->GetLabelMappingsName(), labelMappingsSubItem);
        AddDisplayData("Description", labelMappingsParameter->GetDescription(), labelMappingsSubItem);

        LabelMappingsPushButton* labelMappingsEditorPushButton = new LabelMappingsPushButton(labelMappingsParameter, "Edit Label Mappings", this);
        AddEditableData("", labelMappingsEditorPushButton, labelMappingsSubItem);
        connect(labelMappingsEditorPushButton, &LabelMappingsPushButton::signal_Clicked, this, &FunctionBlockPropertiesWidget::slot_labelMappingsEditorPushButton_clicked);
    }
}

void FunctionBlockPropertiesWidget::createDTCParametersTreeView()
{
    const FunctionDefinition& functionDefinition = m_functionBlock->GetFunctionDefinition();
    if (functionDefinition.FaultDefinitions().count() == 0) return;

    QTreeWidgetItem* dtcParametersRootItem = AddContainer("DTC Parameters");
    foreach (FaultDefinition dtc, functionDefinition.FaultDefinitions())
    {
        DTCSelectorComboBox* dtcSelector = new DTCSelectorComboBox(m_functionBlock, dtc.Name, getProject());
        AddEditableData(dtc.Name, dtcSelector, dtcParametersRootItem);
    }
}

void FunctionBlockPropertiesWidget::createUIResourcesTreeView()
{
    // Try to avoid cluttering the UI with unused tree items, only add the UI Resources
    // tree item if there are any UI Resources to display.
    QList<IconResource> iconResources(m_functionBlock->GetFunctionDefinition().IconResources());
    QList<ImageResource> imageResources(m_functionBlock->GetFunctionDefinition().ImageResources());
    if (iconResources.count() == 0 && imageResources.count() == 0) return;
    QTreeWidgetItem* uiResourcesRootItem = AddContainer("UI Resources");
    uiResourcesRootItem->setExpanded(true);

    if (iconResources.count() > 0)
    {
        QTreeWidgetItem* iconResourcesRootItem = AddContainer("Icons", uiResourcesRootItem);
        iconResourcesRootItem->setExpanded(true);
        foreach (IconResource iconResource, iconResources)
        {
            IconSelectorComboBox* iconSelector = new IconSelectorComboBox(m_functionBlock, iconResource.Name, getProject());
            AddEditableData(iconResource.Name, iconSelector, iconResourcesRootItem);
        }
    }

    if (imageResources.count() > 0)
    {
        QTreeWidgetItem* imageResourcesRootItem = AddContainer("Images", uiResourcesRootItem);
        imageResourcesRootItem->setExpanded(true);
        foreach (ImageResource imageResource, imageResources)
        {
            ImageSelectorComboBox* imageSelector = new ImageSelectorComboBox(m_functionBlock, imageResource.Name, getProject());
            AddEditableData(imageResource.Name, imageSelector, imageResourcesRootItem);
        }
    }
}

void FunctionBlockPropertiesWidget::slot_labelMappingsEditorPushButton_clicked(LabelMappings* labelMappings)
{
    MappingsDialog mappingsDialog(*labelMappings, getProject()->GetLabelDefinition().Labels(), this);

    QDialog::DialogCode result = (QDialog::DialogCode)mappingsDialog.exec();
    if (result == QDialog::Accepted)
    {
        labelMappings->SetMappedLabels(mappingsDialog.GetSelectedLabels());
    }
}

void FunctionBlockPropertiesWidget::beforePopulateSignals()
{
    QTreeWidgetItem* functionBlockNameItem = AddContainer(m_functionBlock->GetFunctionDefinition().GetName());
    AddDisplayData("Library Path", m_functionBlock->GetPathInFunctionLibrary(), functionBlockNameItem);
    functionBlockNameItem->setExpanded(true);

    createTasksTreeView();
}

void FunctionBlockPropertiesWidget::afterPopulateSignals()
{
    createConfigurationParametersTreeView();
    createCalibrationParametersTreeView();
    createLabelMappingsTreeView();
    createDTCParametersTreeView();
    createUIResourcesTreeView();
}
