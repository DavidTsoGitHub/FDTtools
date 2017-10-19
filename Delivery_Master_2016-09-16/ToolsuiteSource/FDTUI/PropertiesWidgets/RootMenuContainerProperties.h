#ifndef ROOTMENUCONTAINERPROPERTIES_H
#define ROOTMENUCONTAINERPROPERTIES_H

#include "ProjectObjectPropertiesWidget.h"

class Project;
class RootMenuItemContainer;

class QCheckBox;
class QComboBox;
class QModelIndex;
class QWidget;

class RootMenuContainerPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit RootMenuContainerPropertiesWidget(Project* project, QWidget *parent = 0);
    ~RootMenuContainerPropertiesWidget();
    void SetRootMenuContainer(RootMenuItemContainer* rootMenuContainer);

signals:
    void signal_IndexModified(const QModelIndex& rootMenuItemIndex);

private slots:
    void slot_wrapCheckBox_toggled(bool checked);
    void slot_startupMenuItemComboBox_valueChanged(int index);

private:
    QModelIndex m_rootMenuItemContainerIndex;
    RootMenuItemContainer* m_rootMenuItemContainer;
    QCheckBox* m_wrapCheckBox;  
    QComboBox* m_startupMenuItemComboBox;

    void populateComboBoxes(void);

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();

};

#endif // ROOTMENUCONTAINERPROPERTIES_H
