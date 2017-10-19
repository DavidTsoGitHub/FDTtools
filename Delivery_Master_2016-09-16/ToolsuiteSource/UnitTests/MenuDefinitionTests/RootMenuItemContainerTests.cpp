#include "RootMenuItemContainerTests.h"

#include "MenuItem.h"
#include "RootMenuItemContainer.h"

void RootMenuItemContainerTests::AddMenuItem_MenuItemIsNull_NoMenuItemAdded()
{
    // Arrange

    // Act
    RootMenuItemContainer target;
    target.AddMenuItem(NULL);

    // Assert
    QCOMPARE(target.MenuItems().count(), 0);
}

void RootMenuItemContainerTests::AddMenuItem_ValidMenuItem_MenuItemAdded()
{
    // Arrange
    const QString expectedName("MenuName");
    MenuItem* menuItem = new MenuItem(expectedName);

    // Act
    RootMenuItemContainer target;
    target.AddMenuItem(menuItem);

    // Assert
    QCOMPARE(target.MenuItems().count(), 1);
    QCOMPARE(target.MenuItems().at(0)->GetName(), expectedName);
}

void RootMenuItemContainerTests::RemoveMenuItem_KnownMenuItem_MenuItemIsRemoved()
{
    // Arrange
    const QString expectedName("MenuName");
    MenuItem* menuItem = new MenuItem(expectedName);
    RootMenuItemContainer target;
    target.AddMenuItem(menuItem);

    // Act
    target.RemoveMenuItem(menuItem);

    // Assert
    QCOMPARE(target.MenuItems().count(), 0);
}

void RootMenuItemContainerTests::RemoveMenuItem_UnknownMenuItem_NoMenuItemRemoved()
{
    // Arrange
    const QString expectedName("MenuName");
    MenuItem* menuItem = new MenuItem(expectedName);
    MenuItem* menuItem2 = new MenuItem("UnknownMenuItem");
    RootMenuItemContainer target;
    target.AddMenuItem(menuItem);

    // Act
    target.RemoveMenuItem(menuItem2);

    // Assert
    QCOMPARE(target.MenuItems().count(), 1);
    QCOMPARE(target.MenuItems().at(0)->GetName(), expectedName);
}

void RootMenuItemContainerTests::RemoveMenuItem_MenuItemIsNull_NoMenuItemRemoved()
{
    // Arrange
    const QString expectedName("MenuName");
    MenuItem* menuItem = new MenuItem(expectedName);
    RootMenuItemContainer target;
    target.AddMenuItem(menuItem);

    // Act
    target.RemoveMenuItem(NULL);

    // Assert
    QCOMPARE(target.MenuItems().count(), 1);
    QCOMPARE(target.MenuItems().at(0)->GetName(), expectedName);
}

void RootMenuItemContainerTests::MoveMenuItemUp_MenuItemIsSecondInList_MenuItemBecomesFirstInList()
{
    // Arrange
    const QString menuItem1Name("Menu1");
    const QString menuItem2Name("Menu2");
    MenuItem* menuItem1 = new MenuItem(menuItem1Name);
    MenuItem* menuItem2 = new MenuItem(menuItem2Name);

    RootMenuItemContainer target;
    target.AddMenuItem(menuItem1);
    target.AddMenuItem(menuItem2);

    // Act
    target.MoveMenuItemUp(menuItem2);

    // Assert
    QCOMPARE(target.MenuItems().at(0)->GetName(), menuItem2Name);
    QCOMPARE(target.MenuItems().at(1)->GetName(), menuItem1Name);
}

void RootMenuItemContainerTests::MoveMenuItemUp_MenuItemIsFirstInList_MenuItemDoesNotMove()
{
    // Arrange
    const QString menuItem1Name("Menu1");
    const QString menuItem2Name("Menu2");
    MenuItem* menuItem1 = new MenuItem(menuItem1Name);
    MenuItem* menuItem2 = new MenuItem(menuItem2Name);

    RootMenuItemContainer target;
    target.AddMenuItem(menuItem1);
    target.AddMenuItem(menuItem2);

    // Act
    target.MoveMenuItemUp(menuItem1);

    // Assert
    QCOMPARE(target.MenuItems().at(0)->GetName(), menuItem1Name);
    QCOMPARE(target.MenuItems().at(1)->GetName(), menuItem2Name);
}

void RootMenuItemContainerTests::MoveMenuItemDown_MenuItemIsFirstInList_MenuItemBecomesSecondInList()
{
    // Arrange
    const QString menuItem1Name("Menu1");
    const QString menuItem2Name("Menu2");
    MenuItem* menuItem1 = new MenuItem(menuItem1Name);
    MenuItem* menuItem2 = new MenuItem(menuItem2Name);

    RootMenuItemContainer target;
    target.AddMenuItem(menuItem1);
    target.AddMenuItem(menuItem2);

    // Act
    target.MoveMenuItemDown(menuItem1);

    // Assert
    QCOMPARE(target.MenuItems().at(0)->GetName(), menuItem2Name);
    QCOMPARE(target.MenuItems().at(1)->GetName(), menuItem1Name);
}

void RootMenuItemContainerTests::MoveMenuItemDown_MenuItemIsLastInList_MenuItemDoesNotMove()
{
    // Arrange
    const QString menuItem1Name("Menu1");
    const QString menuItem2Name("Menu2");
    MenuItem* menuItem1 = new MenuItem(menuItem1Name);
    MenuItem* menuItem2 = new MenuItem(menuItem2Name);

    RootMenuItemContainer target;
    target.AddMenuItem(menuItem1);
    target.AddMenuItem(menuItem2);

    // Act
    target.MoveMenuItemDown(menuItem2);

    // Assert
    QCOMPARE(target.MenuItems().at(0)->GetName(), menuItem1Name);
    QCOMPARE(target.MenuItems().at(1)->GetName(), menuItem2Name);
}
