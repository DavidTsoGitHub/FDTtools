#include "MenuItemTests.h"
#include "MenuItem.h"
#include "LabelWidget.h"
#include "Project.h"

void MenuItemTests::AddSubMenuItem_ValidMenuItem_AddsMenuItemToParent()
{
    // Arrange

    // Act
    MenuItem target("Settings");
    target.AddSubMenuItem(new MenuItem("Language"));

    // Assert
    QCOMPARE(target.SubMenuItems().count(), 1);
    QCOMPARE(target.SubMenuItems().at(0)->GetName(), QString("Language"));
}

void MenuItemTests::AddLabelWidget_ValidLabelWidget_AddsLabelWidgetToParent()
{
    // Arrange
    LabelDefinition labelDefinition;
    Project project;

    // Act
    MenuItem target("Settings");
    target.AddUIWidget(new LabelWidget(labelDefinition, project.GetLayoutDefinition()));

    // Assert
    QCOMPARE(target.UIWidgets().count(), 1);
}

void MenuItemTests::MoveSubMenuItemUp_MenuItemIsSecondInList_MenuItemBecomesFirstInList()
{
    // Arrange
    const QString subMenuItem1Name("SubMenuItem1");
    const QString subMenuItem2Name("SubMenuItem2");
    MenuItem target("Settings");
    MenuItem* subMenuItem1 = new MenuItem(subMenuItem1Name);
    MenuItem* subMenuItem2 = new MenuItem(subMenuItem2Name);
    target.AddSubMenuItem(subMenuItem1);
    target.AddSubMenuItem(subMenuItem2);

    // Act
    target.MoveSubMenuItemUp(subMenuItem2);

    // Assert
    QCOMPARE(target.SubMenuItems().at(0)->GetName(), subMenuItem2Name);
    QCOMPARE(target.SubMenuItems().at(1)->GetName(), subMenuItem1Name);
}

void MenuItemTests::MoveSubMenuItemUp_MenuItemIsFirstInList_MenuItemDoesNotMove()
{
    // Arrange
    const QString subMenuItem1Name("SubMenuItem1");
    const QString subMenuItem2Name("SubMenuItem2");
    MenuItem target("Settings");
    MenuItem* subMenuItem1 = new MenuItem(subMenuItem1Name);
    MenuItem* subMenuItem2 = new MenuItem(subMenuItem2Name);
    target.AddSubMenuItem(subMenuItem1);
    target.AddSubMenuItem(subMenuItem2);

    // Act
    target.MoveSubMenuItemUp(subMenuItem1);

    // Assert
    QCOMPARE(target.SubMenuItems().at(0)->GetName(), subMenuItem1Name);
    QCOMPARE(target.SubMenuItems().at(1)->GetName(), subMenuItem2Name);
}

void MenuItemTests::MoveSubMenuItemDown_MenuItemIsFirstInList_MenuItemBecomesSecondInList()
{
    // Arrange
    const QString subMenuItem1Name("SubMenuItem1");
    const QString subMenuItem2Name("SubMenuItem2");
    MenuItem target("Settings");
    MenuItem* subMenuItem1 = new MenuItem(subMenuItem1Name);
    MenuItem* subMenuItem2 = new MenuItem(subMenuItem2Name);
    target.AddSubMenuItem(subMenuItem1);
    target.AddSubMenuItem(subMenuItem2);

    // Act
    target.MoveSubMenuItemDown(subMenuItem1);

    // Assert
    QCOMPARE(target.SubMenuItems().at(0)->GetName(), subMenuItem2Name);
    QCOMPARE(target.SubMenuItems().at(1)->GetName(), subMenuItem1Name);
}

void MenuItemTests::MoveSubMenuItemDown_MenuItemIsLastInList_MenuItemDoesNotMove()
{
    // Arrange
    const QString subMenuItem1Name("SubMenuItem1");
    const QString subMenuItem2Name("SubMenuItem2");
    MenuItem target("Settings");
    MenuItem* subMenuItem1 = new MenuItem(subMenuItem1Name);
    MenuItem* subMenuItem2 = new MenuItem(subMenuItem2Name);
    target.AddSubMenuItem(subMenuItem1);
    target.AddSubMenuItem(subMenuItem2);

    // Act
    target.MoveSubMenuItemDown(subMenuItem2);

    // Assert
    QCOMPARE(target.SubMenuItems().at(0)->GetName(), subMenuItem1Name);
    QCOMPARE(target.SubMenuItems().at(1)->GetName(), subMenuItem2Name);
}

