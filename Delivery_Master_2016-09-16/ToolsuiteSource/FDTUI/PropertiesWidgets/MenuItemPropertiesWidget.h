#ifndef MENUITEMPROPERTIESWIDGET_H
#define MENUITEMPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class Project;
class ImageDefinition;
class LabelDefinition;
class MenuItem;

class QCheckBox;
class QComboBox;
class QLineEdit;
class QModelIndex;
class QWidget;

class MenuItemPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    MenuItemPropertiesWidget(Project* project, QWidget *parent = 0);
    ~MenuItemPropertiesWidget();

    void SetMenuItem(MenuItem* menuItem);

private:
    Project* m_project;
    LabelDefinition& m_labelDefinition;
    ImageDefinition& m_imageDefinition;
    MenuItem* m_menuItem;
    QLineEdit* m_menuItemNameEditor;
    QComboBox* m_labelIDCombobox;
    QComboBox* m_imageIDCombobox;
    QCheckBox* m_wrapCheckBox;

    void projectUpdated();

    void refreshLabelIDCombobox();
    void labelComboboxChanged(QVariant key);

    void refreshImageIDCombobox();
    void imageComboboxIndexChanged(int index);

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();
};

#endif // MENUITEMPROPERTIESWIDGET_H
