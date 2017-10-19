#ifndef MENUEDITOR_H
#define MENUEDITOR_H

#include "InputToCommandMapping.h"

class MenuDefinition;
class MenuItem;
class MenuItemPreviewWidget;
class MenuItemWidgetsModel;
class MenuTreeModel;
class Project;
class ProjectObject;

class QComboBox;
class QString;
class QWidget;

namespace Ui
{
class MenuEditor;
}

class MenuEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MenuEditor(QWidget *parent = 0);
    ~MenuEditor(void);

    void selectMenuItem(MenuItem* itemToSelect);
    void setupItemWidgetsList(MenuItem* menuItem);
    void RemoveCompleteMenu();
public slots:
    void SetProject(Project* project);
    void ClearProject();
    void SetCurrentProjectObject(ProjectObject* projectObject); // When set from project browser
    void JustSelected();

signals:
    void signal_MenuEditorItemSelected(ProjectObject* projectObject);
    void signal_modified();

private:
    Ui::MenuEditor* m_ui;
    MenuTreeModel* m_menuTreeModel;
    MenuItemWidgetsModel* m_menuItemWidgetsModel;
    MenuDefinition* m_menuDefinition;
    MenuItemPreviewWidget* m_menuItemPreviewWidget;
    Project* m_project;
    ProjectObject* m_currentProjectObject;

    void setupComboBoxes(void);
    void setupComboBox(QComboBox* comboBox);
    void addComboBoxItems(QComboBox* comboBox);

    void populate(void);
    void populateInputMapping(void);
    void populateMenuBrowser(void);
    void selectMappingComboBoxItem(QComboBox* comboBox, MenuCommandID command);
    static QString stringForCommand(MenuCommandID command);    

private slots:
    void slot_inputMappingComboBox_activated(int newIndex);
};

#endif // MENUEDITOR_H
