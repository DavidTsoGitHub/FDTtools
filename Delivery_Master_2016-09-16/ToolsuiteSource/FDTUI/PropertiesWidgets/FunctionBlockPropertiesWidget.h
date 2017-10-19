#ifndef FUNCTIONBLOCKPROPERTIESWIDGET_H
#define FUNCTIONBLOCKPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class FunctionBlock;
class LabelMappings;
class Project;

class QWidget;

class FunctionBlockPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit FunctionBlockPropertiesWidget(Project* project, QWidget* parent = 0);
    ~FunctionBlockPropertiesWidget();
    void SetFunctionBlock(FunctionBlock* functionBlock);

private:
    FunctionBlock* m_functionBlock;

    void createTasksTreeView();
    void createConfigurationParametersTreeView();
    void createCalibrationParametersTreeView();
    void createLabelMappingsTreeView();
    void createDTCParametersTreeView();
    void createUIResourcesTreeView();

private slots:
    void slot_labelMappingsEditorPushButton_clicked(LabelMappings* labelMappings);

protected:
    void beforePopulateSignals();
    void afterPopulateSignals();
};

#endif // FUNCTIONBLOCKPROPERTIESWIDGET_H
