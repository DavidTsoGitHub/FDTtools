#include "LayoutDefinitionTests.h"
#include "LayoutDefinition.h"

void LayoutDefinitionTests::HasMenu_NoMenuDefinitionSet_ReturnsFalse()
{
    // Arrange
    LayoutDefinition target;

    // Act
    bool result = target.HasMenu();

    // Assert
    QCOMPARE(result, false);
}

void LayoutDefinitionTests::HasMenu_MenuDefinitionSet_ReturnsTrue()
{
    // Arrange
    LayoutDefinition target;
    DisplayArea area;
    target.SetMenu(new MenuDefinition(), &area);

    // Act
    bool result = target.HasMenu();

    // Assert
    QCOMPARE(result, true);
}

void LayoutDefinitionTests::GetMenu_NoMenuDefinitionSet_ReturnsNull()
{
    // Arrange
    LayoutDefinition target;

    // Act
    MenuDefinition* result = target.GetMenu();

    // Assert
    QCOMPARE(result, (MenuDefinition*)NULL);
}

void LayoutDefinitionTests::GetMenu_MenuDefinitionSet_ReturnsMenuDefinition()
{
    // Arrange
    LayoutDefinition target;
    DisplayArea area;
    MenuDefinition* expectedMenuDefinition = new MenuDefinition();
    target.SetMenu(expectedMenuDefinition, &area);

    // Act
    MenuDefinition* result = target.GetMenu();

    // Assert
    QCOMPARE(result, expectedMenuDefinition);
}

void LayoutDefinitionTests::SetMenu_NoDisplayArea_CreatesDisplayAreaForMenuDefinition()
{
    // Arrange
    const quint8 expectedMenuAreaIndex = 2;
    LayoutDefinition target;
    DisplayArea area;
    target.AddDisplayArea(&area);
    MenuDefinition* expectedMenuDefinition = new MenuDefinition();

    // Act
    target.SetMenu(expectedMenuDefinition, &area);

    // Assert
    QCOMPARE(target.DisplayAreas().count(), 1);
    QCOMPARE(target.GetMenuAreaIndex(), expectedMenuAreaIndex);
}

void LayoutDefinitionTests::SetMenu_ExistingDisplayArea_ExistingDisplayAreaHoldsMenuDefinition()
{
    // Arrange
    const quint8 expectedMenuAreaIndex = 2;
    LayoutDefinition target;
    DisplayArea* area = new DisplayArea();
    target.AddDisplayArea(area);

    // Act
    MenuDefinition* expectedMenuDefinition = new MenuDefinition();
    area->AppendChild(expectedMenuDefinition);
    target.SetMenu(expectedMenuDefinition, area);

    // Assert
    QCOMPARE(target.DisplayAreas().count(), 1);
    QCOMPARE(target.IsMenuArea(area), true);
    QCOMPARE(target.GetMenuAreaIndex(), expectedMenuAreaIndex);
}
